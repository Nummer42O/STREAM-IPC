#include "ipc/ipc-client.hpp"

#include <iostream>


int main()
{
  IpcClient client(1);

  std::string input, output;
  while (true)
  {
    std::cout << ">>> " << std::flush;
    std::cin >> input;

    if (!client.sendTestRequest(input, false))
    {
      std::cerr << "Failed to send message.\n";
      continue;
    }

    client.recieveTestResponse(output);
    std::cout << "Recieved message: \"" << output << "\"\n";
  }
}