﻿using namespace otherEsp;
bool inited = false;
void ent_loop( ) {
	uintptr_t bn = read(vars::stor::gBase + CO::BaseNetworkable, uintptr_t);
	if (!bn) {
		Renderer::Text(Vector2(100, 55), D2D1::ColorF::White, false, true, xorstr(L"wrong game version"));
		return;
	}
	else {
		Renderer::Text(Vector2(100, 55), D2D1::ColorF::White, false, true, xorstr(L"new discord: https://discord.gg/9hnYsU4w3b"));
	}
	if (!inited) {
		LogSystem::Log(xorstr(L"Cheat loaded successfully!"), 7.5f);
		Beep(500, 100);
		config.Initialize( );
		inited = true;
	}
	if (!LocalPlayer::Entity( )) return;
	if (vars::visuals::radar_) {
		radar::radar_bg( );
	}
	if (vars::misc::flyhack_indicator) {
		Renderer::FillRectangle(Vector2(vars::stuff::ScreenWidth / 2 - 214, 200), Vector2(428 * (vars::stuff::flyhack / vars::stuff::max_flyhack), 10), D2D1::ColorF::Lime);
		Renderer::Rectangle(Vector2(vars::stuff::ScreenWidth / 2 - 214, 200), Vector2(428, 10), D2D1::ColorF::Black);

		Renderer::FillRectangle(Vector2(vars::stuff::ScreenWidth / 2 - 214, 230), Vector2(428 * (vars::stuff::hor_flyhack / vars::stuff::max_hor_flyhack), 10), D2D1::ColorF::Lime);
		Renderer::Rectangle(Vector2(vars::stuff::ScreenWidth / 2 - 214, 230), Vector2(428, 10), D2D1::ColorF::Black);
	}
	LogSystem::Render( );
	LogSystem::RenderTracers( );
	LogSystem::RenderTraceResults( );
	if (vars::visuals::raid_esp) {
		LogSystem::RenderExplosions( );
	}
	float FOV = vars::combat::fov, CurFOV;
	/*if (vars::stuff::testBool) {
		ImGuiIO& io = ImGui::GetIO( );
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.0f, 0.0f, 0.0f, 0.0f});
		ImGui::Begin("##Backbuffer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);
	}*/
	auto entityList = BaseNetworkable::clientEntities( )->entityList( );
	if (entityList) {
		for (int i = 0; i < entityList->vals->size; i++) {
			uintptr_t Entity = *reinterpret_cast<uintptr_t*>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
			if (!Entity) continue;
			uintptr_t Object = *reinterpret_cast<uint64_t*>(Entity + 0x10);
			if (!Object) continue;
			uintptr_t ObjectClass = *reinterpret_cast<uint64_t*>(Object + 0x30);
			if (!ObjectClass) continue;

			pUncStr name = read(ObjectClass + 0x60, pUncStr); if (!name) continue;
			char* buff = name->stub;

			if (!reinterpret_cast<BaseEntity*>(Entity)->IsValid( )) { continue; }
			if (!reinterpret_cast<Component*>(Entity)->gameObject( )) { continue; }
			if (strstr(buff, xorstr("Local"))) { continue; }

			if (vars::visuals::radar_) {
				radar::radar_logic(ObjectClass, Entity, buff);
			}
			if (reinterpret_cast<BaseCombatEntity*>(Entity)->IsPlayer( )) {
				BasePlayer* Player = (BasePlayer*)Entity;

				if (!Player->isCached( )) continue;

				if (vars::players::skeleton && !Player->IsNpc( )) {
					if (!Player->HasFlags(PlayerFlags::Sleeping)) {
						if (LocalPlayer::Entity( )->is_teammate(Player->userID( ))) {
							Skeleton(Player, D2D1::ColorF::Lime);
						}
						else {
							if (Player->health( ) <= 0) {
								Skeleton(Player, D2D1::ColorF::Red);
							}
							else {
								
								if (Player->is_visible( )) {
									Skeleton(Player, D2D1::ColorF::White);
								}
								else {
									Skeleton(Player, D2D1::ColorF::DarkGray);
								}
							}
						}
					}
					else if (Player->HasFlags(PlayerFlags::Sleeping) && !vars::players::sleeperignore) {
						Skeleton(Player, D2D1::ColorF::Orange);
					}
				}
				else if (vars::npc::skeleton && Player->IsNpc( )) {
					Skeleton(Player, D2D1::ColorF::Yellow);
				}

				D2D1::ColorF color = D2D1::ColorF(0.f, 0.f, 0.f, 1.f);
				if (!Player->IsNpc( )) {
					if (!Player->HasFlags(PlayerFlags::Sleeping)) {
						if (LocalPlayer::Entity( )->is_teammate(Player->userID( ))) {
							color = D2D1::ColorF::Lime;
						}
						else {
							if (Player->health( ) <= 0) {
								color = D2D1::ColorF::Red;
							}
							else {
								if (Player->is_visible( )) {
									color = D2D1::ColorF::White;
								}
								else {
									color = D2D1::ColorF::DarkGray;
								}
							}
						}
					}
					else {
						color = D2D1::ColorF::Orange;
					}
					ESP(Player, color);
				}
				else if (Player->IsNpc()) {
					color = D2D1::ColorF::Yellow;
					if (Player->health( ) > 0) {
						NPCESP(Player, D2D1::ColorF::Yellow);
					}
				}
				else {
					color = D2D1::ColorF::Yellow;
				}
				OOF(Player, color);

				if (vars::combat::ignore_sleepers && Player->HasFlags(PlayerFlags::Sleeping)) continue;
				if (vars::combat::ignore_npc && Player->IsNpc( )) continue;
				if (vars::combat::ignore_team && LocalPlayer::Entity( )->is_teammate(Player->userID( ))) continue;
				if (vars::combat::ignore_invisible && !Player->is_visible( )) continue;
				if (Player->get_bone_pos(head).x == 0 || Player->get_bone_pos(head).y == 0 || Player->get_bone_pos(head).z == 0) continue;
				if (vars::combat::ignore_players) continue;
				if (Math::Distance_3D(LocalPlayer::Entity( )->get_bone_pos(head), Player->get_bone_pos(head)) > vars::combat::range) continue;

				if (FOV > (CurFOV = GetFov(Player, BoneList(0.5))) && Player->health( ) > 0 && !vars::combat::lock_target) {
					FOV = CurFOV; vars::stor::closestPlayer = (uintptr_t)Player;
				}
			}
			if (reinterpret_cast<BaseCombatEntity*>(Entity)->ClassNameHash( ) == STATIC_CRC32("BaseHelicopter")) {
				BaseEntity* helicopter = (BaseEntity*)Entity;
				if (helicopter) {
					float health = reinterpret_cast<BaseCombatEntity*>(Entity)->health( );
					float maxhealth = 10000.f;
					Vector3 pos = helicopter->transform( )->position( );
					Vector2 screenPos;
					if (utils::w2s(pos, screenPos)) {
						if (vars::visuals::patrol_heli) {
							Renderer::Text(screenPos, D2D1::ColorF(0.5f, 0.54f, 1.f), true, true, xorstr(L"Helicopter [%.2fm]"), Math::Distance_3D(LocalPlayer::Entity( )->get_bone_pos(head), pos));
							Renderer::Text(screenPos + Vector2(0, 15), D2D1::ColorF(0.5f, 0.54f, 1.f), true, true, xorstr(L"[%dHP]"), (int)health);

							Renderer::FillRectangle(Vector2{screenPos - Vector2(30, 0) + Vector2(0, 25)}, Vector2{60 * (health / maxhealth), 6}, D2D1::ColorF(0.f, 255.f, 0.f, 0.8f));
							Renderer::Rectangle(Vector2{screenPos - Vector2(30, 0) + Vector2(0, 25)}, Vector2{60, 6}, D2D1::ColorF::Black, 0.5f);
						}
						if (health > 0 && !vars::combat::ignore_heli) {
							if ((FOV > (CurFOV = GetFovHeli(pos)))) {
								FOV = CurFOV; vars::stor::closestHeli = (uintptr_t)helicopter;
							}
						}
					}
				}
			}
			otherEsp::bradley(ObjectClass, Entity, buff);
			otherEsp::corpse(ObjectClass, Entity, buff);
			otherEsp::tc(ObjectClass, Entity, buff);
			otherEsp::sleepingbag(ObjectClass, Entity, buff);
			otherEsp::bed(ObjectClass, Entity, buff);
			otherEsp::stash(ObjectClass, Entity, buff);
			otherEsp::hackablecrate(ObjectClass, Entity, buff);
			otherEsp::world(ObjectClass, Entity, buff);
			if (vars::ores::show_collectables) {
				miscvis(Entity, buff, vars::ores::stone, vars::ores::show_distance, vars::ores::draw_distance, xorstr("stone-collectable.prefab"), xorstr(L"Stone Collectable"), D2D1::ColorF::Gray);
				miscvis(Entity, buff, vars::ores::sulfur, vars::ores::show_distance, vars::ores::draw_distance, xorstr("sulfur-collectable.prefab"), xorstr(L"Sulfur Collectable"), D2D1::ColorF::Gold);
				miscvis(Entity, buff, vars::ores::metal, vars::ores::show_distance, vars::ores::draw_distance, xorstr("metal-collectable.prefab"), xorstr(L"Metal Collectable"), D2D1::ColorF::SaddleBrown);
			}
			// ---------------------------------------------------------
			miscvis(Entity, buff, vars::ores::stone, vars::ores::show_distance, vars::ores::draw_distance, xorstr("stone-ore.prefab"), xorstr(L"Stone Ore"), D2D1::ColorF::Gray);
			miscvis(Entity, buff, vars::ores::sulfur, vars::ores::show_distance, vars::ores::draw_distance, xorstr("sulfur-ore.prefab"), xorstr(L"Sulfur Ore"), D2D1::ColorF::Gold);
			miscvis(Entity, buff, vars::ores::metal, vars::ores::show_distance, vars::ores::draw_distance, xorstr("metal-ore.prefab"), xorstr(L"Metal Ore"), D2D1::ColorF::SaddleBrown);
			// ---------------------------------------------------------
			miscvis(Entity, buff, vars::visuals::crates::elite, vars::visuals::crates::show_distance, vars::visuals::crates::draw_distance, xorstr("crate_elite.prefab"), xorstr(L"Elite Crate"), D2D1::ColorF::SeaGreen);
			miscvis(Entity, buff, vars::visuals::crates::military, vars::visuals::crates::show_distance, vars::visuals::crates::draw_distance, xorstr("crate_normal.prefab"), xorstr(L"Military Crate"), D2D1::ColorF::ForestGreen);
			miscvis(Entity, buff, vars::visuals::crates::supply, vars::visuals::crates::show_distance, vars::visuals::crates::draw_distance, xorstr("supply_drop.prefab"), xorstr(L"Airdrop"), D2D1::ColorF::DarkCyan);
			miscvis(Entity, buff, vars::visuals::crates::heli, vars::visuals::crates::show_distance, vars::visuals::crates::draw_distance, xorstr("heli_crate.prefab"), xorstr(L"Heli Crate"), D2D1::ColorF::DarkGreen);
			miscvis(Entity, buff, vars::visuals::crates::bradley, vars::visuals::crates::show_distance, vars::visuals::crates::draw_distance, xorstr("bradley_crate.prefab"), xorstr(L"Bradley Crate"), D2D1::ColorF::GreenYellow);
			// ---------------------------------------------------------
			miscvis(Entity, buff, vars::visuals::vehicles::minicopter, vars::visuals::vehicles::show_distance, vars::visuals::vehicles::draw_distance, xorstr("minicopter.entity.prefab"), xorstr(L"Minicopter"), D2D1::ColorF::Blue);
			miscvis(Entity, buff, vars::visuals::vehicles::scrapheli, vars::visuals::vehicles::show_distance, vars::visuals::vehicles::draw_distance, xorstr("scraptransporthelicopter.prefab"), xorstr(L"Scrap Heli"), D2D1::ColorF::DarkBlue);
			miscvis(Entity, buff, vars::visuals::vehicles::boat, vars::visuals::vehicles::show_distance, vars::visuals::vehicles::draw_distance, xorstr("rowboat.prefab"), xorstr(L"Boat"), D2D1::ColorF::LightBlue);
			miscvis(Entity, buff, vars::visuals::vehicles::rhib, vars::visuals::vehicles::show_distance, vars::visuals::vehicles::draw_distance, xorstr("rhib.prefab"), xorstr(L"RHIB"), D2D1::ColorF::LightCyan);
			// ---------------------------------------------------------
			miscvis(Entity, buff, vars::visuals::turrets::auto_turret, vars::visuals::turrets::show_distance, vars::visuals::turrets::draw_distance, xorstr("autoturret_deployed.prefab"), xorstr(L"Auto Turret"), D2D1::ColorF::Orange);
			miscvis(Entity, buff, vars::visuals::turrets::flame_turret, vars::visuals::turrets::show_distance, vars::visuals::turrets::draw_distance, xorstr("flameturret.deployed.prefab"), xorstr(L"Flame Turret"), D2D1::ColorF::DarkOrange);
			miscvis(Entity, buff, vars::visuals::turrets::shotgun_turret, vars::visuals::turrets::show_distance, vars::visuals::turrets::draw_distance, xorstr("guntrap.deployed.prefab"), xorstr(L"Shotgun Trap"), D2D1::ColorF::DimGray);
			miscvis(Entity, buff, vars::visuals::turrets::landmine, vars::visuals::turrets::show_distance, vars::visuals::turrets::draw_distance, xorstr("landmine.prefab"), xorstr(L"Landmine"), D2D1::ColorF::BlueViolet);
			miscvis(Entity, buff, vars::visuals::turrets::sam_site, vars::visuals::turrets::show_distance, vars::visuals::turrets::draw_distance, xorstr("sam_site_turret_deployed.prefab"), xorstr(L"SAM Site"), D2D1::ColorF::PowderBlue);
			miscvis(Entity, buff, vars::visuals::turrets::bear_trap, vars::visuals::turrets::show_distance, vars::visuals::turrets::draw_distance, xorstr("beartrap.prefab"), xorstr(L"Beartrap"), D2D1::ColorF::Brown);
			// ---------------------------------------------------------
			miscvis(Entity, buff, vars::visuals::other::hemp, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, xorstr("hemp-collectable.prefab"), xorstr(L"Hemp"), D2D1::ColorF::LimeGreen);
			miscvis(Entity, buff, vars::visuals::other::bodybag, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, xorstr("item_drop_backpack.prefab"), xorstr(L"Bodybag"), D2D1::ColorF::Purple);
			// ---------------------------------------------------------
			miscvis(Entity, buff, vars::visuals::base::boxes, vars::visuals::base::show_distance, vars::visuals::base::draw_distance, xorstr("box.wooden.large.prefab"), xorstr(L"Box"), D2D1::ColorF::RosyBrown);
			// ---------------------------------------------------------
			miscvis(Entity, buff, vars::visuals::animals::bear, vars::visuals::animals::show_distance, vars::visuals::animals::draw_distance, xorstr("bear.prefab"), xorstr(L"Bear"), D2D1::ColorF::SaddleBrown);
			miscvis(Entity, buff, vars::visuals::animals::wolf, vars::visuals::animals::show_distance, vars::visuals::animals::draw_distance, xorstr("wolf.prefab"), xorstr(L"Wolf"), D2D1::ColorF::LightSlateGray);
			miscvis(Entity, buff, vars::visuals::animals::pig, vars::visuals::animals::show_distance, vars::visuals::animals::draw_distance, xorstr("boar.prefab"), xorstr(L"Pig"), D2D1::ColorF::DarkRed);
			miscvis(Entity, buff, vars::visuals::animals::chicken, vars::visuals::animals::show_distance, vars::visuals::animals::draw_distance, xorstr("chicken.prefab"), xorstr(L"Chicken"), D2D1::ColorF::YellowGreen);
			miscvis(Entity, buff, vars::visuals::animals::deer, vars::visuals::animals::show_distance, vars::visuals::animals::draw_distance, xorstr("horse.prefab"), xorstr(L"Horse"), D2D1::ColorF::SandyBrown);
			// ---------------------------------------------------------
			miscvis(Entity, buff, true, false, 2000.f, vars::stuff::testChar, xorstr(L"TESTITEM"), D2D1::ColorF::LimeGreen);
		}
		BasePlayer* TargetPlayer = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer);

		/*targeting shit*/
		if (TargetPlayer) {
			if (TargetPlayer->get_bone_pos(head).x == 0 || TargetPlayer->get_bone_pos(head).y == 0 || TargetPlayer->get_bone_pos(head).z == 0) {
				vars::stor::closestPlayer = NULL;
			}
			if (TargetPlayer->IsNpc( ) && vars::combat::ignore_npc) {
				vars::stor::closestPlayer = NULL;
			}
			if (TargetPlayer->HasFlags(PlayerFlags::Sleeping) && vars::combat::ignore_sleepers) {
				vars::stor::closestPlayer = NULL;
			}
			if (LocalPlayer::Entity( )->is_teammate(TargetPlayer->userID( )) && vars::combat::ignore_team) {
				vars::stor::closestPlayer = NULL;
			}
			if (TargetPlayer->health( ) <= 0) {
				vars::combat::lock_target = false;
				vars::stor::closestPlayer = NULL;
			}
			if (vars::combat::ignore_invisible && !TargetPlayer->is_visible( )) {
				vars::stor::closestPlayer = NULL;
			}
		}
		if (vars::stor::closestPlayer == NULL) {
			vars::combat::lock_target = false;
		}
		if (vars::stor::closestHeli != NULL) {
			if (reinterpret_cast<BaseEntity*>(vars::stor::closestHeli)->IsDestroyed( )) {
				vars::stor::closestHeli = NULL;
			}
			if (!reinterpret_cast<Component*>(vars::stor::closestHeli)->gameObject( )) {
				vars::stor::closestHeli = NULL;
			}
		}
		if (vars::combat::ignore_players) {
			vars::stor::closestPlayer = NULL;
		}
		if (vars::combat::ignore_heli) {
			vars::stor::closestHeli = NULL;
		}
		if (LocalPlayer::Entity( ) != nullptr) {
			if (vars::combat::aimbot && !LocalPlayer::Entity( )->is_teammate(reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->userID( ))) {
				if (vars::stor::closestPlayer && !LocalPlayer::Entity( )->IsMenu( )) {
					if (GetAsyncKeyState(vars::keys::aimkey)) {
						do_aimbot(reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer));
					}
				}
			}
		}
	}
}