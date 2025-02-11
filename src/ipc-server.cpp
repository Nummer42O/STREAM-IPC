#include <filesystem>
namespace fs = std::filesystem;

#include <cstring>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "ipc/ipc-server.hpp"
#include "util.hpp"

#define FN_SEND_RESPONSE(ResponseType)                            \
  bool IpcServer::send##ResponseType(                             \
    const ResponseType &response, pid_t receiverId, bool wait)    \
  {                                                               \
    util::ResponseMsg<ResponseType> msg{                          \
      .key = util::makeMsgKey(ResponseType::msgType, receiverId), \
      .payload = response                                         \
    };                                                            \
                                                                  \
    return util::sendMsg(mMsgQueueId, msg, wait);                 \
  }

#define FN_RECEIVE_REQUEST(RequestType)                           \
  RequestType IpcServer::receive##RequestType(                    \
    requestId_t &oRequestId, pid_t &oSenderId, bool wait)         \
  {                                                               \
    util::RequestMsg<RequestType> msg;                            \
    msgKey_t msgKey = util::makeMsgKey(RequestType::msgType);     \
    util::receiveMsg(mMsgQueueId, msg, msgKey, wait);             \
                                                                  \
    oRequestId = msg.requestId;                                   \
    oSenderId = msg.senderId;                                     \
    return msg.payload;                                           \
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


FN_RECEIVE_REQUEST(NodeRequest);
FN_SEND_RESPONSE(NodeResponse);
FN_SEND_RESPONSE(NodeAliveUpdate);
FN_SEND_RESPONSE(NodePublishesToUpdate);
FN_SEND_RESPONSE(NodeSubscribesToUpdate);
FN_SEND_RESPONSE(NodeServicesUpdate);
FN_SEND_RESPONSE(NodeClientsUpdate);

FN_RECEIVE_REQUEST(TopicRequest);
FN_SEND_RESPONSE(TopicResponse);
FN_SEND_RESPONSE(TopicPublishersUpdate);
FN_SEND_RESPONSE(TopicSubscribersUpdate);

FN_RECEIVE_REQUEST(ProcessRequest);
FN_SEND_RESPONSE(ProcessResponse);
FN_SEND_RESPONSE(ProcessChildrenUpdate);

FN_RECEIVE_REQUEST(NamespaceRequest);
FN_SEND_RESPONSE(NamespaceResponse);

FN_RECEIVE_REQUEST(SearchRequest);
FN_SEND_RESPONSE(SearchResponse);

FN_RECEIVE_REQUEST(InitRequest);
FN_SEND_RESPONSE(InitResponse);

FN_RECEIVE_REQUEST(UnsubscribeRequest);
FN_SEND_RESPONSE(UnsubscribeResponse);

FN_RECEIVE_REQUEST(MsgRequest);
FN_SEND_RESPONSE(MsgResponse);