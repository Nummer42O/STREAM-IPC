#include <filesystem>
namespace fs = std::filesystem;

#include <cassert>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <unistd.h>

#include "ipc/ipc-client.hpp"
#include "ipc/ipc-exceptions.hpp"
#include "ipc/util.hpp"

#define FN_SEND_REQUEST(RequestType, MsgTypeNr)             \
  bool IpcClient::send##RequestType(                        \
    const RequestType &request,                             \
    requestId_t &oRequestId,                                \
    bool wait                                               \
  ) const                                                   \
  {                                                         \
    oRequestId = ++smRequestIdCounter;                       \
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
  std::optional<ResponseType> IpcClient::receive##ResponseType(           \
    bool wait                                                             \
  ) const                                                                 \
  {                                                                       \
    util::ResponseMsg<ResponseType> msg;                                  \
    msgKey_t msgKey = util::makeMsgKey(MsgTypeNr, mPid);                  \
    if (util::receiveMsg(mMsgQueueId, msg, msgKey, wait))                 \
      return msg.payload;                                                 \
    else                                                                  \
      return {};                                                          \
  }


requestId_t IpcClient::smRequestIdCounter = 0u;

IpcClient::IpcClient(int projectId)
{
  mMsgQueueId = util::getMsgQueueId(projectId);
  mPid = ::getpid();
}

IpcClient::IpcClient(IpcClient &&ipcClient)
{
  mMsgQueueId = std::move(ipcClient.mMsgQueueId);
  mPid        = std::move(ipcClient.mPid);
}

IpcClient &IpcClient::operator=(IpcClient &&ipcClient)
{
  mMsgQueueId = std::move(ipcClient.mMsgQueueId);
  mPid        = std::move(ipcClient.mPid);
  return *this;
}

FN_SEND_REQUEST(NodeRequest, MSG_TYPE_NODE_REQUEST);
FN_RECEIVE_RESPONSE(NodeResponse, MSG_TYPE_NODE_RESPONSE);
FN_RECEIVE_RESPONSE(NodePublishersToUpdate, MSG_TYPE_NODE_PUBLISHES_TO_UPDATE);
FN_RECEIVE_RESPONSE(NodeSubscribersToUpdate, MSG_TYPE_NODE_SUBSCRIBES_TO_UPDATE);
FN_RECEIVE_RESPONSE(NodeIsServerForUpdate, MSG_TYPE_NODE_ISSERVERFOR_UPDATE);
FN_RECEIVE_RESPONSE(NodeIsClientOfUpdate, MSG_TYPE_NODE_ISCLIENTOF_UPDATE);
FN_RECEIVE_RESPONSE(NodeIsActionServerForUpdate, MSG_TYPE_NODE_ISACTIONSERVERFOR_UPDATE);
FN_RECEIVE_RESPONSE(NodeIsActionClientOfUpdate, MSG_TYPE_NODE_ISACTIONCLIENTOF_UPDATE);
FN_RECEIVE_RESPONSE(NodeTimerToUpdate, MSG_TYPE_NODE_TIMERTO_UPDATE);
FN_RECEIVE_RESPONSE(NodeStateUpdate, MSG_TYPE_NODE_STATE_UPDATE);

FN_SEND_REQUEST(TopicRequest, MSG_TYPE_TOPIC_REQUEST);
FN_RECEIVE_RESPONSE(TopicResponse, MSG_TYPE_TOPIC_RESPONSE);
FN_RECEIVE_RESPONSE(TopicPublishersUpdate, MSG_TYPE_TOPIC_PUBLISHERS_UPDATE);
FN_RECEIVE_RESPONSE(TopicSubscribersUpdate, MSG_TYPE_TOPIC_SUBSCRIBERS_UPDATE);

FN_SEND_REQUEST(SearchRequest, MSG_TYPE_SEARCH_REQUEST);
FN_RECEIVE_RESPONSE(SearchResponse, MSG_TYPE_SEARCH_RESPONSE);

FN_SEND_REQUEST(InitRequest, MSG_TYPE_INIT_REQUEST);
FN_RECEIVE_RESPONSE(InitResponse, MSG_TYPE_INIT_RESPONSE);

FN_SEND_REQUEST(UnsubscribeRequest, MSG_TYPE_UNSUBSCRIBE_REQUEST);
FN_RECEIVE_RESPONSE(UnsubscribeResponse, MSG_TYPE_UNSUBSCRIBE_RESPONSE);

FN_SEND_REQUEST(MsgRequest, MSG_TYPE_MSG_REQUEST);
FN_RECEIVE_RESPONSE(MsgResponse, MSG_TYPE_MSG_RESPONSE);

FN_SEND_REQUEST(SingleAttributesRequest, GETSINGLEATTRIBUTE_REQUEST);
FN_RECEIVE_RESPONSE(SingleAttributesResponse, GETSINGLEATTRIBUTE_RESPONSE);

FN_SEND_REQUEST(AggregatedAttributesRequest, GETAGGREGATEDATTRIBUTE_REQUEST);
FN_RECEIVE_RESPONSE(AggregatedAttributesResponse, GETAGGREGATEDATTRIBUTE_RESPONSE);

FN_SEND_REQUEST(CustomAttributesRequest, GETCUSTOMATTRIBUTE_REQUEST);
FN_RECEIVE_RESPONSE(CustomAttributesResponse, GETCUSTOMATTRIBUTE_RESPONSE);

FN_SEND_REQUEST(AggregatedMemberRequest, GETAGGREGATEDMEMBER_REQUEST);
FN_RECEIVE_RESPONSE(AggregatedMemberResponse, GETAGGREGATEDMEMBER_RESPONSE);

FN_SEND_REQUEST(CustomMemberRequest, GETCUSTOMMEMBER_REQUEST);
FN_RECEIVE_RESPONSE(CustomMemberResponse, GETCUSTOMMEMBER_RESPONSE);

FN_SEND_REQUEST(SHMAddressRequest, GETSHMADDRESS_REQUEST);
FN_RECEIVE_RESPONSE(SHMAddressResponse, GETSHMADDRESS_RESPONSE);

FN_SEND_REQUEST(NodeSwitchRequest, MSG_TYPE_INTERN_PROC_SWITCH_REQUEST);