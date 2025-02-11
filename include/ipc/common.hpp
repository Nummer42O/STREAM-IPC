#pragma once

#include <sys/types.h>
#include <cstdint>

#define MAX_STRING_SIZE 64ul
#define MAX_ARRAY_SIZE  16ul

#define MAKE_STRING(fieldName) \
  char fieldName[MAX_STRING_SIZE] = {0}
#define MAKE_ARRAY(fieldType, fieldName) \
  fieldType fieldName[MAX_ARRAY_SIZE] = {0}
#define MAKE_STRING_ARRAY(fieldName) \
  char fieldName[MAX_ARRAY_SIZE][MAX_STRING_SIZE] = {0}

static const uint32_t _INITIAL_COUNTER_VALUE = __COUNTER__;
#define MAKE_MSG_TYPE \
  static const msgType_t msgType = (__COUNTER__ - _INITIAL_COUNTER_VALUE)

using msgKey_t                = __syscall_slong_t;
using msgType_t               = int32_t;
using primaryKey_t            = uint32_t;
using requestId_t             = uint32_t;
using sharedMemoryLocation_t  = uint32_t; //! TODO: needs actual type
