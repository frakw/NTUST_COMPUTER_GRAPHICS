#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include <iostream>
#include <string>
using namespace std;
struct Texture {
    unsigned int id;
    string type;
    string path;
};
unsigned int TextureFromFile(const char* path, bool gamma = false);
#endif // !_TEXTURE_H_