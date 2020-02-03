/*
 * RSAOracle.cpp
 */

#include <cassert>
#include <iostream>
#include "RSAOracle.h"
#include "PublicKeyAlgorithmBox.h"

using namespace std;

RSAOracle::RSAOracle(Integer& p, Integer& q, Integer& d) {
    //Konstruktor von RSAOracle
    //setze n auf das Ergebnis der Multiplikation der beiden Primzahlen p und q
    n = p * q;
    //Weise dem Integer-Attribut d des Objekts das übergebenen d zu
    this->d = d;
}

RSAOracle::~RSAOracle() {
}

// #half()
bool RSAOracle::half(const Integer& y) const {
    //Methode half(), die für einen Geheimtext das höchstwertigste Bit des
    //zugehörigen Klartexts zurücklieferts

    //Erzeugen eines Objekts der Klasse PublicKeyAlgorithmBox, in dem die
    //anschließende Modulare Exponentation zum entschlüsseln des Geheimtexts
    //implementiert ist
    PublicKeyAlgorithmBox pb;

    //Entschlüsseln des übergebenen Geheimtexts y und abspeichern in der 
    //Variable x. Dazu wird die in der Klasse PublicKeyAlgorithmBox implementierte
    //Methode zur Modularen Exponentation eingesetzt und y^d (mod n) berechnet,
    //was der Entschlüsselung im RSA-Kryptosystem entspricht
    Integer x = pb.modularExponentation(y, d, n);

    //Rückgabe von true, wenn berechnete x größer als n/2 ist bzw. False ansonsten
    //Dies entspricht dem höchstwertigsten Bit des Klartexts
    return x > n / 2;
}

// #parity()
bool RSAOracle::parity(const Integer& y) const {
  return false;
}
