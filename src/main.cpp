#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <cstdlib>
#include <unistd.h>

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

std::vector<std::string> getPATHDirectories(const char* path){
  std::vector<std::string> directories;

  if(path == nullptr){
    return directories;
  }

  std::string pathStr(path);
  size_t start = 0;

  #ifdef _WIN32
    char delimiter = ';';
  #else
    char delimiter = ':';
  #endif

  while(true){
    size_t end = pathStr.find(delimiter, start);
    if(end == std::string::npos){
      directories.push_back(pathStr.substr(start));
      break;
    }

    directories.push_back(pathStr.substr(start, end - start));
    start = end + 1;
  }

  return directories;
}


int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::vector<std::string> built_in_command = {"echo", "exit", "type"};
  const char* pathENV = std::getenv("PATH");

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
    }else if(command == "echo"){
      for (size_t i = 1; i < tokens.size(); i++)
      {
        if(i > 1) std::cout << " ";
        std::cout << tokens[i];
      }
      std::cout << std::endl;
    }else if(command == "type"){
      if(tokens.size() < 2){
        std::cerr << "type command requires an argument" << std::endl;
        continue;
      }

      auto command_arg = tokens[1];
      if (std::find(built_in_command.begin(), built_in_command.end(), command_arg) != built_in_command.end())
      {
        std::cout << command_arg << " is a shell builtin" << std::endl;
      }
      else
      {
        auto pathDirs = getPATHDirectories(pathENV);
        bool found = false;

        for (const auto &dir : pathDirs)
        {
          std::string fullPath = dir + "/" + command_arg;
          // check if file exists and is executable
          if (access(fullPath.c_str(), X_OK) == 0)
          {
            std::cout << command_arg << " is " << fullPath << std::endl;
            found = true;
            break;
          }
        }

        if(!found)
        {
          std::cout << command_arg + ": not found" << std::endl;
        }
      }
    }
    else
    {
      std::cout << command + ": command not found" << std::endl;
    }
  }
  
}

