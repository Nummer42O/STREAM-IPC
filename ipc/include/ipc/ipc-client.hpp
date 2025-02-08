#pragma once

#include <string>
#include <string_view>
#include <unistd.h>

#include "ipc/common.hpp"
#include "ipc/datastructs/information-datastructs.hpp"
#include "ipc/datastructs/namespace-datastructs.hpp"
#include "ipc/datastructs/search-datastructs.hpp"
#include "ipc/datastructs/misc-datastructs.hpp"


class IpcClient
{
public:
  IpcClient(int projectId);

  /**
   * @brief Send request for message with name.
   *
   * @param name name that should be in the message
   * @param wait wether to block when message queue is full or return false
   *
   * @return returns wether message was send when wait is false, otherwise always true
   */
  bool sendTestRequest(
    std::string_view name,
    bool wait = true
  );

  /**
   * @brief Read response from message queue.
   *
   * @param oMessage output variable for msg field of response
   * @param wait wether to block/wait for message or return immedeatly if queue is empty
   */
  void receiveTestResponse(
    std::string &oMessage,
    bool wait = true
  );

  /**
   * @brief Send request for Namespace children.
   *
   * @param payload analyzed namespace
   * @param wait wether to block when message queue is full or return false
   *
   * @return returns wether message was send when wait is false, otherwise always true
   */
  bool sendNamespaceRequest(
    NamespaceDataRequest payload,
    bool wait = true
  );

  /**
   * @brief Send request for primary-key of node or topic.
   *
   * @param payload whether to search for nodes or topics and name to search for
   * @param wait wether to block when message queue is full or return false
   *
   * @return returns wether message was send when wait is false, otherwise always true
   */
  bool sendSearchRequest(
    SearchDataRequest payload,
    bool wait = true
  );

  /**
   * @brief Send request for continuous message flow.
   *
   * @param payload topic (its primary-key) to get messages from with a given target frequency
   * @param wait wether to block when message queue is full or return false
   *
   * @return returns wether message was send when wait is false, otherwise always true
   */
  bool sendMsgRequest(
    MsgDataRequest payload,
    bool wait = true
  );

  /**
   * @brief Send request for initialising filters.
   *
   * @param payload entitities that should be ignored
   * @param wait wether to block when message queue is full or return false
   *
   * @return returns wether message was send when wait is false, otherwise always true
   */
  bool sendInitRequest(
    InitDataRequest payload,
    bool wait
  );

  /**
   * @brief Send request for unsubscribing message.
   *
   * @param payload request (by its id) that shall be disabled
   * @param wait wether to block when message queue is full or return false
   *
   * @return returns wether message was send when wait is false, otherwise always true
   */
  bool sendUnsubscribeRequest(
    UnsubscribeDataRequest payload,
    bool wait = true
  );

  /**
   * @brief Read NamespaceResponse from message queue.
   *
   * @param wait wether to block/wait for message or return immedeatly if queue is empty
   */
  NamespaceDataResponse receiveNamespaceResponse(
    bool wait = true
  );

  /**
   * @brief Read SearchResponse from message queue.
   *
   * @param wait wether to block/wait for message or return immedeatly if queue is empty
   */
  SearchDataResponse receiveSearchResponse(
    bool wait = true
  );

  /**
   * @brief Read MsgResponse from message queue.
   *
   * @param wait wether to block/wait for message or return immedeatly if queue is empty
   */
  MsgDataResponse receiveMsgResponse(
    bool wait = true
  );

  /**
   * @brief Read InitResponse from message queue.
   *
   * @param wait wether to block/wait for message or return immedeatly if queue is empty
   */
  InitDataResponse receiveInitResponse(
    bool wait = true
  );

  /**
   * @brief Read UnsubscribResponse from message queue.
   *
   * @param wait wether to block/wait for message or return immedeatly if queue is empty
   */
  UnsubscribeDataResponse receiveUnsubscribeResponse(
    bool wait = true
  );

private:
  int mMsgQueueId;
  pid_t mPid;
};
