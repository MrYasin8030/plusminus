#include <vector>
typedef void(__stdcall* ProcessAttack)(DWORD64, DWORD64);
typedef float(__stdcall* get_time)();
typedef void(__stdcall* StartAttackCooldown)(DWORD64, float);
typedef DWORD64(__stdcall* GetTransform)(DWORD64);
void DoMeleeAttack(Target target, DWORD64 Held, bool transform) {
	if (!target.valid || !Held) return;

	if (read(Held + 0x230, float) >= ((get_time)(Storage::gBase + CO::get_time))()) { return; }
	if (read(Held + 0x23C, float) < read(Held + 0x1D8, float)) { return; }

	DWORD64 staticHitTest = read(Storage::gBase + CO::HitTest, DWORD64); if (!staticHitTest) return;
	DWORD64 newHitTest = il2cpp_object_new(staticHitTest);

	DWORD64 trans; Ray ray = Ray(LocalPlayer->GetBoneByID(neck), (target.position - LocalPlayer->GetBoneByID(neck)).Normalized());
	if (!target.entity) return;
	if (transform) { trans = target.entity->GrabTransform(head); }
	else { trans = utils::GetTransform((DWORD64)target.entity); } if (!trans) return;
	write(newHitTest + 0x34, 1000.f, float);
	write(newHitTest + 0xB0, trans, DWORD64);
	write(newHitTest + 0x14, ray, Ray);
	write(newHitTest + 0x66, true, bool);
	write(newHitTest + 0x88, target.entity, BasePlayer*);
	write(newHitTest + 0x90, utils::TransformToPoint(trans, target.position), Vector3);
	write(newHitTest + 0x9C, Vector3(0, 0, 0), Vector3);
	write(newHitTest + 0x68, read(Held + 0x268, DWORD64), DWORD64);
	((StartAttackCooldown)(Storage::gBase + CO::StartAttackCooldown))(Held, read(Held + 0x1DC, float));
	return ((ProcessAttack)(Storage::gBase + CO::ProcessAttack))(Held, newHitTest);
}
typedef list<uintptr_t>*(__stdcall* get_Renderers)(uintptr_t);
typedef uintptr_t(__stdcall* get_material)(uintptr_t);
typedef uintptr_t(__stdcall* get_shader)(uintptr_t);
typedef void(__stdcall* SetInt)(uintptr_t, Str, int);
typedef void(__stdcall* SetColor)(uintptr_t, int, Color);
typedef int(__stdcall* PropertyToId)(Str);
uintptr_t shader;
int property;
void DoChams(uintptr_t target, Color col) {
	if (!PlayerEsp::chams) return;
	if (target) {
		if (!property) {
			property = ((PropertyToId)(Storage::gBase + CO::PropertyToId))(Str(xorstr(L"_Color")));
		}
		auto mainRendList = ((get_Renderers)(Storage::gBase + CO::get_Renderers))(target);
		for (int idx = 0; idx < mainRendList->get_size(); idx++) {
			uintptr_t renderer = mainRendList->get_value(idx);
			if (renderer) {
				uintptr_t material = ((get_material)(Storage::gBase + CO::get_material))(renderer);
				if (shader != ((get_shader)(Storage::gBase + CO::get_shader))(material)) {
					if (!shader) 
						shader = utils::ShaderFind(Str(xorstr(L"Hidden/Internal-Colored")));
					il2cpp::unity::set_shader(material, shader);
					((SetColor)(Storage::gBase + CO::SetColor))(material, property, col);
					((SetInt)(Storage::gBase + CO::SetInt))(material, Str(xorstr(L"_ZTest")), 8);
				}
			}
		}
	}
}
float LastKnock = 0.f; float LastOpen = 0.f; float LastHatch = 0.f;
void SpamKnock(uintptr_t Door) {
	typedef void(__stdcall* DoorFunction)(uintptr_t, BasePlayer*);
	if (LocalPlayer->Time() > LastKnock + 0.5f) {
		((DoorFunction)(Storage::gBase + CO::KnockDoor))(Door, LocalPlayer);
		LastKnock = LocalPlayer->Time();
	}
	if (LocalPlayer->Time() > LastOpen + 0.1f) {
		((DoorFunction)(Storage::gBase + CO::OpenDoor))(Door, LocalPlayer);
		LastOpen = LocalPlayer->Time();
	}
	if (LocalPlayer->Time() > LastHatch + 0.1f) {
		((DoorFunction)(Storage::gBase + CO::OpenHatch))(Door, LocalPlayer);
		LastHatch = LocalPlayer->Time();
	}
}
float LastPickup = 0.f;
void PickupPlayer(BasePlayer* ent) {
	typedef void(__stdcall* AssistPlayer)(BasePlayer*, BasePlayer*);
	if (!LocalPlayer->IsTeamMate(ent->GetSteamID()) && Misc::AssistTeamOnly) return;
	if (LocalPlayer->Time() > LastPickup + 0.5f) {
		((AssistPlayer)(Storage::gBase + CO::AssistPlayer))(ent, LocalPlayer);
		LastPickup = LocalPlayer->Time();
	}
}
void PickupItem(DWORD64 item) {
	typedef void(__stdcall* Pick)(DWORD64, Str);
	return ((Pick)(Storage::gBase + CO::ServerRPC))(item, Str(xorstr(L"Pickup")));
}
float ValueD = 0.f; float MaxValue = 10.f;
void Test() {
	ValueD += 0.02f;
	if (ValueD > MaxValue) {
		ValueD = 0;
		Global::doneHits = 0;
	}
	Renderer::Rectangle(Vector2((Global::ScreenWidth / 2) - 50, Global::ScreenHigh - 200), Vector2(100, 5), D2D1::ColorF::Black, 1.f);
	Renderer::FillRectangle(Vector2((Global::ScreenWidth / 2) - 50, Global::ScreenHigh - 200), Vector2(100 * (ValueD / MaxValue), 5), D2D1::ColorF(0.f, 255.f, 0.f, 0.8f));
}