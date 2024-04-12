cmake_minimum_required(VERSION 3.8.2)

project (test)
add_executable(${PROJECT_NAME} test.c)

find_package(Vulkan REQUIRED) # error

add_subdirectory(external/glfw)
add_subdirectory(external/cglm)
target_link_libraries(${PROJECT_NAME} cglm)
target_link_libraries(${PROJECT_NAME} glfw)

target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
