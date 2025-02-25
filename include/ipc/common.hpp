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


namespace ipc
{

static const uint32_t _INITIAL_COUNTER_VALUE = __COUNTER__;
#define GET_COUNTER (__COUNTER__ - _INITIAL_COUNTER_VALUE)

/**
 * @addtogroup ipc
 * @{
 */

//! Type for the message key field. Usually called msgtyp, we encode sender-PID and a message type into it.
using msgKey_t                = __syscall_slong_t;
//! Type for _our_ msg type macro values.
using msgType_t               = int32_t;
//! Type for node/topic(/process) primary keys.
using primaryKey_t            = uint32_t;
//! Type for request ID.
using requestId_t             = uint32_t;
//! Type for identifier of shared memory location.
using sharedMemoryLocation_t  = uint32_t; //! TODO: needs actual type

//! @}

}
