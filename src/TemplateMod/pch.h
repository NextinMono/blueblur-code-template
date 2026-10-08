#pragma once

//This makes Windows.h include only necessary files
//https://learn.microsoft.com/en-us/windows/win32/winprog/using-the-windows-headers#faster-builds-with-smaller-header-files
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
//Include BlueBlur API by Skyth
#include <BlueBlur.h>
//Include CommonLib by Hyper, this has all the necessary
//macros and utilities to make code mods.
#include <CommonLib.h>