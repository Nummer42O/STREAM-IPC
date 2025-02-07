#pragma once

#include "ipc/common.hpp"

struct SearchData {
  enum Type {
    NODE,
    TOPIC
  } type;
  std::string name;
};