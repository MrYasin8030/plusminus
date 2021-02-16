#include <map>
namespace a {
	double CalcBulletDrop(double height, double DepthPlayerTarget, float velocity, float gravity) {
		double pitch = (Vector3::my_atan2(height, DepthPlayerTarget));
		double BulletVelocityXY = velocity * Vector3::my_cos(pitch);
		double Time = DepthPlayerTarget / BulletVelocityXY;
		double TotalVerticalDrop = (0.5f * gravity * Time * Time);
		return TotalVerticalDrop * 10;
	}
#define powFFFFFFFFFFFFFFFFFFFFFF(n) (n)*(n)
	void Prediction(Vector3 local, Vector3& target, Vector3 targetvel, float bulletspeed, float gravity) {
		float Dist = Math::Calc3D_Dist(target, local);
		float BulletTime = Dist / bulletspeed;

		Vector3 vel = Vector3(targetvel.x, 0, targetvel.z) * 0.75f;

		Vector3 PredictVel = vel * BulletTime;

		target += PredictVel;

		double height = target.y - local.y;
		Vector3 dir = target - local;
		float DepthPlayerTarget = Vector3::my_sqrt(powFFFFFFFFFFFFFFFFFFFFFF(dir.x) + powFFFFFFFFFFFFFFFFFFFFFF(dir.z));

		float drop = CalcBulletDrop(height, DepthPlayerTarget, bulletspeed, gravity);

		target.y += drop;
	}
}
float GetFov(BasePlayer* Entity, BoneList Bone) {
	Vector2 ScreenPos;
	if (!utils::w2s(Entity->get_bone_pos(Bone), ScreenPos)) return 1000.f;
	return Math::Calc2D_Dist(Vector2(vars::stuff::ScreenWidth / 2, vars::stuff::ScreenHeight / 2), ScreenPos);
}
float GetFovHeli(Vector3 pos) {
	Vector2 ScreenPos;
	if (!utils::w2s(pos, ScreenPos)) return 1000.f;
	return Math::Calc2D_Dist(Vector2(vars::stuff::ScreenWidth / 2, vars::stuff::ScreenHeight / 2), ScreenPos);
}
// 785728077 -1691396643 51984655
float GetGravity(int ammoid) {
	switch (ammoid) {
	case 14241751:
		return 1.f;
	case -1234735557:
		return 0.75f;
	case 215754713:
		return 0.75f;
	case -1023065463:
		return 0.5f;
	case -2097376851:
		return 0.75f;
	case -1321651331:
		return 1.25f;
	default:
		return 1.f;
	}
}


float GetBulletSpeed()
{
	Item* active = LocalPlayer->GetActiveWeapon();
	Weapon tar = active->Info();
	int ammo = active->LoadedAmmo();
	if (ammo == 0) return vars::weapons::fast_bullets ? tar.ammo[0].speed * 1.3 : tar.ammo[0].speed; //melee
	for (Ammo am : tar.ammo) {
		for (int id : am.id) {
			if (id == ammo) {
				return vars::weapons::fast_bullets ? am.speed * 1.3 : am.speed;
			}
		}
		if (am.id[0] == 0) return vars::weapons::fast_bullets ? am.speed * 1.3 : am.speed;
	}
	return vars::weapons::fast_bullets ? 250.f * 1.3 : 250.f;
}
Vector3 HeliPrediction(const Vector3& LP_Pos) {
	Item* active = LocalPlayer->GetActiveWeapon();
	Weapon tar = active->Info();
	int ammo = active->LoadedAmmo();
	Vector3 TargetedHeli = read(vars::stor::closestHeliObj + 0x90, Vector3) + Vector3(0, 2, 0);
	float Dist = Math::Calc3D_Dist(LP_Pos, TargetedHeli);
	if (Dist > 0.001f) {
		Weapon tar = active->Info();
		int ammo = active->LoadedAmmo();
		float speed = GetBulletSpeed();
		float gravity = GetGravity(ammo);
		float BulletTime = Dist / speed;
		Vector3 vel;
		if (vars::stor::closestHeli != NULL) {
			vel = reinterpret_cast<BaseEntity*>(vars::stor::closestHeli)->GetWorldVelocity();
		}
		else {
			vel = Vector3(0, 0, 0);
		}
		Vector3 PredictVel = vel * BulletTime * 0.75f;
		TargetedHeli += PredictVel;
		TargetedHeli.y += (4.905f * BulletTime * BulletTime) * gravity;
	}
	return TargetedHeli;
}
Vector3 Prediction(BasePlayer* Player) {
	Item* active = LocalPlayer->GetActiveWeapon();
	Weapon tar = active->Info();
	int ammo = active->LoadedAmmo();
	Vector3 BonePos = Player->get_bone_pos(head);
	Vector3 Local;
	if (vars::misc::long_neck && GetAsyncKeyState(vars::keys::longneck)) { Local = LocalPlayer->get_bone_pos(head) + Vector3(0, 1.15, 0); }
	else { Local = LocalPlayer->get_bone_pos(head); }
	float Dist = Math::Calc3D_Dist(Local, BonePos);
	if (Dist > 0.001f) {
		float speed = GetBulletSpeed();
		float gravity = GetGravity(ammo);
		float BulletTime = Dist / speed;
		Vector3 PredictVel = Player->GetVelocity() * BulletTime * 0.75f;
		BonePos += PredictVel;
		float predicty = (4.905f * BulletTime * BulletTime) * gravity;

		if ((active->GetID() == 1443579727 || active->GetID() == 1953903201 || active->GetID() == 884424049) 
			&& active->LoadedAmmo() != -1023065463/*hv arrow*/) { // bow and nailgun and compound
			if (Dist <= 220.f) {
				BonePos.y += predicty;
			}
			else if (Dist > 220.f && Dist <= 260.f) {
				BonePos.y += 1.045f * predicty;
			}
			else if (Dist > 260.f && Dist <= 271.f) {
				BonePos.y += 1.05f * predicty;
			}
			else if (Dist > 271.f && Dist <= 284.5f) {
				BonePos.y += 1.06f * predicty;
			}
			else if (Dist > 284.5f && Dist <= 291.f) {
				BonePos.y += 1.07f * predicty;
			}
			else if (Dist > 291.f) {
				BonePos.y += 1.08f * predicty;
			}
		}
		else if (active->GetID() == -75944661) { // eoka
			if (Dist <= 180.f) {
				BonePos.y += predicty;
			}
			else if (Dist > 180.f && Dist <= 220.f) {
				BonePos.y += 1.5f * predicty;
			}
			else if (Dist > 220.f && Dist <= 240.f) {
				BonePos.y += 2.1f * predicty;
			}
			else if (Dist > 240.f && Dist <= 255.f) {
				BonePos.y += 2.5f * predicty;
			}
			else if (Dist > 255.f && Dist <= 265.f) {
				BonePos.y += 3.0f * predicty;
			}
			else if (Dist > 265.f && Dist <= 273.f) {
				BonePos.y += 3.5f * predicty;
			}
			else if (Dist > 273.f) {
				BonePos.y += 3.8f * predicty;
			}
		}
		else {
			BonePos.y += predicty;
		}
	}
	return BonePos;
}
void Normalize(float& Yaw, float& Pitch) {
	if (Pitch < -89) Pitch = -89;
	else if (Pitch > 89) Pitch = 89;
	if (Yaw < -360) Yaw += 360;
	else if (Yaw > 360) Yaw -= 360;
}
void GoToTarget(BasePlayer* player) {
	Vector3 Local = LocalPlayer->get_bone_pos(head);
	Vector3 PlayerPos = Prediction(player);
	Vector2 Offset = Math::CalcAngle(Local, PlayerPos) - LocalPlayer->GetVA();
	Normalize(Offset.y, Offset.x);
	Vector2 AngleToAim = LocalPlayer->GetVA() + Offset;
	Normalize(AngleToAim.y, AngleToAim.x);
	LocalPlayer->SetVA(AngleToAim);
}

void Aim(BasePlayer* AimEntity) {
	if (vars::combat::aimbot && !LocalPlayer->IsTeamMate(AimEntity->GetSteamID())) {
		if (AimEntity && !LocalPlayer->IsMenu()) {
			if (GetAsyncKeyState(vars::keys::aimkey)) GoToTarget(AimEntity);
		}
	}
}