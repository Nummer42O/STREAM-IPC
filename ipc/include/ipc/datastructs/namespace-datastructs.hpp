#pragma once

#include "ipc/common.hpp"

struct NamespaceData {
  requestId_t id;
  std::string path;
  bool updates;
};