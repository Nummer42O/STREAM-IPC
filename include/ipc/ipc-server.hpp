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

  DECLARE_SEND_RESPONSE(NodeRequest);
  DECLARE_RECEIVE_REQUEST(NodeResponse);
  DECLARE_RECEIVE_REQUEST(NodeAliveUpdate);
  DECLARE_RECEIVE_REQUEST(NodePublishesToUpdate);
  DECLARE_RECEIVE_REQUEST(NodeSubscribesToUpdate);
  DECLARE_RECEIVE_REQUEST(NodeServicesUpdate);
  DECLARE_RECEIVE_REQUEST(NodeClientsUpdate);

  DECLARE_SEND_RESPONSE(TopicRequest);
  DECLARE_RECEIVE_REQUEST(TopicResponse);
  DECLARE_RECEIVE_REQUEST(TopicPublishersUpdate);
  DECLARE_RECEIVE_REQUEST(TopicSubscribersUpdate);
  //! TODO: writeTopicDataStreamObject

  DECLARE_SEND_RESPONSE(ProcessRequest);
  DECLARE_RECEIVE_REQUEST(ProcessResponse);
  DECLARE_RECEIVE_REQUEST(ProcessChildrenUpdate);
  //! TODO: writeProcessDataStreamObject
  //! TODO: writeProcessAccumulatedDataStreamObject

  DECLARE_SEND_RESPONSE(NamespaceRequest);
  DECLARE_RECEIVE_REQUEST(NamespaceResponse);

  DECLARE_SEND_RESPONSE(SearchRequest);
  DECLARE_RECEIVE_REQUEST(SearchResponse);

  DECLARE_SEND_RESPONSE(InitRequest);
  DECLARE_RECEIVE_REQUEST(InitResponse);

  DECLARE_SEND_RESPONSE(UnsubscribeRequest);
  DECLARE_RECEIVE_REQUEST(UnsubscribeResponse);

  DECLARE_SEND_RESPONSE(MsgRequest);
  DECLARE_RECEIVE_REQUEST(MsgResponse);

private:
  int mMsgQueueId;
};
