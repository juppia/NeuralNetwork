// Copyright
// by Gordey Balaban 27.01.2020

#include <cmath>
#include <ctime>
#include <cstring>
#include "imgReader.h"

const float EXP = 2.718281828f;

float sigmoid(float x) {
	return 1.0f / (1.0f + pow(EXP, -x));
}

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
std::vector<t> sigmoid(std::vector<t> in){
    std::vector<t> r = in;
    for(auto& i : r){
        i = sigmoid(i);
    }
    return r;
}

template<typename t>
std::vector<t> relu(std::vector<t> in){
    std::vector<t> r = in;
    for(auto& i : r){
        i = relu(i);
    }
    return r;
}

template<typename T>
std::vector<T> activation(std::vector<T> in){
    std::vector<T> r = in;
    for(auto& i : r){
        T a = sigmoid(i);
        i = a * (1 -a);
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

float activation(float x) {
	float a = sigmoid(x);
	return a * (1 - a);
}

namespace Math {
	// Транспонирование матрици
	template<typename T>
	std::vector<std::vector<T>> Transpose(std::vector<std::vector<T>> a)
	{
		std::vector<std::vector<T>> result;
		size_t aS = a.size();
		size_t bS = a[0].size();
		result.resize(bS);
		for (auto& i : result) {
			i.resize(aS);
		}
		for (size_t i = 0; i < a.size(); i++) {
			for (size_t j = 0; j < a[i].size(); j++) {
				result[j][i] = a[i][j];
			}
		}
		return result;
	}

	// Скалярное произведение векторов
	template<typename T>
	T Multiply(std::vector<T> a, std::vector<T> b)
	{
		T result = 0;
		if (a.size() != b.size()) {
			std::cout << "Error: cannot multiply vectors with different sizes or you forgot to transpose a matrix\n";
			return 0;
		}
		for (size_t i = 0; i < a.size(); i++) {
			result += a[i] * b[i];
		}
		return result;
	}

	// Умножение вектора на число
	template<typename T>
	std::vector<T> Multiply(std::vector<T> a, T b)
	{
		std::vector<T> result = a;
		for (size_t i = 0; i < a.size(); i++) {
			result[i] *= b;
		}
		return result;
	}

	// Умножение матрици 'a' на вектор 'b'
	template<typename T>
	std::vector<T> Multiply(std::vector<std::vector<T>> a, std::vector<T> b)
	{
	    if(a[0].size() != b.size()){
            std::cout << "[Matrix by vector] error\n";
	    }
		std::vector<T> result(a.size());
		for (size_t i = 0; i < a.size(); i++) {
			result[i] = Math::Multiply(a[i], b);
		}
		return result;
	}

	// Сложение векторов
	template<typename T>
	std::vector<T> Plus(std::vector<T> a, std::vector<T> b)
	{
		if (a.size() != b.size()) {
			std::cout << "[+] Error: cannot add vectors with different sizes\n";
		}
		std::vector<T> result(a.size());
		for (size_t i = 0; i < a.size(); i++) {
			result[i] = a[i] + b[i];
		}
		return result;
	}

    // Вычитание векторов
	template<typename T>
	std::vector<T> Minus(std::vector<T> a, std::vector<T> b)
	{
		if (a.size() != b.size()) {
			std::cout << "[+] Error: cannot add vectors with different sizes\n";
		}
		std::vector<T> result(a.size());
		for (size_t i = 0; i < a.size(); i++) {
			result[i] = a[i] - b[i];
		}
		return result;
	}
};

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
    std::vector<std::vector<float>> weights_in;
    std::vector<std::vector<float>> weights_h;
    std::vector<std::vector<float>> weights_out;

    /* Входящий слой: 3 нейрона
     * первый скрытый слой: hl1 нейронов
     * второй скрытый слой: hl2 нейронов
     * выходной слой: 3 нейрона
    */
    weights_in.resize(3, std::vector<float>(hl1));
    weights_h.resize(hl1, std::vector<float>(hl2));
    weights_out.resize(hl2, std::vector<float>(3));

    // инициализируем веса случайными значениями от -1 до 1
    for(size_t i = 0; i < weights_in.size(); i++){
        for(size_t j = 0; j < weights_in[i].size(); j++){
		    weights_in[i][j] = ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2) - 1;
        }
    }

    for(size_t i = 0; i < weights_h.size(); i++){
        for(size_t j = 0; j < weights_h[i].size(); j++){
		    weights_h[i][j] = ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2) - 1;
        }
    }

    for(size_t i = 0; i < weights_out.size(); i++){
        for(size_t j = 0; j < weights_out[i].size(); j++){
		    weights_out[i][j] = ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2) - 1;
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
                std::vector<float> pixelInData = {inImg[y][x][0]/(float)255,
                                                inImg[y][x][1]/(float)255,
                                                inImg[y][x][2]/(float)255};

                std::vector<float> pixelOutData = {outImg[y][x][0]/(float)255,
                                                outImg[y][x][1]/(float)255,
                                                outImg[y][x][2]/(float)255};

                auto t_in = Math::Transpose(weights_in);
                auto t_h = Math::Transpose(weights_h);
                auto t_out = Math::Transpose(weights_out);
                std::vector<float> hiddenResult1 = Math::Multiply(t_in, pixelInData);
                std::vector<float> hiddenResult2 = relu(Math::Multiply(t_h, hiddenResult1));
                std::vector<float> result = relu(Math::Multiply(t_out, hiddenResult2));
                auto err = Math::Minus(pixelOutData, result);
                allErr[0] += pow(err[0], 2);
                allErr[1] += pow(err[1], 2);
                allErr[2] += pow(err[2], 2);
                oo++;

                auto hiddenErr2 = Math::Multiply(weights_out, err);
                auto hiddenErr1 = Math::Multiply(weights_h, hiddenErr2);

                for(size_t f = 0; f < weights_in.size(); f++){
                    for(size_t b = 0; b < weights_in[f].size(); b++){
                        weights_in[f][b] += learning * hiddenErr1[b] * pixelInData[f] * derivative(hiddenResult1[b]);
                    }
                }

                for(size_t f = 0; f < weights_h.size(); f++){
                    for(size_t b = 0; b < weights_h[f].size(); b++){
                        weights_h[f][b] += learning * hiddenErr2[b] * hiddenResult1[f] * derivative(hiddenResult2[b]);
                    }
                }

                for(size_t f = 0; f < weights_out.size(); f++){
                    for(size_t b = 0; b < weights_out[f].size(); b++){
                        weights_out[f][b] += learning * err[b] * hiddenResult2[f] * derivative(result[b]);
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
            std::vector<float> pixelInData = {inImg[y][x][0]/(float)255,
                                            inImg[y][x][1]/(float)255,
                                            inImg[y][x][2]/(float)255};

            std::vector<float> pixelOutData(3);

            auto t_in = Math::Transpose(weights_in);
            auto t_h = Math::Transpose(weights_h);
            auto t_out = Math::Transpose(weights_out);

            std::vector<float> hiddenResult1 = Math::Multiply(t_in, pixelInData);
            std::vector<float> hiddenResult2 = relu(Math::Multiply(t_h, hiddenResult1));
            std::vector<float> result = relu(Math::Multiply(t_out, hiddenResult2));

            resultImg[y][x][0] = static_cast<int>(result[0]*255);
            resultImg[y][x][1] = static_cast<int>(result[1]*255);
            resultImg[y][x][2] = static_cast<int>(result[2]*255);
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
