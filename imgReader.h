#ifndef BMP_READER_H
#define BMP_READER_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>

bool readPPM(const char* filename, std::vector<std::vector<std::vector<int>>>& out);
bool writePPM(const char* filename, std::vector<std::vector<std::vector<int>>>& data);

#endif
