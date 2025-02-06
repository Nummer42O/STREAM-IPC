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
  common::recieveMsg(mMsgQueueId, response, mPid, wait);

  oMessage = common::to_string(response.msg, sizeof(common::TestResponse::msg));
}