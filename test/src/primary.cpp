#include "ipc/ipc-server.hpp"
#include "ipc/common.hpp"

#include <iostream>
#include <sstream>


int main()
{
  IpcServer server(1);

  std::string input, output;
  msgKey_t senderId;
  while (true)
  {
    server.receiveTestRequest(input, senderId);

    output = "Hello " + input + "!";

    if (!server.sendTestResponse(output, senderId, false))
      std::cerr << "Failed to send reponse.\n";
  }
}