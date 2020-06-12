#include <iostream>
#include <fstream>
#include <string>
#include "dictionary.h"

Dictionary::Dictionary(string filename)
{
    std::ifstream inpFile;
    inpFile.open(filename);
    if(!inpFile.is_open())
    {
        throw std::invalid_argument("Problems with file");
    }
    std::string buf;
    while (!inpFile.eof())
    {
        inpFile >> buf;
        insert(buf);
    }
    inpFile.close();
}