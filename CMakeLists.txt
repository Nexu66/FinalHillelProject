cmake_minimum_required(VERSION 3.19)
project(HillelFinal LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)


add_subdirectory(src/app)
add_subdirectory(src/dialog)
