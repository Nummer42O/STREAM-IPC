#pragma once

#include "ipc/common.hpp"


struct NamespaceRequest {
  MAKE_MSG_TYPE;

  requestId_t id;
  MAKE_STRING(path);
  bool updates;
};

struct NamespaceResponse {
  MAKE_MSG_TYPE;

  MAKE_STRING_ARRAY(children);
  uint32_t nrOfChildren;
};
