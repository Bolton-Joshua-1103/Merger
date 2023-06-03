#include <iostream>
#include <list> // Should this be a list?
#include <memory>
#include <algorithm>
#include "FileReader.h"

using namespace std;

void removeEmptyFiles(std::list<shared_ptr<FileReader>>& files) {
   //How do you remove more than one file at a time? My solution was keep removing one elemnt until
   // we iterated through without modifying.
   bool EmptyFiles = true;
   while(EmptyFiles==true){
      EmptyFiles = false;
      for (auto ele = files.begin(); ele != files.end(); ele++) {
         /*if (!((*ele)->currentline().has_value())) {*/
         if (!((*ele)->validFile())) {
            EmptyFiles = true;
            ele = files.erase(ele);
            break;
         }
      }
   }

}

std::string minimumLineFromFile(std::list<shared_ptr<FileReader>>& files) {
   shared_ptr<FileReader> minLineFile = files.front();
   for (auto ele : files) {
      if (std::stoi(ele->currentline().value()) < std::stoi(minLineFile->currentline().value())) {
         minLineFile = ele;
      }
   }
   string rtrnstring = minLineFile->currentline().value();
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