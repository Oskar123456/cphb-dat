cmake_minimum_required(VERSION 3.8.2)

project (main LANGUAGES C)

add_executable(${PROJECT_NAME} src/main.c src/glad.c src/shader_utils.c)

# DEBUG ######
if(CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG)
endif()
##############

target_include_directories(${PROJECT_NAME} PUBLIC
  ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE
  external/stb/
  external/sds/)

add_library(sds STATIC external/sds/sds.c)

add_subdirectory(external/glfw)
add_subdirectory(external/cglm)

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} cglm)
target_link_libraries(${PROJECT_NAME} sds)
target_link_libraries(${PROJECT_NAME} m)

MESSAGE(CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}")
