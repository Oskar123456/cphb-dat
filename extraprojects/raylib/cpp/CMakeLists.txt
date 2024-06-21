cmake_minimum_required(VERSION 3.22)

project(main)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)

# add_subdirectory(external/raylib)
add_subdirectory(external/cglm)
add_subdirectory(external/glfw)

# target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE cglm)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
