#include <iostream>
#include <cstring>
#include <stdio.h>
#include <openssl/aes.h>

using namespace std;

static const unsigned char key[] = "65446536434362151256315856651227";

class EncryptionManager {
    public:
        string encrypt(string);
        string decrypt(string);
};


string EncryptionManager::encrypt(string text) {

    // determine this string length
    int len =(text.length());

    // number of splits we need to make (16 byte blocks)
    int iterations = ((len)/16)+1;

    // arrays to handle the input/output to the encrypt
    unsigned char enc_out[16];
    unsigned char enc_in[16];

    // a nicer array to be converted to a string later
    char result[iterations*16];

    AES_KEY enc_key;
    AES_set_encrypt_key(key, 256, &enc_key);

    for (int i = 0; i <= iterations; i++){
        for (int j = 0; j < 16; j++)
            enc_in[j]=text[(i*16)+j];

        AES_encrypt(enc_in, enc_out, &enc_key);

        for (int j = 0; j < 16; j++)
            result[(i*16)+j]=enc_out[j];
    }

    return string(result);
}

string EncryptionManager::decrypt(string text) {

    // determine this string length
    int len = text.length();

    // number of splits we need to make (16 byte blocks)
    int iterations = ((len)/16)+1;

    // arrays to handle the input/output to the decrypt
    unsigned char dec_out[16];
    unsigned char dec_in[16];

    // a nicer array to be converted to a string later
    char result[iterations*16];

    AES_KEY dec_key;
    AES_set_decrypt_key(key,256,&dec_key);

    for (int i = 0; i <= iterations; i++){
        for (int j = 0; j < 16; j++)
            dec_in[j]=text[(i*16)+j];

        AES_decrypt(dec_in, dec_out, &dec_key);

        for (int j = 0; j < 16; j++)
            result[(i*16)+j]=dec_out[j];
        }

    return string(result);
}

// test driver
int main()
{
    string test = "Here is a test of a really long string!\n\nIt has escape characters!1235675890!@#$%^&*()<>The quick brown fox blablabla\n\n\n\n\t\t\tFOR REAL";

    EncryptionManager em;
    string encrypted = em.encrypt(test);
    string decrypted = em.decrypt(encrypted);

    cout << "INPUT:     " << test << "\n\n\n";
    cout << "ENCRYPTED: " << encrypted << "\n\n\n";
    cout << "DECRYPTED: " << decrypted << "\n\n\n";

    return 0;
}

