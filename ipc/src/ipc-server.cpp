#include <filesystem>
namespace fs = std::filesystem;

#include <cstring>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "ipc/ipc-server.hpp"
#include "util.hpp"


IpcServer::IpcServer(int projectId)
{
  if (!fs::exists(KEY_LOCATION))
    fs::create_directories(KEY_LOCATION);

  mMsgQueueId = util::getMsgQueueId(projectId, true);
}

IpcServer::~IpcServer()
{
  int status = ::msgctl(mMsgQueueId, IPC_RMID, nullptr);
  if (status == -1)
    std::clog \
      << "Error " << strerrorname_np(errno) \
      << " closing message queue: " << strerrordesc_np(errno) << '\n';
}

bool IpcServer::sendTestResponse(std::string_view msg, msgKey_t recieverId, bool wait)
{
  util::TestResponse response{
    .type = util::makeMsgKey(recieverId, 0) //! NOTE: would need appropriate value, in this example its not used
  };
  std::strncpy(response.msg, msg.cbegin(), sizeof(util::TestResponse::msg));

  return util::sendMsg(mMsgQueueId, response, wait);
}

void IpcServer::recieveTestRequest(std::string &oName, msgKey_t &oSenderId, bool wait)
{
  util::TestRequest request;
  util::recieveMsg(mMsgQueueId, request, SERVER_MSG_TYPE, wait);

  oName = util::to_string(request.name, sizeof(util::TestRequest::name));
  oSenderId = request.senderId;
}
