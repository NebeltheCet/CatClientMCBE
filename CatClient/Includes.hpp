#pragma once

/* Defines */
//Used for the Signature Function
#define INRANGE(x,a,b)   (x >= a && x <= b)
#define GET_BITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))

/* Includes */
#include <Windows.h>
#include <Psapi.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

/* External Includes */
#include "SDK/Minhook/minhook.h"

#include "SDK/Utils/Utils.hpp"

#include "Hooks/Hooks.hpp"