void game_thread_loop() {
	DWORD64 BaseNetworkable;
	BaseNetworkable = read(vars::stor::gBase + CO::BaseNetworkable, DWORD64);

	if (!LocalPlayer::Entity()) return;

	if (vars::players::chams) {
		lol::update_chams();
	}
	auto entityList = BaseNetworkable::clientEntities()->entityList();
	if (entityList) {
		for (int i = 1; i < entityList->vals->size; i++) {
			uintptr_t Entity = *reinterpret_cast<uintptr_t*>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
			if (!Entity) continue;
			uintptr_t Object = *reinterpret_cast<uint64_t*>(Entity + 0x10);
			if (!Object) continue;
			uintptr_t ObjectClass = *reinterpret_cast<uint64_t*>(Object + 0x30);
			if (!ObjectClass) continue;

			pUncStr name = read(ObjectClass + 0x60, pUncStr); if (!name) continue;
			char* buff = name->stub;

			if (!reinterpret_cast<BaseEntity*>(Entity)->IsValid()) { continue; }
			if (!reinterpret_cast<Component*>(Entity)->gameObject()) { continue; }
			Item* weapon = LocalPlayer::Entity()->GetActiveWeapon();
			DWORD64 active = weapon->entity( );
			char* classname = weapon->ClassName();

			bool weaponmelee = weapon && classname && (strcmp(classname, xorstr("BaseMelee")) || strcmp(classname, xorstr("Jackhammer")));
			if (strstr(buff, xorstr("player.prefab"))) {
				BasePlayer* lol = (BasePlayer*)Entity;

				if (vars::players::chams && lol) {
					if (lol->health() > 0) {
						uintptr_t playermodel = read(Entity + 0x4A8, uintptr_t);
						uintptr_t multimesh = read(playermodel + 0x280, uintptr_t);
						if (!lol->HasFlags(PlayerFlags::Sleeping)) {
							if (LocalPlayer::Entity()->is_teammate(lol->GetSteamID())) {
								lol::chams(multimesh, Color(0, 1, 0, 1));
							}
							else {
								lol::chams(multimesh, Color(vars::colors::chams.x, vars::colors::chams.y, vars::colors::chams.z, 1));
							}
						}
						else if (lol->HasFlags(PlayerFlags::Sleeping) && !vars::players::sleeperignore) {
							lol::chams(multimesh, Color(1, 0.5, 0, 1));
						}
					}
				}
				if (vars::misc::auto_revive || vars::misc::insta_revive) {
					UINT64 gameObject = read(ObjectClass + 0x30, UINT64);
					Vector3 local = utils::ClosestPoint(LocalPlayer::Entity(), utils::GetEntityPosition(gameObject));
					if (vars::misc::auto_revive && (BasePlayer*)Entity && lol->HasFlags(PlayerFlags::Wounded) && Math::Distance_3D(local, utils::GetEntityPosition(gameObject)) < 3.f) {
						lol::pickup_player((BasePlayer*)Entity);
					}
					if (vars::misc::insta_revive && (BasePlayer*)Entity && lol->HasFlags(PlayerFlags::Wounded) && Math::Distance_3D(local, utils::GetEntityPosition(gameObject)) < 3.f && LocalPlayer::Entity()->GetKeyState(Button::USE)
						&& read(LocalPlayer::Entity() + 0x4E8, uintptr_t) == Entity) {
						lol::pickup_player((BasePlayer*)Entity);
					}
				}
				if (vars::combat::silent_melee && weaponmelee && Math::Distance_3D(lol->get_bone_pos(head), LocalPlayer::Entity()->get_bone_pos(head)) <= 3.5f) {
					f_object target = f_object::get_melee_target((BasePlayer*)Entity, active);
					lol::do_attack(target, active, true);
				}
			}
			/*else if (vars::misc::auto_grade && strstr((char*)read(read(Entity, DWORD64) + 0x10, DWORD64), xorstr("BuildingBlock"))
				&& !strstr((char*)read(read(Entity, DWORD64) + 0x10, DWORD64), xorstr("Door"))
				&& !strstr((char*)read(read(Entity, DWORD64) + 0x10, DWORD64), xorstr("Deployable"))
				&& !strstr(buff, xorstr("furnace"))
				&& !strstr(buff, xorstr("bars"))
				&& !strstr(buff, xorstr("gates.external.high.stone.prefab"))
				&& !strstr(buff, xorstr("gates.external.high.wood.prefab"))
				&& !strstr(buff, xorstr("wall.external.high.stone.prefab"))
				&& !strstr(buff, xorstr("wall.external.high.wood.prefab"))
				) {
				UINT64 gameObject = read(ObjectClass + 0x30, UINT64);
				Vector3 local = utils::ClosestPoint(LocalPlayer::Entity(), utils::GetEntityPosition(gameObject));
				if (Math::Distance_3D(local, utils::GetEntityPosition(gameObject)) >= 3.f) { continue; }
				if (vars::misc::auto_grade) {
					lol::AutoGrade(Entity);
				}
			}*/
			else if (vars::misc::auto_pickup && strstr(buff, xorstr("/collectable/"))) {
				UINT64 gameObject = read(ObjectClass + 0x30, UINT64);
				Vector3 local = utils::ClosestPoint(LocalPlayer::Entity(), utils::GetEntityPosition(gameObject));
				if (Math::Distance_3D(local, utils::GetEntityPosition(gameObject)) < 3.f) {
					utils::ServerRPC(Entity, Str(xorstr(L"Pickup")));
				}
			}
			else if (vars::misc::annoyer && strstr((char*)read(read(Entity, DWORD64) + 0x10, DWORD64), xorstr("Door"))) {
				UINT64 gameObject = read(ObjectClass + 0x30, UINT64);
				Vector3 local = utils::ClosestPoint(LocalPlayer::Entity(), utils::GetEntityPosition(gameObject));
				if (Entity && Math::Distance_3D(local, utils::GetEntityPosition(gameObject)) < 3.f) {
					lol::knocker(Entity);
				}
			}
		}
	}
}