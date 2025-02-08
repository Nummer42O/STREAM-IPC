#pragma once

#include <string>
#include <string_view>
#include <unistd.h>

#include "ipc/common.hpp"
#include "ipc/datastructs/information-datastructs.hpp"
#include "ipc/datastructs/misc-datastructs.hpp"
#include "ipc/datastructs/namespace-datastructs.hpp"
#include "ipc/datastructs/search-datastructs.hpp"


class IpcServer
{
public:
  IpcServer(int projectId);
  ~IpcServer();

  /**
   * @brief Read requests from message queue.
   *
   * @param oName output variable for name field of request
   * @param oSenderId output variable for senderId field of request
   * @param wait wether to block/wait for message or return immedeatly if queue is empty
   */
  void receiveTestRequest(
    std::string &oName,
    msgKey_t &oSenderId,
    bool wait = true
  );

  /**
   * @brief Send response message back to requesting process.
   *
   * @param msg response message
   * @param receiverId process id of the requesting process
   * @param wait wether to block when message queue is full or return false
   *
   * @return wether message was send when wait is false, otherwise always true
   */
  bool sendTestResponse(
    std::string_view msg,
    msgKey_t receiverId,
    bool wait = true
  );

  NamespaceDataRequest receiveNamespaceRequest(
    bool wait = true
  );

  SearchDataRequest receiveSearchRequest(
    bool wait = true
  );

  MsgDataRequest receiveMsgRequest(
    bool wait = true
  );

  InitDataRequest receiveInitRequest(
    bool wait = true
  );

  UnsubscribeDataRequest receiveUnsubscribeRequest(
    bool wait = true
  );

  bool sendNamespaceResponse(
    NamespaceDataResponse response,
    msgKey_t receiverId,
    bool wait = true
  );

  bool sendSearchResponse(
    SearchDataResponse response,
    msgKey_t receiverId,
    bool wait = true
  );

  bool sendMsgResponse(
    MsgDataResponse response,
    msgKey_t receiverId,
    bool wait = true
  );

  bool sendInitResponse(
    InitDataResponse response,
    msgKey_t receiverId,
    bool wait = true
  );

  bool sendUnsubscribeResponse(
    msgKey_t receiverId,
    bool wait = true
  );


private:
  int mMsgQueueId;
};
