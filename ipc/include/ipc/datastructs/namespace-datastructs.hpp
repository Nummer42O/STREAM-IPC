#pragma once

#include "ipc/common.hpp"

struct NamespaceDataRequest {
  requestId_t id;
  std::string path;
  bool updates;
};

struct NamespaceDataResponse {
  uint32_t nrOfAccChildren;
  char children[MAX_ARRAY_LENGTH][MAX_STRING_LENGTH];
};