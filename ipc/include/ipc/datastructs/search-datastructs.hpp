#pragma once

#include "ipc/common.hpp"

struct SearchRequest {
  enum Type {
    NODE,
    TOPIC
  } type;
  char name[MAX_STRING_LENGTH];
};