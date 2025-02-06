#include "ipc/ipc-exceptions.hpp"

#include <sstream>
#include <cstring>


IpcException::IpcException(std::string_view sourceFunction)
{
  mMsg = (std::stringstream() \
    << "Error " << strerrorname_np(errno) \
    << " in " << sourceFunction << ": " \
    << std::strerror(errno)
  ).str();
}

const char *IpcException::what() const noexcept
{
  return mMsg.c_str();
}
