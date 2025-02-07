#include <filesystem>
namespace fs = std::filesystem;

#include <cassert>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <unistd.h>

#include "ipc/ipc-client.hpp"
#include "ipc/ipc-exceptions.hpp"
#include "util.hpp"


IpcClient::IpcClient(int projectId)
{
  mMsgQueueId = util::getMsgQueueId(projectId);
  mPid = ::getpid();
}

bool IpcClient::sendTestRequest(std::string_view name, bool wait)
{
  util::TestRequest request{
    .senderId = mPid
  };
  std::strncpy(request.name, name.cbegin(), sizeof(util::TestRequest::name));

  return util::sendMsg(mMsgQueueId, request, wait);
}

void IpcClient::recieveTestResponse(std::string &oMessage, bool wait)
{
  util::TestResponse response;
  msgKey_t msgKey = util::makeMsgKey(mPid, 0); //! NOTE: would need appropriate value, in this example its not used
  util::recieveMsg(mMsgQueueId, response, msgKey, wait);

  oMessage = util::to_string(response.msg, sizeof(util::TestResponse::msg));
}
