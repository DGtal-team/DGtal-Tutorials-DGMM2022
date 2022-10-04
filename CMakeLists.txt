project(DGtal-DGMM2022-tutorials)

cmake_minimum_required (VERSION 3.11)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(dgtal)

add_executable(helloworld helloworld.cpp)
target_link_libraries(helloworld  DGtal)

add_executable(exampleBoard2D exampleBoard2D.cpp)
target_link_libraries(exampleBoard2D  DGtal)
