#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <optional>

class FileReader
{
   private:
      std::ifstream doc;
      std::optional<std::string> line;
      size_t linenumber = 0;

   public:
      FileReader(std::string);
      ~FileReader();

      size_t getlinenum();
      std::optional<std::string> currentline();
      void nextline();
      bool validFile();

};

