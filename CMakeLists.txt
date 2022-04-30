cmake_minimum_required(VERSION 3.20)
project(Quadtree)

set(CMAKE_CXX_STANDARD 17)

#find_package( OpenCV REQUIRED )
add_executable(Quadtree main.cpp Quadtree.cpp Quadtree.h)
#include_directories( ${OpenCV_INCLUDE_DIRS} )
#target_link_libraries( Quadtree ${OpenCV_LIBS} )
