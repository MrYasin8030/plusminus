﻿bool matrix_found = false;
using namespace otherEsp;
bool inited = false;
void ent_loop() {
	uintptr_t bn = read(vars::stor::gBase + CO::BaseNetworkable, uintptr_t);
	if (!bn) {
		Renderer::String(Vector2(100, 55), xorstr(L"if you're reading this, you're on the wrong game version"), D2D1::ColorF(1.f, 1.f, 1.f, 1.f), true, false);
		return;
	}
	if (!LocalPlayer::Entity()) return;

	if (vars::visuals::radar_) {
		radar::radar_bg();
	}
	if (vars::misc::flyhack_indicator) {
		Renderer::FillRectangle(Vector2(vars::stuff::ScreenWidth / 2 - 214, 200), Vector2(428 * (vars::stuff::flyhack / vars::stuff::max_flyhack), 10), D2D1::ColorF::Lime);
		Renderer::Rectangle(Vector2(vars::stuff::ScreenWidth / 2 - 214, 200), Vector2(428, 10), D2D1::ColorF::Black);

		Renderer::FillRectangle(Vector2(vars::stuff::ScreenWidth / 2 - 214, 230), Vector2(428 * (vars::stuff::hor_flyhack / vars::stuff::max_hor_flyhack), 10), D2D1::ColorF::Lime);
		Renderer::Rectangle(Vector2(vars::stuff::ScreenWidth / 2 - 214, 230), Vector2(428, 10), D2D1::ColorF::Black);
	}
	LogSystem::Render();
	if (vars::visuals::raid_esp) {
		LogSystem::RenderExplosions();
	}
	float FOV = vars::combat::fov, CurFOV;
	if (!pViewMatrix || !matrix_found) {
		pViewMatrix = il2cpp::unity::getViewMatrix();
		matrix_found = true;
		return;
	}
	if (!inited) {
		LogSystem::Log(c_wxor(L"Cheat loaded successfully!"), 7.5f);
		inited = true;
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
			
			if (vars::visuals::radar_) {
				radar::radar_logic(ObjectClass, Object, buff);
			}
			if (reinterpret_cast<BaseCombatEntity*>(Entity)->IsPlayer()) {
				BasePlayer* Player = (BasePlayer*)Entity;

				if (vars::players::skeleton && !Player->IsNpc()) {
					if (!Player->HasFlags(16)) {
						if (LocalPlayer::Entity()->is_teammate(Player->GetSteamID())) {
							Skeleton(Player, D2D1::ColorF::Lime);
						}
						else {
							if (Player->GetHealth() <= 0) {
								Skeleton(Player, D2D1::ColorF::Red);
							}
							else {
								Skeleton(Player, D2D1::ColorF::White);
							}
						}
					}
					else if (Player->HasFlags(16) && !vars::players::sleeperignore) {
						Skeleton(Player, D2D1::ColorF::Orange);
					}
				}
				else if (vars::npc::skeleton && Player->IsNpc()) {
					Skeleton(Player, D2D1::ColorF::Yellow);
				}
				if (!Player->IsNpc()) {
					if (!Player->HasFlags(16)) {
						if (LocalPlayer::Entity()->is_teammate(Player->GetSteamID())) {
							ESP(Player, LocalPlayer::Entity(), D2D1::ColorF::Lime);
						}
						else {
							if (Player->GetHealth() <= 0) {
								ESP(Player, LocalPlayer::Entity(), D2D1::ColorF::Red);
							}
							else {
								ESP(Player, LocalPlayer::Entity(), D2D1::ColorF::White);
							}
						}
					}
					else {
						ESP(Player, LocalPlayer::Entity(), D2D1::ColorF::Orange);
					}
				}
				else if (Player->IsNpc()) {
					if (Player->GetHealth() > 0 && Player->GetHealth() != 100.0f) {
						NPCESP(Player, LocalPlayer::Entity(), D2D1::ColorF::Yellow);
					}
				}
				if (vars::combat::ignore_sleepers && Player->HasFlags(16)) continue;
				if (vars::combat::ignore_npc && Player->IsNpc()) continue;
				if (vars::combat::ignore_team && LocalPlayer::Entity()->is_teammate(Player->GetSteamID())) continue;
				if (Player->get_bone_pos(head).x == 0 || Player->get_bone_pos(head).y == 0 || Player->get_bone_pos(head).z == 0) continue;
				if (vars::combat::ignore_players) continue;
				if (Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), Player->get_bone_pos(head)) > vars::combat::range) continue;
				if (Player->IsNpc() && Player->GetHealth() == 100.0f) continue;

				if (FOV > (CurFOV = GetFov(Player, BoneList(vars::stuff::BoneToAim))) && Player->GetHealth() > 0 && !vars::combat::lock_target) {
					FOV = CurFOV; vars::stor::closestPlayer = (uintptr_t)Player;
				}
			}
			if (reinterpret_cast<BaseCombatEntity*>(Entity)->ClassNameHash() == STATIC_CRC32("BaseHelicopter")) {
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				float health = read(Entity + oHealth, float);
				float maxhealth = 10000.f;
				Vector3 pos = utils::GetEntityPosition(gameObject);
				Vector2 screenPos;
				if (utils::w2s(pos, screenPos)) {
					wchar_t buffer[256];
					wchar_t buffera[256];
					if (vars::visuals::patrol_heli) {
						swprintf(buffer, xorstr(L"Helicopter [%dm]"), (int)Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), pos));
						swprintf(buffera, xorstr(L"[%dHP]"), (int)health);
						Renderer::String(screenPos, buffer, D2D1::ColorF(0.5f, 0.54f, 1.f), true, true);
						Renderer::String(screenPos + Vector2(0, 15), buffera, D2D1::ColorF(0.5f, 0.54f, 1.f), true, true);
						Renderer::FillRectangle(Vector2{ screenPos - Vector2(30, 0) + Vector2(0, 25) }, Vector2{ 60 * (health / maxhealth), 6 }, D2D1::ColorF(0.f, 255.f, 0.f, 0.8f));
						Renderer::Rectangle(Vector2{ screenPos - Vector2(30, 0) + Vector2(0, 25) }, Vector2{ 60, 6 }, D2D1::ColorF::Black, 0.5f);
					}
					if (health > 0 && !vars::combat::ignore_heli) {
						if ((FOV > (CurFOV = GetFovHeli(pos)))) {
							FOV = CurFOV; vars::stor::closestHeli = Entity; vars::stor::closestHeliObj = gameObject;
						}
					}
				}
			}
			otherEsp::bradley(ObjectClass, Object, buff);
			otherEsp::corpse(ObjectClass, Object, buff);
			otherEsp::tc(ObjectClass, Object, buff);
			otherEsp::sleepingbag(ObjectClass, Object, buff);
			otherEsp::bed(ObjectClass, Object, buff);
			otherEsp::stash(ObjectClass, Object, buff);
			otherEsp::hackablecrate(ObjectClass, Object, buff);
			otherEsp::world(ObjectClass, Object, buff);
			if (vars::ores::show_collectables) {
				miscvis(ObjectClass, buff, vars::ores::stone, vars::ores::show_distance, vars::ores::draw_distance, xorstr("stone-collectable.prefab"), xorstr(L"Stone Collectable"), D2D1::ColorF::Gray);
				miscvis(ObjectClass, buff, vars::ores::sulfur, vars::ores::show_distance, vars::ores::draw_distance, xorstr("sulfur-collectable.prefab"), xorstr(L"Sulfur Collectable"), D2D1::ColorF::Gold);
				miscvis(ObjectClass, buff, vars::ores::metal, vars::ores::show_distance, vars::ores::draw_distance, xorstr("metal-collectable.prefab"), xorstr(L"Metal Collectable"), D2D1::ColorF::SaddleBrown);
			}
			// ---------------------------------------------------------
			miscvis(ObjectClass, buff, vars::ores::stone, vars::ores::show_distance, vars::ores::draw_distance, xorstr("stone-ore.prefab"), xorstr(L"Stone Ore"), D2D1::ColorF::Gray);
			miscvis(ObjectClass, buff, vars::ores::sulfur, vars::ores::show_distance, vars::ores::draw_distance, xorstr("sulfur-ore.prefab"), xorstr(L"Sulfur Ore"), D2D1::ColorF::Gold);
			miscvis(ObjectClass, buff, vars::ores::metal, vars::ores::show_distance, vars::ores::draw_distance, xorstr("metal-ore.prefab"), xorstr(L"Metal Ore"), D2D1::ColorF::SaddleBrown);
			// ---------------------------------------------------------
			miscvis(ObjectClass, buff, vars::visuals::crates::elite, vars::visuals::crates::show_distance, vars::visuals::crates::draw_distance, xorstr("crate_elite.prefab"), xorstr(L"Elite Crate"), D2D1::ColorF::SeaGreen);
			miscvis(ObjectClass, buff, vars::visuals::crates::military, vars::visuals::crates::show_distance, vars::visuals::crates::draw_distance, xorstr("crate_normal.prefab"), xorstr(L"Military Crate"), D2D1::ColorF::ForestGreen);
			miscvis(ObjectClass, buff, vars::visuals::crates::supply, vars::visuals::crates::show_distance, vars::visuals::crates::draw_distance, xorstr("supply_drop.prefab"), xorstr(L"Airdrop"), D2D1::ColorF::DarkCyan);
			miscvis(ObjectClass, buff, vars::visuals::crates::heli, vars::visuals::crates::show_distance, vars::visuals::crates::draw_distance, xorstr("heli_crate.prefab"), xorstr(L"Heli Crate"), D2D1::ColorF::DarkGreen);
			miscvis(ObjectClass, buff, vars::visuals::crates::bradley, vars::visuals::crates::show_distance, vars::visuals::crates::draw_distance, xorstr("bradley_crate.prefab"), xorstr(L"Bradley Crate"), D2D1::ColorF::GreenYellow);
			// ---------------------------------------------------------
			miscvis(ObjectClass, buff, vars::visuals::vehicles::minicopter, vars::visuals::vehicles::show_distance, vars::visuals::vehicles::draw_distance, xorstr("minicopter.entity.prefab"), xorstr(L"Minicopter"), D2D1::ColorF::Blue);
			miscvis(ObjectClass, buff, vars::visuals::vehicles::scrapheli, vars::visuals::vehicles::show_distance, vars::visuals::vehicles::draw_distance, xorstr("scraptransporthelicopter.prefab"), xorstr(L"Scrap Heli"), D2D1::ColorF::DarkBlue);
			miscvis(ObjectClass, buff, vars::visuals::vehicles::boat, vars::visuals::vehicles::show_distance, vars::visuals::vehicles::draw_distance, xorstr("rowboat.prefab"), xorstr(L"Boat"), D2D1::ColorF::LightBlue);
			miscvis(ObjectClass, buff, vars::visuals::vehicles::rhib, vars::visuals::vehicles::show_distance, vars::visuals::vehicles::draw_distance, xorstr("rhib.prefab"), xorstr(L"RHIB"), D2D1::ColorF::LightCyan);
			// ---------------------------------------------------------
			miscvis(ObjectClass, buff, vars::visuals::turrets::auto_turret, vars::visuals::turrets::show_distance, vars::visuals::turrets::draw_distance, xorstr("autoturret_deployed.prefab"), xorstr(L"Auto Turret"), D2D1::ColorF::Orange);
			miscvis(ObjectClass, buff, vars::visuals::turrets::flame_turret, vars::visuals::turrets::show_distance, vars::visuals::turrets::draw_distance, xorstr("flameturret.deployed.prefab"), xorstr(L"Flame Turret"), D2D1::ColorF::DarkOrange);
			miscvis(ObjectClass, buff, vars::visuals::turrets::shotgun_turret, vars::visuals::turrets::show_distance, vars::visuals::turrets::draw_distance, xorstr("guntrap.deployed.prefab"), xorstr(L"Shotgun Trap"), D2D1::ColorF::DimGray);
			miscvis(ObjectClass, buff, vars::visuals::turrets::landmine, vars::visuals::turrets::show_distance, vars::visuals::turrets::draw_distance, xorstr("landmine.prefab"), xorstr(L"Landmine"), D2D1::ColorF::BlueViolet);
			miscvis(ObjectClass, buff, vars::visuals::turrets::bear_trap, vars::visuals::turrets::show_distance, vars::visuals::turrets::draw_distance, xorstr("beartrap.prefab"), xorstr(L"Beartrap"), D2D1::ColorF::Brown);
			// ---------------------------------------------------------
			miscvis(ObjectClass, buff, vars::visuals::other::hemp, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, xorstr("hemp-collectable.prefab"), xorstr(L"Hemp"), D2D1::ColorF::LimeGreen);
			// ---------------------------------------------------------
			miscvis(ObjectClass, buff, vars::visuals::base::boxes, vars::visuals::base::show_distance, vars::visuals::base::draw_distance, xorstr("box.wooden.large.prefab"), xorstr(L"Box"), D2D1::ColorF::RosyBrown);
			// ---------------------------------------------------------
			miscvis(ObjectClass, buff, vars::visuals::animals::bear, vars::visuals::animals::show_distance, vars::visuals::animals::draw_distance, xorstr("bear.prefab"), xorstr(L"Bear"), D2D1::ColorF::SaddleBrown);
			miscvis(ObjectClass, buff, vars::visuals::animals::wolf, vars::visuals::animals::show_distance, vars::visuals::animals::draw_distance, xorstr("wolf.prefab"), xorstr(L"Wolf"), D2D1::ColorF::LightSlateGray);
			miscvis(ObjectClass, buff, vars::visuals::animals::pig, vars::visuals::animals::show_distance, vars::visuals::animals::draw_distance, xorstr("boar.prefab"), xorstr(L"Pig"), D2D1::ColorF::DarkRed);
			miscvis(ObjectClass, buff, vars::visuals::animals::chicken, vars::visuals::animals::show_distance, vars::visuals::animals::draw_distance, xorstr("chicken.prefab"), xorstr(L"Chicken"), D2D1::ColorF::YellowGreen);
			miscvis(ObjectClass, buff, vars::visuals::animals::deer, vars::visuals::animals::show_distance, vars::visuals::animals::draw_distance, xorstr("horse.prefab"), xorstr(L"Horse"), D2D1::ColorF::SandyBrown);
			// ---------------------------------------------------------
			miscvis(ObjectClass, buff, true, false, 2000.f, vars::stuff::testChar, xorstr(L"TESTITEM"), D2D1::ColorF::LimeGreen);
		}
		static DWORD64 GOM = 0;
		if (!GOM) GOM = RVA(FindPattern((PBYTE)"\x48\x8B\x15\x00\x00\x00\x00\x66\x39", xorstr("xxx????xx"), xorstr(L"UnityPlayer.dll")), 7);
		DWORD64 manager = read(GOM, DWORD64); if (!manager) return;
		for (DWORD64 object = read(manager + 0x8, DWORD64); (object && (object != read(manager, DWORD64))); object = read(object + 0x8, DWORD64)) {
			DWORD64 gameobject = read(object + 0x10, DWORD64);
			WORD tag = read(gameobject + 0x54, WORD);
			if (tag == 6 || tag == 5 || tag == 20011) {
				DWORD64 objectclass = read(gameobject + 0x30, DWORD64);
				DWORD64	entity = read(objectclass + 0x18, DWORD64);
				if (tag == 20011) {
					DWORD64 dome = read(entity + 0x28, DWORD64); // TOD_Sky
					DWORD64 todCycle = read(dome + 0x38, DWORD64);
					DWORD64 todDay = read(dome + 0x50, DWORD64);

					if (vars::misc::custom_time) {
						write(todCycle + 0x10, vars::misc::time, float);
					}
					if (vars::misc::bright_ambient) {
						write(todDay + 0x50, 1.f, float);
						write(todDay + 0x54, 1.f, float);
					}
				}
			}
		}
		auto* TargetPlayer = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer);
		/*targeting shit*/
		if (TargetPlayer->get_bone_pos(head).x == 0 || TargetPlayer->get_bone_pos(head).y == 0 || TargetPlayer->get_bone_pos(head).z == 0) {
			vars::stor::closestPlayer = NULL;
		}
		if (TargetPlayer->IsNpc() && vars::combat::ignore_npc) {
			vars::stor::closestPlayer = NULL;
		}
		if (TargetPlayer->IsNpc() && TargetPlayer->GetHealth() == 100.0f) {
			vars::stor::closestPlayer = NULL;
		}
		if (TargetPlayer->HasFlags(16) && vars::combat::ignore_sleepers) {
			vars::stor::closestPlayer = NULL;
		}
		if (LocalPlayer::Entity()->is_teammate(TargetPlayer->GetSteamID()) && vars::combat::ignore_team) {
			vars::stor::closestPlayer = NULL;
		}
		if (TargetPlayer->GetHealth() <= 0) {
			vars::combat::lock_target = false;
			vars::stor::closestPlayer = NULL;
		}
		if ((int)ceil(read(vars::stor::closestHeli + 0x20C, float)) <= 0) {
			vars::stor::closestHeli = NULL;
			vars::stor::closestHeliObj = NULL;
		}
		if (vars::combat::ignore_players) {
			vars::stor::closestPlayer = NULL;
		}
		if (vars::combat::ignore_heli) {
			vars::stor::closestHeli = NULL;
			vars::stor::closestHeliObj = NULL;
		}
		if (utils::GetEntityPosition(vars::stor::closestHeliObj).y > 1500 || utils::GetEntityPosition(vars::stor::closestHeliObj).y < -1500) {
			vars::stor::closestHeli = NULL;
			vars::stor::closestHeliObj = NULL;
		}
		if (LocalPlayer::Entity() != nullptr) {
			if (vars::combat::aimbot && !LocalPlayer::Entity()->is_teammate(reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->GetSteamID())) {
				if (vars::stor::closestPlayer && !LocalPlayer::Entity()->IsMenu()) {
					if (GetAsyncKeyState(vars::keys::aimkey)) {
						do_aimbot(reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer));
					}
				}
			}
		}
	}
}