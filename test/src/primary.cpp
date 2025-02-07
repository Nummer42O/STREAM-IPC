#include "ipc/ipc-server.hpp"

#include <iostream>
#include <sstream>


int main()
{
  IpcServer server(1);

  std::string input, output;
  IpcServer::msgKey_t senderId;
  while (true)
  {
    server.recieveTestRequest(input, senderId);

    output = "Hello " + input + "!";

    if (!server.sendTestResponse(output, senderId, false))
      std::cerr << "Failed to send reponse.\n";
  }
}