#pragma once

#include <iostream>
#include <string>

#include "ipc/common.hpp"


#define KEY_LOCATION "/tmp/example.ipc-key"
#define SERVER_MSG_TYPE 1l


namespace util
{

// REQUESTS/RESPONSES:

struct TestRequest
{
  msgKey_t type = SERVER_MSG_TYPE;
  pid_t senderId;
  char name[32];
};

struct TestResponse
{
  msgKey_t type;
  char msg[64];
};

// FUNCTIONS:

/**
 * @brief Get the ID of the message queue.
 *
 * @param projectId An ID unique to the communication channel that should be opened.
 *                  The least significant 8bits must be greater then 0.
 * @param create Wether the msg queue should be explicitly created or an existing id is expected.
 *
 * @throw IpcException When creating the key or getting/creating the message queue id fails.
 * @return ID of the message queue.
 */
int getMsgQueueId(
  int projectId,
  bool create = false
);

/**
 * @brief Send a request/response message.
 *
 * @tparam T A request/response struct like {long id; ...}.
 * @param msgQueueId The ID of the msg queue on which should be communicated.
 * @param payload The request/response to be send.
 * @param wait Wether to block when the message queue is full or return false.
 *
 * @throw IpcException When sending a message fails. Does not include EAGAIN.
 * @return Wether the message was send when wait is false, otherwise always true.
 */
template<typename T>
bool sendMsg(
  int msgQueueId,
  const T &payload,
  bool wait
);
#define DECLARE_TEMPLATE_SEND_MSG(templateType) template bool sendMsg<templateType>(int, const templateType &, bool)

/**
 * @brief Recieve a request/response message.
 *
 * @tparam T A request/response struct like {long id; ...}.
 * @param msgQueueId The ID of the msg queue on which should be communicated.
 * @param payload The output variable for the request/response.
 * @param msgType The type of message to recieve.
 * @param wait Wether to block/wait for message or return immedeatly if queue is empty.
 *
 * @throw IpcException When recieving a message fails.
 * @return The number of bytes of payload (everything except the id) read from the message queue.
 */
template<typename T>
ssize_t recieveMsg(
  int msgQueueId,
  T &payload,
  long msgType,
  bool wait
);
#define DECLARE_TEMPLATE_RECIEVE_MSG(templateType) template ssize_t recieveMsg<templateType>(int, templateType &, long , bool)

/**
 * @brief Convert maybe null terminated character array of fixed size to std::string
 *
 * @param src Pointer to initial element of source character array.
 * @param size Size of source character array.
 *
 * @return Converted string.
 */
std::string to_string(
  const char *src,
  size_t size
);

/**
 * @brief Encode combine PID and msg type into one key value for msg_typ to enable identifying request types per application.
 *
 * @param msgType Identifier for the type of request/response.
 * @param pid PID of calling/receiving process.
 *
 * @return Key with MSB = msgType & LSB = PID.
 */
msgKey_t makeMsgKey(
  msgType_t msgType,
  pid_t pid
);

}
