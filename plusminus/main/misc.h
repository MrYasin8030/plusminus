void weapon_set( ) {
	Item* weapon = LocalPlayer::Entity( )->GetActiveWeapon( );
	if (weapon == nullptr) return;

	//-3 - hammer, -2 - bows, -1 - eoka, 0-nopatch, 1 - meele, 2 - semiautomatic, 3 - automatic
	Weapon target = weapon->Info( );
	if (!target.id) return;
	if (!target.category) return;
	if (target.category == -2) {
		weapon->NoSway( );
		weapon->FastBow( );
		return;
	}
	if (target.category == 3) {
		weapon->NoSway( );
		weapon->RapidFire( );
		return;
	}
	if (target.category == 2) {
		weapon->NoSway( );
		weapon->RapidFire( );
		weapon->SetAutomatic( );
		return;
	}
	if (target.category == -1) {
		weapon->NoSway( );
		weapon->EokaTap( );
		return;
	}
}
float w_last_syringe = 0.f;
void misc_set( ) {
	if (w_last_syringe == 0.f) {
		w_last_syringe = LocalPlayer::Entity( )->lastSentTickTime( );
	}
	Item* weapon = LocalPlayer::Entity( )->GetActiveWeapon( );
	if ((weapon->GetID( ) == 1079279582 || weapon->GetID( ) == -2072273936) && vars::misc::faster_healing) {
		DWORD64 Held = weapon->entity( );
		bool deployed = read(Held + 0x190, bool);
		float curtime = LocalPlayer::Entity( )->lastSentTickTime( );
		if (LocalPlayer::Entity( )->GetKeyState(Button::FIRE_PRIMARY) && deployed && curtime > w_last_syringe + 0.7f) {
			reinterpret_cast<void(_stdcall*)(DWORD64, Str)>(vars::stor::gBase + CO::ServerRPC)(Held, Str(xorstr(L"UseSelf")));
			w_last_syringe = curtime;
		}
	}
	if (vars::misc::gravity) {
		if (GetAsyncKeyState(vars::keys::gravitykey))
			LocalPlayer::Entity( )->SetGravity(vars::misc::gravity_modifier);
		else LocalPlayer::Entity( )->SetGravity(2.5f);
	}
	auto klass = read(vars::stor::gBase + CO::ConvarGraphics, DWORD64);
	auto static_fields = read(klass + 0xB8, DWORD64);
	if (GetAsyncKeyState(vars::keys::zoom)) {
		write(static_fields + 0x18, 10.f, float);
	}
	else {
		write(static_fields + 0x18, vars::misc::fov, float);
	}
	if (vars::misc::spiderman) {
		uintptr_t Movement = read(LocalPlayer::Entity( ) + 0x4D8, uintptr_t);
		write(Movement + 0xB8, 0.f, float);
	}
	LocalPlayer::Entity( )->PatchCamspeed( );
	if (vars::misc::fakeadmin)
		LocalPlayer::Entity( )->FakeAdmin( );
}
namespace lol {
	void do_attack(f_object target, uintptr_t Held, bool transform) {
		if (!target.valid || !Held) return;

		if (read(Held + 0x238, float) >= Time::time( )) { return; }
		if (read(Held + 0x244, float) < read(Held + 0x1E0, float)) { return; }

		uintptr_t staticHitTest = read(vars::stor::gBase + CO::HitTest, DWORD64); if (!staticHitTest) return;
		uintptr_t newHitTest = il2cpp::il2cpp_object_new(staticHitTest); if (!newHitTest) return;

		uintptr_t trans; Ray ray = Ray(LocalPlayer::Entity( )->get_bone_pos(neck), (target.position - LocalPlayer::Entity( )->get_bone_pos(neck)).Normalized( ));
		if (!target.entity) return;
		if (transform) {
			trans = reinterpret_cast<BasePlayer*>(target.entity)->mono_transform(head);
		}
		else {
			//trans = utils::GetTransform((DWORD64)target.entity);
			trans = (DWORD64)reinterpret_cast<Object*>(target.entity)->transform( );
		} if (!trans) {
			LogSystem::Log(xorstr(L"No transform found"), 1.f);
			return;
		}

		write(newHitTest + 0x34, 1000.f, float);
		write(newHitTest + 0xB0, trans, uintptr_t);
		write(newHitTest + 0x14, ray, Ray);
		write(newHitTest + 0x66, true, bool);
		write(newHitTest + 0x88, reinterpret_cast<BasePlayer*>(target.entity), BasePlayer*);
		write(newHitTest + 0x90, reinterpret_cast<Transform*>(trans)->InverseTransformPoint(target.position), Vector3);
		write(newHitTest + 0x9C, reinterpret_cast<Transform*>(trans)->InverseTransformPoint(target.position), Vector3);
		write(newHitTest + 0x68, read(Held + 0x270, uintptr_t), uintptr_t);
		reinterpret_cast<void(*)(uintptr_t, float)>(vars::stor::gBase + CO::StartAttackCooldown)(Held, read(Held + 0x1E4, float));
		return reinterpret_cast<void(*)(uintptr_t, uintptr_t)>(vars::stor::gBase + CO::ProcessAttack)(Held, newHitTest);
	}
	// 0, -9.1 * gravityModifier, 0
	TraceResult traceProjectile(Vector3 position, Vector3 velocity, float drag, Vector3 gravity, Vector3 targetPoint) {
		constexpr float num = 0.03125f;
		Vector3 prevPosition = position;
		float prevDist = FLT_MAX;
		Line resultLine = Line(position, position);
		float travelTime = 0.f;
		TraceResult result;

		for (; travelTime < 8.f; ) {
			prevPosition = position;
			position += velocity * num;

			Line line = Line(prevPosition, position);
			Vector3 nearest = line.ClosestPoint(targetPoint);

			float dst = (nearest - targetPoint).Length( );

			if (dst > prevDist) {
				break;
			}
			prevDist = dst;
			resultLine = line;

			velocity += gravity * num;
			velocity -= velocity * drag * num;
			travelTime += num;
		}

		Vector3 hitPos = resultLine.ClosestPoint(targetPoint);

		result.hitDist = (hitPos - targetPoint).Length( );
		result.hitPosition = hitPos;
		result.outVelocity = velocity;
		result.hitTime = travelTime - num;
		return result;
	};
	uintptr_t shader;
	int property;
	void chams(uintptr_t target, Color col, bool npc = false) {
		if (!vars::players::chams) return;
		if (target) {
			if (!property) {
				property = reinterpret_cast<int(*)(Str)>(vars::stor::gBase + CO::PropertyToId)(Str(xorstr(L"_Color")));
			}
			auto mainRendList = reinterpret_cast<List<uintptr_t>*(*)(uintptr_t)>(vars::stor::gBase + CO::get_Renderers)(target);
			for (int idx = 0; idx < mainRendList->get_size( ); idx++) {
				uintptr_t renderer = mainRendList->get_value(idx);
				if (renderer) {
					uintptr_t material = reinterpret_cast<uintptr_t(*)(uintptr_t)>(vars::stor::gBase + CO::get_material)(renderer);
					if (shader != reinterpret_cast<uintptr_t(*)(uintptr_t)>(vars::stor::gBase + CO::get_shader)(material)) {
						if (!shader)
							shader = utils::ShaderFind(Str(xorstr(L"Hidden/Internal-Colored")));
						reinterpret_cast<void(*)(uintptr_t, uintptr_t)>(vars::stor::gBase + CO::set_shader)(material, shader);
						reinterpret_cast<void(*)(uintptr_t, int, Color)>(vars::stor::gBase + CO::SetColor)(material, property, col);
						if (!npc) {
							if (vars::players::chams_xqz) {
								reinterpret_cast<void(*)(uintptr_t, Str, int)>(vars::stor::gBase + CO::SetInt)(material, Str(xorstr(L"_ZTest")), 8);
							}
						}
						else {
							if (vars::npc::chams_xqz) {
								reinterpret_cast<void(*)(uintptr_t, Str, int)>(vars::stor::gBase + CO::SetInt)(material, Str(xorstr(L"_ZTest")), 8);
							}
						}
					}
				}
			}
		}
	}
	float LastUpdate = 0.f;
	void update_chams( ) {
		if (LocalPlayer::Entity( )->lastSentTickTime( ) > LastUpdate + 15.f) {
			reinterpret_cast<void(*)()>(vars::stor::gBase + CO::RebuildAll)();
			LastUpdate = LocalPlayer::Entity( )->lastSentTickTime( );
		}
	}
	float LastGrade = 0.f;
	void auto_grade(uintptr_t buildingblocc) {
		BuildingBlock* block = reinterpret_cast<BuildingBlock*>(buildingblocc);
		//LogSystem::Log(xorstr(L"xd"), 5.f);
		if (LocalPlayer::Entity( )->lastSentTickTime( ) > LastGrade + 0.35f
			&& block->CanAffordUpgrade((BuildingGrade)vars::misc::grade_, LocalPlayer::Entity( ))
			&& block->CanChangeToGrade((BuildingGrade)vars::misc::grade_, LocalPlayer::Entity( ))
			&& !block->IsUpgradeBlocked( )) {
			block->UpgradeToGrade((BuildingGrade)vars::misc::grade_, LocalPlayer::Entity( ));
			LastGrade = LocalPlayer::Entity( )->lastSentTickTime( );
		}
	}
	float LastKnock = 0.f; float LastOpen = 0.f; float LastHatch = 0.f;
	void knocker(uintptr_t Door) {
		typedef void(__stdcall* DoorFunction)(uintptr_t, BasePlayer*);
		if (LocalPlayer::Entity( )->lastSentTickTime( ) > LastKnock + 0.5f) {
			((DoorFunction)(vars::stor::gBase + CO::KnockDoor))(Door, LocalPlayer::Entity( ));
			LastKnock = LocalPlayer::Entity( )->lastSentTickTime( );
		}
		if (LocalPlayer::Entity( )->lastSentTickTime( ) > LastOpen + 0.1f) {
			((DoorFunction)(vars::stor::gBase + CO::OpenDoor))(Door, LocalPlayer::Entity( ));
			LastOpen = LocalPlayer::Entity( )->lastSentTickTime( );
		}
		if (LocalPlayer::Entity( )->lastSentTickTime( ) > LastHatch + 0.1f) {
			((DoorFunction)(vars::stor::gBase + CO::OpenHatch))(Door, LocalPlayer::Entity( ));
			LastHatch = LocalPlayer::Entity( )->lastSentTickTime( );
		}
	}
	float LastPickup = 0.f;
	void pickup_player(BasePlayer* ent) {
		typedef void(__stdcall* AssistPlayer)(BasePlayer*, BasePlayer*);
		if (!LocalPlayer::Entity( )->is_teammate(ent->userID( )) && vars::misc::revive_team_only) return;
		if (LocalPlayer::Entity( )->lastSentTickTime( ) > LastPickup + 0.5f) {
			((AssistPlayer)(vars::stor::gBase + CO::AssistPlayer))(ent, LocalPlayer::Entity( ));
			LastPickup = LocalPlayer::Entity( )->lastSentTickTime( );
		}
	}
	void cachePlayer(BasePlayer* player) {
		auto model = player->model( );
		if (model) {
			auto cache = new BoneCache( );

			cache->head = model->resolve(STATIC_CRC32("head"));
			cache->neck = model->resolve(STATIC_CRC32("neck"));
			cache->root = model->resolve(STATIC_CRC32("root"));
			cache->spine4 = model->resolve(STATIC_CRC32("spine4"));
			cache->spine1 = model->resolve(STATIC_CRC32("spine1"));
			cache->l_clavicle = model->resolve(STATIC_CRC32("l_clavicle"));
			cache->l_upperarm = model->resolve(STATIC_CRC32("l_upperarm"));
			cache->l_forearm = model->resolve(STATIC_CRC32("l_forearm"));
			cache->l_hand = model->resolve(STATIC_CRC32("l_hand"));
			cache->r_clavicle = model->resolve(STATIC_CRC32("r_clavicle"));
			cache->r_upperarm = model->resolve(STATIC_CRC32("r_upperarm"));
			cache->r_forearm = model->resolve(STATIC_CRC32("r_forearm"));
			cache->r_hand = model->resolve(STATIC_CRC32("r_hand"));
			cache->pelvis = model->resolve(STATIC_CRC32("pelvis"));
			cache->l_hip = model->resolve(STATIC_CRC32("l_hip"));
			cache->l_knee = model->resolve(STATIC_CRC32("l_knee"));
			cache->l_ankle_scale = model->resolve(STATIC_CRC32("l_ankle_scale"));
			cache->l_foot = model->resolve(STATIC_CRC32("l_foot"));
			cache->r_hip = model->resolve(STATIC_CRC32("r_hip"));
			cache->r_knee = model->resolve(STATIC_CRC32("r_knee"));
			cache->r_ankle_scale = model->resolve(STATIC_CRC32("r_ankle_scale"));
			cache->r_foot = model->resolve(STATIC_CRC32("r_foot"));

			if (!map_contains_key(cachedBones, player->userID( )))
				cachedBones.insert(std::make_pair(player->userID( ), cache));
			else
				cachedBones[ player->userID( ) ] = cache;
		}
	}
	void auto_farm_loop(bool weaponmelee, uintptr_t active) {
		if (vars::misc::auto_pickup) {
			f_object entity = f_object::get_closest_object(LocalPlayer::Entity( )->get_bone_pos(head),
				xorstr("/collectable/"),
				Vector3::Zero( ),
				Vector3::Zero( ),
				Vector3::Zero( ),
				false
			);
			if (entity.valid) {
				Vector3 local = utils::ClosestPoint(LocalPlayer::Entity( ), entity.position);
				if (Math::Distance_3D(local, entity.position) < 3.f) {
					if (reinterpret_cast<BaseEntity*>(entity.entity)->IsValid( )) {
						utils::ServerRPC(entity.entity, Str(xorstr(L"Pickup")));
					}
				}
			}
		}
		if (vars::misc::auto_grade) {
			f_object building_block = f_object::get_closest_object(LocalPlayer::Entity( )->get_bone_pos(head),
				xorstr(""),
				Vector3::Zero( ),
				Vector3::Zero( ),
				Vector3::Zero( ),
				true,
				xorstr("BuildingBlock"));
			if (building_block.valid) {
				Vector3 local = utils::ClosestPoint(LocalPlayer::Entity( ), building_block.position);
				if (Math::Distance_3D(local, building_block.position) <= 2.f) {
					if (reinterpret_cast<BaseEntity*>(building_block.entity)->IsValid( )) {
						lol::auto_grade(building_block.entity);
					}
				}
			}
		}
		if (vars::misc::auto_farm_barrel) {
			if (weaponmelee) {
				f_object ore_hot_spot = f_object::get_closest_object(LocalPlayer::Entity( )->get_bone_pos(head),
					xorstr("barrel"),
					Vector3::Zero( ),
					Vector3::Zero( ),
					Vector3::Zero( ),
					false,
					xorstr(""));
				if (ore_hot_spot.valid) {
					Vector3 local = utils::ClosestPoint(LocalPlayer::Entity( ), ore_hot_spot.position);
					if (Math::Distance_3D(local, ore_hot_spot.position) <= 2.f) {
						if (reinterpret_cast<BaseEntity*>(ore_hot_spot.entity)->IsValid( )) {
							lol::do_attack(ore_hot_spot, active, false);
						}
					}
				}
			}
		}
		if (vars::misc::auto_farm_ore) {
			if (weaponmelee) {
				f_object ore_hot_spot = f_object::get_closest_object(LocalPlayer::Entity( )->get_bone_pos(head),
					xorstr(""),
					Vector3::Zero( ),
					Vector3::Zero( ),
					Vector3::Zero( ),
					true,
					xorstr("OreHotSpot"));
				if (ore_hot_spot.valid) {
					Vector3 local = utils::ClosestPoint(LocalPlayer::Entity( ), ore_hot_spot.position);
					if (Math::Distance_3D(local, ore_hot_spot.position) <= 2.f) {
						if (reinterpret_cast<BaseEntity*>(ore_hot_spot.entity)->IsValid( )) {
							lol::do_attack(ore_hot_spot, active, false);
						}
					}
				}
			}
		}
		if (vars::misc::auto_farm_tree) {
			if (weaponmelee) {
				f_object tree_entity = f_object::get_closest_object(LocalPlayer::Entity( )->get_bone_pos(head),
					xorstr(""),
					Vector3::Zero( ),
					Vector3::Zero( ),
					Vector3::Zero( ),
					true,
					xorstr("TreeEntity"));
				if (tree_entity.valid) {
					tree_entity.position = Vector3::Zero( );
					f_object tree_marker = f_object::get_closest_object(LocalPlayer::Entity( )->get_bone_pos(head),
						xorstr(""),
						Vector3::Zero( ),
						Vector3::Zero( ),
						Vector3::Zero( ),
						true,
						xorstr("TreeMarker"));
					if (tree_marker.valid) {
						Vector3 locala = utils::ClosestPoint(LocalPlayer::Entity( ), tree_marker.position);
						if (Math::Distance_3D(locala, tree_marker.position) <= 2.f) {
							tree_entity.position = tree_marker.position;
							Vector3 local = utils::ClosestPoint(LocalPlayer::Entity( ), tree_entity.position);
							if (Math::Distance_3D(local, tree_entity.position) <= 2.f) {
								if (reinterpret_cast<BaseEntity*>(tree_entity.entity)->IsValid( )) {
									lol::do_attack(tree_entity, active, false);
								}
							}
						}
					}
				}
			}
		}
	}
}
float flyhackDistanceVertical = 0.f;
float flyhackDistanceHorizontal = 0.f;
float flyhackPauseTime;
void TestFlying( ) {
	flyhackPauseTime = Mathf::Max(0.f, flyhackPauseTime - Time::deltaTime( ));
	bool inAir = false;
	float radius = reinterpret_cast<float(*)(BasePlayer*)>(vars::stor::gBase + CO::GetRadius)(LocalPlayer::Entity( ));
	float height = reinterpret_cast<float(*)(BasePlayer*, bool)>(vars::stor::gBase + CO::GetHeight)(LocalPlayer::Entity( ), false);
	Vector3 vector = (LocalPlayer::Entity( )->lastSentTick( )->position( ) + read(read(LocalPlayer::Entity( ) + 0x4B0, uintptr_t) + 0x1D8, Vector3)) * 0.5f;
	Vector3 vector2 = vector + Vector3(0.f, radius - 2.f, 0.f);
	Vector3 vector3 = vector + Vector3(0.f, height - radius, 0.f);
	float radius2 = radius - 0.05f;
	bool a = Physics::CheckCapsule(vector2, vector3, radius2, 1503731969, QueryTriggerInteraction::Ignore);
	inAir = !a;

	if (inAir) {
		bool flag = false;

		Vector3 vector4 = read(read(LocalPlayer::Entity( ) + 0x4B0, uintptr_t) + 0x1D8, Vector3) - LocalPlayer::Entity( )->lastSentTick( )->position( );
		float num3 = Mathf::Abs(vector4.y);
		float num4 = reinterpret_cast<float(*)(Vector3)>(vars::stor::gBase + CO::Magnitude2D)(vector4);
		if (vector4.y >= 0.f) {
			flyhackDistanceVertical += vector4.y;
			flag = true;
		}
		if (num3 < num4) {
			flyhackDistanceHorizontal += num4;
			flag = true;
		}
		if (flag) {
			float num5 = Mathf::Max((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
			float num6 = LocalPlayer::Entity( )->GetJumpHeight( ) + num5;
			if (flyhackDistanceVertical > num6) {
				//return true;
			}
			float num7 = Mathf::Max((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
			float num8 = 5.f + num7;
			if (flyhackDistanceHorizontal > num8) {
				//return true;
			}
		}
	}
	else {
		flyhackDistanceHorizontal = 0.f;
		flyhackDistanceVertical = 0.f;
	}
}
void CheckFlyhack( ) {
	TestFlying( );
	float num5 = Mathf::Max((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
	float num6 = LocalPlayer::Entity( )->GetJumpHeight( ) + num5;
	vars::stuff::max_flyhack = num6;
	if (flyhackDistanceVertical <= num6) {
		vars::stuff::flyhack = flyhackDistanceVertical;
	}

	float num7 = Mathf::Max((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
	float num8 = 5.f + num7;
	vars::stuff::max_hor_flyhack = num8;
	if (flyhackDistanceHorizontal <= num8) {
		vars::stuff::hor_flyhack = flyhackDistanceHorizontal;
	}
}
/*if (should_interactive) {
				bool left = LocalPlayer::Entity( )->GetKeyState(Button::LEFT);
				bool right = LocalPlayer::Entity( )->GetKeyState(Button::RIGHT);
				bool forward = LocalPlayer::Entity( )->GetKeyState(Button::FORWARD);
				bool back = LocalPlayer::Entity( )->GetKeyState(Button::BACKWARD);
				bool up = LocalPlayer::Entity( )->GetKeyState(Button::JUMP);
				bool down = LocalPlayer::Entity( )->GetKeyState(Button::DUCK);

				Vector3 sheesh = Vector3::Zero( );
				if (left) {
					sheesh += Vector3(-1, 0, 0);
				}
				if (right) {
					sheesh += Vector3(1, 0, 0);
				}
				if (forward) {
					sheesh += Vector3(0, 0, 1);
				}
				if (back) {
					sheesh += Vector3(0, 0, -1);do
				}
				if (up) {
					sheesh += Vector3(0, 1, 0);
				}
				if (back) {
					sheesh += Vector3(0, -1, 0);
				}

				*reinterpret_cast<Vector3*>(LocalPlayer::Entity( )->eyes( ) + 0x38) = sheesh;
			}*/
			//if (vars::stuff::testBool && vars::stor::closestPlayer != null) {
						//	uintptr_t mag = read(LocalPlayer::Entity( )->GetActiveWeapon( )->entity( ) + 0x2A8, uintptr_t);
						//	if (mag == null) { LogSystem::Log(xorstr(L"mag == null"), 5.f); }

						//	uintptr_t ammotype = read(mag + 0x20, uintptr_t);
						//	if (ammotype == null) { LogSystem::Log(xorstr(L"ammotype == null"), 5.f); }

						//	uintptr_t component = reinterpret_cast<uintptr_t(*)(uintptr_t, uintptr_t)>(vars::stor::gBase + 0x102AF10)(
						//		ammotype,
						//		read(vars::stor::gBase + 0x3194970, uintptr_t)); // ItemModProjectile
						//	if (component == null) { LogSystem::Log(xorstr(L"component == null"), 5.f); }

						//	uintptr_t v70 = read(component + 0x18, uintptr_t);
						//	if (v70 == null) { LogSystem::Log(xorstr(L"v70 == null"), 5.f); }

						//	uintptr_t v71 = reinterpret_cast<uintptr_t(*)(uintptr_t, uintptr_t)>(vars::stor::gBase + 0x8EBAA0)(v70,
						//		read(vars::stor::gBase + 0x3189118, uintptr_t));
						//	if (v71 == null) { LogSystem::Log(xorstr(L"v71 == null"), 5.f); }

						//	uintptr_t v73 = reinterpret_cast<uintptr_t(*)(uintptr_t, uintptr_t)>(vars::stor::gBase + 0x10315E0)(v71,
						//		read(vars::stor::gBase + 0x318D9B8, uintptr_t));
						//	if (v73 == null) { LogSystem::Log(xorstr(L"v73 == null"), 5.f); }

						//	Projectile* prrrr = reinterpret_cast<Projectile*>(v73);

						//	TraceResult f = lol::traceProjectile(LocalPlayer::Entity( )->eyes( )->get_position( ), prrrr->initialVelocity( )
						//		, prrrr->drag( ), Vector3(0, -9.1 * prrrr->gravityModifier( ), 0),
						//		reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->get_bone_pos(head));
						//	LogSystem::AddTraceResult(f);
						//}