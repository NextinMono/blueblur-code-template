//HE1ML
#include <ModLoader.h>

/// Code modding in Sonic Generations now takes advantage of Hyper's CommonLib
/// If you have any questions, you can check the documentation page
/// https://hedge-dev.github.io/CommonLib/index.html


/// Basic hook structure (applies to __cdecl, __fastcall, __stdcall)
///  1. Return type
///  2. Calling convention, which can be __cdecl, __fastcall, __stdcall. In the case you need to hook 
///     __thiscall, __usercall or __userpurge, see the other examples.
///  3. Function name (you can set it to anything you like)
///  4. Address of the function
///  5+. Function arguments, if the function is __fastcall, the second argument has to be `void* edx`
/// You can then call the original function by doing `original_FunctionName(arguments)`
HOOK(void, __fastcall, Sonic_CApplicationPC_Update, 0xE7BED0, void* This, void* Edx, float elapsedTime, uint8_t a3)
{
	//Call the original function
	original_Sonic_CApplicationPC_Update(This, Edx, elapsedTime, a3);

	// You can use the `FUNCTION_PTR` macros to call functions from the game, there are 2 variants
	// =========================================================================================
	//  - FUNCTION_PTR:			this applies to functions that are *not* optimized, so those 
	//							with a calling convention that isn't __usercall or __userpurge
	//							this includes __thiscall functions, like this
	// 
	//							FUNCTION_PTR(void, __thiscall, SetMusicVolume, 0x75EEF0, void* This, int a2, float volume);
	//  
	//  - USER_FUNCTION_PTR:	this applies to functions that *are* optimized, so those with
	//							__usercall or __userpurge calling conventions. These are trickier to define
	//							as disassemblers like IDA might not get it right on the first try.
	// 
	//							For example, a function with this signature:
	//							`bool __usercall sub_D57980@<al>(Sonic::CGameParameter *a1@<eax>)`
	//							would be defined like this:
	//							USER_FUNCTION_PTR(bool, __usercall, sub_D57980, 0xD57980, USER_RETURN(AL) | USER_PARAM(0, EAX), 1, Sonic::CGameParameter* a1);
	//					

	printf("Hello world!\n");
}

/// Thiscall hook structure (applies only to __thiscall)
/// The hook structure is the same as the one above, but parameters are slightly different
///    1. Return type
///	  2. `THISCALL`, which is a special macro for this purpose
///	  3. Function name (you can set it to anything you like)
///	  4. Address of the function
///	  5. `THISCALL_PARAMS`, it takes in a type and defines an argument called `self` which you can use
///	  6+. Any additional arguments
HOOK(void, THISCALL, MsgEventRequestCreate, 0x10FE910, THISCALL_PARAMS(void), void* in_rMsg)
{
	//To call the original function, you need to use THISCALL_RETURN_PARAMS as the first argument
	original_MsgEventRequestCreate(THISCALL_RETURN_PARAMS, in_rMsg);
}

// User hook structure (applies to __usercall, __userpurge)
// These hooks only apply to *optimized* functions
// These are trickier to define as disassemblers like IDA might not get the signature right on the first try. So they might require
// a little bit of assembly knowledge to get them to work correctly.
// 1. Return type
// 2. Calling convention, which can only be __usercall or __userpurge
// 3. Function name (you can set it to anything you like)
// 4. Address of the function
// 5. USER HOOK parameters, these include:
//		- USER_RETURN(register):		defines the target register for the return value
//		- USER_PARAM(index, register):	defines an argument taken passed by a specific register
// 6. Argument count
// 7+. Function arguments

//This function in particular has the following signature:
//float * __userpurge Sonic::CGameDocument::ComputeScreenPosition@<eax>(float *esi0@<esi>, void *a2, void *a3)
USER_HOOK(float*, __userpurge, ComputeScreenPosition, 0x00D616E0, USER_RETURN(EAX) | USER_PARAM(0, ESI), 3, float* a1, int a2, int a3)
{
	return original_ComputeScreenPosition(a1, a2, a3);
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

	//You can call this to INSTALL (register) hooks, you can also
	//add `STATIC_` before any HOOK macro to make them automatically install on
	//mod initialization.
	INSTALL_HOOK(Sonic_CApplicationPC_Update);

	//USER_HOOKs require their own install macro
	INSTALL_USER_HOOK(ComputeScreenPosition);

	//You can also use some helpers to write directly to addresses in memory, such as:
	//WRITE(0x01E75518, uint8_t, true);
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
