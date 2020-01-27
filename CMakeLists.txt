cmake_minimum_required(VERSION 2.6)

project(NeuralNetwork)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O3")

file(GLOB
    CPPS "*.cpp")

add_executable(${PROJECT_NAME} ${CPPS})
