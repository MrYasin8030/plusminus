﻿#include "hooks/defs.h"
int yeet = 0;
int flick = 0;
int yaw = 100;
bool waslagging = false;
using namespace hk_defs;
namespace hk {
	namespace exploit {
	}
	namespace misc {
		void Jump(uintptr_t playerwalkmovement, uintptr_t modelstate, bool indirection) {
			if (vars::misc::better_jump) {
				bool climbing = read(playerwalkmovement + 0x132, bool);
				bool sliding = read(playerwalkmovement + 0x134, bool);
				bool st = (climbing = (sliding = false));

				typedef bool(__stdcall* get_jumped)(uintptr_t);
				bool get_jmpd = ((get_jumped)(vars::stor::gBase + CO::get_jumped))(modelstate);
				bool st2 = (get_jmpd = true);

				write(playerwalkmovement + 0x130, st, bool);
				write(playerwalkmovement + 0x138, st2, bool);
				write(playerwalkmovement + 0xC0, Time::time(), float);
				write(playerwalkmovement + 0xA8, null, uintptr_t);

				typedef void(__stdcall* set_velocity)(uintptr_t, Vector3);
				typedef Vector3(__stdcall* get_velocity)(uintptr_t);
				uintptr_t rigid = read(playerwalkmovement + 0x90, uintptr_t);
				//this.body.velocity += Vector3.Lerp(this.Owner.eyes.BodyForward() * 9f, Vector3.zero, this.modify.drag);
				Vector3 targetVel = ((get_velocity)(vars::stor::gBase + CO::get_velocity))(rigid);

				((set_velocity)(vars::stor::gBase + CO::set_velocity))(rigid, Vector3(targetVel.x, 10, targetVel.z));
			}
			else { return original_jump(playerwalkmovement, modelstate, indirection); }
		}
		void OnLand(BasePlayer* ply, float fVel) {
			if (!vars::misc::no_fall)
				return original_onland(ply, fVel);
		}
		void ForcePositionTo(BasePlayer* pl, Vector3 pos) {
			if (!GetAsyncKeyState(vars::keys::forcepos))
				return original_forcepos(pl, pos);
		}
		void VisUpdateUsingCulling(BasePlayer* pl, float dist, bool vis) {
			if (vars::players::chams && vars::players::chams_xqz) {
				return original_UnregisterFromVisibility(pl, 2.f, true);
			}
			else {
				return original_UnregisterFromVisibility(pl, dist, vis);
			}
		}
		std::string C4 = c_xor("C4");
		std::string Satchel = c_xor("Satchel");
		std::string IncenRocket = c_xor("Incendiary Rocket");
		std::string Rocket = c_xor("Rocket");

		std::wstring wC4 = xorstr(L"C4");
		std::wstring wSatchel = xorstr(L"Satchel");
		std::wstring wIncenRocket = xorstr(L"Incendiary Rocket");
		std::wstring wRocket = xorstr(L"Rocket");
		uintptr_t CreateEffect(pUncStr strPrefab, uintptr_t effect) {
			auto effectName = strPrefab->str;
			auto position = read(effect + 0x5C, Vector3);
			if (vars::visuals::raid_esp && effect && strPrefab->str) {
				switch (RUNTIME_CRC32_W(effectName)) {
				case STATIC_CRC32("assets/prefabs/tools/c4/effects/c4_explosion.prefab"):
					LogSystem::LogExplosion(C4, position);
					LogSystem::Log(StringFormat::format(c_wxor(L"%ls explosion %.2f meters away from you."), wC4.c_str(), Math::Calc3D_Dist(LocalPlayer->get_bone_pos(head), position)), 15.f);
					break;
				case STATIC_CRC32("assets/prefabs/weapons/satchelcharge/effects/satchel-charge-explosion.prefab"):
					LogSystem::LogExplosion(Satchel, position);
					LogSystem::Log(StringFormat::format(c_wxor(L"%ls explosion %.2f meters away from you."), wSatchel.c_str(), Math::Calc3D_Dist(LocalPlayer->get_bone_pos(head), position)), 15.f);
					break;
				case STATIC_CRC32("assets/prefabs/weapons/rocketlauncher/effects/rocket_explosion_incendiary.prefab"):
					LogSystem::LogExplosion(IncenRocket, position);
					LogSystem::Log(StringFormat::format(c_wxor(L"%ls explosion %.2f meters away from you."), wIncenRocket.c_str(), Math::Calc3D_Dist(LocalPlayer->get_bone_pos(head), position)), 15.f);
					break;
				case STATIC_CRC32("assets/prefabs/weapons/rocketlauncher/effects/rocket_explosion.prefab"):
					LogSystem::LogExplosion(Rocket, position);
					LogSystem::Log(StringFormat::format(c_wxor(L"%ls explosion %.2f meters away from you."), wRocket.c_str(), Math::Calc3D_Dist(LocalPlayer->get_bone_pos(head), position)), 15.f);
					break;
				}
			}
			return original_createeffect(strPrefab, effect);
		}
		void HandleRunning(void* a1, void* a2, bool wantsRun) {
			if (vars::misc::omnidirectional_sprinting) wantsRun = true;
			return original_handleRunning(a1, a2, wantsRun);
		}
		void HandleJumping(void* a1, void* a2, bool wantsJump, bool jumpInDirection = false) { // recreated
			if (vars::misc::inf_jump) {
				if (!wantsJump) {
					return;
				}
				reinterpret_cast<void(_fastcall*)(void*, void*, bool)>(vars::stor::gBase + CO::Jump)(a1, a2, jumpInDirection);
			}
			else {
				return original_handleJumping(a1, a2, wantsJump, jumpInDirection);
			}
		}
		Vector3 get_position(DWORD64 playereyes) {
			if (vars::misc::long_neck) {
				if (GetAsyncKeyState(vars::keys::longneck)) {
					return Vector3(LocalPlayer->get_bone_pos(head)) + Vector3(0, 1.15, 0);
				}
			}
			return original_geteyepos(playereyes);
		}
		void __fastcall SetFlying(void* a1, bool a2) {}
		void SendClientTick(BasePlayer* baseplayer) {
			if (!LocalPlayer) return original_sendclienttick(baseplayer);
			if (vars::misc::anti_aim) {
				auto input = read(baseplayer + 0x4C8, uintptr_t);
				auto state = read(input + 0x20, uintptr_t);
				auto current = read(state + 0x10, uintptr_t); if (!current) { return original_sendclienttick(baseplayer); }
				yeet += vars::misc::anti_aim_speed;
				if (yeet >= 999) { // reset number
					yeet = 0;
				}
				if (vars::misc::anti_aim_yaw == 0) {
					yaw = 100;
				}
				if (vars::misc::anti_aim_yaw == 1) {
					yaw = -100;
				}
				write(current + 0x18, Vector3(yaw, yeet, 0), Vector3);
			}
			return original_sendclienttick(baseplayer);
		}
		void DoFixedUpdate(uintptr_t movement, uintptr_t modelstate) {
			float speed = reinterpret_cast<float(*)(BasePlayer*)>(vars::stor::gBase + CO::GetMaxSpeed)(LocalPlayer);
			if (vars::misc::farmbot) {
				OreTarget closest = FindOreTarget(LocalPlayer->get_bone_pos(head));
				if (closest.valid) {
					
					Vector3 direction = (closest.position - LocalPlayer->get_bone_pos(head)).Normalized();
					direction = direction * speed;
					typedef void(__stdcall* set_velocity)(uintptr_t, Vector3);
					uintptr_t rigid = read(movement + 0x90, uintptr_t);
					((set_velocity)(vars::stor::gBase + CO::set_velocity))(rigid, Vector3(0, -10, 0));
					write(movement + 0x34, direction, Vector3);
				}
			}
			original_dofixedupdate(movement, modelstate);
		}
		void ClientInput(BasePlayer* baseplayah, DWORD64 ModelState) {
			vars::stuff::anti_aim_ = yeet;
			if (!LocalPlayer) return original_clientinput(baseplayah, ModelState);
			typedef void(__stdcall* set_rayleigh)(float);
			if (vars::misc::rayleigh_changer) {
				((set_rayleigh)(vars::stor::gBase + CO::set_rayleigh))(vars::misc::rayleigh);
			}
			else {
				((set_rayleigh)(vars::stor::gBase + CO::set_rayleigh))(vars::misc::rayleigh);
			}
			if (vars::misc::mass_suicide) {
				reinterpret_cast<void(_fastcall*)(BasePlayer*, float)>(vars::stor::gBase + CO::OnLand)(LocalPlayer, -50);
			}
			if (vars::misc::suicide && GetAsyncKeyState(vars::keys::suicide) && LocalPlayer->GetHealth() > 0 && !LocalPlayer->IsMenu()) {
				reinterpret_cast<void(_fastcall*)(BasePlayer*, float)>(vars::stor::gBase + CO::OnLand)(LocalPlayer, -50);
			}
			auto* TargetPlayer = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer);
			if (vars::combat::psilent_autoshoot && vars::stor::closestPlayer != null && vars::combat::psilent && !LocalPlayer->IsMenu()) {
				Item* weapon = LocalPlayer->GetActiveWeapon();
				DWORD64 basepr = read(weapon + oHeldEntity, DWORD64);
				DWORD64 mag = read(basepr + 0x2A0, DWORD64);
				int contents = read(mag + 0x1C, int);
				if (contents > 0 && utils::LineOfSight(TargetPlayer->get_bone_pos(head), (vars::misc::long_neck && GetAsyncKeyState(vars::keys::longneck)) ? LocalPlayer->get_bone_pos(head) + Vector3(0, 1.15, 0) : LocalPlayer->get_bone_pos(head))) {
					INPUT    Input = { 0 };
					// left down 
					Input.type = INPUT_MOUSE;
					Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
					::SendInput(1, &Input, sizeof(INPUT));

					// left up
					::ZeroMemory(&Input, sizeof(INPUT));
					Input.type = INPUT_MOUSE;
					Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
					::SendInput(1, &Input, sizeof(INPUT));
				}
			}
			EntityThreadLoop();
			if (!waslagging && vars::misc::fake_lag) {
				write(LocalPlayer + 0x5C8, 0.4f, float);
				waslagging = true;
			}
			else if (waslagging && !vars::misc::fake_lag) {
				write(LocalPlayer + 0x5C8, 0.05f, float);
				waslagging = false;
			}
			il2cpp::unity::IgnoreLayerCollision(layer::PlayerMovement, layer::Water, !vars::misc::jesus);
			il2cpp::unity::IgnoreLayerCollision(layer::PlayerMovement, layer::Tree, vars::misc::walker);
			il2cpp::unity::IgnoreLayerCollision(layer::PlayerMovement, layer::AI, vars::misc::walker);
			WeaponPatch();
			MiscFuncs();
			//LocalPlayer->AddFlag(ModelStateFlag::OnGround);
			typedef void(__stdcall* ClientInput)(BasePlayer*, DWORD64);
			((ClientInput)original_clientinput)(baseplayah, ModelState);
			if (vars::misc::spoof_ladderstate) 
				LocalPlayer->add_modelstate_flag(ModelStateFlag::OnLadder);
			if (vars::misc::farmbot) 
				LocalPlayer->add_modelstate_flag(ModelStateFlag::Sprinting);
			//LocalPlayer->AddFlag(ModelStateFlag::OnGround);
		}
		void UpdateAmbient(uintptr_t TOD_Sky) {
			if (!vars::misc::bright_ambient) {
				return original_updateambient(TOD_Sky);
			}
			//static int cases = 0;
			//static float r = 1.00f, g = 0.00f, b = 1.00f;
			//switch (cases) {
			//case 0: { r -= 0.05f; if (r <= 0) cases += 1; break; }
			//case 1: { g += 0.05f; b -= 0.05f; if (g >= 1) cases += 1; break; }
			//case 2: { r += 0.05f; if (r >= 1) cases += 1; break; }                       // RAINBOW
			//case 3: { b += 0.05f; g -= 0.05f; if (b >= 1) cases = 0; break; }
			//default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
			//}
			RenderSettings::set_ambientMode(RenderSettings::AmbientMode::Flat);
			RenderSettings::set_ambientIntensity(6.f);
			RenderSettings::set_ambientLight(Color({ vars::colors::ambient_color.x, vars::colors::ambient_color.y, vars::colors::ambient_color.z, 1 }));
		}
		pUncStr Run(ConsoleOptions* options, pUncStr strCommand, DWORD64 args) {
			if (options->IsFromServer()) {
				std::wstring cmd = std::wstring(strCommand->str);
				if (cmd.find(xorstr(L"noclip")) != std::wstring::npos || cmd.find(xorstr(L"debugcamera")) != std::wstring::npos || cmd.find(xorstr(L"camspeed")) != std::wstring::npos || cmd.find(xorstr(L"admintime")) != std::wstring::npos) {
					strCommand = nullptr;
				}
			}
			return original_consolerun(options, strCommand, args);
		}
		void DoHitNotify(BaseCombatEntity* entity, HitInfo* info) {
			if (entity->IsPlayer()) {
				if (vars::misc::hit_logs) {
					LogSystem::Log(StringFormat::format(c_wxor(L"Hit %s in %s for %.2f damage"), reinterpret_cast<BasePlayer*>(entity)->GetName(), utils::StringPool::Get(info->HitBone())->buffer, info->damageTypes()->Total()), 5.f);
				}
				if (vars::misc::custom_hitsound) {
					PlaySoundA(xorstr("C:\\plusminus\\hit.wav"), NULL, SND_ASYNC);
				}
				else {
					return original_dohitnotify(entity, info);
				}
			}
			else {
				return original_dohitnotify(entity, info);
			}
		}
		bool get_isHeadshot(DWORD64 hitinfo) {
			if (vars::misc::custom_hitsound) { return false; }
			else { return original_getisheadshot(hitinfo); }
		}
		void Play(DWORD64 viewmodel, pUncStr name) {
			if (vars::weapons::remove_attack_anim) {
				static auto ptr = METHOD("Assembly-CSharp::BaseProjectile::DoAttack(): Void");
				if (!CALLED_BY(ptr, 0x800) || LocalPlayer->GetActiveWeapon()->GetID() == -75944661) {
					return original_viewmodelplay(viewmodel, name);
				}
			}
			else {
				return original_viewmodelplay(viewmodel, name);
			}
		}
	}
	namespace combat {
		float GetRandomVelocity(uintptr_t mod) {
			//LogSystem::Log(StringFormat::format(c_wxor(L"%.2f"), original_getrandomvelocity(mod)), 3.5f);
			return vars::weapons::fast_bullets ? original_getrandomvelocity(mod) * 1.3 : original_getrandomvelocity(mod);
		}
		void AddPunch(uintptr_t a1, Vector3 a2, float duration) {
			if (vars::weapons::no_recoil) {
				a2 *= vars::weapons::recoil_control / 100.f;
			}
			return original_addpunch(a1, a2, duration);
		}
		Vector3 MoveTowards(Vector3 a1, Vector3 a2, float maxDelta) {
			static auto ptr = METHOD("Assembly-CSharp::BaseProjectile::SimulateAimcone(): Void");
			if (CALLED_BY(ptr, 0x800)) {
				if (vars::weapons::no_recoil) {
					a2 *= vars::weapons::recoil_control / 100.f;
					maxDelta *= vars::weapons::recoil_control / 100.f;
				}
			}
			return original_movetowards(a1, a2, maxDelta);
		}
		bool DoHit(Projectile* pr, HitTest* test, Vector3 point, Vector3 normal) {
			if (vars::combat::ignore_team) {
				if (LocalPlayer->IsTeamMate(reinterpret_cast<BasePlayer*>(test->HitEntity())->GetSteamID())) {
					if (reinterpret_cast<BaseCombatEntity*>(test->HitEntity())->IsPlayer()) {
						return false;
					}
				}
			}

			return original_dohit(pr, test, point, normal);
		}
		void Launch(Projectile* prdoj) {
			Weapon tar = LocalPlayer->GetActiveWeapon()->Info();
			int ammo = LocalPlayer->GetActiveWeapon()->LoadedAmmo();
			//prdoj->gravityModifier(GetGravity(ammo));
			prdoj->invisible(false);
			//LogSystem::Log(StringFormat::format(c_wxor(L"%.2f %d"), prdoj->gravityModifier(), LocalPlayer->GetActiveWeapon()->LoadedAmmo()), 3.5f);
			if (vars::weapons::no_spread) {
				write(prdoj->mod() + 0x38, 0.f, float);
			}
			return original_launch(prdoj);
		}
		bool CanHoldItems(void* a1, void* a2) {
			if (vars::weapons::minicopter_aim) return true;
			return original_canholditems(a1, a2);
		}
		void SendProjectileAttack(void* a1, void* a2) {
			uintptr_t PlayerAttack = read((uintptr_t)a2 + 0x18, uintptr_t); // PlayerAttack playerAttack;
			uintptr_t Attack = read(PlayerAttack + 0x18, uintptr_t); // public Attack attack;
			uint32_t hitID = read(Attack + 0x2C, uint32_t);
			if (vars::weapons::spoof_hitdistance) {
				write(a2 + 0x2C, vars::weapons::hitdistance, float);
			}
			if (vars::combat::hitbox_override || vars::combat::always_heli_rotor) {
				BaseCombatEntity* entity = BaseNetworkable::clientEntities()->Find<BaseCombatEntity*>(hitID);
				if (vars::combat::hitbox_override) {
					if (entity->ClassNameHash() == STATIC_CRC32("BasePlayer")) {
						uint32_t bone;
						if (rand() % 100 < vars::combat::hs_percentage) { bone = utils::StringPool::Get(c_xor("head")); }
						else { bone = utils::StringPool::Get(c_xor("spine4")); }
						write(Attack + 0x30, bone, uint32_t);
						write(Attack + 0x64, 16144115, uint32_t);
					}
				}
				if (vars::combat::always_heli_rotor) {
					if (entity->ClassNameHash() == STATIC_CRC32("BaseHelicopter")) {
						int health = (int)ceil(read(vars::stor::closestHeli + 0x20C, float));
						if (health <= 5000) {
							write(Attack + 0x30, utils::StringPool::Get(c_xor("tail_rotor_col")), uint32_t);
						}
						else {
							write(Attack + 0x30, utils::StringPool::Get(c_xor("engine_col")), uint32_t);
						}
						write(Attack + 0x2C, read(read(vars::stor::closestHeli + 0x50, uintptr_t) + 0x10, uint32_t), uint32_t);
					}
				}
			}
			return original_sendprojectileattack(a1, a2);
		}
		Projectile* CreateProjectile(uintptr_t BaseProjectileA, void* prefab_pathptr, Vector3 pos, Vector3 forward, Vector3 velocity) {
			Projectile* projectile = original_create_projectile(BaseProjectileA, prefab_pathptr, pos, forward, velocity);
			if (vars::weapons::thick_bullet) {
				projectile->thickness(1.f);
			}
			else {
				projectile->thickness(0.1f);
			}
			return projectile;
		}
		bool CanAttack(void* a1, void* a2) {
			if (vars::misc::can_attack) return true;
			return original_canattack(a1, a2);
		}
		void TraceAll(HitTest* test, uintptr_t traces, uintptr_t layerMask) {
			if (vars::weapons::penetrate) {
				layerMask &= ~LayerMasks::Tree;
				layerMask &= ~LayerMasks::Deployed;
				layerMask &= ~LayerMasks::Water;
			}
			return original_traceall(test, traces, layerMask);
		}
		Vector3 GetModifiedAimConeDirection(float aimCone, Vector3 inputVec, bool anywhereInside = true) {
			auto* TargetPlayer = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer);
			Vector3 heliDir = (HeliPrediction(LocalPlayer->get_bone_pos(head)) - LocalPlayer->get_bone_pos(head)).Normalized(); Vector3 playerDir;

			Vector3 target = TargetPlayer->get_bone_pos(head);
			if (vars::misc::long_neck && GetAsyncKeyState(vars::keys::longneck)) {
				a::Prediction(LocalPlayer->get_bone_pos(head) + Vector3(0, 1.15, 0), target, TargetPlayer->GetVelocity(), GetBulletSpeed(), GetGravity(LocalPlayer->GetActiveWeapon()->LoadedAmmo()));
				playerDir = (target - (LocalPlayer->get_bone_pos(head) + Vector3(0, 1.15, 0))).Normalized();
			}
			else {
				a::Prediction(LocalPlayer->get_bone_pos(head), target, TargetPlayer->GetVelocity(), GetBulletSpeed(), GetGravity(LocalPlayer->GetActiveWeapon()->LoadedAmmo()));
				playerDir = (target - LocalPlayer->get_bone_pos(head)).Normalized();
			}

			if (vars::combat::psilent) {
				if (!vars::combat::psilentonkey) {
					if (vars::combat::psilenttarget == 0 && vars::stor::closestPlayer != NULL) {
						inputVec = playerDir;
					}
					if (vars::combat::psilenttarget == 1 && vars::stor::closestHeli != NULL && vars::stor::closestHeliObj != NULL) {
						inputVec = heliDir;
					}
				}
				else {
					if (GetAsyncKeyState(vars::keys::psilent)) {
						if (vars::combat::psilenttarget == 0 && vars::stor::closestPlayer != NULL) {
							inputVec = playerDir;
						}
						if (vars::combat::psilenttarget == 1 && vars::stor::closestHeli != NULL && vars::stor::closestHeliObj != NULL) {
							inputVec = heliDir;
						}
					}
				}
			}
			if (vars::weapons::no_spread) {
				aimCone = 0.f;
			}
			return original_aimconedirection(aimCone, inputVec, anywhereInside);
		}
	}

}
void hk_(void* Function, void** Original, void* Detour, bool autoEnable = true) {
	if (MH_Initialize() != MH_OK && MH_Initialize() != MH_ERROR_ALREADY_INITIALIZED) { std::cout << (xorstr("Failed to initialize MinHook")) << std::endl; return; }
	MH_CreateHook(Function, Detour, Original);
	if (autoEnable)
		MH_EnableHook(Function);
}

inline void hk__() {
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::set_flying), (void**)&original_setflying, hk::misc::SetFlying);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::SendProjectileAttack), (void**)&original_sendprojectileattack, hk::combat::SendProjectileAttack);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::CanAttack), (void**)&original_canattack, hk::combat::CanAttack);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::SendClientTick), (void**)&original_sendclienttick, hk::misc::SendClientTick);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::HandleRunning), (void**)&original_handleRunning, hk::misc::HandleRunning);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::HandleJumping), (void**)&original_handleJumping, hk::misc::HandleJumping);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::GetModifiedAimConeDirection), (void**)&original_aimconedirection, hk::combat::GetModifiedAimConeDirection);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::CreateProjectile), (void**)&original_create_projectile, hk::combat::CreateProjectile);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::CanHoldItems), (void**)&original_canholditems, hk::combat::CanHoldItems);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::Run), (void**)&original_consolerun, hk::misc::Run);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::CreateEffect), (void**)&original_createeffect, hk::misc::CreateEffect);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::get_position), (void**)&original_geteyepos, hk::misc::get_position);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::Play), (void**)&original_viewmodelplay, hk::misc::Play);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::VisUpdateUsingCulling), (void**)&original_UnregisterFromVisibility, hk::misc::VisUpdateUsingCulling);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::TraceAll), (void**)&original_traceall, hk::combat::TraceAll);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::GetRandomVelocity), (void**)&original_getrandomvelocity, hk::combat::GetRandomVelocity);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::AddPunch), (void**)&original_addpunch, hk::combat::AddPunch);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::MoveTowards), (void**)&original_movetowards, hk::combat::MoveTowards);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::Launch), (void**)&original_launch, hk::combat::Launch);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::DoFixedUpdate), (void**)&original_dofixedupdate, hk::misc::DoFixedUpdate);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::DoHit), (void**)&original_dohit, hk::combat::DoHit);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::UpdateAmbient), (void**)&original_updateambient, hk::misc::UpdateAmbient);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::Jump), (void**)&original_jump, hk::misc::Jump);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::ClientInput), (void**)&original_clientinput, hk::misc::ClientInput);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::DoHitNotify), (void**)&original_dohitnotify, hk::misc::DoHitNotify);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::get_isHeadshot), (void**)&original_getisheadshot, hk::misc::get_isHeadshot);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::ForceToPos), (void**)&original_forcepos, hk::misc::ForcePositionTo);
	hk_((void*)(uintptr_t)(GetModBase(xorstr(L"GameAssembly.dll")) + CO::OnLand), (void**)&original_onland, hk::misc::OnLand);
}