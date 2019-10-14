project(comphoassn2)
cmake_minimum_required(VERSION 3.15)

find_package(OpenCV 4.1.1 REQUIRED COMPONENTS core imgcodecs imgproc)
find_package(Boost 1.71 REQUIRED COMPONENTS program_options)

add_library(comphoassn2 src/convolution.cpp src/filter.cpp src/util.cpp src/image.cpp)
target_link_libraries(comphoassn2 PUBLIC ${OpenCV_LIBS})
target_include_directories(comphoassn2 PUBLIC ${OpenCV_INCLUDE_DIRS})
target_compile_options(comphoassn2 PRIVATE -Wall -Wextra)

add_executable(main.out src/main.cpp)
target_link_libraries(main.out PRIVATE comphoassn2 Boost::program_options)
target_compile_options(main.out PRIVATE -Wall -Wextra)

