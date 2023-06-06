#include <iostream>
#include <list> // Should this be a list?
#include <memory>
#include <algorithm>
#include "FileReader.h"

using namespace std;

void removeEmptyFiles(std::list<shared_ptr<FileReader>>& files) {
   files.remove_if([](shared_ptr<FileReader> ele) {return !(*ele).validFile(); });
}

std::string minimumLineFromFile(std::list<shared_ptr<FileReader>>& files) {

   string rtrnstring{};
   shared_ptr<FileReader> minLineFile = files.front();

   //Find smallest of the current lines
   for (auto ele : files) {
      if (std::stoi(ele->currentline().value()) < std::stoi(minLineFile->currentline().value())) {
         minLineFile = ele;
      }
   }

   //Store smallest line, advance file of smallest line, remove any empty collections, return
   rtrnstring = minLineFile->currentline().value();
   minLineFile->nextline();
   removeEmptyFiles(files);
   return rtrnstring;
}

int main() {

   std::list<shared_ptr<FileReader>> files;
   std::list<std::string> filenames{ "doc1.txt", "doc2.txt","doc3.txt", "doc4.txt","empty.txt","empty1.txt" };
   /* creating the vector manually, later will learn command arguements*/

   //Loads list with primed filer objects
   for (string filename : filenames) {
      files.push_back(std::make_shared<FileReader>(filename));
   }

   std::ofstream masterfile;
   masterfile.open("masterLog.txt");
   removeEmptyFiles(files);
   while (!files.empty()) {
      masterfile << minimumLineFromFile(files) << endl;
   }


}