cmake_minimum_required(VERSION 3.8)
project(ImageMark)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp ConstW.h ImageInsert.cpp ImageInsert.h ZigZag.cpp ZigZag.h DetectBlind.cpp DetectBlind.h
        RS/berlekamp.cpp RS/crcgen.cpp RS/ecc.h RS/galois.cpp RS/rs.cpp Common.cpp Common.h PSNR.cpp PSNR.h)
add_executable(ImageMark ${SOURCE_FILES})
target_link_libraries(ImageMark ${OpenCV_LIBS})