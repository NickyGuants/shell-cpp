#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string &str)
{
  std::vector<std::string> tokens;
  std::istringstream iss(str);
  std::string token;

  while (iss >> token)
  {
    tokens.push_back(token);
  }

  return tokens;
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true)
  {
    // display prompt
    std::cout << "$ ";

    // read user input
    std::string input;
    std::getline(std::cin, input);

    //Process input
    //Split the input into individual words (tokens) => result ["command", "arg"]
    auto tokens = split(input);

    if(tokens.empty()) continue;

    const auto& command = tokens[0];

    if(command == "exit"){
      int code = tokens.size() > 1 ? std::stoi(tokens[1]) : 0;
      exit(code);
    }else{
      std::cout << command + ": command not found" + "\n";
    }
  }
  
}

