#include <cassert>
#include <cstring>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "util.hpp"
#include "ipc/ipc-exceptions.hpp"

#include "ipc/datastructs/information-datastructs.hpp"
#include "ipc/datastructs/namespace-datastructs.hpp"
#include "ipc/datastructs/search-datastructs.hpp"
#include "ipc/datastructs/misc-datastructs.hpp"


namespace util {

using namespace ipc;

int getMsgQueueId(int projectId, bool create)
{
  assert((projectId & 0x000000FF) > 0);

  key_t key = ::ftok(KEY_LOCATION, projectId);
  if (key == -1)
    throw IpcException("ftok");

  int msgQueueId = ::msgget(key, 0666 | (create ? IPC_CREAT : 0));
  if (msgQueueId == -1)
    throw IpcException("msgget");

  return msgQueueId;
}

msgKey_t makeMsgKey(msgType_t msgType, pid_t pid)
{
  msgKey_t \
    msb = (static_cast<msgKey_t>(msgType) << 32 & 0xFFFFFFFF00000000),
    lsb = (static_cast<msgKey_t>(pid) & 0x00000000FFFFFFFF);
  return msb | lsb;
}

msgKey_t makeMsgKey(msgType_t msgType)
{
  msgKey_t \
    msb = (static_cast<msgKey_t>(msgType) << 32 & 0xFFFFFFFF00000000),
    lsb = (static_cast<msgKey_t>(SERVER_PSEUDO_PID) & 0x00000000FFFFFFFF);
  return msb | lsb;
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

template<typename T>
bool receiveMsg(int msgQueueId, T &payload, long msgType, bool wait)
{
  ssize_t nrBytes = ::msgrcv(
    msgQueueId,
    &payload, sizeof(T) - sizeof(msgKey_t),
    msgType,
    wait ? 0 : IPC_NOWAIT
  );
  if (nrBytes == -1)
  {
    if (errno == ENOMSG)
      return false;

    throw IpcException("msgrcv");
  }

  return true;
}

DECLARE_MSG_TEMPLATES(datastructs::NodeRequest);
DECLARE_MSG_TEMPLATES(datastructs::NodeResponse);
DECLARE_MSG_TEMPLATES(datastructs::NodeAliveUpdate);
DECLARE_MSG_TEMPLATES(datastructs::NodePublishesToUpdate);
DECLARE_MSG_TEMPLATES(datastructs::NodeSubscribesToUpdate);
DECLARE_MSG_TEMPLATES(datastructs::NodeServicesUpdate);
DECLARE_MSG_TEMPLATES(datastructs::NodeClientsUpdate);

DECLARE_MSG_TEMPLATES(datastructs::TopicRequest);
DECLARE_MSG_TEMPLATES(datastructs::TopicResponse);
DECLARE_MSG_TEMPLATES(datastructs::TopicPublishersUpdate);
DECLARE_MSG_TEMPLATES(datastructs::TopicSubscribersUpdate);

DECLARE_MSG_TEMPLATES(datastructs::ProcessRequest);
DECLARE_MSG_TEMPLATES(datastructs::ProcessResponse);
DECLARE_MSG_TEMPLATES(datastructs::ProcessChildrenUpdate);

DECLARE_MSG_TEMPLATES(datastructs::NamespaceRequest);
DECLARE_MSG_TEMPLATES(datastructs::NamespaceResponse);

DECLARE_MSG_TEMPLATES(datastructs::SearchRequest);
DECLARE_MSG_TEMPLATES(datastructs::SearchResponse);

DECLARE_MSG_TEMPLATES(datastructs::InitRequest);
DECLARE_MSG_TEMPLATES(datastructs::InitResponse);

DECLARE_MSG_TEMPLATES(datastructs::UnsubscribeRequest);
DECLARE_MSG_TEMPLATES(datastructs::UnsubscribeResponse);

DECLARE_MSG_TEMPLATES(datastructs::MsgRequest);
DECLARE_MSG_TEMPLATES(datastructs::MsgResponse);

std::string parseString(const char (&src)[MAX_STRING_SIZE])
{
  return std::string(src, ::strnlen(src, MAX_STRING_SIZE));
}

void parseString(char(&dst)[MAX_STRING_SIZE], const std::string &src)
{
  std::strncpy(dst, src.c_str(), MAX_STRING_SIZE);
}

std::vector<std::string> parseStringArray(const char(&src)[MAX_ARRAY_SIZE][MAX_STRING_SIZE])
{
  std::vector<std::string> dst(MAX_ARRAY_SIZE);
  for (size_t i = 0ul; i < MAX_ARRAY_SIZE && src[i][0] != '\0'; i++)
  {
    dst.push_back(util::parseString(src[i]));
  }

  return dst;
}

void parseStringArray(char(&dst)[MAX_ARRAY_SIZE][MAX_STRING_SIZE], const std::vector<std::string> &src)
{
  size_t size = std::min(src.size(), MAX_ARRAY_SIZE);
  for (int i = 0; i < size; i++) {
    parseString(dst[i], src[i]);
  }

  if (size < MAX_ARRAY_SIZE)
    dst[size][0] = '\0';
}

template<typename T>
std::vector<T> parseArray(const T(&src)[MAX_ARRAY_SIZE])
{
  static_assert(std::is_arithmetic<T>::value);

  std::vector<T> dst(MAX_ARRAY_SIZE);
  for (size_t i = 0ul; i < MAX_ARRAY_SIZE && src[i] != 0; i++)
    dst.push_back(src[i]);

  return dst;
}

template<typename T>
void parseArray(T(&dst)[MAX_ARRAY_SIZE], const std::vector<T> &src)
{
  static_assert(std::is_arithmetic<T>::value);

  size_t size = std::min(src.size(), MAX_ARRAY_SIZE);
  std::memcpy(dst, src.data(), size);

  if (size < MAX_ARRAY_SIZE)
    dst[size] = 0;
}

DECLARE_PARSE_ARRAY_TEMPLATE(primaryKey_t);
DECLARE_PARSE_ARRAY_TEMPLATE(pid_t);

}