#include "imgReader.h"

bool loadBMP(const char* filename, std::vector<std::vector<std::vector<int>>>& out)
{
    FILE* f = fopen(filename, "rb");

    if(f == NULL){
        throw "Argument Exception";
        return false;
    }

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    std::cout << "width: " << width;
    std::cout << "\nheight: " << height << "\n";

    int row_padded = (width*3 + 3) & (~3);
    std::cout << "row: " << row_padded << "\n";
    unsigned char* data = new unsigned char[row_padded];
    unsigned char tmp;

    out.resize(height, std::vector<std::vector<int>>(width, std::vector<int>(3)));

    for(int i = 0; i < height; i++)
    {
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < width*3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)
            tmp = data[j];
            data[j] = data[j+2];
            data[j+2] = tmp;

            out[i][j/3][0] = (int)data[j];
            out[i][j/3][1] = (int)data[j+1];
            out[i][j/3][2] = (int)data[j+2];

            //std::cout << "R: "<< (int)data[j] << " G: " << (int)data[j+1]<< " B: " << (int)data[j+2]<< std::endl;
            //printf("R: %d G: %d B: %d\n", (int)data[j], int(data[j + 1]), int(data[j + 2]));
        }
    }

    fclose(f);
    return true;
}

bool saveBMP(const char* filename, std::vector<std::vector<std::vector<int>>>& data)
{
    return true;
}

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

