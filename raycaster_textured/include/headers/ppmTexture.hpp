#ifndef PPM_TEXTURE_HPP
#define PPM_TEXTURE_HPP

#include <fstream>
#include <iostream>
#include <string>

void get_ppm_texture(uint8_t *arr, const char *filename)
{
    std::streampos size;
    char *memblock;
    std::string s = filename;
    std::ifstream infile(s.c_str(), std::fstream::in | std::fstream::binary | std::fstream::ate);

    if (infile.is_open())
    {
        size = infile.tellg();
        memblock = new char[size];
        infile.seekg(0, std::fstream::beg);
        infile.read(memblock, size);
        infile.close();
        for (int i = 13; i < size; i++)
        {
            arr[i - 13] = (uint8_t)memblock[i];
        }
        delete[] memblock;
    }
    else
    {
        std::cout << "couldn't open that ppm file.\n";
    }
}

#endif