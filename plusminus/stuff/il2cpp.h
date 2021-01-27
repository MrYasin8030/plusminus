#include <map>
#include "lazyimp.hpp"
namespace il2cpp {
	namespace wrapper {
		auto gameAssembly = GetModuleHandleA(xorstr("GameAssembly.dll"));

#define ProcAddr(func) GetProcAddress(gameAssembly, func)

		template<typename T, typename... Args>
		inline T call(const char* func, Args... args) {
			return reinterpret_cast<T(__fastcall*)(Args...)>(ProcAddr(func))(args...);
		}

		class String {
		public:
			char pad_0000[0x10];
			int len;
			wchar_t buffer[0];

			static String* New(const char* str) {
				return call<String*, const char*>(xorstr("il2cpp_string_new"), str);
			}
		};

		class Il2CppType {
		public:
			char* name() {
				return call<char*, Il2CppType*>(xorstr("il2cpp_type_get_name"), this);
			}
		};

		class Il2CppMethod {
		public:
			uint32_t paramCount() {
				return call<uint32_t, Il2CppMethod*>(xorstr("il2cpp_method_get_param_count"), this);
			}

			Il2CppType* retType() {
				return call<Il2CppType*, Il2CppMethod*>(xorstr("il2cpp_method_get_return_type"), this);
			}

			Il2CppType* getParam(uint32_t idx) {
				return call<Il2CppType*, Il2CppMethod*, uint32_t>(xorstr("il2cpp_method_get_param"), this, idx);
			}

			char* name() {
				return call<char*, Il2CppMethod*>(xorstr("il2cpp_method_get_name"), this);
			}
		};

		uint64_t il2cpp_resolve_icall(const char* str) {
			return call<uint64_t, const char*>(xorstr("il2cpp_resolve_icall"), str);
		}

		class Il2CppField {
		public:
			char* name() {
				return call<char*, Il2CppField*>(xorstr("il2cpp_field_get_name"), this);
			}

			uint32_t offset() {
				return call<uint32_t, Il2CppField*>(xorstr("il2cpp_field_get_offset"), this);
			}
		};

		class Il2CppClass {
		public:
			class Il2CppImage* image; //0x0000
			char pad_0008[8]; //0x0008
			char* name; //0x0010
			char* namespaze; //0x0018
			char pad_0020[152]; //0x0020
			void* static_fields; //0x00B8

			Il2CppMethod* methods(void* iter) {
				return call<Il2CppMethod*, Il2CppClass*, void*>(xorstr("il2cpp_class_get_methods"), this, iter);
			}

			Il2CppField* fields(void* iter) {
				return call<Il2CppField*, Il2CppClass*, void*>(xorstr("il2cpp_class_get_fields"), this, iter);
			}
		}; //Size: 0x00C0

		class Il2CppImage {
		public:
			char* assemblyName; //0x0000
			char* name; //0x0008

			uint64_t classcount() {
				return call<uint64_t, Il2CppImage*>(xorstr("il2cpp_image_get_class_count"), this);
			}

			Il2CppClass* get_class(uint64_t idx) {
				return call<Il2CppClass*, Il2CppImage*, uint64_t>(xorstr("il2cpp_image_get_class"), this, idx);
			}
		}; //Size: 0x0010

		template<typename T = uint64_t>
		T il2cpp_array_new(Il2CppClass* klazz, uint64_t length) {
			return call<T, Il2CppClass*, uint64_t>(xorstr("il2cpp_array_new"), klazz, length);
		}

		class Il2CppAssembly {
		public:
			uint64_t buffer;
		};

		class Il2CppDomain {
		public:
			size_t assemblyCount() {
				size_t size = 0;
				auto assemblies = call<Il2CppAssembly**, Il2CppDomain*, void*>(xorstr("il2cpp_domain_get_assemblies"), this, &size);

				return size;

			}

			Il2CppAssembly** assemblies() {
				size_t size = 0;

				return call<Il2CppAssembly**, Il2CppDomain*, void*>(xorstr("il2cpp_domain_get_assemblies"), this, &size);
			}
		};

		Il2CppDomain* il2cpp_domain_get() {
			return call<Il2CppDomain*>(xorstr("il2cpp_domain_get"));
		}

		void* il2cpp_runtime_invoke(void* method_ptr, void* obj, void** param, void** exc) {
			return call<void*, void*, void*, void**, void**>("il2cpp_runtime_invoke", method_ptr, obj, param, exc);
		}

		void* il2cpp_object_get_virtual_method(void* obj, void* method) {
			return call<void*, void*, void*>("il2cpp_object_get_virtual_method", obj, method);
		}
	}
	namespace methods {
		template<typename T1, typename T2> bool map_contains_key(T1 map, T2 key) {
			return map.count(key) > 0;
		}
		using il2cpp_domain_get = uintptr_t(*)();
		static auto domain_get = LI_FIND_DEF(il2cpp_domain_get);
		using il2cpp_class_get_methods = uintptr_t(*)(uintptr_t, uintptr_t*);
		static auto class_get_methods = LI_FIND_DEF(il2cpp_class_get_methods);
		using il2cpp_method_get_param_count = int (*)(uintptr_t);
		static auto method_get_param_count = LI_FIND_DEF(il2cpp_method_get_param_count);
		using il2cpp_assembly_get_image = uintptr_t(*)(uintptr_t);
		static auto assembly_get_image = LI_FIND_DEF(il2cpp_assembly_get_image);
		using il2cpp_domain_get_assemblies = uintptr_t * (*)(void* domain, uintptr_t* size);
		static auto domain_get_assemblies = LI_FIND_DEF(il2cpp_domain_get_assemblies);
		using il2cpp_object_new = uintptr_t(*)(uintptr_t);
		static auto object_new = LI_FIND_DEF(il2cpp_object_new);
		using il2cpp_class_from_name = uintptr_t(*)(uintptr_t, const char*, const char*);
		static auto class_from_name = LI_FIND_DEF(il2cpp_class_from_name);
		using il2cpp_resolve_icall = uintptr_t(*)(const char*);
		static auto resolve_icall = LI_FIND_DEF(il2cpp_resolve_icall);
		using il2cpp_field_static_get_value = uintptr_t(*)(uintptr_t, uintptr_t*);
		static auto field_static_get_value = LI_FIND_DEF(il2cpp_field_static_get_value);
		using il2cpp_class_get_fields = uintptr_t(*)(uintptr_t, uintptr_t*);
		static auto class_get_fields = LI_FIND_DEF(il2cpp_class_get_fields);
		using il2cpp_field_get_offset = uintptr_t(*)(uintptr_t);
		static auto field_get_offset = LI_FIND_DEF(il2cpp_field_get_offset);
		using il2cpp_runtime_class_init = uintptr_t(*)(uintptr_t);
		static auto runtime_class_init = LI_FIND_DEF(il2cpp_runtime_class_init);
		using il2cpp_string_new_wrapper = uintptr_t(*)(const char*);
		static auto new_string = LI_FIND_DEF(il2cpp_string_new_wrapper);
	}
	DWORD64 il2cpp_object_new(DWORD64 klass) {
		DWORD64 objnew = il2cpp::methods::object_new(klass);
		return objnew;
	}
	uintptr_t init_class(const char* name, const char* name_space = c_xor("")) {
		uintptr_t domain = il2cpp::methods::domain_get();
		uintptr_t nrofassemblies = 0;
		uintptr_t* assemblies;
		assemblies = il2cpp::methods::domain_get_assemblies((void*)domain, &nrofassemblies);
		for (int i = 0; i < nrofassemblies; i++) {
			uintptr_t img = il2cpp::methods::assembly_get_image(assemblies[i]);
			uintptr_t kl = il2cpp::methods::class_from_name(img, name_space, name);
			if (!kl) continue;
			return kl;
		}
		return 0;
	}
	uintptr_t il2cpp_field(uintptr_t klass, char* field_name, bool get_offset = true) {
		uintptr_t out = 0;
		uintptr_t il2cpp_field;
		while (il2cpp_field = il2cpp::methods::class_get_fields(klass, &out)) {
			char* name = (char*)*reinterpret_cast<uintptr_t*>(il2cpp_field);
			if (!name)
				break;
			if (!m_strcmp(name, field_name)) {
				continue;
			}
			if (!get_offset)
				return il2cpp_field;
			uintptr_t offset = il2cpp::methods::field_get_offset(il2cpp_field);
			return offset;
		}
		return 0;
	}
	uintptr_t il2cpp_value(const char* kl, char* name, bool get_offset = true) {
		auto klass = init_class(kl);
		if (get_offset) {
			auto field_offset = il2cpp_field(klass, name);
			return field_offset;
		}
		else {
			auto _field = il2cpp_field(klass, name, false);
			uintptr_t ret;
			il2cpp::methods::field_static_get_value(_field, &ret);
			return ret;
		}
		return 0;
	}
	uintptr_t il2cpp_method(const char* kl, const char* name, int argument_number = -1, char* arg_name = "", const char* name_space = "", int selected_argument = -1) {
		uintptr_t iter = 0;
		uintptr_t f;
		auto klass = init_class(kl, name_space);
		while (f = il2cpp::methods::class_get_methods(klass, &iter)) {
			char* st = *reinterpret_cast<char**>(f + 0x10);
			if (m_strcmp(st, (char*)name)) {
				if (selected_argument >= 0 && arg_name) {
					uintptr_t args = read(f + 0x28, uintptr_t);
					int method_count = il2cpp::methods::method_get_param_count(f);
					if (selected_argument > method_count || (argument_number >= 0 && method_count != argument_number)) continue;
					char* argname;
					if (method_count > 0) {
						argname = *reinterpret_cast<char**>(args + (selected_argument - 1) * 0x18);
					}
					else
						argname = (char*)c_xor("-");
					if (!argname || !m_strcmp(argname, arg_name)) continue;
				}
				return f;
			}
		}
		return 0;
	}
	using namespace wrapper;
	static std::map<uint32_t, uint64_t> offsets = std::map<uint32_t, uint64_t>();
	uint64_t method(uint32_t path) {
		if (methods::map_contains_key(offsets, path))
			return offsets[path];
		wrapper::Il2CppDomain* domain = wrapper::il2cpp_domain_get();
		wrapper::Il2CppAssembly** assemblies = domain->assemblies();
		for (int i = 0; i < domain->assemblyCount(); i++) {
			wrapper::Il2CppImage* image = *reinterpret_cast<wrapper::Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
			for (int c = 0; c < image->classcount(); c++) {
				std::string temp(image->assemblyName);
				temp.erase(temp.find(xorstr(".dll")), 4);
				wrapper::Il2CppClass* klass = image->get_class(c);
				if (!klass) continue;
				char* name = klass->name;
				char* ns = klass->namespaze;
				if (std::string(ns).empty())
					temp = temp + c_xor("::") + name;
				else
					temp = temp + c_xor("::") + ns + c_xor("::") + name;
				wrapper::Il2CppMethod* mthd;
				void* iter = NULL;
				while (mthd = klass->methods(&iter)) {
					if (!mthd) continue;
					std::string temp2(temp + c_xor("::") + mthd->name());
					if (mthd->paramCount() > 0) {
						temp2 = temp2 + c_xor("(");
						for (int p = 0; p < mthd->paramCount(); p++) {
							std::string t(mthd->getParam(p)->name());
							t = t.substr(t.find(xorstr(".")) + 1);
							temp2 = temp2 + t + c_xor(",");
						}
						std::string t(mthd->retType()->name());
						temp2 = temp2.substr(0, temp2.size() - 1);
						temp2 = temp2 + c_xor("): ") + t.substr(t.find(".") + 1);
					}
					else {
						std::string t(mthd->retType()->name());
						temp2 = temp2 + c_xor("(): ") + t.substr(t.find(".") + 1);
					}
					if (RUNTIME_CRC32(temp2.c_str()) == path) {
						offsets.insert(std::make_pair(path, std::uint64_t(mthd)));
						return std::uint64_t(mthd);
					}
				}
			}
		}
		return 0;
	}
	uint64_t field(uint32_t path) {
		if (il2cpp::methods::map_contains_key(offsets, path))
			return std::uint32_t(offsets[path]);

		Il2CppDomain* domain = il2cpp_domain_get();
		Il2CppAssembly** assemblies = domain->assemblies();

		for (int i = 0; i < domain->assemblyCount(); i++) {
			Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
			for (int c = 0; c < image->classcount(); c++) {
				std::string temp(image->assemblyName);
				temp.erase(temp.find(c_xor(".dll")), 4);

				Il2CppClass* klass = image->get_class(c);
				char* name = klass->name;
				char* ns = klass->namespaze;
				if (std::string(ns).empty())
					temp = temp + c_xor("::") + name;
				else
					temp = temp + c_xor("::") + ns + c_xor("::") + name;

				Il2CppField* field;
				void* iter = NULL;
				while (field = klass->fields(&iter)) {
					if (!field) continue;

					std::string t(temp + c_xor("::") + field->name());
					if (RUNTIME_CRC32(t.c_str()) == path) {
						uint32_t off = field->offset();
						offsets.insert(std::make_pair(path, off));

						return off;
					}
				}
			}
		}

		return 0;
	}
	wrapper::Il2CppClass* klass(uint32_t path) {
		if (il2cpp::methods::map_contains_key(offsets, path))
			return reinterpret_cast<wrapper::Il2CppClass*>(offsets[path]);
		wrapper::Il2CppDomain* domain = wrapper::il2cpp_domain_get();
		wrapper::Il2CppAssembly** assemblies = domain->assemblies();
		for (int i = 0; i < domain->assemblyCount(); i++) {
			wrapper::Il2CppImage* image = *reinterpret_cast<wrapper::Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
			for (int c = 0; c < image->classcount(); c++) {
				std::string temp(image->assemblyName);
				temp.erase(temp.find(xorstr(".dll")), 4);
				wrapper::Il2CppClass* klass = image->get_class(c);
				char* name = klass->name;
				char* ns = klass->namespaze;
				if (std::string(ns).empty())
					temp = temp + c_xor("::") + name;
				else
					temp = temp + c_xor("::") + ns + c_xor("::") + name;
				if (path == RUNTIME_CRC32(temp.c_str())) {
					uint64_t ptr = std::uint64_t(klass);
					offsets.insert(std::make_pair(path, ptr));
					return klass;
				}
			}
		}
		return nullptr;
	}
	
	void init_classes() {
		Il2CppDomain* domain = il2cpp_domain_get();
		Il2CppAssembly** assemblies = domain->assemblies();

		for (int i = 0; i < domain->assemblyCount(); i++) {
			Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
			for (int c = 0; c < image->classcount(); c++) {
				std::string temp(image->assemblyName);
				temp.erase(temp.find(xorstr(".dll")), 4);

				Il2CppClass* klass = image->get_class(c);
				char* name = klass->name;
				char* ns = klass->namespaze;
				if (std::string(ns).empty())
					temp = temp + c_xor("::") + name;
				else
					temp = temp + c_xor("::") + ns + c_xor("::") + name;

				uint64_t ptr = std::uint64_t(klass);

				offsets.insert(std::make_pair(RUNTIME_CRC32(temp.c_str()), ptr));
			}
		}
	}
	void init_methods() {
		Il2CppDomain* domain = il2cpp_domain_get();
		Il2CppAssembly** assemblies = domain->assemblies();

		for (int i = 0; i < domain->assemblyCount(); i++) {
			Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
			for (int c = 0; c < image->classcount(); c++) {
				std::string temp(image->assemblyName);
				temp.erase(temp.find(xorstr(".dll")), 4);

				Il2CppClass* klass = image->get_class(c);
				if (!klass) continue;

				char* name = klass->name;
				char* ns = klass->namespaze;
				if (std::string(ns).empty())
					temp = temp + c_xor("::") + name;
				else
					temp = temp + c_xor("::") + ns + c_xor("::") + name;

				Il2CppMethod* mthd;
				void* iter = NULL;
				while (mthd = klass->methods(&iter)) {
					if (!mthd) continue;

					std::string temp2(temp + c_xor("::") + mthd->name());

					if (mthd->paramCount() > 0) {
						temp2 = temp2 + c_xor("(");
						for (int p = 0; p < mthd->paramCount(); p++) {
							std::string t(mthd->getParam(p)->name());
							t = t.substr(t.find(c_xor(".")) + 1);
							temp2 = temp2 + t + c_xor(",");
						}
						std::string t(mthd->retType()->name());
						temp2 = temp2.substr(0, temp2.size() - 1);
						temp2 = temp2 + c_xor("): ") + t.substr(t.find(".") + 1);
					}
					else {
						std::string t(mthd->retType()->name());
						temp2 = temp2 + c_xor("(): ") + t.substr(t.find(".") + 1);
					}

					offsets.insert(std::make_pair(RUNTIME_CRC32(temp2.c_str()), std::uint64_t(mthd)));
				}
			}
		}
	}
	void init_fields() {
		Il2CppDomain* domain = il2cpp_domain_get();
		Il2CppAssembly** assemblies = domain->assemblies();

		for (int i = 0; i < domain->assemblyCount(); i++) {
			Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
			for (int c = 0; c < image->classcount(); c++) {
				std::string temp(image->assemblyName);
				temp.erase(temp.find(c_xor(".dll")), 4);

				Il2CppClass* klass = image->get_class(c);
				char* name = klass->name;
				char* ns = klass->namespaze;
				if (std::string(ns).empty())
					temp = temp + c_xor("::") + name;
				else
					temp = temp + c_xor("::") + ns + c_xor("::") + name;

				Il2CppField* field;
				void* iter = NULL;
				while (field = klass->fields(&iter)) {
					if (!field) continue;

					std::string t(temp + c_xor("::") + field->name());
					uint32_t off = field->offset();
					offsets.insert(std::make_pair(RUNTIME_CRC32(t.c_str()), off));
				}
			}
		}
	}
	class default_t {
	public:
		template<typename T>
		operator T() const { return T(); }
	};

	default_t const defaultt = default_t();
#define METHOD(path) *reinterpret_cast<uint64_t*>(il2cpp::method(STATIC_CRC32(path)))
#define CLASS(path) il2cpp::klass(STATIC_CRC32(path))
#define ASSIGN_HOOK(method_path,hook) hook = reinterpret_cast<decltype(hook)>(METHOD(method_path))
#define OFFSET(path) il2cpp::field(STATIC_CRC32(path))
#define NP(type) type nonptr = il2cpp::defaultt; if(!this) return nonptr;
#define FIELD(field_path,name,type) type& name() { \
		NP(type) \
		static auto off = OFFSET(field_path); \
		return *reinterpret_cast<type*>(this + off); }
	namespace game {
		static auto get_iconSprite = reinterpret_cast<uintptr_t(*)(BaseProjectile* nigga)>(*reinterpret_cast<uintptr_t*>(il2cpp_method(c_xor("Item"), c_xor("get_iconSprite"), 0, xorstr(""), c_xor(""))));
		static auto get_texture = reinterpret_cast<uintptr_t(*)(uintptr_t sprite)>(*reinterpret_cast<uintptr_t*>(il2cpp_method(c_xor("Sprite"), c_xor("get_texture"), 0, xorstr(""), c_xor("UnityEngine"))));
		static auto get_rect = reinterpret_cast<Rect(*)(uintptr_t sprite)>(*reinterpret_cast<uintptr_t*>(il2cpp_method(c_xor("Sprite"), c_xor("get_rect"), 0, xorstr(""), c_xor("UnityEngine"))));
		static auto set_color = reinterpret_cast<void(*)(Color)>(*reinterpret_cast<uintptr_t*>(il2cpp_method(c_xor("GUI"), c_xor("set_color"), -1, xorstr(""), c_xor("UnityEngine"))));
		static auto DrawTexture = reinterpret_cast<void (*)(Rect, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp_method(c_xor("GUI"), c_xor("DrawTexture"), 2, xorstr("image"), c_xor("UnityEngine"), 2)));
	}
	namespace unity {
		static auto set_shader = reinterpret_cast<void(*)(uintptr_t, uintptr_t)>(il2cpp::methods::resolve_icall(xorstr("UnityEngine.Material::set_shader()")));
		static auto IgnoreLayerCollision = reinterpret_cast<void(*)(layer, layer, bool)>(il2cpp::methods::resolve_icall(xorstr("UnityEngine.Physics::IgnoreLayerCollision()")));
	}
	
	static void InitIL() {
		using il2cpp_string_new_wrapper = uintptr_t(*)(const char*);
		methods::new_string = LI_FIND_DEF(il2cpp_string_new_wrapper);
		using il2cpp_domain_get = uintptr_t(*)();
		methods::domain_get = LI_FIND_DEF(il2cpp_domain_get);
		using il2cpp_class_get_methods = uintptr_t(*)(uintptr_t, uintptr_t*);
		methods::class_get_methods = LI_FIND_DEF(il2cpp_class_get_methods);
		using il2cpp_method_get_param_count = int (*)(uintptr_t);
		methods::method_get_param_count = LI_FIND_DEF(il2cpp_method_get_param_count);
		using il2cpp_assembly_get_image = uintptr_t(*)(uintptr_t);
		methods::assembly_get_image = LI_FIND_DEF(il2cpp_assembly_get_image);
		using il2cpp_domain_get_assemblies = uintptr_t * (*)(void* domain, uintptr_t* size);
		methods::domain_get_assemblies = LI_FIND_DEF(il2cpp_domain_get_assemblies);
		using il2cpp_class_from_name = uintptr_t(*)(uintptr_t, const char*, const char*);
		methods::class_from_name = LI_FIND_DEF(il2cpp_class_from_name);
		using il2cpp_resolve_icall = uintptr_t(*)(const char*);
		methods::resolve_icall = LI_FIND_DEF(il2cpp_resolve_icall);
		using il2cpp_field_static_get_value = uintptr_t(*)(uintptr_t, uintptr_t*);
		methods::field_static_get_value = LI_FIND_DEF(il2cpp_field_static_get_value);
		using il2cpp_class_get_fields = uintptr_t(*)(uintptr_t, uintptr_t*);
		methods::class_get_fields = LI_FIND_DEF(il2cpp_class_get_fields);
		using il2cpp_field_get_offset = uintptr_t(*)(uintptr_t);
		methods::field_get_offset = LI_FIND_DEF(il2cpp_field_get_offset);
		using il2cpp_object_new = uintptr_t(*)(uintptr_t);
		methods::object_new = LI_FIND_DEF(il2cpp_object_new);
		using il2cpp_runtime_class_init = uintptr_t(*)(uintptr_t);
		methods::runtime_class_init = LI_FIND_DEF(il2cpp_runtime_class_init);

		game::get_iconSprite = reinterpret_cast<uintptr_t(*)(BaseProjectile * nigga)>(*reinterpret_cast<uintptr_t*>(il2cpp_method(c_xor("Item"), c_xor("get_iconSprite"), 0, xorstr(""), c_xor(""))));
		game::get_texture = reinterpret_cast<uintptr_t(*)(uintptr_t sprite)>(*reinterpret_cast<uintptr_t*>(il2cpp_method(c_xor("Sprite"), c_xor("get_texture"), 0, xorstr(""), c_xor("UnityEngine"))));
		game::get_rect = reinterpret_cast<Rect(*)(uintptr_t sprite)>(*reinterpret_cast<uintptr_t*>(il2cpp_method(c_xor("Sprite"), c_xor("get_rect"), 0, xorstr(""), c_xor("UnityEngine"))));
		game::set_color = reinterpret_cast<void(*)(Color)>(*reinterpret_cast<uintptr_t*>(il2cpp_method(c_xor("GUI"), c_xor("set_color"), -1, xorstr(""), c_xor("UnityEngine"))));
		game::DrawTexture = reinterpret_cast<void (*)(Rect, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp_method(c_xor("GUI"), c_xor("DrawTexture"), 2, xorstr("image"), c_xor("UnityEngine"), 2)));

		unity::set_shader = reinterpret_cast<void(*)(uintptr_t, uintptr_t)>(il2cpp::methods::resolve_icall(xorstr("UnityEngine.Material::set_shader()")));
		unity::IgnoreLayerCollision = reinterpret_cast<void(*)(layer, layer, bool)>(il2cpp::methods::resolve_icall(xorstr("UnityEngine.Physics::IgnoreLayerCollision()")));
	}
}