cmake_minimum_required(VERSION 3.18)
project(gtktest LANGUAGES C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED IMPORTED_TARGET gtk+-3.0)

add_executable(gtktest main.c)
target_link_libraries(gtktest PRIVATE PkgConfig::GTK3)