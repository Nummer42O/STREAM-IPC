#pragma once

#include "ipc/common.hpp"

struct NamespaceRequest {
  msgKey_t key;
  requestId_t id;
  char path[MAX_STRING_LENGTH];
  bool updates;
};