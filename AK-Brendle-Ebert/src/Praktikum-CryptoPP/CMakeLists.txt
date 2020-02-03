# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src/Praktikum-5
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

FIND_PACKAGE(CryptoPP REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

INCLUDE_DIRECTORIES(${CRYPTOPP_INCLUDE_DIR})
LINK_DIRECTORIES(/usr/local/lib)

ADD_EXECUTABLE(cryptopp-intro cryptopp-intro PRNG BlumBlumShubGenerator ../Praktikum-CMake/Polybius/Toolbox)
TARGET_LINK_LIBRARIES(cryptopp-intro ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})


