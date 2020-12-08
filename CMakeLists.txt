cmake_minimum_required(VERSION 3.17)
project(AnomalyDetector)

set(CMAKE_CXX_STANDARD 14)

add_executable(AnomalyDetector main.cpp anomaly_detection_util.cpp)