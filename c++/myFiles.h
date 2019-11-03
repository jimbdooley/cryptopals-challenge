#include <string>
#include <fstream>
#include <iostream>

std::string b64_file_to_string(std::string file_location){
    std::string rtn;
	std::string line;
  	std::ifstream myfile (file_location);
	int key, value;
	if (myfile.is_open()){
		while ( getline (myfile,line) )
		{
		    rtn += line;
		}
		myfile.close();
  	}
	else std::cout << "Unable to open file";
  	return rtn;
}

