# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src/Praktikum-Public-Key
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

FIND_PACKAGE(CryptoPP REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

INCLUDE_DIRECTORIES(${CRYPTOPP_INCLUDE_DIR})

ADD_EXECUTABLE(pubkey pubkey RabinDecryptor PublicKeyAlgorithmBox)
TARGET_LINK_LIBRARIES(pubkey ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

ADD_EXECUTABLE(rabin rabin RabinEncryptor RabinDecryptor RabinAttack PublicKeyAlgorithmBox)
TARGET_LINK_LIBRARIES(rabin ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

ADD_EXECUTABLE(rsa rsa PublicKeyAlgorithmBox RSAOracle RSAEncryptor RSADecryptor RSAAttack)
TARGET_LINK_LIBRARIES(rsa ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

