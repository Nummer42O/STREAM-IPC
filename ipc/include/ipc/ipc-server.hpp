#pragma once

#include <string>
#include <string_view>
#include <unistd.h>


class IpcServer
{
public:
  using msgKey_t = __syscall_slong_t;

public:
  IpcServer(int projectId);
  ~IpcServer();

  /**
   * @brief Send response message back to requesting process.
   *
   * @param msg response message
   * @param recieverId process id of the requesting process
   * @param wait wether to block when message queue is full or return false
   *
   * @return wether message was send when wait is false, otherwise always true
   */
  bool sendTestResponse(
    std::string_view msg,
    msgKey_t recieverId,
    bool wait = true
  );

  /**
   * @brief Read requests from message queue.
   *
   * @param oName output variable for name field of request
   * @param oSenderId output variable for senderId field of request
   * @param wait wether to block/wait for message or return immedeatly if queue is empty
   */
  void recieveTestRequest(
    std::string &oName,
    msgKey_t &oSenderId,
    bool wait = true
  );

private:
  int mMsgQueueId;
};
