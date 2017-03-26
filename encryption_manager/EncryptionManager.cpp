#include <iostream>
#include <string>
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
    int len = text.length()-1;

    // number of splits we need to make (16 byte blocks)
    int iterations = ((len)/16)+1;

    // arrays to handle the input/output to the encrypt
    unsigned char enc_out[16];
    unsigned char enc_in[16];

    // a nicer array to be converted to a string later
    char result[iterations*16+1];

    AES_KEY enc_key;
    AES_set_encrypt_key(key, 256, &enc_key);

    for (int i = 0; i < iterations; i++){

        for (int j = 0; j < 16; j++)
            enc_in[j]=text[i+j];

        AES_encrypt(enc_in, enc_out, &enc_key);

        for (int j = 0; j < 16; j++)
            result[i+j]=enc_out[j];
    }
    result[iterations*16] = '\0';
    return string(result);
}

// to be expanded
string EncryptionManager::decrypt(string text) {

    // determine this string length
    int len = text.length()-1;

    // number of splits we need to make (16 byte blocks)
    int iterations = ((len)/16)+1;

    // arrays to handle the input/output to the decrypt
    unsigned char dec_out[16];
    unsigned char dec_in[16];

    // a nicer array to be converted to a string later
    char result[iterations*16+1];

    AES_KEY dec_key;
    AES_set_decrypt_key(key,256,&dec_key);

    for (int i = 0; i < iterations; i++){

        for (int j = 0; j < 16; j++)
            dec_in[j]=text[i+j];

        AES_decrypt(dec_in, dec_out, &dec_key);

        for (int j = 0; j < 16; j++)
            result[i+j]=dec_out[j];

    }

    result[iterations*16] = '\0';

    return string(result);
}

int main()
{
    //this string appears to work
    string test = "hello world!";

    // this string doesn't work.  Anything over the 16 byte size doesn't
    // seem to play nice.  TODO: fix.
    string test2 = "hello world! asdasdasdasdasd";

    printf("input string length = %d\n", test.length());
    EncryptionManager em;
    string encrypted = em.encrypt(test);
    printf("encrypted string length = %d\n", encrypted.length());
    string decrypted = em.decrypt(encrypted);

    cout << "input:     " << test << "\n";
    cout << "encrypted: " << encrypted << "\n";
    cout << "decrypted: " << decrypted << "\n";


    printf("\n\ninput string length = %d\n", test2.length());

    string encrypted2 = em.encrypt(test2);
    printf("encrypted2 string length = %d\n", encrypted2.length());
    string decrypted2 = em.decrypt(encrypted2);

    cout << "input:     " << test2 << "\n";
    cout << "encrypted: " << encrypted2 << "\n";
    cout << "decrypted: " << decrypted2 << "\n";



    return 0;



}

