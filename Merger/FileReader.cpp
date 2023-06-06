#include "FileReader.h"

FileReader::FileReader(std::string file_name) {
   doc.open(file_name);
   nextline();
}

FileReader::~FileReader() {
   doc.close();
}

size_t FileReader::getlinenum() {
   return linenumber;
}

std::optional<std::string> FileReader::currentline() {
   return line;
}

void FileReader::nextline() {
      std::string tmpstr{};
      if (std::getline(doc, tmpstr)) {
         line = std::make_optional<std::string>(tmpstr);
         linenumber++;
      }
      else {
         line = std::nullopt;
      }


}

bool FileReader::validFile() {
   if (line != std::nullopt) {
      return true;
   }
   else return false;
}

