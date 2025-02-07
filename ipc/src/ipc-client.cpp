#include <filesystem>
namespace fs = std::filesystem;

#include <cassert>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <unistd.h>

#include "ipc/ipc-client.hpp"
#include "ipc/ipc-exceptions.hpp"
#include "common.hpp"


IpcClient::IpcClient(int projectId)
{
  mMsgQueueId = common::getMsgQueueId(projectId);
  mPid = ::getpid();
}

bool IpcClient::sendTestRequest(std::string_view name, bool wait)
{
  common::TestRequest request{
    .senderId = mPid
  };
  std::strncpy(request.name, name.cbegin(), sizeof(common::TestRequest::name));

  return common::sendMsg(mMsgQueueId, request, wait);
}

void IpcClient::recieveTestResponse(std::string &oMessage, bool wait)
{
  common::TestResponse response;
  msgKey_t msgKey = common::makeMsgKey(mPid, 0); //! NOTE: would need appropriate value, in this example its not used
  common::recieveMsg(mMsgQueueId, response, msgKey, wait);

  oMessage = common::to_string(response.msg, sizeof(common::TestResponse::msg));
}
