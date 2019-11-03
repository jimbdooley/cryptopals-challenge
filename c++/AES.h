#include <math.h>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <iostream>

int multiplication_over_2_8(int byte_a, int byte_b){
    std::vector<int> a = byte_to_bin_list(byte_a);
    std::vector<int> b = byte_to_bin_list(byte_b);
    std::vector<int> c(15, 0);
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if( (a[7-i]==1) && (b[7-j]==1) ){
                c[14-i-j] ^= 1;
            }
        }
    }
    int c_int = 0;
    for(int i = 0; i < 15; i++){
        c_int += c[i] * (int)pow(2, (double)(14-i));
    }
    if(c_int == 0){
        return 0;
    }
    int reducing_poly = 283;
    int t;
    while( floor(log2((double)reducing_poly)) <= floor(log2((double)c_int)) ){
        t = reducing_poly;
        while( floor(log2((double)t)) != floor(log2((double)c_int)) ){
            t *= 2;
        }
        c_int ^= t;
    }

    return c_int;
}

void inverse_mix_columns(std::vector<int> &d){
    std::vector<int> rtn;
    rtn.reserve(d.size());
    int M[4][4] = {{14, 11, 13, 9}, {9, 14, 11, 13}, {13, 9, 14, 11}, {11, 13, 9, 14}};
    int temp[16];
    int b;
    for(int block = 0; block<d.size(); block += 16){
        for(int column = 0; column < 4; column++){
            for(int element = 0; element<4; element++){
                b = 0;
                for(int i = 0; i < 4; i++){
                    b ^= multiplication_over_2_8(d[block+4*column+i], M[element][i]);
                }
                temp[column*4 + element] = b;
            }
        }
        for(int i = 0; i < 16; i++){
            d[block + i] = temp[i];
        }
    }
}

void mix_columns(std::vector<int> &d){
    std::vector<int> rtn;
    rtn.reserve(d.size());
    int M[4][4] = {{2, 3, 1, 1}, {1, 2, 3, 1}, {1, 1, 2, 3}, {3, 1, 1, 2}};
    int temp[16];
    int b;
    for(int block = 0; block<d.size(); block += 16){
        for(int column = 0; column < 4; column++){
            for(int element = 0; element<4; element++){
                b = 0;
                for(int i = 0; i < 4; i++){
                    b ^= multiplication_over_2_8(d[block+4*column+i], M[element][i]);
                }
                temp[column*4 + element] = b;
            }
        }
        for(int i = 0; i < 16; i++){
            d[block + i] = temp[i];
        }
    }
}

void shift_rows(std::vector<int> &a){
    int new_positions[16] = {0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12, 1, 6, 11};
    int temp[16];
    for(int block = 0; block < a.size(); block += 16){
        for(int i = 0; i < 16; i++){
            temp[i] = a[block + new_positions[i]];
        }
        for(int i = 0; i < 16; i++){
            a[block + i] = temp[i];
        }

    }
}

void inverse_shift_rows(std::vector<int> & a){
    int new_positions[16] = {0, 13, 10, 7, 4, 1, 14, 11, 8, 5, 2, 15, 12, 9, 6, 3};
    int temp[16];
    for(int block = 0; block < a.size(); block += 16){
        for(int i = 0; i < 16; i++){
            temp[i] = a[block + new_positions[i]];
        }
        for(int i = 0; i < 16; i++){
            a[block + i] = temp[i];
        }

    }
}

class S_Box{
private:
    int S_box[16][16];
    int inverse_S_box[16][16];
public:
    S_Box(){
        printf("creating an S-BOX!\n\n");
        std::string temp_num;
	    std::string line;
  	    std::ifstream myfile ("resources/inverse_S_box");
        int row = 0;
        int col = 0;
        if (myfile.is_open()){
            while ( getline (myfile,line) )
            {   
                col = 0;
                for(int i = 0; i < line.size(); i++){
                    if(line[i] != '-'){
                        temp_num += line[i];
                    }
                    else if((line[i] == '-')&&(temp_num.size()!= 0)){
                        inverse_S_box[row][col] = b16_to_int_two_char_max(temp_num);
                        col += 1;
                        temp_num = "";
                    }
                }
                row += 1;
            }
            myfile.close();
        }
        else std::cout << "Unable to open file"; 

        std::ifstream myfile2 ("resources/S_box");
        row = 0;
        if (myfile2.is_open()){
            while ( getline (myfile2,line) )
            {   
                col = 0;
                for(int i = 0; i < line.size(); i++){
                    if(line[i] != '-'){
                        temp_num += line[i];
                    }
                    else if((line[i] == '-')&&(temp_num.size()!= 0)){
                        S_box[row][col] = b16_to_int_two_char_max(temp_num);
                        col += 1;
                        temp_num = "";
                    }
                }
                row += 1;
            }
            myfile2.close();
        }
        else std::cout << "Unable to open file"; 
    }
    void apply(std::vector<int> &v){
        for(int i = 0; i < v.size(); i++){
            v[i] = S_box[v[i]/16][v[i]%16];
        }
    }
    void apply_inverse(std::vector<int> &v){
        for(int i = 0; i < v.size(); i++){
            v[i] = inverse_S_box[v[i]/16][v[i]%16];
        }
    }
    void show(bool inverse=false){
        for(int i = 0; i < 16; i++){
            for(int j = 0; j < 16; j++){
                if(inverse)
                    printf("%d ", inverse_S_box[i][j]);
                else
                    printf("%d ", S_box[i][j]);
            }
            printf("\n");
        }
    }
};

std::vector<int> rotate_word(std::vector<int> &v){
    std::vector<int> rtn(v.size(), 0);
    for(int i = 0; i < v.size(); i++){
        rtn[i] = v[(i+1)%v.size()];
    }
    return rtn;
}

std::vector<int> rcon(int i){
    std::vector<int> rtn{0, 0, 0, 0};
    if( (i>=1) && (i<=8) ){
        rtn[0] = (int)pow(2, (double)(i-1));
    }
    else if(i==9){
        rtn[0] = 27;
    }
    else if(i==10){
        rtn[0] = 54;
    }
    return rtn;
}

std::vector<std::vector<int>> key_expansion(const std::vector<int> &bytes, S_Box &s_box){
    int N = bytes.size()/4;
    int R = 11 + N - 4;
    std::vector<std::vector<int>> K;
    for(int i = 0; i < 4; i++){
        std::vector<int> temp(4, 0);
        K.push_back(temp);
    }
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            K[i][j] = bytes[4*i + j];
        }
    }
    std::vector<std::vector<int>> W;
    for(int i = 0; i < 4*R; i++){
        if(i < N){
            W.push_back(K[i]);
        }
        else if( (i>=N) && (i%N==0) ){
            std::vector<int> temp(4, 0);
            std::vector<int> rotated_word = rotate_word(W[i-1]);
            s_box.apply(rotated_word);
            std::vector<int> rcon_word = rcon((int)(i/N));
            for(int j = 0; j < 4; j++){
                temp[j] = W[i-N][j]^rotated_word[j]^rcon_word[j];
            }
            W.push_back(temp);
        }
        else if( (i>=N) && (N>6) && (i%N==4%N) ){
            std::vector<int> temp(4, 0);
            std::vector<int> copy(W[i-1]);
            s_box.apply(copy);
            for(int j = 0; j < 4; j++){
                temp[j] = W[i-N][j]^copy[j];
            }
            W.push_back(temp);
        }
        else{
            std::vector<int> temp(4, 0);
            for(int j = 0; j < 4; j++){
                temp[j] = W[i-N][j]^W[i-1][j];
            }
            W.push_back(temp);
        }
    }

    std::vector<std::vector<int>> rtn;
    rtn.reserve(R);
    for(int i = 0; i < R; i++){
        std::vector<int> temp(bytes.size(),0);
        for(int j = 0; j < 16; j++){
            temp[j] = W[4*i + (int)(j/4)][j%4];
        }
        rtn.push_back(temp);
    }
    return rtn;
}

std::vector<std::vector<int>> key_expansion(const std::vector<int> &bytes){
    S_Box s_box = S_Box();
    return key_expansion(bytes, s_box);
}

void _inverse(std::vector<int> &bytes, const std::vector<int> &key, S_Box &s_box){
    int rounds = key.size()/4 + 6;
    std::vector<std::vector<int>> keys = key_expansion(key, s_box);
    int key_i = keys.size() - 1;
    repeating_key_xor(bytes, keys[key_i]);
    key_i -= 1;
    inverse_shift_rows(bytes);
    s_box.apply_inverse(bytes);
    for(int i = 0 ; i < rounds-1; i++){
        repeating_key_xor(bytes, keys[key_i]);
        key_i -= 1;
        inverse_mix_columns(bytes);
        inverse_shift_rows(bytes);
        s_box.apply_inverse(bytes);
    }
    repeating_key_xor(bytes, keys[key_i]);
}

void _inverse(std::vector<int> &bytes, const std::vector<int> &key){
    S_Box s_box = S_Box();
    _inverse(bytes, key, s_box);
}

void inverse_ECB(std::vector<int> &bytes, const std::vector<int> &key){
    _inverse(bytes, key);
}

void inverse_CBC(std::vector<int> &bytes, const std::vector<int> &key, const std::vector<int> &IV){
    S_Box s_box = S_Box();
    std::vector<int> temp(16, 0);
    std::vector<int> running_IV(IV);
    int IV_holder;
    for(int i = 0; i < bytes.size(); i+= key.size()){
        for(int j = 0; j < key.size(); j++){
            temp[j] = bytes[i+j];
        }
        _inverse(temp, key, s_box);
        for(int j = 0; j < key.size(); j++){
            IV_holder = bytes[i+j];
            bytes[i+j] = temp[j]^running_IV[j];
            running_IV[j] = IV_holder;
        }
    }
}
/*
def encrypt_CBC(bytes, key, iv):
	rtn = []
	for i in range(len(bytes)//len(key)):
		segment = bytes[len(key)*i:len(key)*(i+1)]
		xored_segment = [iv[_]^segment[_] for _ in range(len(key))]
		encrypted_segment = encrypt_ECB(xored_segment, key)
		rtn += encrypted_segment
		iv = encrypted_segment
	return rtn


def encrypt_ECB(bytes, key):
	rounds = len(key)//4+6
	keys = key_expansion(key)
	state = bytes
	key_i = 0
	state = repeating_key_xor(keys[key_i], state)
	key_i += 1
	for i in range(rounds-1):
		state = S_box(state)
		state = shift_rows(state)
		state = mix_columns(state)
		state = repeating_key_xor(keys[key_i], state)
		key_i += 1
	state = S_box(state)
	state = shift_rows(state)
	state = repeating_key_xor(keys[key_i], state)
	return state

*/