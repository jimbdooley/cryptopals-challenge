#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <unordered_map>
#include <vector>
#include "translations.h"
#include "algos.h"
#include "AES.h"
#include "myFiles.h"

void show_vector(std::vector<int> &v){
	for(int i = 0; i < v.size(); i++){
		printf("%d ", v[i]);
	}
	printf("\n");
}

int main(){
	std::string key_string = "YELLOW SUBMARINE";
	std::vector<int> key = string_b256_to_bytes(key_string);
	std::vector<int> IV(16, 0);
	std::string encrypted = b64_file_to_string("resources/set_2_challenge_10");
	std::vector<int> bytes = string_b64_to_bytes(encrypted);
	inverse_CBC(bytes, key, IV);
	std::string message = bytes_to_string(bytes);
	std::cout << message << std::endl;
	return 0;
}

