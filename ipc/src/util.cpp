#include <cassert>
#include <cstring>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "util.hpp"
#include "ipc/ipc-exceptions.hpp"


namespace util {

int getMsgQueueId(int projectId, bool create)
{
  assert(projectId & 0x0000FFFF > 0);

  key_t key = ::ftok(KEY_LOCATION, projectId);
  if (key == -1)
    throw IpcException("ftok");

  int msgQueueId = ::msgget(key, 0666 | (create ? IPC_CREAT : 0));
  if (msgQueueId == -1)
    throw IpcException("msgget");

  return msgQueueId;
}

template<typename T>
bool sendMsg(int msgQueueId, const T &payload, bool wait)
{
  int status = ::msgsnd(
    msgQueueId,
    &payload, sizeof(T) - sizeof(msgKey_t),
    wait ? 0 : IPC_NOWAIT
  );
  if (status == -1)
  {
    if (errno == EAGAIN)
      return false;

    throw IpcException("msgsnd");
  }

  return true;
}
DECLARE_TEMPLATE_SEND_MSG(TestRequest);
DECLARE_TEMPLATE_SEND_MSG(TestResponse);

template<typename T>
ssize_t recieveMsg(int msgQueueId, T &payload, long msgType, bool wait)
{
  ssize_t nrBytes = ::msgrcv(
    msgQueueId,
    &payload, sizeof(T), msgType,
    wait ? 0 : IPC_NOWAIT
  );
  if (nrBytes == -1)
    throw IpcException("msgrcv");

  return nrBytes;
}
DECLARE_TEMPLATE_RECIEVE_MSG(TestRequest);
DECLARE_TEMPLATE_RECIEVE_MSG(TestResponse);

std::string to_string(const char *src, size_t size)
{
  return std::string(src, ::strnlen(src, size));
}

msgKey_t makeMsgKey(msgType_t msgType, pid_t pid)
{
  msgKey_t \
    msb = (static_cast<msgKey_t>(msgType) << 32 & 0xFFFFFFFF00000000),
    lsb = (static_cast<msgKey_t>(pid) & 0x00000000FFFFFFFF);
  return msb | lsb;
}

}