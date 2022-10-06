project(DGtal-DGMM2022-tutorials)

cmake_minimum_required (VERSION 3.11)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(dgtal)
include(polyscope)

include_directories(${DGTAL_INCLUDE_DIRS})

add_executable(helloworld helloworld.cpp)
target_link_libraries(helloworld ${DGTAL_LIBRARIES})

add_executable(exampleBoard2D exampleBoard2D.cpp)
target_link_libraries(exampleBoard2D ${DGTAL_LIBRARIES})

add_executable(examplePolyscope examplePolyscope.cpp)
target_link_libraries(examplePolyscope ${DGTAL_LIBRARIES} polyscope)
