#include <iostream>
#include <list>
#include <memory>
#include <algorithm>
#include <type_traits>
#include "FileReader.h"

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>


using namespace std;

boost::program_options::variables_map parseCommandLine(int argc, const char* argv[]) {
   using namespace boost::program_options;
   variables_map variables_map;

   try
   {
      options_description description{ "Options" };
      description.add_options()
         ("help,h", "Help info")
         ("output,o", value<std::string>(), "Output file.  Default: masterLog.txt")
         ("input,i", value<std::vector<std::string>>()->multitoken(), "Input file(s)");

      // Allow a list of input files to be passed without a specific command line option
      positional_options_description pos_desc;
      pos_desc.add("input", -1);

      command_line_parser parser{ argc, argv }; //load parser with cmd line args
      parser.options(description).positional(pos_desc).allow_unregistered(); //formatting how to parce
      parsed_options parsed_options = parser.run();
      store(parsed_options, variables_map); // Loads variables_map with parsed cmd line arguments, this should be my input files?

      // Print the help to cout if help option specified
      if (variables_map.count("help"))
      {
         std::cout << "Utility program to combine multiple log files into a single log file." << std::endl;
         std::cout << "Assumes that individual log files are sorted (with numeric data as first field)." << std::endl;
         std::cout << description << std::endl;
      }
   }
   catch (const error& ex)
   {
      std::cout << ex.what() << std::endl;
   }

   
   return variables_map;

}


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

void MergeFiles(boost::program_options::variables_map& vm, const std::string& destination_file) {


   std::list<shared_ptr<FileReader>> files;
   std::vector<std::string> input_files;

   //Fills contents of string vector (file names)
   if (vm.count("input"))
   {
      input_files = vm["input"].as<std::vector<std::string>>(); // loads vector with strings
   }
   else { 
      cout << "No input files" << endl;
      return;
   }

   //Fills content of FileReader smart point list using file names
   for (const auto&ele : input_files) {
      files.push_back(std::make_shared <FileReader>(ele));
   }

   //Out puts input file names to command line.
   std::cout << "Input files: " << std::endl;
   for (const auto& file_name : input_files) std::cout << "   " << file_name << std::endl;


   //Create and Open destination file
   std::ofstream destination;
   destination.open(destination_file);

   // Clear any empty input collections
   removeEmptyFiles(files);

   //begin merging into destination file
   while (!files.empty()) {
      destination << minimumLineFromFile(files) << endl;
   }
}

int main(int argc, const char* argv[]) {

   // Parse through command line arguements
   auto && variables_map = parseCommandLine(argc, argv);

   // Output file options
   std::string masterfile = "masterLog.txt";
   if (variables_map.count("output")) {
      masterfile = variables_map["output"].as<std::string>();
      cout << "Output file will be in project directory" << endl;
   }
   std::cout << "Output file: " << masterfile << std::endl;

   MergeFiles(variables_map, masterfile);
}