//
//  main.cpp
//  DES
//
//  Created by 李文坤 on 2017/5/10.
//  Copyright © 2017年 李文坤. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "des.hpp"

typedef char byte;

using namespace std;

void _des(string, string, string, bool);

void des::des_encrypt(string input_file, string output_file, string hex_key) {
    _des(input_file, output_file, hex_key, true);
}

void des::des_decrypt(string input_file, string output_file, string hex_key) {
    _des(input_file, output_file, hex_key, false);
}

int table_keyShift[] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
};

int table_ip[] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

int table_reverse_ip[] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

int table_pc1[] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4,
};

int table_pc2[] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

int table_s[8][4][16] = {
    
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
    
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
    
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
    
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
    
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
    
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
    
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
    
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}
};

static int table_P[] = {
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25
};

static int table_E[] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

// print error msg
void msg(string msg) {
    cout << msg << endl;
}


uint64_t ip(uint64_t raw, bool inverse) {
    int* table = inverse ? table_reverse_ip : table_ip;
    uint32_t table_ip_size = 64; // size of table_ip is 64
    uint64_t result = 0L;
    uint64_t mask = 1L << 63;
    for (int i = 0; i < table_ip_size; i++) {
        // retrieve the bit in raw data at the position represented by table[i]
        if ((raw & (mask >> (table[i] - 1))) != 0) {
            result |= (mask >> i); // set bit to 1
        }
    }
    return result;
}

// expand R to 48 bits length according to table_E
// we cannot find a data type of size 48, so we use unit64,
// the lower 48 bits is valid
uint64_t E(uint32_t raw) {
    uint32_t table_E_size = 48; // size of table_E is 48
    uint64_t result = 0L;
    uint32_t int_mask = 1 << 31;
    uint64_t long_mask = 1L << 47; // put the mask at the first position
    for (int i = 0; i < table_E_size; i++) {
        if ((raw & (int_mask >> (table_E[i] - 1))) != 0) {
            result |= (long_mask >> i);
        }
    }
    return result;
}

// only use lower 6 bits, checked
byte S(byte input, int n) {
    
    int x = (input & 0x01); // low bit
    x += ((input & 0x20) >> 4); // high bit
    // get the value of 4 middle bits
    int y = (input & 0x1e) >> 1;
    
    return (byte) table_s[n][x][y];
}

uint32_t P(uint32_t raw) {
    int table_p_size = 32;
    uint32_t result = 0;
    uint32_t mask = (1 << 31);
    for (int i = 0; i < table_p_size; i++) {
        if ((raw & (mask >> (table_P[i] - 1))) != 0) {
            result |= (mask >> i);
        }
    }
    return result;
}

// R has 32 bits, and only use lower 48 bits of key
uint32_t f(uint32_t R, uint64_t key) {
    uint64_t expanded_R = E(R); // expand R to 48 bits
    uint64_t s_input = expanded_R ^ key; // (R, K) -> S
    uint32_t p_input = 0;
    for (int i = 0, shift = 42; i < 8; i++, shift -= 6) {
        p_input <<= 4;
        // only use lower 6 bits of s_input and lower 4 bits of the result
        p_input |= (S((byte) (s_input >> shift), i) & 0x0f);
    }
    return P(p_input);
}


uint64_t pc_2(uint64_t raw) {
    
    uint64_t result = 0L;
    uint64_t mask = 1L << 55;
    for (int i = 0; i < 48; i++) {
        if ((raw & (mask >> (table_pc2[i] - 1))) != 0) {
            result |= (mask >> (i + 8));
        }
    }
    return result;
}

uint64_t pc_1(uint64_t init_key) {
    uint64_t result = 0L;
    uint64_t mask = 1L << 63;
    for (int i = 0; i < 56; i++) {
        if ((init_key & (mask >> (table_pc1[i] - 1))) != 0) {
            result |= (mask >> (i + 8));
        }
    }
    return result;
}

// init keys
void init_keys(uint64_t init_key, uint64_t out_keys[], bool encrypt) {
    
    uint64_t pc_1_result = pc_1(init_key);
    uint32_t C = (uint32_t) ((pc_1_result >> 28) & 0xfffffff);
    uint32_t D = (uint32_t) (pc_1_result & 0xfffffff);
    
    long tmp_key = 0L;

    for (int i = 0; i < 16; i++) {
        
        int shift = table_keyShift[i];
        C = ((C >> (28 - shift)) | (C << shift)) & 0x0fffffff;
        D = ((D >> (28 - shift)) | (D << shift)) & 0x0fffffff;
        
        tmp_key |= C;
        tmp_key <<= 28;
        tmp_key |= D;
        
        if (encrypt) {
            out_keys[i] = pc_2(tmp_key);
        } else  {
            out_keys[15 - i] = pc_2(tmp_key);
        }
        tmp_key = 0L; // don't forget clear tmp_key for next iteration
    }
}

// read 64 bits from this file input stream
int read(ifstream* reader, byte out_data[]) {
    reader->read(out_data, 8);
    int count = (int) reader->gcount();
    return count;
}

// add padding data if segment's size less than 64
void add_padding(byte data[], int full_size, int curr_size) {
    int padding_size = full_size - curr_size;
    for (int i = curr_size; i < full_size; i++) {
        data[i] = (byte) padding_size;
    }
}

// size : size of this array
uint64_t bytes2uint64(byte data[]) {
    uint64_t result = 0L;
    for (int i = 0; i < 8; i++) {
        result <<= 8;
        result |= (data[i] & 0xff); // because data[i] is a signed char
    }
    return result;
}

void uint642bytes(uint64_t data, byte out_bytes[]) {
    for (int i = 7, shift = 0; i >= 0; i--, shift += 8) {
        out_bytes[i] = (byte) (data >> shift);
    }
}

void write(ofstream* writer, byte data[], int size) {
    writer->write(data, size);
}

uint64_t stringToUnit64(string hex_key) {
    uint64_t binary_key = 0L;
    for (int i = 0; i < hex_key.length(); i++) {
        binary_key <<= 4;
        char& c = hex_key.at(i);
        if (c >= '0' && c <= '9') {
            binary_key |= ((c - '0') & 0xfL);
        } else if (c >= 'a' && c <= 'f')  {
            binary_key |= ((c - 'a' + 10 & 0xfL));
        } else if (c >= 'A' && c <= 'F') {
            binary_key |= ((c - 'A' + 10 & 0xfL));
        } else {
            throw runtime_error("key is not in hex format");
        }
    }
    return binary_key;
}

void _des(string input_file, string output_file, string hex_key, bool encrypt) {
    
    ifstream* reader = new ifstream(input_file, ios_base::in|ios_base::binary);
    ofstream* writer = new ofstream(output_file, ios_base::out|ios_base::binary);
    // read or write buffer
    byte buf[8];
    byte buf2[8];
    byte* bufptr = buf;
    byte* emptybufptr = buf2;
    byte* tmpptr; // used for exchange
    // initial key , using a long number to store it
    uint64_t initial_key = stringToUnit64(hex_key);
    uint64_t origin, key, permuted_input, result = 0L;
    uint32_t L, R;
    uint32_t tmp; // used to change L and R
    uint64_t keys[16];
    bool padding_added = false;
    // we should init_key before encrypting/dencrypting
    init_keys(initial_key, keys, encrypt);
    // size of bytes that we really get from file
    int size = read(reader, bufptr);
    
    while (size > 0 || !padding_added) {
        if (size < 8) {
            add_padding(bufptr, 8, size);
            padding_added = true;
        }
        // convert bytes to unit64, this is initial input
        origin = bytes2uint64(bufptr);
        // apply ip transformation
        permuted_input = ip(origin, false);
        // get the lower 32 bits as R
        R = (uint32_t) permuted_input;
        // get the higher 32 bits as L
        L = (uint32_t) (permuted_input >> 32);
        // 16 iterations
        for (int i = 0; i < 16; i++) {
            // get key for i th iteration
            key = keys[i];
            // we should store R for it will be override later
            tmp = R;
            // apply f transformation
            R = L ^ f(R, key);
            // let L be tmp
            L = tmp;
        }
        // combine R and L as pre output
        result |= (R & 0xffffffffL); // attention ! int to long is signed extension
        result <<= 32;
        result |= (L & 0xffffffffL);
        // apply inverse_ip transformation
        result = ip(result, true);
        // pre-read next buf
        size = read(reader, emptybufptr);
        // convert unit64 to bytes
        uint642bytes(result, bufptr);
        
        if (!encrypt && size == 0) {
            writer->write(bufptr, 8 - bufptr[7]);
            break;
        }
        // don't forget to clear result for next iteration
        result = 0L;
        write(writer, bufptr, 8);
        
        tmpptr = bufptr;
        bufptr = emptybufptr;
        emptybufptr = tmpptr;
    }

    if (reader) reader->close();
    if (writer) writer->close();
}


