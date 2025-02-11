#pragma once

#include <string>
#include <string_view>
#include <unistd.h>

#include "ipc/common.hpp"
#include "ipc/datastructs/information-datastructs.hpp"
#include "ipc/datastructs/misc-datastructs.hpp"
#include "ipc/datastructs/namespace-datastructs.hpp"
#include "ipc/datastructs/search-datastructs.hpp"

#define DECLARE_SEND_RESPONSE(ResponseType) \
  bool send##ResponseType(const ResponseType &response, pid_t receiverId, bool wait = true)

#define DECLARE_RECEIVE_REQUEST(RequestType) \
  RequestType receive##RequestType(requestId_t &oRequestId, pid_t &oSenderId, bool wait = true)


class IpcServer
{
public:
  IpcServer(int projectId);
  ~IpcServer();

  DECLARE_RECEIVE_REQUEST(NodeRequest);
  DECLARE_SEND_RESPONSE(NodeResponse);
  DECLARE_SEND_RESPONSE(NodeAliveUpdate);
  DECLARE_SEND_RESPONSE(NodePublishesToUpdate);
  DECLARE_SEND_RESPONSE(NodeSubscribesToUpdate);
  DECLARE_SEND_RESPONSE(NodeServicesUpdate);
  DECLARE_SEND_RESPONSE(NodeClientsUpdate);

  DECLARE_RECEIVE_REQUEST(TopicRequest);
  DECLARE_SEND_RESPONSE(TopicResponse);
  DECLARE_SEND_RESPONSE(TopicPublishersUpdate);
  DECLARE_SEND_RESPONSE(TopicSubscribersUpdate);
  //! TODO: writeTopicDataStreamObject

  DECLARE_RECEIVE_REQUEST(ProcessRequest);
  DECLARE_SEND_RESPONSE(ProcessResponse);
  DECLARE_SEND_RESPONSE(ProcessChildrenUpdate);
  //! TODO: writeProcessDataStreamObject
  //! TODO: writeProcessAccumulatedDataStreamObject

  DECLARE_RECEIVE_REQUEST(NamespaceRequest);
  DECLARE_SEND_RESPONSE(NamespaceResponse);

  DECLARE_RECEIVE_REQUEST(SearchRequest);
  DECLARE_SEND_RESPONSE(SearchResponse);

  DECLARE_RECEIVE_REQUEST(InitRequest);
  DECLARE_SEND_RESPONSE(InitResponse);

  DECLARE_RECEIVE_REQUEST(UnsubscribeRequest);
  DECLARE_SEND_RESPONSE(UnsubscribeResponse);

  DECLARE_RECEIVE_REQUEST(MsgRequest);
  DECLARE_SEND_RESPONSE(MsgResponse);

private:
  int mMsgQueueId;
};
