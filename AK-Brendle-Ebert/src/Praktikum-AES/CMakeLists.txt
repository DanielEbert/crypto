# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src/Utils
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


add_executable(aestests aestests-main.cpp AESMath.cpp AESMath.h datatypes.h
        AESState.cpp AESState.h
        AESKeySchedule.cpp AESKeySchedule.h
        AESCipher.cpp AESCipher.h
        BlockCipher.h BlockCipher.cpp)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/aes-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/aes-getopt.c
        COMMAND ${GENGETOPT_EXECUTABLE} --include-getopt --unamed-opts
        -i aes-getopt.ggo -F aes-getopt
        DEPENDS aes-getopt.ggo
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(aes aes-main.cpp
        datatypes.h
        AESMath.h AESMath.cpp
        AESKeySchedule.h AESKeySchedule.cpp
        BlockCipher.h BlockCipher.cpp
        AESCipher.h AESCipher.cpp
        AESState.h AESState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aes-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/aes-getopt.c

        )