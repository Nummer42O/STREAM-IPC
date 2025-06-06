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

#define DECLARE_SEND_RESPONSE(ResponseType) \
  bool send##ResponseType(const ResponseType &response, pid_t receiverId, bool wait = true) const

#define DECLARE_RECEIVE_REQUEST(RequestType) \
  std::optional<RequestType> receive##RequestType(requestId_t &oRequestId, pid_t &oSenderId, bool wait = true) const


class IpcServer
{
public:
  IpcServer(int projectId);
  IpcServer(IpcServer &&ipcServer);
  IpcServer &operator=(IpcServer &&ipcServer);
  IpcServer(const IpcServer &) = delete;
  IpcServer &operator=(const IpcServer &) = delete;
  ~IpcServer();

  DECLARE_RECEIVE_REQUEST(NodeRequest);
  DECLARE_SEND_RESPONSE(NodeResponse);
  DECLARE_SEND_RESPONSE(NodePublishersToUpdate);
  DECLARE_SEND_RESPONSE(NodeSubscribersToUpdate);
  DECLARE_SEND_RESPONSE(NodeIsServerForUpdate);
  DECLARE_SEND_RESPONSE(NodeIsClientOfUpdate);
  DECLARE_SEND_RESPONSE(NodeIsActionServerForUpdate);
  DECLARE_SEND_RESPONSE(NodeIsActionClientOfUpdate);
  DECLARE_SEND_RESPONSE(NodeTimerToUpdate);
  DECLARE_SEND_RESPONSE(NodeStateUpdate);

  DECLARE_RECEIVE_REQUEST(TopicRequest);
  DECLARE_SEND_RESPONSE(TopicResponse);
  DECLARE_SEND_RESPONSE(TopicPublishersUpdate);
  DECLARE_SEND_RESPONSE(TopicSubscribersUpdate);

  DECLARE_RECEIVE_REQUEST(SearchRequest);
  DECLARE_SEND_RESPONSE(SearchResponse);

  DECLARE_RECEIVE_REQUEST(InitRequest);
  DECLARE_SEND_RESPONSE(InitResponse);

  DECLARE_RECEIVE_REQUEST(UnsubscribeRequest);
  DECLARE_SEND_RESPONSE(UnsubscribeResponse);

  DECLARE_RECEIVE_REQUEST(MsgRequest);
  DECLARE_SEND_RESPONSE(MsgResponse);

  DECLARE_RECEIVE_REQUEST(SingleAttributesRequest);
  DECLARE_SEND_RESPONSE(SingleAttributesResponse);

  DECLARE_RECEIVE_REQUEST(AggregatedAttributesRequest);
  DECLARE_SEND_RESPONSE(AggregatedAttributesResponse);

  DECLARE_RECEIVE_REQUEST(CustomAttributesRequest);
  DECLARE_SEND_RESPONSE(CustomAttributesResponse);

  DECLARE_RECEIVE_REQUEST(AggregatedMemberRequest);
  DECLARE_SEND_RESPONSE(AggregatedMemberResponse);

  DECLARE_RECEIVE_REQUEST(CustomMemberRequest);
  DECLARE_SEND_RESPONSE(CustomMemberResponse);

  DECLARE_RECEIVE_REQUEST(SHMAddressRequest);
  DECLARE_SEND_RESPONSE(SHMAddressResponse);

  DECLARE_RECEIVE_REQUEST(NodeSwitchRequest);

private:
  int mMsgQueueId;
};
