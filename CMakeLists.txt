cmake_minimum_required(VERSION 3.11)

PROJECT(speedLimit)

SET(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

SET(OpenCV_LIBS 3rdparty/opencv/lib/libopencv_core.so 3rdparty/opencv/lib/libopencv_highgui.so 3rdparty/opencv/lib/libopencv_imgproc.so 3rdparty/opencv/lib/libopencv_imgcodecs.so)

INCLUDE_DIRECTORIES(/3rdparty/opencv/include ./include)

AUX_SOURCE_DIRECTORY(./src DIR_SRCS)

ADD_EXECUTABLE(speedLimit main.cpp ./src/speedLimit.cpp)

TARGET_LINK_LIBRARIES(speedLimit ${OpenCV_LIBS})

ADD_LIBRARY(spLimit SHARED ${DIR_SRCS})

SET_TARGET_PROPERTIES(spLimit PROPERTIES output_name "spLimit")
