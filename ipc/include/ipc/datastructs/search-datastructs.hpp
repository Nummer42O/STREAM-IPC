#pragma once

#include "ipc/common.hpp"

struct SearchDataRequest {
  enum Type : uint8_t {
    NODE,
    TOPIC
  } type;
  std::string name;
};

struct SearchDataResponse {
  primaryKey_t primaryKey;
};