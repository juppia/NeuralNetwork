#define BOOST_UBLAS_NDEBUG
// Copyright
// by Gordey Balaban 29.01.2020

#include <cmath>
#include <ctime>
#include <cstring>
#include "imgReader.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>

using namespace boost::numeric::ublas;

float relu(float x){
    if(x > 1){
        return 1 + 0.01f*(x-1);
    }
    else if(x < 0){
        return 0.01*x;
    }
    else{
        return x;
    }
}

template<typename t>
vector<t> relu(vector<t> in){
    vector<t> r = in;
    for(auto& i : r){
        i = relu(i);
    }
    return r;
}

float derivative(float x){
    if(x > 1 || x < 0){
        return 0.01f;
    }
    else
        return 1.0f;
}

int main(int argc, char** argv) {
	setlocale(LC_ALL, "ru_RU");

	int hl1 = 10;
	int hl2 = 10;
	int epoch = 30;
	std::string ilf = "in.ppm";
	std::string olf = "out.ppm";
	std::string rf = "result.ppm";
	float learning = 0.0001f;

    if(argc > 1){
        if(strcmp(argv[1], "--help") == 0){
            std::cout << "arguments:\nhiddenLayer1(int)\nhiddenLayer2(int)\ninput_learning_file(char*)\noutput_learning_file(char*)\nresult_file(char*)\nlearning_rate(float)\nepoch(int)\n";
            return 0;
        }
        else{
            hl1 = atoi(argv[1]);
            hl2 = atoi(argv[2]);
            ilf = argv[3];
            olf = argv[4];
            rf = argv[5];
            learning = atof(argv[6]);
            epoch = atof(argv[7]);
        }
    }

	srand(std::time(NULL));

    std::vector<std::vector<std::vector<int>>> inImg;
    if(!readPPM(ilf.c_str(), inImg)){
        return -1;
    }

    std::vector<std::vector<std::vector<int>>> outImg;
    if(!readPPM(olf.c_str(), outImg)){
        return -2;
    }

    // веса
    matrix<float> weights_in(3, hl1);
    matrix<float> weights_h(hl1, hl2);
    matrix<float> weights_out(hl2, 3);

    /* Входящий слой: 3 нейрона
     * первый скрытый слой: hl1 нейронов
     * второй скрытый слой: hl2 нейронов
     * выходной слой: 3 нейрона
    */
    // инициализируем веса случайными значениями от -1 до 1
    for(size_t i = 0; i < weights_in.size1(); i++){
        for(size_t j = 0; j < weights_in.size2(); j++){
		    weights_in(i, j) = ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2) - 1;
        }
    }

    for(size_t i = 0; i < weights_h.size1(); i++){
        for(size_t j = 0; j < weights_h.size2(); j++){
		    weights_h(i, j) = ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2) - 1;
        }
    }

    for(size_t i = 0; i < weights_out.size1(); i++){
        for(size_t j = 0; j < weights_out.size2(); j++){
		    weights_out(i, j) = ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2) - 1;
        }
    }

    int width = inImg[0].size();
    int height = inImg.size();

    std::cout << "Learning:" << std::endl;
    for(int i = 0; i < epoch; i++){
        float allErr[3] = {0.0f, 0.0f, 0.0f};
        int oo = 0;
        //printf("\repoch: %d of %d", i+1, eS);
        //fflush(stdout);
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                vector<float> pixelInData(3);
                pixelInData(0) = inImg[y][x][0]/(float)255;
                pixelInData(1) = inImg[y][x][1]/(float)255;
                pixelInData(2) = inImg[y][x][2]/(float)255;

                vector<float> pixelOutData(3);
                pixelOutData(0) = outImg[y][x][0]/(float)255;
                pixelOutData(1) = outImg[y][x][1]/(float)255;
                pixelOutData(2) = outImg[y][x][2]/(float)255;

                auto t_in = trans(weights_in);
                auto t_h = trans(weights_h);
                auto t_out = trans(weights_out);
                vector<float> hiddenResult1 = prod(t_in, pixelInData);
                vector<float> hiddenResult2 = prod(t_h, hiddenResult1);
                hiddenResult2 = relu(hiddenResult2);

                vector<float> result = prod(t_out, hiddenResult2);
                result = relu(result);
                vector<float> err = pixelOutData - result;
                allErr[0] += pow(err(0), 2);
                allErr[1] += pow(err(1), 2);
                allErr[2] += pow(err(2), 2);
                oo++;

                vector<float> hiddenErr2 = prod(weights_out, err);
                vector<float> hiddenErr1 = prod(weights_h, hiddenErr2);

                for(size_t f = 0; f < weights_in.size1(); f++){
                    for(size_t b = 0; b < weights_in.size2(); b++){
                        weights_in(f, b) += learning * hiddenErr1(b) * pixelInData(f) * derivative(hiddenResult1(b));
                    }
                }

                for(size_t f = 0; f < weights_h.size1(); f++){
                    for(size_t b = 0; b < weights_h.size2(); b++){
                        weights_h(f, b) += learning * hiddenErr2(b) * hiddenResult1(f) * derivative(hiddenResult2(b));
                    }
                }

                for(size_t f = 0; f < weights_out.size1(); f++){
                    for(size_t b = 0; b < weights_out.size2(); b++){
                        weights_out(f, b) += learning * err(b) * hiddenResult2(f) * derivative(result(b));
                    }
                }
            }
        }
        // мне через printf удобнее
        printf("epoch: %d of %d, error: R %.2f G %.2f B %.2f\n", i+1, epoch, (allErr[0]/oo)*100.0f, (allErr[1]/oo)*100.0f, (allErr[2]/oo)*100.0f);
    }

    std::cout << "Processing" << std::endl;
    std::vector<std::vector<std::vector<int>>> resultImg;
    resultImg.resize(height, std::vector<std::vector<int>>(width, std::vector<int>(3)));
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            vector<float> pixelInData(3);
            pixelInData(0) = inImg[y][x][0]/(float)255;
            pixelInData(1) = inImg[y][x][1]/(float)255;
            pixelInData(2) = inImg[y][x][2]/(float)255;

            vector<float> pixelOutData(3);
            pixelOutData(0) = outImg[y][x][0]/(float)255;
            pixelOutData(1) = outImg[y][x][1]/(float)255;
            pixelOutData(2) = outImg[y][x][2]/(float)255;

            auto t_in = trans(weights_in);
            auto t_h = trans(weights_h);
            auto t_out = trans(weights_out);
            vector<float> hiddenResult1 = prod(t_in, pixelInData);
            vector<float> hiddenResult2 = prod(t_h, hiddenResult1);
            hiddenResult2 = relu(hiddenResult2);
            vector<float> result = prod(t_out, hiddenResult2);
            result = relu(result);

            resultImg[y][x][0] = static_cast<int>(result(0)*255);
            resultImg[y][x][1] = static_cast<int>(result(1)*255);
            resultImg[y][x][2] = static_cast<int>(result(2)*255);
            if(resultImg[y][x][0] < 0)
                resultImg[y][x][0] = 0;
            if(resultImg[y][x][1] < 0)
                resultImg[y][x][1] = 0;
            if(resultImg[y][x][2] < 0)
                resultImg[y][x][2] = 0;
        }
    }

    if(!writePPM(rf.c_str(), resultImg)){
        std::cout << "Failed to save a result image" << std::endl;
        return -4;
    }
    std::cout << "Result was wrote into " << rf << std::endl;

    return 0;
}
