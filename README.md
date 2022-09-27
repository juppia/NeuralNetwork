# NeuralNetwork
A simple neural network written in C++

## Compiling
```Compiling
mkdir build
cd build
cmake ..
make
```

## Usage

--help: shows help

hiddenLayer1(int) - number of neurons in the first hidden layer

hiddenLayer2(int) - number of neurons in the second hidden layer

input_learning_file(char*) - input filename that will be used in the learning process

output_learning_file(char*) - output filename that will be used in the learning process

result_file(char*) - the result of a neural network

learning_rate(float) - speed and accuracy of learning 

epoch(int) - number of epochs

## Example
```Example
./NeuralNetwork 15 10 example-in.ppm example-out.ppm result.ppm 0.001 10
```
