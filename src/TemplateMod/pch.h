#pragma once

//This makes Windows.h include only necessary files
//https://learn.microsoft.com/en-us/windows/win32/winprog/using-the-windows-headers#faster-builds-with-smaller-header-files
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>
//Include BlueBlur API by Skyth
#include <BlueBlur.h>
//Definitions of macros for HOOKs
#include <Helpers.h>
//Detours (necessary for Helpers.h)
#include <detours.h>