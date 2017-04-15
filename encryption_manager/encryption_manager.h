#pragma once
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <vector>
#include <openssl/aes.h>

using namespace std;


class EncryptionManager {
public:
	void encrypt(string, unsigned char*, int);
	string decrypt(vector<unsigned char>);
};