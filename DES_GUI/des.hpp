/*
 *  des_dylib.hpp
 *  des_dylib
 *
 *  Created by 李文坤 on 2017/5/13.
 *  Copyright © 2017年 李文坤. All rights reserved.
 *
 */

#ifndef des_
#define des_
#include <string>

/* The classes below are exported */
//#pragma GCC visibility push(default)

using namespace std;

class des {
    public:
    static void des_encrypt(std::string input_file, std::string output_file, std::string hex_key);
    static void des_decrypt(std::string input_file, std::string output_file, std::string hex_key);
};

//#pragma GCC visibility pop
#endif
