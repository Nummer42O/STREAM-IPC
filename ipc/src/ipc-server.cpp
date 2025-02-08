#include <filesystem>
namespace fs = std::filesystem;

#include <cstring>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "ipc/ipc-server.hpp"
#include "util.hpp"

#include "msgs/information-msgs.hpp"
#include "msgs/namespace-msgs.hpp"
#include "msgs/search-msgs.hpp"
#include "msgs/misc-msgs.hpp"

IpcServer::IpcServer(int projectId)
{
  if (!fs::exists(KEY_LOCATION))
    fs::create_directories(KEY_LOCATION);

  mMsgQueueId = util::getMsgQueueId(projectId, true);
}

IpcServer::~IpcServer()
{
  int status = ::msgctl(mMsgQueueId, IPC_RMID, nullptr);
  if (status == -1)
    std::clog \
      << "Error " << strerrorname_np(errno) \
      << " closing message queue: " << strerrordesc_np(errno) << '\n';
}

void IpcServer::receiveTestRequest(std::string &oName, msgKey_t &oSenderId, bool wait)
{
  util::TestRequest request;
  util::receiveMsg(mMsgQueueId, request, SERVER_MSG_TYPE, wait);

  oName = util::to_string(request.name, sizeof(util::TestRequest::name));
  oSenderId = request.senderId;
}

bool IpcServer::sendTestResponse(std::string_view msg, msgKey_t receiverId, bool wait)
{
  util::TestResponse response{
    .type = util::makeMsgKey(receiverId, 0) //! NOTE: would need appropriate value, in this example its not used
  };
  std::strncpy(response.msg, msg.cbegin(), sizeof(util::TestResponse::msg));

  return util::sendMsg(mMsgQueueId, response, wait);
}

NamespaceDataRequest IpcServer::receiveNamespaceRequest(bool wait) {
  NamespaceRequest request;
  util::receiveMsg(mMsgQueueId, request, SERVER_MSG_TYPE, wait);

  return NamespaceDataRequest {
    .id = request.id,
    .path = util::to_string(request.path, MAX_STRING_LENGTH),
    .updates = request.updates
  };
}

SearchDataRequest IpcServer::receiveSearchRequest(bool wait) {
  SearchRequest request;
  util::receiveMsg(mMsgQueueId, request, SERVER_MSG_TYPE, wait);

  return SearchDataRequest {
    .type = static_cast<SearchDataRequest::Type>(request.type),
    .name = util::to_string(request.name, MAX_STRING_LENGTH)
  };
}

MsgDataRequest IpcServer::receiveMsgRequest(bool wait) {
  MsgRequest request;
  util::receiveMsg(mMsgQueueId, request, SERVER_MSG_TYPE, wait);

  return MsgDataRequest {
    .id = request.id,
    .primaryKey = request.primaryKey,
    .targetFrequency = request.targetFrequency
  };
}

InitDataRequest IpcServer::receiveInitRequest(bool wait) {
  InitRequest request;
  util::receiveMsg(mMsgQueueId, request, SERVER_MSG_TYPE, wait);

  return InitDataRequest {
    .ignoredTopic = util::to_string(request.ignoredTopic, MAX_STRING_LENGTH)
  };
}

UnsubscribeDataRequest IpcServer::receiveUnsubscribeRequest(bool wait) {
  UnsubscribeRequest request;
  util::receiveMsg(mMsgQueueId, request, SERVER_MSG_TYPE, wait);

  return UnsubscribeDataRequest {
    .id = request.id
  };
}

bool IpcServer::sendNamespaceResponse(NamespaceDataResponse response, msgKey_t receiverId, bool wait) {
  NamespaceResponse msg{
    .key = receiverId,
    .nrOfAccChildren = response.nrOfAccChildren
  };
  util::parseVectorToStringArray(response.children, msg.children);

  return util::sendMsg(mMsgQueueId, msg, wait);
}

bool IpcServer::sendSearchResponse(SearchDataResponse response, msgKey_t receiverId, bool wait) {
  SearchResponse msg{
    .key = receiverId,
    .primaryKey = response.primaryKey
  };

  return util::sendMsg(mMsgQueueId, msg, wait);
}

bool IpcServer::sendMsgResponse(MsgDataResponse response, msgKey_t receiverId, bool wait) {
  MsgResponse msg{
    .key = receiverId,
    .shmlPtr = response.shmlPtr
  };

  return util::sendMsg(mMsgQueueId, msg, wait);
}

bool IpcServer::sendInitResponse(InitDataResponse response, msgKey_t receiverId, bool wait) {
  InitResponse msg{
    .key = receiverId,
  };
  util::parseVectorToStringArray(response.ignoredTopics, msg.ignoredTopics);

  return util::sendMsg(mMsgQueueId, msg, wait);
}

bool IpcServer::sendUnsubscribeResponse(msgKey_t receiverId, bool wait) {
  UnsubscribeResponse msg{
    .key = receiverId
  };

  return util::sendMsg(mMsgQueueId, msg, wait);
}
