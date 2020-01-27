# NeuralNetwork
A simple neural network that is written in C++

##Compiling
```Compiling
mkdir build
cd build
cmake ..
make
```

## Usage

--help: is help page

hiddenLayer1(int) - number of a neurons in first hidden layer

hiddenLayer2(int) - number of a neurons in second hidden layer

input_learning_file(char*) - input filename that will be used in the learning process

output_learning_file(char*) - output filename that will be used in the learning process

result_file(char*) - a result of neural network

learning_rate(float) - speed and accuracy of learning 

epoch(int) - number of epoch

## Example
```Example
./NeuralNetwork 15 10 example-in.ppm example-out.ppm result.ppm 0.001 10
```
