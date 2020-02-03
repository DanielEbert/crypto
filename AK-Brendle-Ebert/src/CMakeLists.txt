# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

project(AngewandteKryptographie)
cmake_minimum_required(VERSION 3.4)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE false)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin )
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules )
add_subdirectory(Praktikum-CMake)

SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
        ${PROJECT_SOURCE_DIR}/CMakeModules)
ADD_SUBDIRECTORY( Praktikum-CryptoPP )

ADD_SUBDIRECTORY( Praktikum-Vigenere )

ADD_SUBDIRECTORY( Praktikum-DES )

ADD_SUBDIRECTORY( Praktikum-Primzahlen )

ADD_SUBDIRECTORY( Praktikum-AES )

ADD_SUBDIRECTORY( Praktikum-DH-Protokoll )

ADD_SUBDIRECTORY( Praktikum-Public-Key )
