#pragma once

#include <vector>

#include "ipc/common.hpp"

struct NamespaceDataRequest {
  requestId_t id;
  std::string path;
  bool updates;
};

struct NamespaceDataResponse {
  uint32_t nrOfAccChildren;
  std::vector<std::string> children;
};