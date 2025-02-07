#pragma once

#include <string>
#include <string_view>
#include <unistd.h>

#include "ipc/common.hpp"
#include "ipc/datastructs/information-datastructs.hpp"
#include "ipc/datastructs/misc-datastructs.hpp"
#include "ipc/datastructs/namespace-datastructs.hpp"
#include "ipc/datastructs/search-datastructs.hpp"


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

private:
  int mMsgQueueId;
  pid_t mPid;
};
