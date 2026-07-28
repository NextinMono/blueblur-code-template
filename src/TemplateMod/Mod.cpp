//HE1ML
#include <ModLoader.h>

/// Basic hook structure
///  1. Return type
///  2. Calling convention, which can be __cdecl, __fastcall, __stdcall. In the case you get a __thiscall, you use __fastcall.
///     However, to call and hook __usercall and __userpurge functions, a bit of assembly is required, and HOOK cannot be used.
///  3. Function name (you can set it to anything you like)
///  4. Address of the function
///  5+. Function arguments, if the function is __fastcall, the second argument has to be `void* edx`
HOOK(void, __fastcall, Sonic_CApplicationPC_Update, 0xE7BED0, void* This, void* Edx, float elapsedTime, uint8_t a3)
{
	//Call this function to call the original function, if this isn't called, the original func will not run.
	originalSonic_CApplicationPC_Update(This, Edx, elapsedTime, a3);

	//You can use this to call functions from the game itself, this is just an example
	//These can also be __thiscall	
	FUNCTION_PTR(void, __thiscall, SetMusicVolume, 0x75EEF0, void* This, int a2, float volume);
	uint32_t* pCSoundModuleManager = *(uint32_t**)0x1E77290;
	float volume = 0.5f;
	SetMusicVolume(pCSoundModuleManager, 0, volume * 0.01f * 0.63f);
}
/// <summary>
/// This is the first thing that runs when the mod is loaded by the mod loader.
/// </summary>
/// <param name="modInfo">Mod and modloader info</param>
extern "C" __declspec(dllexport) void PreInit(ModInfo_t* modInfo)
{
	//This is not necessary, but if your mod requires more than the 2GB of available memory for
	//Generations, uncomment this.
	//https://learn.microsoft.com/en-us/cpp/build/reference/largeaddressaware-handle-large-addresses
	//modInfo->API->SendMessageToLoader(ML_MSG_REQ_LARGE_ADDRESS_AWARE, nullptr);
}
/// <summary>
/// This is the second thing that runs, initialize your hooks and various classes in here.
/// </summary>
/// <param name="modInfo">Mod and modloader info</param>
extern "C" __declspec(dllexport) void Init(ModInfo_t* modInfo)
{
#ifdef _DEBUG
	if(!IsDebuggerPresent())
		MessageBox(nullptr, TEXT("Attach Debugger to see hooks."), TEXT("TemplateMod"), MB_ICONINFORMATION);
#endif

	//This is just some example code to show what to do with ModInfo_t
	printf("Current mod name: %s\n", modInfo->CurrentMod->Name);
	printf("All mods enabled:\n");
	for (auto& mod : *modInfo->ModList)
	{
		printf("\t- \"%s\" | ID: %s\n", mod->Name, mod->ID);
	}

	//You need to call this for every HOOK you make, otherwise it wont run
	INSTALL_HOOK(Sonic_CApplicationPC_Update);
	//You can also use some helpers to write directly to addresses in memory, such as:
	//WRITE_MEMORY(0x01E75518, uint8_t, true);
	//
	//NOTE: the new string must not surpass the original string in length
	//WRITE_STRING(0x0154C180, "Text");
	
	//And you can also write assembly hooks and patch instructions
	//These are necessary for __usercall functions, as these can't be hooked with HOOK
	//WRITE_JUMP(0x(A), 0x(B))
	//WRITE_JUMP(0x(A), Function)
	//WRITE_NOP(0x(A), count);

}
/// <summary>
/// This is the third thing that runs.
/// </summary>
/// <param name="modInfo">Mod and modloader info</param>
extern "C" __declspec(dllexport) void PostInit(ModInfo_t* modInfo)
{

}
/// <summary>
/// This runs every frame, do anything that isn't related to the game's updates here.
/// </summary>
/// <param name="modInfo">Mod and modloader info</param>
extern "C" __declspec(dllexport) void OnFrame(ModInfo_t* modInfo)
{

}
