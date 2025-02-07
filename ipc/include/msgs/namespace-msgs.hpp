#pragma once

#include "ipc/common.hpp"

struct NamespaceRequest {
  msgKey_t key;
  requestId_t id;
  char path[MAX_STRING_LENGTH];
  bool updates;
};
#define NAMESPACE_REQUEST_MSG_TYPE __COUNTER__

struct NamespaceResponse {
  msgKey_t key;
  uint32_t nrOfAccChildren;
  char children[MAX_ARRAY_LENGTH][MAX_STRING_LENGTH];
};
#define NAMESPACE_RESPONSE_MSG_TYPE __COUNTER__