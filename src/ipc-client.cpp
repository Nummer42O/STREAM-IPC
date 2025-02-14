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

#define FN_SEND_REQUEST(RequestType, MsgTypeNr)             \
  bool IpcClient::send##RequestType(                        \
    const RequestType &request, requestId_t &oRequestId,    \
    bool wait)                                              \
  {                                                         \
    oRequestId = ++mRequestIdCounter;                       \
    util::RequestMsg<RequestType> msg{                      \
      .key = util::makeMsgKey(MsgTypeNr),                   \
      .requestId = oRequestId,                              \
      .senderId = mPid,                                     \
      .payload = request                                    \
    };                                                      \
                                                            \
    return util::sendMsg(mMsgQueueId, msg, wait);           \
  }

#define FN_RECEIVE_RESPONSE(ResponseType, MsgTypeNr)                      \
  std::optional<ResponseType> IpcClient::receive##ResponseType(bool wait) \
  {                                                                       \
    util::ResponseMsg<ResponseType> msg;                                  \
    msgKey_t msgKey = util::makeMsgKey(MsgTypeNr, mPid);                  \
    if (util::receiveMsg(mMsgQueueId, msg, msgKey, wait))                 \
      return msg.payload;                                                 \
    else                                                                  \
      return {};                                                          \
  }


IpcClient::IpcClient(int projectId)
{
  mRequestIdCounter = 0;
  mMsgQueueId = util::getMsgQueueId(projectId);
  mPid = ::getpid();
}


FN_SEND_REQUEST(NodeRequest, MSG_TYPE_NODE_REQUEST);
FN_RECEIVE_RESPONSE(NodeResponse, MSG_TYPE_NODE_RESPONSE);
FN_RECEIVE_RESPONSE(NodeAliveUpdate, MSG_TYPE_NODE_ALIVE_UPDATE);
FN_RECEIVE_RESPONSE(NodePublishesToUpdate, MSG_TYPE_NODE_PUBLISHES_TO_UPDATE);
FN_RECEIVE_RESPONSE(NodeSubscribesToUpdate, MSG_TYPE_NODE_SUBSCRIBES_TO_UPDATE);
FN_RECEIVE_RESPONSE(NodeServicesUpdate, MSG_TYPE_NODE_SERVICES_UPDATE);
FN_RECEIVE_RESPONSE(NodeClientsUpdate, MSG_TYPE_NODE_CLIENTS_UPDATE);

FN_SEND_REQUEST(TopicRequest, MSG_TYPE_TOPIC_REQUEST);
FN_RECEIVE_RESPONSE(TopicResponse, MSG_TYPE_TOPIC_RESPONSE);
FN_RECEIVE_RESPONSE(TopicPublishersUpdate, MSG_TYPE_TOPIC_PUBLISHERS_UPDATE);
FN_RECEIVE_RESPONSE(TopicSubscribersUpdate, MSG_TYPE_TOPIC_SUBSCRIBERS_UPDATE);

FN_SEND_REQUEST(ProcessRequest, MSG_TYPE_PROCESS_REQUEST);
FN_RECEIVE_RESPONSE(ProcessResponse, MSG_TYPE_PROCESS_RESPONSE);
FN_RECEIVE_RESPONSE(ProcessChildrenUpdate, MSG_TYPE_PROCESS_CHILDREN_UPDATE);

FN_SEND_REQUEST(NamespaceRequest, MSG_TYPE_NAMESPACE_REQUEST);
FN_RECEIVE_RESPONSE(NamespaceResponse, MSG_TYPE_INIT_RESPONSE);

FN_SEND_REQUEST(SearchRequest, MSG_TYPE_SEARCH_REQUEST);
FN_RECEIVE_RESPONSE(SearchResponse, MSG_TYPE_SEARCH_RESPONSE);

FN_SEND_REQUEST(InitRequest, MSG_TYPE_INIT_REQUEST);
FN_RECEIVE_RESPONSE(InitResponse, MSG_TYPE_INIT_RESPONSE);

FN_SEND_REQUEST(UnsubscribeRequest, MSG_TYPE_UNSUBSCRIBE_REQUEST);
FN_RECEIVE_RESPONSE(UnsubscribeResponse, MSG_TYPE_UNSUBSCRIBE_RESPONSE);

FN_SEND_REQUEST(MsgRequest, MSG_TYPE_MSG_REQUEST);
FN_RECEIVE_RESPONSE(MsgResponse, MSG_TYPE_MSG_RESPONSE);

FN_SEND_REQUEST(ProcSwitchRequest, MSG_TYPE_INTERN_PROC_SWITCH_REQUEST);
FN_RECEIVE_RESPONSE(ProcSwitchResponse, MSG_TYPE_INTERN_PROC_SWITCH_RESPONSE);