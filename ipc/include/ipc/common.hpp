#pragma once

#include <sys/types.h>
#include <cstdint>

#define MAX_STRING_LENGTH 64
#define MAX_ARRAY_LENGTH 16

using msgKey_t  = __syscall_slong_t;    // key consists PID+msgType
using msgType_t = int32_t;
using primaryKey_t = uint32_t;
using requestId_t = uint32_t;