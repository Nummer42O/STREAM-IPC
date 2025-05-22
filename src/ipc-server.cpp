#include <filesystem>
namespace fs = std::filesystem;

#include <cstring>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "ipc/ipc-server.hpp"
#include "ipc/util.hpp"

#define FN_SEND_RESPONSE(ResponseType, MsgTypeNr)               \
  bool IpcServer::send##ResponseType(                           \
    const ResponseType &response,                               \
    pid_t receiverId,                                           \
    bool wait                                                   \
  ) const                                                       \
  {                                                             \
    util::ResponseMsg<ResponseType> msg{                        \
      .key = util::makeMsgKey(MsgTypeNr, receiverId),           \
      .payload = response                                       \
    };                                                          \
                                                                \
    return util::sendMsg(mMsgQueueId, msg, wait);               \
  }

#define FN_RECEIVE_REQUEST(RequestType, MsgTypeNr)              \
  std::optional<RequestType> IpcServer::receive##RequestType(   \
    requestId_t &oRequestId,                                    \
    pid_t &oSenderId,                                           \
    bool wait                                                   \
  ) const                                                       \
  {                                                             \
    util::RequestMsg<RequestType> msg;                          \
    msgKey_t msgKey = util::makeMsgKey(MsgTypeNr);              \
    if (util::receiveMsg(mMsgQueueId, msg, msgKey, wait))       \
    {                                                           \
      oRequestId = msg.requestId;                               \
      oSenderId = msg.senderId;                                 \
      return msg.payload;                                       \
    }                                                           \
    else                                                        \
      return {};                                                \
  }


IpcServer::IpcServer(int projectId)
{
  if (!fs::exists(KEY_LOCATION))
    fs::create_directories(KEY_LOCATION);

  mMsgQueueId = util::getMsgQueueId(projectId, true);
}

IpcServer::IpcServer(IpcServer &&ipcServer)
{
  mMsgQueueId = std::move(ipcServer.mMsgQueueId);
}

IpcServer &IpcServer::operator=(IpcServer &&ipcServer)
{
  mMsgQueueId = std::move(ipcServer.mMsgQueueId);
  return *this;
}

IpcServer::~IpcServer()
{
  int status = ::msgctl(mMsgQueueId, IPC_RMID, nullptr);
  if (status == -1)
    std::clog \
      << "Error " << strerrorname_np(errno) \
      << " closing message queue: " << strerrordesc_np(errno) << '\n';
}


FN_RECEIVE_REQUEST(NodeRequest, MSG_TYPE_NODE_REQUEST);
FN_SEND_RESPONSE(NodeResponse, MSG_TYPE_NODE_RESPONSE);
FN_SEND_RESPONSE(NodeAliveUpdate, MSG_TYPE_NODE_ALIVE_UPDATE);
FN_SEND_RESPONSE(NodePublishersToUpdate, MSG_TYPE_NODE_PUBLISHES_TO_UPDATE);
FN_SEND_RESPONSE(NodeSubscribersToUpdate, MSG_TYPE_NODE_SUBSCRIBES_TO_UPDATE);
FN_SEND_RESPONSE(NodeIsServerForUpdate, MSG_TYPE_NODE_ISSERVERFOR_UPDATE);
FN_SEND_RESPONSE(NodeIsClientOfUpdate, MSG_TYPE_NODE_ISCLIENTOF_UPDATE);
FN_SEND_RESPONSE(NodeIsActionServerForUpdate, MSG_TYPE_NODE_ISACTIONSERVERFOR_UPDATE);
FN_SEND_RESPONSE(NodeIsActionClientOfUpdate, MSG_TYPE_NODE_ISACTIONCLIENTOF_UPDATE);

FN_RECEIVE_REQUEST(TopicRequest, MSG_TYPE_TOPIC_REQUEST);
FN_SEND_RESPONSE(TopicResponse, MSG_TYPE_TOPIC_RESPONSE);
FN_SEND_RESPONSE(TopicPublishersUpdate, MSG_TYPE_TOPIC_PUBLISHERS_UPDATE);
FN_SEND_RESPONSE(TopicSubscribersUpdate, MSG_TYPE_TOPIC_SUBSCRIBERS_UPDATE);

FN_RECEIVE_REQUEST(ProcessRequest, MSG_TYPE_PROCESS_REQUEST);
FN_SEND_RESPONSE(ProcessResponse, MSG_TYPE_PROCESS_RESPONSE);
FN_SEND_RESPONSE(ProcessChildrenUpdate, MSG_TYPE_PROCESS_CHILDREN_UPDATE);

FN_RECEIVE_REQUEST(NamespaceRequest, MSG_TYPE_NAMESPACE_REQUEST);
FN_SEND_RESPONSE(NamespaceResponse, MSG_TYPE_NAMESPACE_RESPONSE);

FN_RECEIVE_REQUEST(SearchRequest, MSG_TYPE_SEARCH_REQUEST);
FN_SEND_RESPONSE(SearchResponse, MSG_TYPE_SEARCH_RESPONSE);

FN_RECEIVE_REQUEST(InitRequest, MSG_TYPE_INIT_REQUEST);
FN_SEND_RESPONSE(InitResponse, MSG_TYPE_INIT_RESPONSE);

FN_RECEIVE_REQUEST(UnsubscribeRequest, MSG_TYPE_UNSUBSCRIBE_REQUEST);
FN_SEND_RESPONSE(UnsubscribeResponse, MSG_TYPE_UNSUBSCRIBE_RESPONSE);

FN_RECEIVE_REQUEST(MsgRequest, MSG_TYPE_MSG_REQUEST);
FN_SEND_RESPONSE(MsgResponse, MSG_TYPE_MSG_RESPONSE);

FN_RECEIVE_REQUEST(SingleAttributesRequest, GETSINGLEATTRIBUTE_REQUEST);
FN_SEND_RESPONSE(SingleAttributesResponse, GETSINGLEATTRIBUTE_RESPONSE);

FN_RECEIVE_REQUEST(AggregatedAttributesRequest, GETAGGREGATEDATTRIBUTE_REQUEST);
FN_SEND_RESPONSE(AggregatedAttributesResponse, GETAGGREGATEDATTRIBUTE_RESPONSE);

FN_RECEIVE_REQUEST(CustomAttributesRequest, GETCUSTOMATTRIBUTE_REQUEST);
FN_SEND_RESPONSE(CustomAttributesResponse, GETCUSTOMATTRIBUTE_RESPONSE);

FN_RECEIVE_REQUEST(AggregatedMemberRequest, GETAGGREGATEDMEMBER_REQUEST);
FN_SEND_RESPONSE(AggregatedMemberResponse, GETAGGREGATEDMEMBER_RESPONSE);

FN_RECEIVE_REQUEST(CustomMemberRequest, GETCUSTOMMEMBER_REQUEST);
FN_SEND_RESPONSE(CustomMemberResponse, GETCUSTOMMEMBER_RESPONSE);

FN_RECEIVE_REQUEST(SHMAddressRequest, GETSHMADDRESS_REQUEST);
FN_SEND_RESPONSE(SHMAddressResponse, GETSHMADDRESS_RESPONSE);

// INTERN
FN_RECEIVE_REQUEST(NodeSwitchRequest, MSG_TYPE_INTERN_PROC_SWITCH_REQUEST);
