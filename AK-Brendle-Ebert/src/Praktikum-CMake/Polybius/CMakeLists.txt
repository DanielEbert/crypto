# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src/Praktikum-CMake/Polybius
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

find_package(Gengetopt REQUIRED)

include_directories(.. ${CMAKE_SOURCE_DIR})

add_executable(polybius-test
        polybius-test.cpp
        PolybiusCipher
        Toolbox)
        
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/polybius-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/polybius-getopt.c
        COMMAND ${GENGETOPT_EXECUTABLE} --include-getopt --unamed-opts
        -i polybius-getopt.ggo -F polybius-getopt
        DEPENDS polybius-getopt.ggo
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

add_executable(polybius
        polybius-main
        PolybiusCipher
        Toolbox
        ${CMAKE_CURRENT_SOURCE_DIR}/polybius-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/polybius-getopt.c)