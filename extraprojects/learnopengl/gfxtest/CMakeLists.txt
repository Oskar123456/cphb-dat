cmake_minimum_required(VERSION 3.27)
project(gfxtest)

set(CMAKE_CXX_STANDARD 17)

add_executable(gfxtest main.cpp
        includes/glad/gl.h
        includes/glad/khrplatform.h
        includes/glad/vk_platform.h
        includes/glad/vulkan.h
        includes/GLFW/glfw3.h
        includes/GLFW/glfw3native.h
        includes/getopt.c
        includes/getopt.h
        includes/glad_gl.c
        includes/glad_vulkan.c
        includes/linmath.h
        includes/nuklear.h
        includes/nuklear_glfw_gl2.h
        includes/stb_image_write.h
        includes/tinycthread.c
        includes/tinycthread.h)
