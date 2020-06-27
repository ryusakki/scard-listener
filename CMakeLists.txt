cmake_minimum_required(VERSION 3.13.0)

project(SCardListener)

add_executable(${PROJECT_NAME} src/SCardListener.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE inc)
target_link_libraries(${PROJECT_NAME} PRIVATE Winscard)