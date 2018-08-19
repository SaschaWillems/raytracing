SET(FOLDER ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB SOURCE *.cpp)
add_executable(raytracing01 raytracing01.cpp ${SOURCE})