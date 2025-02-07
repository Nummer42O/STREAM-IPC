#include <filesystem>
namespace fs = std::filesystem;

#include <cassert>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <unistd.h>

#include "ipc/ipc-client.hpp"
#include "ipc/ipc-exceptions.hpp"
#include "util.hpp"

#include "msgs/information-msgs.hpp"
#include "msgs/namespace-msgs.hpp"
#include "msgs/search-msgs.hpp"
#include "msgs/misc-msgs.hpp"

IpcClient::IpcClient(int projectId)
{
  mMsgQueueId = util::getMsgQueueId(projectId);
  mPid = ::getpid();
}

bool IpcClient::sendTestRequest(std::string_view name, bool wait)
{
  util::TestRequest request{
    .senderId = mPid
  };
  std::strncpy(request.name, name.cbegin(), sizeof(util::TestRequest::name));

  return util::sendMsg(mMsgQueueId, request, wait);
}

void IpcClient::receiveTestResponse(std::string &oMessage, bool wait)
{
  util::TestResponse response;
  msgKey_t msgKey = util::makeMsgKey(mPid, 0);
  util::receiveMsg(mMsgQueueId, response, msgKey, wait);

  oMessage = util::to_string(response.msg, sizeof(util::TestResponse::msg));
}

bool IpcClient::sendNamespaceRequest(NamespaceDataRequest payload, bool wait) {
  msgKey_t key = util::makeMsgKey(NAMESPACE_REQUEST_MSG_TYPE, mPid);

  NamespaceRequest request {
    .key = key,
    .id = mPid,
    .updates = payload.updates
  };
  std::strncpy(request.path, payload.path.c_str(), MAX_STRING_LENGTH);

  return util::sendMsg(mMsgQueueId, request, wait);
}

bool IpcClient::sendSearchRequest(SearchDataRequest payload, bool wait) {
  msgKey_t key = util::makeMsgKey(SEARCH_REQUEST_MSG_TYPE, mPid);

  SearchRequest request {
    .key = key,
    .type = static_cast<SearchRequest::Type>(payload.type),
  };
  std::strncpy(request.name, payload.name.c_str(), MAX_STRING_LENGTH);

  return util::sendMsg(mMsgQueueId, request, wait);
}

bool IpcClient::sendMsgRequest(MsgDataRequest payload, bool wait) {
  msgKey_t key = util::makeMsgKey(MSG_REQUEST_MSG_TYPE, mPid);

  MsgRequest request {
    .key = key,
    .primaryKey = payload.primaryKey,
    .targetFrequency = payload.targetFrequency
  };

  return util::sendMsg(mMsgQueueId, request, wait);
}

bool IpcClient::sendInitRequest(InitDataRequest payload, bool wait) {
  msgKey_t key = util::makeMsgKey(SEARCH_REQUEST_MSG_TYPE, mPid);

  InitRequest request {
    .key = key,
  };
  std::strncpy(request.ignoredTopic, payload.ignoredTopic.c_str(), MAX_STRING_LENGTH);

  return util::sendMsg(mMsgQueueId, request, wait);
}

bool IpcClient::sendUnsubscribeRequest(UnsubscribeDataRequest payload, bool wait) {
  msgKey_t key = util::makeMsgKey(SEARCH_REQUEST_MSG_TYPE, mPid);

  UnsubscribeRequest request {
    .key = key,
    .id = payload.id
  };

  return util::sendMsg(mMsgQueueId, request, wait);
}

void IpcClient::receiveTestResponse(std::string &oMessage, bool wait)
{
  util::TestResponse response;
  msgKey_t msgKey = util::makeMsgKey(0, mPid);
  util::receiveMsg(mMsgQueueId, response, msgKey, wait);

  oMessage = util::to_string(response.msg, sizeof(util::TestResponse::msg));
}

NamespaceDataResponse IpcClient::receiveNamespaceResponse(bool wait) {
  NamespaceResponse response;
  msgKey_t msgKey = util::makeMsgKey(NAMESPACE_REQUEST_MSG_TYPE, mPid);
  util::receiveMsg(mMsgQueueId, response, msgKey, wait);

  return NamespaceDataResponse {
    TODO
  };
}

SearchDataResponse IpcClient::receiveSearchspaceResponse(bool wait) {
  SearchResponse response;
  msgKey_t msgKey = util::makeMsgKey(SEARCH_REQUEST_MSG_TYPE, mPid);
  util::receiveMsg(mMsgQueueId, response, msgKey, wait);

  return SearchDataResponse {
    TODO
  };
}

MsgDataResponse IpcClient::receiveMsgResponse(bool wait) {
  MsgResponse response;
  msgKey_t msgKey = util::makeMsgKey(MSG_REQUEST_MSG_TYPE, mPid);
  util::receiveMsg(mMsgQueueId, response, msgKey, wait);

  return MsgDataResponse {
    TODO
  };
}

InitDataResponse IpcClient::receiveInitResponse(bool wait) {
  InitResponse response;
  msgKey_t msgKey = util::makeMsgKey(INIT_REQUEST_MSG_TYPE, mPid);
  util::receiveMsg(mMsgQueueId, response, msgKey, wait);

  return InitDataResponse {
    TODO
  };
}

UnsubscribeDataResponse IpcClient::receiveUnsubscribeResponse(bool wait) {
  UnsubscribeResponse response;
  msgKey_t msgKey = util::makeMsgKey(UNSUBSCRIBE_REQUEST_MSG_TYPE, mPid);
  util::receiveMsg(mMsgQueueId, response, msgKey, wait);

  return UnsubscribeDataRequest {
    TODO
  };
}