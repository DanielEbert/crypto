#include "byte.h"
#include "DESCipher.h"
#include <iostream>

using namespace std;

// Aufgabe 3
int main() {
    DESCipher desCipher;

    // Bytearray 0010111101101011:
    cout << "0010111101101011" << " -> ";
    // Bits als Byte codiert abgespeichert
    byte a[2] = {0x2F, 0x6B};
    // Aufruf von printBitField der DESCipher Klasse
    // mit Zeiger auf das Bytefeld, der Größe dieses Byterfelds
    // Das dritte Argument wurde auf 16 gesetzt, damit kein
    // Leerzeichen mit ausgegeben wird.
    desCipher.printBitField(a, 2, 16);

    cout << endl;

    // Bytearray 1111001111000000:
    cout << "1111001111000000" << " -> ";
    byte b[2] = {0xF3, 0xC0};
    desCipher.printBitField(b, 2, 16);

    cout << endl;

    return 0;
}