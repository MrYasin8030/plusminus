﻿using ptr_t = uint64_t;
enum UnmanagedCallingConvention {
	UnmanagedCdecl,
	UnmanagedStdcall,
	UnmanagedFastcall,
};

template<typename t_Function>
class UnmanagedPointer {
public:

	template<typename... t_FunctionParameters>
	auto operator()(t_FunctionParameters... params) {
		using result_type = decltype(std::declval<t_Function>( )(std::declval<t_FunctionParameters>( )...));
		using function_cdecl_ptr_t = result_type(__cdecl*)(t_FunctionParameters...);
		using function_stdcall_ptr_t = result_type(__stdcall*)(t_FunctionParameters...);
		using function_fastcall_ptr_t = result_type(_fastcall*)(t_FunctionParameters...);

		switch (this->m_CallingConvention) {
		case UnmanagedCdecl:
			return reinterpret_cast<function_cdecl_ptr_t>(this->m_Address)(params...);
		case UnmanagedStdcall:
			return reinterpret_cast<function_stdcall_ptr_t>(this->m_Address)(params...);
		case UnmanagedFastcall:
			return reinterpret_cast<function_fastcall_ptr_t>(this->m_Address)(params...);
		}
	}

	UnmanagedPointer(ptr_t dwAddress, UnmanagedCallingConvention unmCallingConvention) {
		this->m_Address = dwAddress;
		this->m_CallingConvention = unmCallingConvention;
	}
private:
	ptr_t m_Address;
	UnmanagedCallingConvention m_CallingConvention;
};
class SafeExecution {
private:
	static int fail(unsigned int code, struct _EXCEPTION_POINTERS* ep) {
		if (code == EXCEPTION_ACCESS_VIOLATION) {
			return EXCEPTION_EXECUTE_HANDLER;
		}
		else {
			return EXCEPTION_CONTINUE_SEARCH;
		};
	}
public:
	template<typename T = void*, typename R = void*, typename... Args>
	static T Execute(uint64_t ptr, R ret, Args... args) {
		__try {
			return reinterpret_cast<T(__stdcall*)(Args...)>(ptr)(args...);
		}
		__except (fail(GetExceptionCode( ), GetExceptionInformation( ))) {
			return ret;
		}
	}
};

#define STATIC_FUNCTION(method_path,name,ta) static inline UnmanagedPointer<ta> name = { METHOD(method_path), UnmanagedStdcall }


class Transform {
public:
	Vector3 position( ) {
		if (!this) return Vector3::Zero( );
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Transform::get_position(): Vector3");
		return SafeExecution::Execute<Vector3>(off, Vector3::Zero( ), this);
	}
	Vector3 InverseTransformPoint(Vector3 position) {
		if (!this) return Vector3::Zero( );
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Transform::InverseTransformPoint(Vector3): Vector3");
		return SafeExecution::Execute<Vector3>(off, Vector3::Zero( ), this, position);
	}

	Vector3 InverseTransformDirection(Vector3 position) {
		if (!this) return Vector3::Zero( );
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Transform::InverseTransformDirection(Vector3): Vector3");
		return SafeExecution::Execute<Vector3>(off, Vector3::Zero( ), this, position);
	}
};
class GameObject {
public:
};
class Object {
public:
	Transform* transform( ) {
		if (!this) return nullptr;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Component::get_transform(): Transform");
		return SafeExecution::Execute<Transform*>(off, nullptr, this);
	}
};
class Type {
public:
	
};
class Component : public Object {
public:
	template<typename T = Component>
	T* GetComponent(Type* type) {
		if (!this || !type) return nullptr;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Component::GetComponent(Type): Component");
		return reinterpret_cast<T * (__fastcall*)(Component*, Type*)>(off)(this, type);
	}
	GameObject* gameObject( ) {
		if (!this) return nullptr;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Component::get_gameObject(): GameObject");
		return SafeExecution::Execute<GameObject*>(off, nullptr, this);
	}
};
class Bone {
public:
	Vector3 position;
	bool visible;

	Bone( ) {
		this->position = Vector3::Zero( );
		this->visible = false;
	}
	Bone(Vector3 position) {
		this->position = position;
		this->visible = false;
	}
	Bone(Vector3 position, bool visible) {
		this->position = position;
		this->visible = visible;
	}
};
class BoneCache {
public:
	Bone* head;
	Bone* neck;
	Bone* root;
	Bone* spine4;
	Bone* spine1;
	Bone* l_clavicle;
	Bone* l_upperarm;
	Bone* l_forearm;
	Bone* l_hand;
	Bone* r_clavicle;
	Bone* r_upperarm;
	Bone* r_forearm;
	Bone* r_hand;
	Bone* pelvis;
	Bone* l_hip;
	Bone* l_knee;
	Bone* l_ankle_scale;
	Bone* l_foot;
	Bone* r_hip;
	Bone* r_knee;
	Bone* r_ankle_scale;
	Bone* r_foot;

	BoneCache( ) {
		head = new Bone( );
		neck = new Bone( );
		root = new Bone( );
		spine4 = new Bone( );
		spine1 = new Bone( );
		l_clavicle = new Bone( );
		l_upperarm = new Bone( );
		l_forearm = new Bone( );
		l_hand = new Bone( );
		r_clavicle = new Bone( );
		r_upperarm = new Bone( );
		r_forearm = new Bone( );
		r_hand = new Bone( );
		pelvis = new Bone( );
		l_hip = new Bone( );
		l_knee = new Bone( );
		l_ankle_scale = new Bone( );
		l_foot = new Bone( );
		r_hip = new Bone( );
		r_knee = new Bone( );
		r_ankle_scale = new Bone( );
		r_foot = new Bone( );
	}
};
std::map<uint64_t, BoneCache*> cachedBones = std::map<uint64_t, BoneCache*>( );
	
class Model;
class ItemModProjectile {

};
class BaseNetworkable : public Component {
public:
	class EntityRealm {
	public:
		template<typename T = BaseNetworkable*> T Find(uint32_t uid) {
			static auto off = METHOD("Assembly-CSharp::EntityRealm::Find(UInt32): BaseNetworkable");
			return reinterpret_cast<T(__fastcall*)(EntityRealm*, uint32_t)>(off)(this, uid);
		}

		FIELD("Assembly-CSharp::EntityRealm::entityList", entityList, ListDictionary*);
	};
	static EntityRealm* clientEntities( ) {
		static auto clazz = CLASS("Assembly-CSharp::BaseNetworkable");
		return *reinterpret_cast<EntityRealm**>(std::uint64_t(clazz->static_fields));
	}
};
class BaseEntity : public BaseNetworkable {
public:
	Vector3 GetWorldVelocity( ) {
		if (!this) return Vector3::Zero( );
		static auto off = METHOD("Assembly-CSharp::BaseEntity::GetWorldVelocity(): Vector3");
		return reinterpret_cast<Vector3(__fastcall*)(BaseEntity*)>(off)(this);
	}
	bool IsDestroyed( ) {
		if (!this) return true;
		static auto off = OFFSET("Assembly-CSharp::BaseNetworkable::<IsDestroyed>k__BackingField");
		return *reinterpret_cast<bool*>(this + off);
	}
	bool IsValid( ) {
		if (!this) return false;
		return !this->IsDestroyed( );
	}
	FIELD("Assembly-CSharp::BaseEntity::model", model, Model*);
};
class RenderSettings {
public:
	enum class AmbientMode {
		Skybox,
		Trilight,
		Flat,
		Custom
	};
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::RenderSettings::set_ambientMode(Rendering.AmbientMode): Void", set_ambientMode, void(AmbientMode));
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::RenderSettings::set_ambientIntensity(Single): Void", set_ambientIntensity, void(float));
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::RenderSettings::set_ambientLight(Color): Void", set_ambientLight, void(Color));
};
class Input {
public:
	STATIC_FUNCTION("UnityEngine.InputLegacyModule::UnityEngine::Input::GetKeyDown(KeyCode): Boolean", GetKeyDown, bool(KeyCode));
	STATIC_FUNCTION("UnityEngine.InputLegacyModule::UnityEngine::Input::GetKey(KeyCode): Boolean", GetKey, bool(KeyCode));
};
class BaseCombatEntity : public BaseEntity {
public:
	const char* ClassName( ) {
		if (!this) return "";
		auto oc = *reinterpret_cast<uint64_t*>(this);
		if (!oc) return "";
		return *reinterpret_cast<char**>(oc + 0x10);
	}
	uint32_t ClassNameHash( ) {
		if (!this) return 0;
		auto oc = *reinterpret_cast<uint64_t*>(this);
		if (!oc) return 0;
		const char* name = *reinterpret_cast<char**>(oc + 0x10);
		return RUNTIME_CRC32(name);
	}
	bool IsPlayer( ) {
		if (!this) return false;
		return !strcmp(this->ClassName( ), xorstr("BasePlayer")) ||
			!strcmp(this->ClassName( ), xorstr("NPCPlayerApex")) ||
			!strcmp(this->ClassName( ), xorstr("NPCMurderer")) ||
			!strcmp(this->ClassName( ), xorstr("NPCPlayer")) ||
			!strcmp(this->ClassName( ), xorstr("HumanNPC")) ||
			!strcmp(this->ClassName( ), xorstr("Scientist")) ||
			!strcmp(this->ClassName( ), xorstr("TunnelDweller")) ||
			!strcmp(this->ClassName( ), xorstr("HTNPlayer"));
	}
	FIELD("Assembly-CSharp::BaseCombatEntity::_health", health, float);
	FIELD("Assembly-CSharp::BaseCombatEntity::sendsHitNotification", sendsHitNotification, bool);
	FIELD("Assembly-CSharp::BaseCombatEntity::sendsMeleeHitNotification", sendsMeleeHitNotification, bool);
	FIELD("Assembly-CSharp::BaseCombatEntity::sendsMeleeHitNotification", lastNotifyFrame, int);
};
class Time {
public:
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_time(): Single", time, float( ));
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_deltaTime(): Single", deltaTime, float( ));
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_fixedTime(): Single", fixedTime, float( ));
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_unscaledTime(): Single", unscaledTime, float( ));
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_unscaledDeltaTime(): Single", unscaledDeltaTime, float( ));
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_fixedDeltaTime(): Single", fixedDeltaTime, float( ));
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_maximumDeltaTime(): Single", maximumDeltaTime, float( ));
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_smoothDeltaTime(): Single", smoothDeltaTime, float( ));
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_timeScale(): Single", timeScale, float( ));
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_frameCount(): Int32", frameCount, int( ));
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_renderedFrameCount(): Int32", renderedFrameCount, int( ));
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_realtimeSinceStartup(): Single", realtimeSinceStartup, float( ));
};

class Physics {
public:
	STATIC_FUNCTION("UnityEngine.PhysicsModule::UnityEngine::Physics::get_gravity(): Vector3", get_gravity, Vector3( ));
	static void IgnoreLayerCollision(int layer1, int layer2, bool ignore) {
		return reinterpret_cast<void(*)(int, int, bool)>(il2cpp::il2cpp_resolve_icall(xorstr("UnityEngine.Physics::IgnoreLayerCollision")))(layer1, layer2, ignore);
	}
	static bool CheckCapsule(Vector3 start, Vector3 end, float radius, int layerMask, QueryTriggerInteraction queryTriggerInteraction) {
		return SafeExecution::Execute<bool>(vars::stor::gBase + CO::CheckCapsule, false, start, end, radius, layerMask, queryTriggerInteraction);
	}
};

class ItemDefinition : public Component {
public:
	FIELD("Assembly-CSharp::ItemDefinition::itemid", itemid, int);
};
class BaseProjectile /*: public AttackEntity*/ {
public:
	class Magazine {
	public:
		FIELD("Assembly-CSharp::Magazine::ammoType", ammoType, ItemDefinition*);
	};
	FIELD("Assembly-CSharp::BaseProjectile::primaryMagazine", primaryMagazine, Magazine*);
	FIELD("Assembly-CSharp::BaseProjectile::projectileVelocityScale", projectileVelocityScale, float);
};

class DamageTypeList {
public:
	enum class DamageType {
		Generic,
		Hunger,
		Thirst,
		Cold,
		Drowned,
		Heat,
		Bleeding,
		Poison,
		Suicide,
		Bullet,
		Slash,
		Blunt,
		Fall,
		Radiation,
		Bite,
		Stab,
		Explosion,
		RadiationExposure,
		ColdExposure,
		Decay,
		ElectricShock,
		Arrow,
		AntiVehicle,
		Collision,
		Fun_Water,
		LAST
	};
	float Total( ) {
		if (!this) return false;
		static auto off = METHOD("Assembly-CSharp::Rust::DamageTypeList::Total(): Single");
		return reinterpret_cast<float(__fastcall*)(DamageTypeList*)>(off)(this);
	}
};
class HitInfo {
public:
	FIELD("Assembly-CSharp::HitInfo::Initiator", Initiator, BaseEntity*);
	FIELD("Assembly-CSharp::HitInfo::WeaponPrefab", WeaponPrefab, BaseEntity*);
	FIELD("Assembly-CSharp::HitInfo::DoHitEffects", DoHitEffects, bool);
	FIELD("Assembly-CSharp::HitInfo::DoDecals", DoDecals, bool);
	FIELD("Assembly-CSharp::HitInfo::IsPredicting", IsPredicting, bool);
	FIELD("Assembly-CSharp::HitInfo::UseProtection", UseProtection, bool);
	FIELD("Assembly-CSharp::HitInfo::DidHit", DidHit, bool);
	FIELD("Assembly-CSharp::HitInfo::HitEntity", HitEntity, BaseEntity*);
	FIELD("Assembly-CSharp::HitInfo::HitBone", HitBone, uint32_t);
	FIELD("Assembly-CSharp::HitInfo::HitPart", HitPart, uint32_t);
	FIELD("Assembly-CSharp::HitInfo::HitMaterial", HitMaterial, uint32_t);
	FIELD("Assembly-CSharp::HitInfo::HitPositionWorld", HitPositionWorld, Vector3);
	FIELD("Assembly-CSharp::HitInfo::HitPositionLocal", HitPositionLocal, Vector3);
	FIELD("Assembly-CSharp::HitInfo::HitNormalWorld", HitNormalWorld, Vector3);
	FIELD("Assembly-CSharp::HitInfo::HitNormalLocal", HitNormalLocal, Vector3);
	FIELD("Assembly-CSharp::HitInfo::PointStart", PointStart, Vector3);
	FIELD("Assembly-CSharp::HitInfo::PointEnd", PointEnd, Vector3);
	FIELD("Assembly-CSharp::HitInfo::ProjectileID", ProjectileID, int);
	FIELD("Assembly-CSharp::HitInfo::ProjectileDistance", ProjectileDistance, float);
	FIELD("Assembly-CSharp::HitInfo::ProjectileVelocity", ProjectileVelocity, Vector3);
	FIELD("Assembly-CSharp::HitInfo::damageTypes", damageTypes, DamageTypeList*);
};
struct ProjectileWeaponModModifier {
};
class TOD_Sky {
public:
};
class BossFormController {
public:
};
class Collider {
public:
};
class Effect {
public:
};
class PlayerWalkMovement {
public:
};
class InputState {
public:
	bool IsDown(Button b) {
		if (!this) return false;
		return reinterpret_cast<bool(__fastcall*)(InputState*, Button)>(vars::stor::gBase + 0x57D210)(this, b);
	}
	bool WasJustPressed(Button b) {
		if (!this) return false;
		return reinterpret_cast<bool(__fastcall*)(InputState*, Button)>(vars::stor::gBase + 0x57D300)(this, b);
	}
};
class ModelState {
public:
};
class HeldEntity {
public:
};
class BaseMountable {
public:
};
class PlayerProjectileAttack {
public:
};
class TraceInfo {
public:
};
class ViewModel {
public:
};
class PlayerNameID {
public:
	wchar_t* username( ) {
		auto username = (pUncStr)(*reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x18));
		return username->str;
	}

	unsigned long user_id( ) {
		return *reinterpret_cast<unsigned long*>((uintptr_t)this + 0x20);
	}
};
class PlayerEyes : public Component {
public:
	Quaternion get_rotation( ) {
		typedef Quaternion(__stdcall* get_rotation)(PlayerEyes*);
		Quaternion result = ((get_rotation)(vars::stor::gBase + CO::get_rotation))(this);
		return result;
	}
	Vector3 get_position( ) {
		typedef Vector3(__stdcall* get_position)(PlayerEyes*);
		Vector3 result = ((get_position)(vars::stor::gBase + CO::get_position))(this);
		return result;
	}

	Vector3 MovementForward( ) {
		if (!this) return Vector3::Zero( );
		static auto off = METHOD("Assembly-CSharp::PlayerEyes::MovementForward(): Vector3");
		return reinterpret_cast<Vector3(__fastcall*)(PlayerEyes*)>(off)(this);
	}
	Vector3 MovementRight( ) {
		if (!this) return Vector3::Zero( );
		static auto off = METHOD("Assembly-CSharp::PlayerEyes::MovementRight(): Vector3");
		return reinterpret_cast<Vector3(__fastcall*)(PlayerEyes*)>(off)(this);
	}
};


class PlayerTick {
public:
	Vector3 position( ) {
		Vector3 th = read(this + 0x20, Vector3);
		if (!th.empty( )) {
			return th;
		}
		else { return Vector3::Zero( ); }
	}
};
class ConsoleOptions {
public:
	bool IsFromServer( ) { return read(this + 0x6, bool); }
};
class Item {
public:
	uintptr_t entity( ) {
		return read(this + 0x98, uintptr_t);
	}
	int GetID( ) {
		DWORD64 Info = read((DWORD64)this + 0x20, DWORD64); // public ItemDefinition info;
		int ID = read(Info + 0x18, int); // public int itemid;
		return ID;
	}
	char* ClassName( ) {
		return (char*)read(read(this->entity( ), DWORD64) + 0x10, DWORD64);
	}
	Weapon Info( ) {
		int ID = GetID( );
		for (Weapon k : info) {
			if (k.id == ID) {
				return k;
			}
		}
		return Weapon{0};
	}
	int LoadedAmmo( ) {
		uintptr_t Held = this->entity( );
		if (!Held) return 0;

		uintptr_t Magazine = read(Held + 0x2A8, uintptr_t);
		if (!Magazine) return 0;

		uintptr_t ammoType = read(Magazine + 0x20, uintptr_t);
		if (!ammoType) return 0;

		int ammo = read(ammoType + 0x18, int);
		return ammo;
	}
	wchar_t* get_name( ) {
		uintptr_t Info = read(this + 0x20, uintptr_t);
		uintptr_t DispName = read(Info + 0x28, uintptr_t);
		pUncStr Str = ((pUncStr)read(DispName + 0x18, uintptr_t)); // default
		if (!Str) return nullptr; return Str->str;
	}
	int count( ) {
		return read(this + 0x30, int);
	}
	int uid( ) {
		return read(this + 0x28, int);
	}
	void RapidFire( ) {
		if (vars::weapons::rapid_fire) {
			DWORD64 heldentity = this->entity( );
			write(heldentity + 0x1E4, 0.09f, float);
		}
	}
	void NoSway( ) {
		DWORD64 Heldd = this->entity( );
		if (vars::weapons::no_sway) {
			write(Heldd + 0x2C0, 0.f, float);
			write(Heldd + 0x2C4, 0.f, float);
		}
	}
	void SetAutomatic( ) {
		if (vars::weapons::automatic) {
			DWORD64 Heldd = this->entity( );
			write(Heldd + 0x278, 1, bool);
		}
	}
	void FastBow( ) {
		if (vars::weapons::fastbow) {
			DWORD64 Heldd = this->entity( );
			write(Heldd + 0x350, true, bool);
			write(Heldd + 0x354, 1.f, float);
		}
	}
	void EokaTap( ) {
		if (vars::weapons::eokatap) {
			DWORD64 Heldd = this->entity( );
			write(Heldd + 0x350, 1.f, float); // successFraction
			write(Heldd + 0x360, true, bool); // didSparkThisFrame
		}
	}
};

class BasePlayer : public BaseCombatEntity {
public:
	const wchar_t* _displayName( ) {
		if (!this) return L"";
		static auto off = OFFSET("Assembly-CSharp::BasePlayer::_displayName");
		return (*reinterpret_cast<il2cpp::String**>(this + off))->buffer;
	}
	PlayerEyes* eyes( ) { return read(this + 0x640, PlayerEyes*); }
	PlayerTick* lastSentTick( ) { return read(this + 0x610, PlayerTick*); }
	FIELD("Assembly-CSharp::BasePlayer::userID", userID, uint64_t);
	bool isCached( ) {
		return map_contains_key(cachedBones, this->userID( ));
	}
	bool is_visible( ) {
		if (cachedBones[ this->userID( ) ]->head->visible ||
			cachedBones[ this->userID( ) ]->spine4->visible ||
			cachedBones[ this->userID( ) ]->pelvis->visible ||
			cachedBones[ this->userID( ) ]->r_foot->visible ||
			cachedBones[ this->userID( ) ]->l_foot->visible) {
			
			return true;
		}
		else {
			return false;
		}
	}

	void set_viewangles(const Vector2& VA) {
		DWORD64 Input = read(this + 0x4D0, DWORD64);
		write(Input + 0x3C, VA, Vector2);
	}
	Vector2 viewangles( ) {
		DWORD64 Input = read(this + 0x4D0, DWORD64);
		return read(Input + 0x3C, Vector2);
	}
	Vector3 newVelocity( ) {
		DWORD64 PlayerModel = read(this + 0x4B0, DWORD64);
		return read(PlayerModel + 0x1FC, Vector3);
	}
	float GetHealth( ) {
		return this->health( );
	}
	bool IsNpc( ) {
		DWORD64 PlayerModel = read(this + 0x4B0, DWORD64);
		return read(PlayerModel + 0x2C0, bool);
	}
	ULONG64 mono_transform(int bone) {
		DWORD64 entity_model = read(this + 0x120, DWORD64);
		DWORD64 bone_dict = read(entity_model + 0x48, DWORD64);
		DWORD64 val = read(bone_dict + 0x20 + bone * 0x8, DWORD64);
		return val;
	}
	void SetFov(float val) {
		auto klass = read(vars::stor::gBase + CO::ConvarGraphics, DWORD64);
		auto static_fields = read(klass + 0xB8, DWORD64);
		write(static_fields + 0x18, val, float);
	}
	void PatchCamspeed( ) {
		auto klass = read(vars::stor::gBase + CO::Client, DWORD64);
		auto static_fields = read(klass + 0xB8, DWORD64);
		write(static_fields + 0x2C, 1.f, float);
		write(static_fields + 0x20, 1.f, float);
		write(static_fields + 0x34, 1.f, float);
		write(static_fields + 0x28, 1.f, float);
	}
	bool HasFlags(int Flg) {
		return (read(this + 0x638, int) & Flg);
	}

	FIELD("Assembly-CSharp::Model::boneTransforms", boneTransforms, Array<Transform*>*);
	FIELD("Assembly-CSharp::Model::boneNames", boneNames, Array<il2cpp::String*>*);

	Vector3 get_bone_pos(BoneList bone) {
		uintptr_t player_model = read(this + 0x120, uintptr_t);// public Model model
		uintptr_t boneTransforms = read(player_model + 0x48, uintptr_t);//public Transform[] boneTransforms;
		Transform* transform = read(boneTransforms + 0x20 + bone * 0x8, Transform*);
		//const Transform* transform = read(BoneValue + 0x10, Transform*);

		if (!transform)
			return Vector3::Zero( );
		//Vector3 pos = Vector3::Zero( );
		//static auto get_position = reinterpret_cast<void(__fastcall*)(Transform*, Vector3&)>(std::uint64_t(vars::stor::uBase + 0xDD2160));
		//get_position(transform, pos);
		return transform->position( );
	}
	DWORD64 GetTeammateByPos(int Id) {
		DWORD64 ClientTeam = read(this + 0x580, DWORD64);
		DWORD64 members = read(ClientTeam + 0x30, DWORD64);
		DWORD64 List = read(members + 0x10, DWORD64);
		DWORD64 player = read(List + 0x20 + (Id * 0x8), DWORD64);
		return read(player + 0x20, DWORD64);
	}
	bool is_teammate(DWORD64 SteamID) {
		bool ret = false;
		for (int i = 0; i < 8; i++) {
			if (GetTeammateByPos(i) == SteamID) {
				ret = true;
				break;
			}
		}
		return ret;
	}

	bool IsMenu( ) {
		return reinterpret_cast<bool(*)()>(vars::stor::gBase + CO::cursor_get_visible)();
	}
	void add_modelstate_flag(ModelStateFlag flag) {
		DWORD64 mstate = read(this + 0x5C8, DWORD64);
		int flags = read(mstate + 0x24, int);
		write(mstate + 0x24, flags |= (int)flag, int);
	}
	void remove_modelstate_flag(ModelStateFlag flag) {
		DWORD64 mstate = read(this + 0x5C8, DWORD64);
		int flags = read(mstate + 0x24, int);
		write(mstate + 0x24, flags &= (int)flag, int);
	}
	bool has_modelstate_flag(ModelStateFlag flag) {
		DWORD64 mstate = read(this + 0x5C8, DWORD64);
		int flags = read(mstate + 0x24, int);
		return flags & (int)flag;
	}
	float GetJumpHeight( ) {
		return reinterpret_cast<float(_fastcall*)(BasePlayer*)>(vars::stor::gBase + CO::GetJumpHeight)(this);
	}

	bool GetKeyState(Button b) {
		DWORD64 InputState = read(read(this + 0x4D0, DWORD64) + 0x20, DWORD64);
		DWORD64 Cur = read(InputState + 0x10, DWORD64);
		if (!Cur) return false;
		int btn = read(Cur + 0x14, int);
		return ((btn & (int)b) == (int)b);
	}
	Item* GetWeaponInfo(int Id) {
		DWORD64 Inventory = read(this + 0x648, DWORD64);
		DWORD64 Belt = read(Inventory + 0x28, DWORD64); // containerBelt
		DWORD64 ItemList = read(Belt + 0x38, DWORD64);// public List<Item> itemList;
		DWORD64 Items = read(ItemList + 0x10, DWORD64); //	public List<InventoryItem.Amount> items;
		return (Item*)read(Items + 0x20 + (Id * 0x8), DWORD64);
	}
	List<Item*>* item_list_b( ) {
		DWORD64 Inventory = read(this + 0x648, DWORD64);
		DWORD64 Belt = read(Inventory + 0x28, DWORD64); // containerBelt
		return read(Belt + 0x38, List<Item*>*);// public List<Item> itemList;
	}
	Item* GetClothesInfo(int Id) {
		DWORD64 Inventory = read(this + 0x648, DWORD64);
		DWORD64 Belt = read(Inventory + 0x30, DWORD64); // containerWear
		DWORD64 ItemList = read(Belt + 0x38, DWORD64);// public List<Item> itemList;
		DWORD64 Items = read(ItemList + 0x10, DWORD64); //	public List<InventoryItem.Amount> items;
		return (Item*)read(Items + 0x20 + (Id * 0x8), DWORD64);
	}
	List<Item*>* item_list_w( ) {
		DWORD64 Inventory = read(this + 0x648, DWORD64);
		DWORD64 Belt = read(Inventory + 0x30, DWORD64); // containerWear
		return read(Belt + 0x38, List<Item*>*);// public List<Item> itemList;
	}
	Item* GetActiveWeapon( ) {
		int ActUID = read(this + 0x5B0, int);
		if (!ActUID) return nullptr;
		Item* ActiveWeapon;
		for (int i = 0; i < 6; i++)
			if (ActUID == (ActiveWeapon = GetWeaponInfo(i))->uid( ))
				return ActiveWeapon;
		return nullptr;
	}
	float lastSentTickTime( ) {
		return read(this + 0x60C, float);
	}
	void FakeAdmin( ) {
		int Flags = read(this + 0x638, int);
		write(this + 0x638, (Flags |= 4), int);
	}
	void SetGravity(float val) {
		DWORD64 Movement = read(this + 0x4D8, DWORD64);
		write(Movement + 0x74, val, float);
	}
};
class LocalPlayer {
public:
	static BasePlayer* Entity( ) {
		return reinterpret_cast<BasePlayer * (*)()>(vars::stor::gBase + CO::get_Entity)();
	}
};

class Mathf {
public:
	static float Abs(float a) {
		return reinterpret_cast<float(_fastcall*)(float)>(vars::stor::gBase + CO::MathfAbs)(a);
	}
	static float Max(float a, float b) {
		return reinterpret_cast<float(_fastcall*)(float, float)>(vars::stor::gBase + CO::MathfMax)(a, b);
	}
};
class BuildingBlock {
public:
	bool IsUpgradeBlocked( ) {
		typedef bool(__stdcall* IsUpgradeBlocked)(BuildingBlock*);
		return ((IsUpgradeBlocked)(vars::stor::gBase + CO::IsUpgradeBlocked))(this);
	}
	bool CanChangeToGrade(BuildingGrade i, BasePlayer* ply) {
		typedef bool(__stdcall* CanChangeToGrade)(BuildingBlock*, BuildingGrade, BasePlayer*);
		return ((CanChangeToGrade)(vars::stor::gBase + CO::CanChangeToGrade))(this, i, ply);
	}
	bool CanAffordUpgrade(BuildingGrade i, BasePlayer* ply) {
		typedef bool(__stdcall* CanAffordUpgrade)(BuildingBlock*, BuildingGrade, BasePlayer*);
		return ((CanAffordUpgrade)(vars::stor::gBase + CO::CanAffordUpgrade))(this, i, ply);
	}
	void UpgradeToGrade(BuildingGrade i, BasePlayer* ply) {
		typedef void(__stdcall* UpgradeToGrade)(BuildingBlock*, BuildingGrade, BasePlayer*);
		((UpgradeToGrade)(vars::stor::gBase + CO::UpgradeToGrade))(this, i, ply);
	}
};



class HitTest {
public:
	BaseEntity* HitEntity( ) { return read(this + 0x88, BaseEntity*); }
	void HitEntity(BaseEntity* en) { write(this + 0x88, en, BaseEntity*); }

	FIELD("Assembly-CSharp::HitTest::AttackRay", AttackRay, Ray);
	FIELD("Assembly-CSharp::HitTest::Radius", Radius, float);
	FIELD("Assembly-CSharp::HitTest::Forgiveness", Forgiveness, float);
	FIELD("Assembly-CSharp::HitTest::MaxDistance", MaxDistance, float);
	FIELD("Assembly-CSharp::HitTest::MultiHit", MultiHit, bool);
	FIELD("Assembly-CSharp::HitTest::BestHit", BestHit, bool);
	FIELD("Assembly-CSharp::HitTest::DidHit", DidHit, bool);
	FIELD("Assembly-CSharp::HitTest::ignoreEntity", ignoreEntity, BaseEntity*);
	FIELD("Assembly-CSharp::HitTest::HitPoint", HitPoint, Vector3);
	FIELD("Assembly-CSharp::HitTest::HitNormal", HitNormal, Vector3);
	FIELD("Assembly-CSharp::HitTest::HitMaterial", HitMaterial, il2cpp::String*);
	FIELD("Assembly-CSharp::HitTest::HitDistance", HitDistance, float);
	FIELD("Assembly-CSharp::HitTest::HitTransform", HitTransform, Transform*);
	FIELD("Assembly-CSharp::HitTest::HitPart", HitPart, uint32_t);

	Vector3 HitPointWorld( ) {
		if (!this) return Vector3( );
		static auto off = METHOD("Assembly-CSharp::HitTest::HitPointWorld(): Vector3");
		return reinterpret_cast<Vector3(__fastcall*)(HitTest*)>(off)(this);
	}

	Vector3 HitNormalWorld( ) {
		if (!this) return Vector3( );
		static auto off = METHOD("Assembly-CSharp::HitTest::HitNormalWorld(): Vector3");
		return reinterpret_cast<Vector3(__fastcall*)(HitTest*)>(off)(this);
	}

	uintptr_t BuildAttackMessage( ) {
		if (!this) return 0;
		return reinterpret_cast<uintptr_t(__fastcall*)(HitTest*)>(vars::stor::gBase + 0x7A8E80)(this);
	}
};
class Projectile {
public:
	Vector3 initialVelocity( ) { return read(this + 0x18, Vector3); }
	float drag( ) { return read(this + 0x24, float); }
	float gravityModifier( ) { return read(this + 0x28, float); }
	float thickness( ) { return read(this + 0x2C, float); }
	float initialDistance( ) { return read(this + 0x30, float); }
	bool remainInWorld( ) { return read(this + 0x34, bool); }
	float stickProbability( ) { return read(this + 0x38, float); }
	float breakProbability( ) { return read(this + 0x3C, float); }
	float conditionLoss( ) { return read(this + 0x40, float); }
	float ricochetChance( ) { return read(this + 0x44, float); }
	float penetrationPower( ) { return read(this + 0x48, float); }
	uintptr_t damageProperties( ) { return read(this + 0x50, uintptr_t); }
	uintptr_t damageDistances( ) { return read(this + 0x58, uintptr_t); }
	uintptr_t damageMultipliers( ) { return read(this + 0x60, uintptr_t); }
	uintptr_t rendererToScale( ) { return read(this + 0x70, uintptr_t); }
	uintptr_t firstPersonRenderer( ) { return read(this + 0x78, uintptr_t); }
	bool createDecals( ) { return read(this + 0x80, bool); }
	bool doDefaultHitEffects( ) { return read(this + 0x81, bool); }
	uintptr_t flybySound( ) { return read(this + 0x88, uintptr_t); }
	float flybySoundDistance( ) { return read(this + 0x90, float); }
	uintptr_t closeFlybySound( ) { return read(this + 0x98, uintptr_t); }
	float closeFlybyDistance( ) { return read(this + 0xA0, float); }
	float tumbleSpeed( ) { return read(this + 0xA4, float); }
	Vector3 tumbleAxis( ) { return read(this + 0xA8, Vector3); }
	Vector3 swimScale( ) { return read(this + 0xB4, Vector3); }
	Vector3 swimSpeed( ) { return read(this + 0xC0, Vector3); }
	BasePlayer* owner( ) { return (BasePlayer*)read(this + 0xD0, uintptr_t); }
	uintptr_t sourceWeaponPrefab( ) { return read(this + 0xD8, uintptr_t); }
	uintptr_t sourceProjectilePrefab( ) { return read(this + 0xE0, uintptr_t); }
	uintptr_t mod( ) { return read(this + 0xE8, uintptr_t); }
	int projectileID( ) { return read(this + 0xF0, int); }
	uint32_t seed( ) { return read(this + 0xF4, uint32_t); }
	bool clientsideEffect( ) { return read(this + 0xF8, bool); }
	bool clientsideAttack( ) { return read(this + 0xF9, bool); }
	float integrity( ) { return read(this + 0xFC, float); }
	float maxDistance( ) { return read(this + 0x100, float); }
	uintptr_t modifier( ) { return read(this + 0x104, uintptr_t); }
	bool invisible( ) { return read(this + 0x114, bool); }
	Vector3 currentVelocity( ) { return read(this + 0x118, Vector3); }
	Vector3 currentPosition( ) { return read(this + 0x124, Vector3); }
	float traveledDistance( ) { return read(this + 0x130, float); }
	float traveledTime( ) { return read(this + 0x134, float); }
	float launchTime( ) { return read(this + 0x138, float); }
	Vector3 sentPosition( ) { return read(this + 0x13C, Vector3); }
	Vector3 previousPosition( ) { return read(this + 0x148, Vector3); }
	Vector3 previousVelocity( ) { return read(this + 0x154, Vector3); }
	float previousTraveledTime( ) { return read(this + 0x160, float); }
	bool isRicochet( ) { return read(this + 0x164, bool); }
	bool isRetiring( ) { return read(this + 0x165, bool); }
	bool flybyPlayed( ) { return read(this + 0x166, bool); }
	bool wasFacingPlayer( ) { return read(this + 0x167, bool); }
	uintptr_t flybyPlane( ) { return read(this + 0x168, uintptr_t); }
	Ray flybyRay( ) { return read(this + 0x178, Ray); }
	uintptr_t cleanupAction( ) { return read(this + 0x190, uintptr_t); }
	HitTest* hitTest( ) { return read(this + 0x198, HitTest*); }
	float swimRandom( ) { return read(this + 0x1A0, float); }

	void initialVelocity(Vector3 a) { write(this + 0x18, a, Vector3); }
	void drag(float a) { write(this + 0x24, a, float); }
	void gravityModifier(float a) { write(this + 0x28, a, float); }
	void thickness(float a) { write(this + 0x2C, a, float); }
	void initialDistance(float a) { write(this + 0x30, a, float); }
	void remainInWorld(bool a) { write(this + 0x34, a, bool); }
	void stickProbability(float a) { write(this + 0x38, a, float); }
	void breakProbability(float a) { write(this + 0x3C, a, float); }
	void conditionLoss(float a) { write(this + 0x40, a, float); }
	void ricochetChance(float a) { write(this + 0x44, a, float); }
	void penetrationPower(float a) { write(this + 0x48, a, float); }
	void damageProperties(uintptr_t a) { write(this + 0x50, a, uintptr_t); }
	void damageDistances(uintptr_t a) { write(this + 0x58, a, uintptr_t); }
	void damageMultipliers(uintptr_t a) { write(this + 0x60, a, uintptr_t); }
	void rendererToScale(uintptr_t a) { write(this + 0x70, a, uintptr_t); }
	void firstPersonRenderer(uintptr_t a) { write(this + 0x78, a, uintptr_t); }
	void createDecals(bool a) { write(this + 0x80, a, bool); }
	void doDefaultHitEffects(bool a) { write(this + 0x81, a, bool); }
	void flybySound(uintptr_t a) { write(this + 0x88, a, uintptr_t); }
	void flybySoundDistance(float a) { write(this + 0x90, a, float); }
	void closeFlybySound(uintptr_t a) { write(this + 0x98, a, uintptr_t); }
	void closeFlybyDistance(float a) { write(this + 0xA0, a, float); }
	void tumbleSpeed(float a) { write(this + 0xA4, a, float); }
	void tumbleAxis(Vector3 a) { write(this + 0xA8, a, Vector3); }
	void swimScale(Vector3 a) { write(this + 0xB4, a, Vector3); }
	void swimSpeed(Vector3 a) { write(this + 0xC0, a, Vector3); }
	void owner(uintptr_t a) { write(this + 0xD0, a, uintptr_t); }
	void sourceWeaponPrefab(uintptr_t a) { write(this + 0xD8, a, uintptr_t); }
	void sourceProjectilePrefab(uintptr_t a) { write(this + 0xE0, a, uintptr_t); }
	void mod(uintptr_t a) { write(this + 0xE8, a, uintptr_t); }
	void projectileID(uint32_t a) { write(this + 0xF0, a, uint32_t); }
	void seed(uint32_t a) { write(this + 0xF4, a, uint32_t); }
	void clientsideEffect(bool a) { write(this + 0xF8, a, bool); }
	void clientsideAttack(bool a) { write(this + 0xF9, a, bool); }
	void integrity(float a) { write(this + 0xFC, a, float); }
	void maxDistance(float a) { write(this + 0x100, a, float); }
	void modifier(uintptr_t a) { write(this + 0x104, a, uintptr_t); }
	void invisible(bool a) { write(this + 0x114, a, bool); }
	void currentVelocity(Vector3 a) { write(this + 0x118, a, Vector3); }
	void currentPosition(Vector3 a) { write(this + 0x124, a, Vector3); }
	void traveledDistance(float a) { write(this + 0x130, a, float); }
	void traveledTime(float a) { write(this + 0x134, a, float); }
	void launchTime(float a) { write(this + 0x138, a, float); }
	void sentPosition(Vector3 a) { write(this + 0x13C, a, Vector3); }
	void previousPosition(Vector3 a) { write(this + 0x148, a, Vector3); }
	void previousVelocity(Vector3 a) { write(this + 0x154, a, Vector3); }
	void previousTraveledTime(float a) { write(this + 0x160, a, float); }
	void isRicochet(bool a) { write(this + 0x164, a, bool); }
	void isRetiring(bool a) { write(this + 0x165, a, bool); }
	void flybyPlayed(bool a) { write(this + 0x166, a, bool); }
	void wasFacingPlayer(bool a) { write(this + 0x167, a, bool); }
	void flybyPlane(uintptr_t a) { write(this + 0x168, a, uintptr_t); }
	void flybyRay(Ray a) { write(this + 0x178, a, Ray); }
	void cleanupAction(uintptr_t a) { write(this + 0x190, a, uintptr_t); }
	void hitTest(uintptr_t a) { write(this + 0x198, a, uintptr_t); }
	void swimRandom(float a) { write(this + 0x1A0, a, float); }

	bool DoHit(HitTest* test, Vector3 point, Vector3 normal) {
		static auto off = METHOD("Assembly-CSharp::Projectile::DoHit(HitTest,Vector3,Vector3): Boolean");
		return reinterpret_cast<bool(__fastcall*)(Projectile*, HitTest*, Vector3, Vector3)>(off)(this, test, point, normal);
	}
	bool isAuthoritative( ) {
		if (!this) return false;
		static auto off = METHOD("Assembly-CSharp::Projectile::get_isAuthoritative(): Boolean");
		return reinterpret_cast<bool(__fastcall*)(Projectile*)>(off)(this);
	}
};
namespace utils {
	bool w2s(Vector3 world, Vector2& screen) {
		const auto matrix = il2cpp::unity::getViewMatrix( ).transpose( );

		const Vector3 translation = {matrix[ 3 ][ 0 ], matrix[ 3 ][ 1 ], matrix[ 3 ][ 2 ]};
		const Vector3 up = {matrix[ 1 ][ 0 ], matrix[ 1 ][ 1 ], matrix[ 1 ][ 2 ]};
		const Vector3 right = {matrix[ 0 ][ 0 ], matrix[ 0 ][ 1 ], matrix[ 0 ][ 2 ]};

		const auto w = translation.dot_product(world) + matrix[ 3 ][ 3 ];

		if (w < 0.1f)
			return false;

		const auto x = right.dot_product(world) + matrix[ 0 ][ 3 ];
		const auto y = up.dot_product(world) + matrix[ 1 ][ 3 ];

		screen =
		{
			(vars::stuff::ScreenWidth / 2) * (1.f + x / w),
			(vars::stuff::ScreenHeight / 2) * (1.f - y / w)
		};

		return true;
	}
	bool OOF(BasePlayer* ply) {
		Vector3 pos = ply->get_bone_pos(head);
		Vector2 screen;

		if (!utils::w2s(pos, screen))
			return true;

		float num = Math::Distance_2D(screen_center, screen);
		return num > 1000.f;
	}
	Vector3 GetEntityPosition(std::uint64_t entity) {
		if (!entity) return Vector3::Zero( );

		uintptr_t plyVis = read(entity + 0x8, uintptr_t);
		if (!plyVis) return Vector3::Zero( );

		uintptr_t visualState = read(plyVis + 0x38, uintptr_t);
		if (!visualState) return Vector3::Zero( );

		Vector3 ret = read(visualState + 0x90, Vector3);
		return ret;
	}
	Vector3 ClosestPoint(BasePlayer* player, Vector3 vec) {
		typedef Vector3(__stdcall* CPoint)(BasePlayer*, Vector3);
		Vector3 result = ((CPoint)(vars::stor::gBase + CO::ClosestPoint))(player, vec);
		return result;
	}
	bool LineOfSight(Vector3 a, Vector3 b) {
		typedef bool(__stdcall* LOS)(Vector3, Vector3, int, float);
		bool result = ((LOS)(vars::stor::gBase + CO::LineOfSight))(a, b, 2162688 | 8388608 | 2097152, 0.f);
		return result;
	}
	uintptr_t ShaderFind(Str name) {
		typedef uintptr_t(__stdcall* ShaderFind)(Str);
		uintptr_t result = ((ShaderFind)(vars::stor::gBase + CO::ShaderFind))(name);
		return result;
	}
	void ServerRPC(DWORD64 ent, Str func) {
		typedef void(__stdcall* Pick)(DWORD64, Str);
		return ((Pick)(vars::stor::gBase + CO::ServerRPC))(ent, func);
	}
	class StringPool {
	public:
		static uint32_t Get(const char* str) {
			static auto off = METHOD("Assembly-CSharp::StringPool::Get(String): UInt32");
			return reinterpret_cast<uint32_t(__fastcall*)(il2cpp::String*)>(off)(il2cpp::String::New(str));
		}

		static il2cpp::String* Get(uint32_t i) {
			static auto off = METHOD("Assembly-CSharp::StringPool::Get(UInt32): String");
			return reinterpret_cast<il2cpp::String * (__fastcall*)(uint32_t)>(off)(i);
		}
	};
}
// need 2 keep here 4 now || TO-DO move up
class Model : public Component {
public:
	FIELD("Assembly-CSharp::Model::boneTransforms", boneTransforms, Array<Transform*>*);
	//FIELD("Assembly-CSharp::BoneDictionary::hashDict", hashDict, List<int, Transform*>*);
	FIELD("Assembly-CSharp::Model::boneNames", boneNames, Array<il2cpp::String*>*);

	Bone* resolve(uint32_t hash) {
		if (!this) return nullptr;

		if (!this->boneNames( ) || !this->boneTransforms( )) return nullptr;

		auto bone_names = this->boneNames( );
		auto bone_transforms = this->boneTransforms( );

		for (int i = 0; i < bone_names->size( ); i++) {
			auto bone_name = bone_names->get(i);
			auto bone_transform = bone_transforms->get(i);
			if (!bone_name || !bone_transform) continue;
			
			if (RUNTIME_CRC32_W(bone_name->buffer) == hash/*wcscmp(bone_name->buffer, name) == 0*/)
				return new Bone(bone_transform->position( ), utils::LineOfSight(bone_transform->position( ), LocalPlayer::Entity( )->eyes()->get_position()));
		}

		return nullptr;
	}
};