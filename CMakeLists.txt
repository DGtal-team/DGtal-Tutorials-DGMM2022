project(DGtal-DGMM2022-tutorials)

cmake_minimum_required (VERSION 3.14)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(dgtal)
include(polyscope)

include_directories(${DGTAL_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})

add_executable(helloworld helloworld.cpp)
target_link_libraries(helloworld ${DGTAL_LIBRARIES})

add_executable(exampleBoard2D exampleBoard2D.cpp)
target_link_libraries(exampleBoard2D ${DGTAL_LIBRARIES})

add_executable(examplePolyscope examplePolyscope.cpp)
target_link_libraries(examplePolyscope ${DGTAL_LIBRARIES} polyscope)



## Practicals

add_executable(homotopic-thinning practical-homotopic-thinning/homotopic-thinning.cpp)
target_link_libraries(homotopic-thinning ${DGTAL_LIBRARIES} polyscope)

add_executable(scaleaxis practical-scaleaxis/scaleaxis.cpp)
target_link_libraries(scaleaxis ${DGTAL_LIBRARIES} polyscope)

add_executable(2D-estimation-template practical-2D-estimation/2D-estimation-template.cpp)
target_link_libraries(2D-estimation-template ${DGTAL_LIBRARIES})

add_executable(3D-estimation-template practical-3D-estimation/3D-estimation-template.cpp)
target_link_libraries(3D-estimation-template ${DGTAL_LIBRARIES} polyscope)

#add_executable(3D-estimation practical-3D-estimation/answers/3D-estimation.cpp)
#target_link_libraries(3D-estimation ${DGTAL_LIBRARIES} polyscope)


