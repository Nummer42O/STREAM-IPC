#pragma once

#include <string>
#include <string_view>
#include <optional>
#include <unistd.h>

#include "ipc/common.hpp"
#include "ipc/datastructs/information-datastructs.hpp"
#include "ipc/datastructs/misc-datastructs.hpp"
#include "ipc/datastructs/namespace-datastructs.hpp"
#include "ipc/datastructs/search-datastructs.hpp"
#include "ipc/datastructs/intern/intern-datastructs.hpp"

#define DECLARE_SEND_REQUEST(RequestType) \
  bool send##RequestType(const RequestType &request, requestId_t &oRequestId, bool wait = true)

#define DECLARE_RECEIVE_RESPONSE(ResponseType) \
  std::optional<ResponseType> receive##ResponseType(bool wait = true)


class IpcClient
{
public:
  IpcClient(int projectId);

  DECLARE_SEND_REQUEST(NodeRequest);
  DECLARE_RECEIVE_RESPONSE(NodeResponse);
  DECLARE_RECEIVE_RESPONSE(NodeAliveUpdate);
  DECLARE_RECEIVE_RESPONSE(NodePublishesToUpdate);
  DECLARE_RECEIVE_RESPONSE(NodeSubscribesToUpdate);
  DECLARE_RECEIVE_RESPONSE(NodeServicesUpdate);
  DECLARE_RECEIVE_RESPONSE(NodeClientsUpdate);

  DECLARE_SEND_REQUEST(TopicRequest);
  DECLARE_RECEIVE_RESPONSE(TopicResponse);
  DECLARE_RECEIVE_RESPONSE(TopicPublishersUpdate);
  DECLARE_RECEIVE_RESPONSE(TopicSubscribersUpdate);
  //! TODO: readTopicDataStreamObject

  DECLARE_SEND_REQUEST(ProcessRequest);
  DECLARE_RECEIVE_RESPONSE(ProcessResponse);
  DECLARE_RECEIVE_RESPONSE(ProcessChildrenUpdate);
  //! TODO: readProcessDataStreamObject
  //! TODO: readProcessAccumulatedDataStreamObject

  DECLARE_SEND_REQUEST(NamespaceRequest);
  DECLARE_RECEIVE_RESPONSE(NamespaceResponse);

  DECLARE_SEND_REQUEST(SearchRequest);
  DECLARE_RECEIVE_RESPONSE(SearchResponse);

  DECLARE_SEND_REQUEST(InitRequest);
  DECLARE_RECEIVE_RESPONSE(InitResponse);

  DECLARE_SEND_REQUEST(UnsubscribeRequest);
  DECLARE_RECEIVE_RESPONSE(UnsubscribeResponse);

  DECLARE_SEND_REQUEST(MsgRequest);
  DECLARE_RECEIVE_RESPONSE(MsgResponse);

  DECLARE_SEND_REQUEST(ProcSwitchRequest);
  DECLARE_RECEIVE_RESPONSE(ProcSwitchResponse);

private:
  int32_t mRequestIdCounter;
  int mMsgQueueId;
  pid_t mPid;
};
