#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "ipc/common.hpp"


#define KEY_LOCATION "/tmp/example.ipc-key"
#define SERVER_PSEUDO_PID 1


namespace util
{

using namespace ipc;

/**
 * @addtogroup util
 * @{
 */

/**
 * @brief Payload-agnostic message type for basic request functionality.
 *
 * @tparam T One of the datastructs defined in the @ref ipc_datastructs "ipc::datastructs" namespace.
 */
template<typename T>
struct RequestMsg
{
  //! Key by which to differentiate the message by its type and sender on the queue.
  msgKey_t key;
  /**
   * @brief Unique ID identifying this message regardless.
   *
   * @note There is no guarantee that different users senders use non-overlapping request IDs.
   */
  requestId_t requestId;
  //! PID of the sender/sending process.
  pid_t senderId;

  //! A struct defining the actual information to be transmitted.
  T payload;
};

/**
 * @brief Payload-agnostic message type for basic response functionality.
 *
 * @tparam T One of the datastructs defined in the @ref ipc_datastructs "ipc::datastructs" namespace.
 */
template<typename T>
struct ResponseMsg
{
  //! Key by which to differentiate the message by its type and sender on the queue.
  msgKey_t key;

  //! A struct defining the actual information to be transmitted.
  T payload;
};


/**
 * @brief Get the ID of the message queue.
 *
 * @param[in]   projectId An ID unique to the communication channel that should be opened.
 *                        The least significant 8bits must be greater then 0.
 * @param[in]   create Wether the msg queue should be explicitly created or an existing id is expected.
 *
 * @throw ipc::IpcException When creating the key or getting/creating the message queue id fails.
 * @return ID of the message queue.
 */
int getMsgQueueId(
  int projectId,
  bool create = false
);

/**
 * @brief Encode PID and msg type into one key value for `msgtyp` to enable identifying request types per application.
 *
 * @param[in]  msgType Identifier for the type of request/response.
 * @param[in]  pid PID of calling/receiving process.
 *
 * @return Key with MSB = @p msgType & LSB = @p pid.
 */
msgKey_t makeMsgKey(
  msgType_t msgType,
  pid_t pid
);

/**
 * @brief Encode `SERVER_PSEUDO_PID` and msg type into one key value for `msgtyp` to enable identifying request types per application.
 *
 * @param[in]  msgType Identifier for the type of request/response.
 *
 * @return Key with MSB = @p msgType & LSB = `SERVER_PSEUDO_PID`.
 */
msgKey_t makeMsgKey(
  msgType_t msgType
);

/**
 * @brief Send a request/response message.
 *
 * @tparam T A request/response struct like `{long id; ...}`.
 * @param[in]  msgQueueId The ID of the msg queue on which should be communicated.
 * @param[in]  payload The request/response to be send.
 * @param[in]  wait Wether to block when the message queue is full or return false.
 *
 * @throw ipc::IpcException When sending a message fails. Does not include EAGAIN.
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
 * @tparam T A request/response struct like `{long id; ...}`.
 * @param[in] msgQueueId The ID of the msg queue on which should be communicated.
 * @param[out] payload The output variable for the request/response.
 * @param[in] msgKey The type of message to receive.
 * @param[in] wait Wether to block/wait for message or return immedeatly if queue is empty.
 *
 * @throw ipc::IpcException When recieving a message fails.
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
  template bool receiveMsg<ResponseMsg<T>>(int, ResponseMsg<T> &, long , bool);

/**
 * @brief Convert character array to STL string.
 *
 * Data source that does not fill out the array is expected to be null terminated.
 *
 * @param[in]  src Source character array.
 *
 * @return STL string.
 */
std::string parseString(
  const char (&src)[MAX_STRING_SIZE]
);

/**
 * @brief Convert STL string to character array.
 *
 * If `src.size() > MAX_STRING_SIZE`, @p dst will be a truncated version of @p src. <br>
 * If `src.size() < MAX_STRING_SIZE`, @p dst will be null terminated.
 *
 * @param[out] dst Destination character array.
 * @param[in]  src Source STL string.
 */
void parseString(
  char (&dst)[MAX_STRING_SIZE],
  const std::string &src
);

/**
 * @brief Convert array of character arrays to a STL vector of STL strings.
 *
 * If the array is not full, it is expected to be null terminated.
 * Similarly, if the string does not fill out the character array it is expected to be null terminated.
 *
 * @param[in]  src Source array of character arrays.
 *
 * @return STL vector of STL strings.
 */
std::vector<std::string> parseStringArray(
  const char (&src)[MAX_ARRAY_SIZE][MAX_STRING_SIZE]
);

/**
 * @brief Convert STL vector of STL strings to array of character arrays.
 *
 * If `src.size() > MAX_ARRAY_SIZE`, @p dst will be a truncated version of @p src. <br>
 * If `src.size() < MAX_ARRAY_SIZE`, @p dst will be null terminated. <br>
 * If `src[i].size() > MAX_STRING_SIZE`, `dst[i]` will be a truncated version of `src[i]`. <br>
 * If `src[i].size() < MAX_STRING_SIZE`, `dst[i]` will be null terminated. <br>
 *
 * @param[out] dst Destination array of character arrays.
 * @param[in]  src Source STL vector of STL strings.
 */
void parseStringArray(
  char (&dst)[MAX_ARRAY_SIZE][MAX_STRING_SIZE],
  const std::vector<std::string> &src
);

/**
 * @brief Convert array of @p T into a STL vector of @p T.
 *
 * @tparam T A trivially construtcable and comparable type. Must be nullable.
 * @param[in]  src Source array of @p T.
 *
 * @return STL vector of @p T.
 */
template<typename T>
std::vector<T> parseArray(
  const T (&src)[MAX_ARRAY_SIZE]
);

/**
 * @brief Convert STL vector of @p T into a array of @p T.
 *
 * @tparam T A trivially constructable and comparable type. Must be nullable.
 * @param[out] dst Destination array of @p T.
 * @param[in]  src Source STL vector of @p T.
 */
template<typename T>
void parseArray(
  T (&dst)[MAX_ARRAY_SIZE],
  const std::vector<T> &src
);

#define DECLARE_PARSE_ARRAY_TEMPLATE(T)                                             \
  template std::vector<T> parseArray<T>(const T (&src)[MAX_ARRAY_SIZE]);            \
  template void parseArray<T>(T (&dst)[MAX_ARRAY_SIZE], const std::vector<T> &src);

//! @}

}
