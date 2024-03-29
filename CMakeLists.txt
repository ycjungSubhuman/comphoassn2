project(comphoassn2)
cmake_minimum_required(VERSION 3.15)

find_package(OpenCV 4.1.1 REQUIRED COMPONENTS core imgcodecs imgproc highgui)
find_package(Boost 1.71 REQUIRED COMPONENTS program_options)

add_library(comphoassn2 src/convolution.cpp src/filter.cpp src/util.cpp src/image.cpp)
target_link_libraries(comphoassn2 PUBLIC ${OpenCV_LIBS})
target_include_directories(comphoassn2 PUBLIC ${OpenCV_INCLUDE_DIRS})
target_compile_options(comphoassn2 PRIVATE -Wall -Wextra)

add_executable(app_unsharp src/app_unsharp.cpp)
target_link_libraries(app_unsharp PRIVATE comphoassn2 Boost::program_options)
target_compile_options(app_unsharp PRIVATE -Wall -Wextra)

add_executable(app_bilateral src/app_bilateral.cpp)
target_link_libraries(app_bilateral PRIVATE comphoassn2 Boost::program_options)
target_compile_options(app_bilateral PRIVATE -Wall -Wextra)

