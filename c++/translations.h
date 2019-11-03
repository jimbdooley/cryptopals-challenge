#include <vector>
#include <math.h>
#include <string>

int char_b64_to_int(char c){
    if( ((int)c >= 65) && ((int)c <= 90) ){
        return (int)c-65;
    }
    if( ((int)c >= 97) && ((int)c <= 122) ){
        return (int)c-97+26;
    }
    if( ((int)c >= 48) && ((int)c <= 57) ){
        return (int)c-48+52;
    }
    if( c == '+' ){
        return 62;
    }
    if( c == '/' ){
        return 63;
    }
    if( c == '=' ){
        return 0;
    }
    return -1;
}

std::vector<int> string_b64_to_bytes(std::string &msg){
    std::vector<int> rtn;
    rtn.reserve(msg.size()*4/3);
    int on_deck = -1;
    int counter = 0;
    int value;
    for(int i = 0; i < msg.size(); i++){
        value = char_b64_to_int(msg[i]);
        if(value == -1){
            continue;
        }
        if(counter == 0){
            on_deck = value*4;
        } 
        else if(counter == 1){
            rtn.push_back(on_deck + (int)(value/16));
            on_deck = 16*(value%16);
        }
        else if(counter == 2){
            rtn.push_back(on_deck + (int)(value/4));
            on_deck = 64*(value%4);
        }
        else if(counter == 3){
            rtn.push_back(on_deck + value);
        }
        counter += 1;
        counter %= 4;
    }
    if(on_deck != -1){
        rtn.push_back(on_deck);
    }
    return rtn;

}

std::vector<int> string_b256_to_bytes(std::string &msg){
    std::vector<int> rtn;
    rtn.reserve(msg.size());
    for(int i = 0; i < msg.size(); i++){
        rtn.push_back((int)msg[i]);
    }
    return rtn;
}

std::string bytes_to_string(const std::vector<int> &bytes){
    std::string rtn;
    rtn.reserve(bytes.size());
    for(int i = 0; i < bytes.size(); i++){
        rtn += (char)bytes[i];
    }
    return rtn;
}

int b16_to_int_two_char_max(std::string s){
    int rtn = 0;
    int temp;
    if(s.size() == 2){
        temp = (int)s[0];
        if( (temp >= 48) && (temp <= 57) ){
            rtn += 16*(temp-48);
        }
        else{
            if( (temp >= 97) && (temp <= 102) ){
                rtn += 16*(temp-97+10);
            }
            if( (temp >= 65) && (temp <= 70) ){
                rtn += 16*(temp-65+10);
            }
        }
        temp = (int)s[1];
    } else{
        temp = (int)s[0];
    }
    
    if( (temp >= 48) && (temp <= 57) ){
        rtn += temp-48;
    }
    else{
        if( (temp >= 97) && (temp <= 102) ){
            rtn += temp-97+10;
        }
        if( (temp >= 65) && (temp <= 70) ){
            rtn += temp-65+10;
        }
    }
    return rtn;
}

std::vector<int> byte_to_bin_list(int n){
    std::vector<int> rtn;
    rtn.reserve(8);
    int compare;
    for(int i = 0; i < 8; i++){
        compare = (int)pow(2, (double)(7-i));
        if( n >= compare ){
            rtn.push_back(1);
            n -= compare;
        } else {
            rtn.push_back(0);
        }
    }
    return rtn;
}
