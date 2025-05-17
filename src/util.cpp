#include <cassert>
#include <cstring>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "ipc/util.hpp"
#include "ipc/ipc-exceptions.hpp"

#include "ipc/datastructs/information-datastructs.hpp"
#include "ipc/datastructs/namespace-datastructs.hpp"
#include "ipc/datastructs/search-datastructs.hpp"
#include "ipc/datastructs/misc-datastructs.hpp"
#include "ipc/datastructs/intern/intern-datastructs.hpp"


namespace util {

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

DECLARE_MSG_TEMPLATES(NodeRequest);
DECLARE_MSG_TEMPLATES(NodeResponse);
DECLARE_MSG_TEMPLATES(NodeAliveUpdate);
DECLARE_MSG_TEMPLATES(NodePublishersToUpdate);
DECLARE_MSG_TEMPLATES(NodeSubscribersToUpdate);
DECLARE_MSG_TEMPLATES(NodeIsServerForUpdate);
DECLARE_MSG_TEMPLATES(NodeIsClientOfUpdate);
DECLARE_MSG_TEMPLATES(NodeIsActionServerForUpdate);
DECLARE_MSG_TEMPLATES(NodeIsActionClientOfUpdate);

DECLARE_MSG_TEMPLATES(TopicRequest);
DECLARE_MSG_TEMPLATES(TopicResponse);
DECLARE_MSG_TEMPLATES(TopicPublishersUpdate);
DECLARE_MSG_TEMPLATES(TopicSubscribersUpdate);

DECLARE_MSG_TEMPLATES(ProcessRequest);
DECLARE_MSG_TEMPLATES(ProcessResponse);
DECLARE_MSG_TEMPLATES(ProcessChildrenUpdate);

DECLARE_MSG_TEMPLATES(NamespaceRequest);
DECLARE_MSG_TEMPLATES(NamespaceResponse);

DECLARE_MSG_TEMPLATES(SearchRequest);
DECLARE_MSG_TEMPLATES(SearchResponse);

DECLARE_MSG_TEMPLATES(InitRequest);
DECLARE_MSG_TEMPLATES(InitResponse);

DECLARE_MSG_TEMPLATES(UnsubscribeRequest);
DECLARE_MSG_TEMPLATES(UnsubscribeResponse);

DECLARE_MSG_TEMPLATES(MsgRequest);
DECLARE_MSG_TEMPLATES(MsgResponse);

DECLARE_MSG_TEMPLATES(SingleAttributesRequest);
DECLARE_MSG_TEMPLATES(SingleAttributesResponse);

DECLARE_MSG_TEMPLATES(AggregatedAttributesRequest);
DECLARE_MSG_TEMPLATES(AggregatedAttributesResponse);

DECLARE_MSG_TEMPLATES(CustomAttributesRequest);
DECLARE_MSG_TEMPLATES(CustomAttributesResponse);

DECLARE_MSG_TEMPLATES(AggregatedMemberRequest);
DECLARE_MSG_TEMPLATES(AggregatedMemberResponse);

DECLARE_MSG_TEMPLATES(CustomMemberRequest);
DECLARE_MSG_TEMPLATES(CustomMemberResponse);

DECLARE_MSG_TEMPLATES(NodeSwitchRequest);

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