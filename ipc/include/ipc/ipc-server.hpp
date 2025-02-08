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

  /**
   * @brief Read NamespaceRequests from message queue.
   *
   * @param wait wether to block/wait for message or return immedeatly if queue is empty
   * 
   * @return request
   */
  NamespaceDataRequest receiveNamespaceRequest(
    bool wait = true
  );

  /**
   * @brief Read SearchRequest from message queue.
   *
   * @param wait wether to block/wait for message or return immedeatly if queue is empty
   * 
   * @return request
   */
  SearchDataRequest receiveSearchRequest(
    bool wait = true
  );

  /**
   * @brief Read MsgRequest from message queue.
   *
   * @param wait wether to block/wait for message or return immedeatly if queue is empty
   * 
   * @return request
   */
  MsgDataRequest receiveMsgRequest(
    bool wait = true
  );

  /**
   * @brief Read InitRequest from message queue.
   *
   * @param wait wether to block/wait for message or return immedeatly if queue is empty
   * 
   * @return request
   */
  InitDataRequest receiveInitRequest(
    bool wait = true
  );

  /**
   * @brief Read UnsubscribeRequest from message queue.
   *
   * @param wait wether to block/wait for message or return immedeatly if queue is empty
   * 
   * @return request
   */
  UnsubscribeDataRequest receiveUnsubscribeRequest(
    bool wait = true
  );

  /**
   * @brief Write NamespaceResponse to message queue.
   *
   * @param payload information about Namespace
   * @param receiverId process id of the requesting process
   * @param wait wether to block when message queue is full or return false
   * 
   * @return request
   */
  bool sendNamespaceResponse(
    NamespaceDataResponse payload,
    msgKey_t receiverId,
    bool wait = true
  );

  /**
   * @brief Write SearchResponse to message queue.
   *
   * @param payload information about requested Object
   * @param receiverId process id of the requesting process
   * @param wait wether to block when message queue is full or return false
   * 
   * @return request
   */
  bool sendSearchResponse(
    SearchDataResponse payload,
    msgKey_t receiverId,
    bool wait = true
  );

  /**
   * @brief Write MsgResponse to message queue.
   *
   * @param payload pointer to shared memory location
   * @param receiverId process id of the requesting process
   * @param wait wether to block when message queue is full or return false
   * 
   * @return request
   */
  bool sendMsgResponse(
    MsgDataResponse payload,
    msgKey_t receiverId,
    bool wait = true
  );

  /**
   * @brief write Acknowledge for InitRequest to message queue.
   *
   * @param receiverId process id of the requesting process
   * @param wait wether to block when message queue is full or return false
   * 
   * @return request
   */
  bool sendInitResponse(
    msgKey_t receiverId,
    bool wait = true
  );

  /**
   * @brief write Acknowledge for UnsubscribeRequest to message queue.
   *
   * @param receiverId process id of the requesting process
   * @param wait wether to block when message queue is full or return false
   * 
   * @return request
   */
  bool sendUnsubscribeResponse(
    msgKey_t receiverId,
    bool wait = true
  );

  /**
   * @brief write Acknowledge for UnsubscribeRequest to message queue.
   *
   * @param receiverId process id of the requesting process
   * @param wait wether to block when message queue is full or return false
   * 
   * @return request
   */
  bool sendAckResponse(
    msgKey_t receiverId,
    bool wait = true
  );


private:
  int mMsgQueueId;
};
