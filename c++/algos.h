#include <fstream>
#include <iostream>
#include <unordered_map> 
#include <math.h>
#include <vector>

int count_duplicates(std::vector<int> bytes, int key_size){
	if(bytes.size()%key_size != 0){
		printf("bytes need padding\n");
		return -1;
	}
	auto tracker = std::unordered_map<std::string, int>();
	std::string key;
	std::unordered_map<std::string, int>::iterator it;
	int max_duplicate = 1;
	for(int i = 0; i < bytes.size(); i+= key_size){
		key = "";
		for(int j = 0; j < key_size; j++){
			key += (char)(48+bytes[i+j]/100);
			key += (char)(48+(bytes[i+j]%100)/10);
			key += (char)(48+bytes[i+j]%10);
		}
		it = tracker.find(key);
		if(it == tracker.end()){
			tracker.insert(std::make_pair(key, 1));
		}
		else{
			(it->second) += 1;
			max_duplicate = std::max(max_duplicate, it->second);
		}
	}
	return max_duplicate;
}

void pad_PKCS_7(std::vector<int> &bytes, int block_size){
	int pad_size = (block_size - bytes.size()%block_size)%16;
	bytes.reserve(bytes.size() + pad_size);
	for(int i = 0; i < pad_size; i++){
		bytes.push_back(pad_size);
	}
}

void repeating_key_xor(std::vector<int> &message, const std::vector<int> &key){
	for(int i = 0; i < message.size(); i++){
		message[i] ^= key[i%key.size()];
	}
}

int get_first_number(std::string s){
	int number_length = 0;
	for(int i = 0; i<s.size(); i++){
		if(s[i] != ' '){
			number_length += 1;
		}
		else{
			break;
		}
	}
	int rtn = 0;
	for(int i = 0; i < number_length; i++){
		rtn += (int(s[i])-48)*pow(10, (double)(number_length-i-1));
	}
	return rtn;
}

int get_second_number(std::string s){
	int spaces_encountered = 0;
	std::string new_s = "";
	for(int i = 0; i < s.size(); i++){
		if(s[i] == ' '){
			spaces_encountered += 1;
			if(spaces_encountered > 2){
				break;
			}
			continue;
		}
		if(spaces_encountered == 2){
			if((s[i]>=48)&&(s[i]<=57)){
				new_s += s[i];
			}
		}
	}
	return get_first_number(new_s);
}

std::unordered_map<int, int> get_ascii_freq(){
	std::unordered_map<int, int> rtn;
	std::string line;
  	std::ifstream myfile ("resources/ascii_freq");
	int key, value;
	if (myfile.is_open()){
		while ( getline (myfile,line) )
		{
		    key = get_first_number(line);
			value = get_second_number(line);
			rtn.insert(std::make_pair(key, value));
		}
		myfile.close();
  	}
	else std::cout << "Unable to open file";
  	return rtn;
}

