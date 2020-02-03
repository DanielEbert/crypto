/*
 * RSAEncryptor.cpp
 */

#include "PublicKeyAlgorithmBox.h"
#include "RSAEncryptor.h"

RSAEncryptor::RSAEncryptor(const Integer& n, const Integer& e) {
    // Setze den öffentlichen Teil des Schlüssels n und e auf die
    // Übergabeparameter
    this->n = n;
    this->e = e;
}

RSAEncryptor::~RSAEncryptor() {}

bool RSAEncryptor::compute(const Integer& x, Integer& y) const {
    // Wenn x >= n wahr ist, dann ist x nicht im Klartextraum und kann deshalb
    // nicht verschlüsselt werden.
    if (x >= n) return false;
    PublicKeyAlgorithmBox pb;
    // Bei der Verschlüsselung mit RSA wird eine zu verschlüsselnde Zahl x
    // mit x^e mod n verschlüsselt. Diese Berechnung kann mit der modularen
    // Exponentation, wie sie in der PublicKeyAlgorithmBox Klasse implementiert
    // ist, gemacht werden. Der verschlüsselte Text/Zahl wird im Integer y
    // gespeichert.
    y = pb.modularExponentation(x, e, n);

    // x ist ein Element des Klartextraums und konnte verschlüsselt werden.
    // Darum wird True zurückgegeben.
    return true;
}
