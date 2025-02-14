#pragma once

#include <exception>
#include <string_view>
#include <string>

namespace ipc
{

class IpcException: public std::exception
{
public:
  IpcException(std::string_view sourceFunction);
  const char *what() const noexcept;

private:
  std::string mMsg;
};

}
