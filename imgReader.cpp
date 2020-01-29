#include "imgReader.h"

bool readPPM(const char* filename, std::vector<std::vector<std::vector<int>>>& out){
    std::ifstream inFile(filename);
    if(!inFile.is_open()){
        std::cout << "Failed to open a file " << filename << "\n";
        return false;
    }
    std::string data;
    int width, height;
    inFile >> data; // header
    inFile >> width >> height;

    std::cout << "width: " << width << "\nheight: " << height << "\n";

    out.resize(height, std::vector<std::vector<int>>(width, std::vector<int>(3)));

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            for(int k = 0; k < 3; k++){
                inFile >> out[i][j][k];
            }
        }
    }
    inFile.close();
    return true;
}

bool writePPM(const char* filename, std::vector<std::vector<std::vector<int>>>& data){
    std::ofstream outFile(filename);
    if(!outFile.is_open()){
        std::cout << "Failed to write a file " << filename << "\n";
        return false;
    }
    int height = data.size();
    int width = data[0].size();
    outFile << "P3" << "\n" << width <<  " " << height << "\n";
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            for(int k = 0; k < 3; k++){
                outFile << data[i][j][k] << "\n";
            }
        }
    }
    outFile.close();
    return true;
}

