#pragma once

#include <string>
#include <string_view>
#include <optional>
#include <unistd.h>

#include "ipc/common.hpp"
#include "ipc/datastructs/information-datastructs.hpp"
#include "ipc/datastructs/misc-datastructs.hpp"
#include "ipc/datastructs/search-datastructs.hpp"
#include "ipc/datastructs/intern/intern-datastructs.hpp"

#define DECLARE_SEND_REQUEST(RequestType) \
  bool send##RequestType(const RequestType &request, requestId_t &oRequestId, bool wait = true) const

#define DECLARE_RECEIVE_RESPONSE(ResponseType) \
  std::optional<ResponseType> receive##ResponseType(bool wait = true) const


class IpcClient
{
public:
  IpcClient(int projectId);
  IpcClient(IpcClient &&ipcClient);
  IpcClient &operator=(IpcClient &&ipcClient);
  IpcClient(const IpcClient &) = delete;
  IpcClient &operator=(const IpcClient &) = delete;

  DECLARE_SEND_REQUEST(NodeRequest);
  DECLARE_RECEIVE_RESPONSE(NodeResponse);
  DECLARE_RECEIVE_RESPONSE(NodePublishersToUpdate);
  DECLARE_RECEIVE_RESPONSE(NodeSubscribersToUpdate);
  DECLARE_RECEIVE_RESPONSE(NodeIsServerForUpdate);
  DECLARE_RECEIVE_RESPONSE(NodeIsClientOfUpdate);
  DECLARE_RECEIVE_RESPONSE(NodeIsActionServerForUpdate);
  DECLARE_RECEIVE_RESPONSE(NodeIsActionClientOfUpdate);
  DECLARE_RECEIVE_RESPONSE(NodeTimerToUpdate);
  DECLARE_RECEIVE_RESPONSE(NodeStateUpdate);


  DECLARE_SEND_REQUEST(TopicRequest);
  DECLARE_RECEIVE_RESPONSE(TopicResponse);
  DECLARE_RECEIVE_RESPONSE(TopicPublishersUpdate);
  DECLARE_RECEIVE_RESPONSE(TopicSubscribersUpdate);

  DECLARE_SEND_REQUEST(SearchRequest);
  DECLARE_RECEIVE_RESPONSE(SearchResponse);

  DECLARE_SEND_REQUEST(InitRequest);
  DECLARE_RECEIVE_RESPONSE(InitResponse);

  DECLARE_SEND_REQUEST(UnsubscribeRequest);
  DECLARE_RECEIVE_RESPONSE(UnsubscribeResponse);

  DECLARE_SEND_REQUEST(MsgRequest);
  DECLARE_RECEIVE_RESPONSE(MsgResponse);

  DECLARE_SEND_REQUEST(SingleAttributesRequest);
  DECLARE_RECEIVE_RESPONSE(SingleAttributesResponse);

  DECLARE_SEND_REQUEST(AggregatedAttributesRequest);
  DECLARE_RECEIVE_RESPONSE(AggregatedAttributesResponse);

  DECLARE_SEND_REQUEST(CustomAttributesRequest);
  DECLARE_RECEIVE_RESPONSE(CustomAttributesResponse);

  DECLARE_SEND_REQUEST(AggregatedMemberRequest);
  DECLARE_RECEIVE_RESPONSE(AggregatedMemberResponse);

  DECLARE_SEND_REQUEST(CustomMemberRequest);
  DECLARE_RECEIVE_RESPONSE(CustomMemberResponse);

  DECLARE_SEND_REQUEST(SHMAddressRequest);
  DECLARE_RECEIVE_RESPONSE(SHMAddressResponse);

  DECLARE_SEND_REQUEST(NodeSwitchRequest);

private:
  static requestId_t smRequestIdCounter;
  int mMsgQueueId;
  pid_t mPid;
};
