mingw32-g++.exe -Wall -fexceptions -g -lopenssl -I.\include -I..\include -c .\EncryptionManager.cpp -o EncryptionManager.o
mingw32-g++.exe  -o encryptionDriver.exe .\EncryptionManager.o   .\lib\VC\libcrypto32MDd.lib .\lib\VC\libssl32MDd.lib
