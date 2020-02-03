# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src/Praktikum-Primzahlen
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

INCLUDE_DIRECTORIES( ${CRYPTOPP_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-CryptoPP)

LINK_DIRECTORIES( ${CRYPTOPP_LIBRARY_DIR} )

ADD_EXECUTABLE( primes primes-main  FortunaGenerator MillerRabinTest
    ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-CryptoPP/PRNG )

TARGET_LINK_LIBRARIES(primes ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

