#pragma warning(disable : 4530)
#define _CRT_SECURE_NO_WARNINGS

#define noauth
//#define disable

#include <stdint.h>
#include <Windows.h>
#include <psapi.h>
#include <d3d11.h>
#include <string>
#include <codecvt>
#include <locale>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <emmintrin.h>
#include "winternl.h"
#include <iostream>
#include <vector>
#include <fstream>
#include "stuff/offsets.h"
#include "crc32.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <signal.h>
#include <sstream>
#include <stdio.h>
#include <thread>
#include "stuff/xorstr.h"
#include <comdef.h>
#include "menu/imgui/imgui.h"
#include <Lmcons.h>
#include "stuff/mem.h"
#include "stuff/structs.h"
#include "vector.h"
#include "stuff/math.h"
#include "stuff/vars.h"
#include "main/weapons.h"
#include "class.h"
#include "stuff/il2cpp.h"
#include "rust.h"
#include "main/f_object.h"
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include "init/d3defs.h"
#include "rendering/rendering.h"
#include "menu/imgui/imgui_impl_win32.h"
#include "menu/imgui/imgui_impl_dx11.h"
#include "menu/imgui/imgui_internal.h"
#include "menu/imgui/imgui_elements.h"
#include "hooks/minhook/include/MinHook.h"
#include "logsystem.h"
#include "main/aim.h"
#include "main/misc.h"
#include "main/esp.h"
#include "gamethread.h"
#include "hooks/hooks.h"
#include "config/config.h"
#include "main/entities.h"
#include "init/pre_draw.h"

#include "menu/menu.h"
#include "init/present.h"
#pragma comment(lib, "winmm.lib")