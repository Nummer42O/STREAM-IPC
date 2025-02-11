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

#define FN_SEND_REQUEST(RequestType)                                  \
  bool IpcClient::send##RequestType(                                  \
    const RequestType &request, requestId_t &oRequestId,              \
    bool wait)                                                        \
  {                                                                   \
    oRequestId = ++mRequestIdCounter;                                 \
    util::RequestMsg<RequestType> msg{                                \
      .key = util::makeMsgKey(RequestType::msgType),                  \
      .requestId = oRequestId,                                        \
      .senderId = mPid,                                               \
      .payload = request                                              \
    };                                                                \
                                                                      \
    return util::sendMsg(mMsgQueueId, msg, wait);                     \
  }

#define FN_RECEIVE_RESPONSE(ResponseType)                             \
  ResponseType IpcClient::receive##ResponseType(bool wait)            \
  {                                                                   \
    util::ResponseMsg<ResponseType> msg;                              \
    msgKey_t msgKey = util::makeMsgKey(ResponseType::msgType, mPid);  \
    util::receiveMsg(mMsgQueueId, msg, msgKey, wait);                 \
                                                                      \
    return msg.payload;                                               \
  }


IpcClient::IpcClient(int projectId)
{
  mRequestIdCounter = 0;
  mMsgQueueId = util::getMsgQueueId(projectId);
  mPid = ::getpid();
}


FN_SEND_REQUEST(NodeRequest);
FN_RECEIVE_RESPONSE(NodeResponse);
FN_RECEIVE_RESPONSE(NodeAliveUpdate);
FN_RECEIVE_RESPONSE(NodePublishesToUpdate);
FN_RECEIVE_RESPONSE(NodeSubscribesToUpdate);
FN_RECEIVE_RESPONSE(NodeServicesUpdate);
FN_RECEIVE_RESPONSE(NodeClientsUpdate);

FN_SEND_REQUEST(TopicRequest);
FN_RECEIVE_RESPONSE(TopicResponse);
FN_RECEIVE_RESPONSE(TopicPublishersUpdate);
FN_RECEIVE_RESPONSE(TopicSubscribersUpdate);

FN_SEND_REQUEST(ProcessRequest);
FN_RECEIVE_RESPONSE(ProcessResponse);
FN_RECEIVE_RESPONSE(ProcessChildrenUpdate);

FN_SEND_REQUEST(NamespaceRequest);
FN_RECEIVE_RESPONSE(NamespaceResponse);

FN_SEND_REQUEST(SearchRequest);
FN_RECEIVE_RESPONSE(SearchResponse);

FN_SEND_REQUEST(InitRequest);
FN_RECEIVE_RESPONSE(InitResponse);

FN_SEND_REQUEST(UnsubscribeRequest);
FN_RECEIVE_RESPONSE(UnsubscribeResponse);

FN_SEND_REQUEST(MsgRequest);
FN_RECEIVE_RESPONSE(MsgResponse);