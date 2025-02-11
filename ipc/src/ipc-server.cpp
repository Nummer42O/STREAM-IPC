#include <filesystem>
namespace fs = std::filesystem;

#include <cstring>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "ipc/ipc-server.hpp"
#include "util.hpp"

#define FN_SEND_RESPONSE(ResponseType, MsgTypeNr)               \
  bool IpcServer::send##ResponseType(                           \
    const ResponseType &response, pid_t receiverId, bool wait)  \
  {                                                             \
    util::ResponseMsg<ResponseType> msg{                        \
      .key = util::makeMsgKey(MsgTypeNr, receiverId),           \
      .payload = response                                       \
    };                                                          \
                                                                \
    return util::sendMsg(mMsgQueueId, msg, wait);               \
  }

#define FN_RECEIVE_REQUEST(RequestType, MsgTypeNr)              \
  RequestType IpcServer::receive##RequestType(                  \
    requestId_t &oRequestId, pid_t &oSenderId, bool wait)       \
  {                                                             \
    util::RequestMsg<RequestType> msg;                          \
    msgKey_t msgKey = util::makeMsgKey(MsgTypeNr);              \
    util::receiveMsg(mMsgQueueId, msg, msgKey, wait);           \
                                                                \
    oRequestId = msg.requestId;                                 \
    oSenderId = msg.senderId;                                   \
    return msg.payload;                                         \
  }


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


FN_SEND_RESPONSE(NodeRequest, MSG_TYPE_NODE_REQUEST);
FN_RECEIVE_REQUEST(NodeResponse, MSG_TYPE_NODE_RESPONSE);
FN_RECEIVE_REQUEST(NodeAliveUpdate, MSG_TYPE_NODE_ALIVE_UPDATE);
FN_RECEIVE_REQUEST(NodePublishesToUpdate, MSG_TYPE_NODE_PUBLISHES_TO_UPDATE);
FN_RECEIVE_REQUEST(NodeSubscribesToUpdate, MSG_TYPE_NODE_SUBSCRIBES_TO_UPDATE);
FN_RECEIVE_REQUEST(NodeServicesUpdate, MSG_TYPE_NODE_SERVICES_UPDATE);
FN_RECEIVE_REQUEST(NodeClientsUpdate, MSG_TYPE_NODE_CLIENTS_UPDATE);

FN_SEND_RESPONSE(TopicRequest, MSG_TYPE_TOPIC_REQUEST);
FN_RECEIVE_REQUEST(TopicResponse, MSG_TYPE_TOPIC_RESPONSE);
FN_RECEIVE_REQUEST(TopicPublishersUpdate, MSG_TYPE_TOPIC_PUBLISHERS_UPDATE);
FN_RECEIVE_REQUEST(TopicSubscribersUpdate, MSG_TYPE_TOPIC_SUBSCRIBERS_UPDATE);

FN_SEND_RESPONSE(ProcessRequest, MSG_TYPE_PROCESS_REQUEST);
FN_RECEIVE_REQUEST(ProcessResponse, MSG_TYPE_PROCESS_RESPONSE);
FN_RECEIVE_REQUEST(ProcessChildrenUpdate, MSG_TYPE_PROCESS_CHILDREN_UPDATE);

FN_SEND_RESPONSE(NamespaceRequest, MSG_TYPE_NAMESPACE_REQUEST);
FN_RECEIVE_REQUEST(NamespaceResponse, MSG_TYPE_NAMESPACE_RESPONSE);

FN_SEND_RESPONSE(SearchRequest, MSG_TYPE_SEARCH_REQUEST);
FN_RECEIVE_REQUEST(SearchResponse, MSG_TYPE_SEARCH_RESPONSE);

FN_SEND_RESPONSE(InitRequest, MSG_TYPE_INIT_REQUEST);
FN_RECEIVE_REQUEST(InitResponse, MSG_TYPE_INIT_RESPONSE);

FN_SEND_RESPONSE(UnsubscribeRequest, MSG_TYPE_UNSUBSCRIBE_REQUEST);
FN_RECEIVE_REQUEST(UnsubscribeResponse, MSG_TYPE_UNSUBSCRIBE_RESPONSE);

FN_SEND_RESPONSE(MsgRequest, MSG_TYPE_MSG_REQUEST);
FN_RECEIVE_REQUEST(MsgResponse, MSG_TYPE_MSG_RESPONSE);