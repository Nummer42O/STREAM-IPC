#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "ipc/common.hpp"


#define KEY_LOCATION "/tmp/example.ipc-key"
#define SERVER_PSEUDO_PID 1


namespace util
{

template<typename T>
struct RequestMsg
{
  msgKey_t key;
  requestId_t requestId;
  pid_t senderId;

  T payload;
};

template<typename T>
struct ResponseMsg
{
  msgKey_t key;

  T payload;
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

msgKey_t makeMsgKey(
  msgType_t msgType
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

/**
 * @brief receive a request/response message.
 *
 * @tparam T A request/response struct like {long id; ...}.
 * @param msgQueueId The ID of the msg queue on which should be communicated.
 * @param payload The output variable for the request/response.
 * @param msgKey The type of message to receive.
 * @param wait Wether to block/wait for message or return immedeatly if queue is empty.
 *
 * @throw IpcException When recieving a message fails.
 * @return Wether a message was received when wait is false, otherwise always true.
 */
template<typename T>
bool receiveMsg(
  int msgQueueId,
  T &payload,
  msgKey_t msgKey,
  bool wait
);

#define DECLARE_MSG_TEMPLATES(T)                                                \
  template struct RequestMsg<T>;                                                \
  template struct ResponseMsg<T>;                                               \
  template bool sendMsg<RequestMsg<T>>(int, const RequestMsg<T> &, bool);       \
  template bool sendMsg<ResponseMsg<T>>(int, const ResponseMsg<T> &, bool);     \
  template bool receiveMsg<RequestMsg<T>>(int, RequestMsg<T> &, long , bool);   \
  template bool receiveMsg<ResponseMsg<T>>(int, ResponseMsg<T> &, long , bool); \



/**
 * @brief Convert maybe null terminated character array of fixed size to std::string
 *
 * @param src Pointer to initial element of source character array.
 *
 * @return Converted string.
 */
std::string parseString(
  const char (&src)[MAX_STRING_SIZE]
);

void parseString(
  char (&dst)[MAX_STRING_SIZE],
  const std::string &src
);

std::vector<std::string> parseStringArray(
  const char (&src)[MAX_ARRAY_SIZE][MAX_STRING_SIZE]
);

void parseStringArray(
  char (&dst)[MAX_ARRAY_SIZE][MAX_STRING_SIZE],
  const std::vector<std::string> &src
);

template<typename T>
std::vector<T> parseArray(
  const T (&src)[MAX_ARRAY_SIZE]
);

template<typename T>
void parseArray(
  T (&dst)[MAX_ARRAY_SIZE],
  const std::vector<T> &src
);

#define DECLARE_PARSE_ARRAY_TEMPLATE(T)                                             \
  template std::vector<T> parseArray<T>(const T (&src)[MAX_ARRAY_SIZE]);            \
  template void parseArray<T>(T (&dst)[MAX_ARRAY_SIZE], const std::vector<T> &src);

}
