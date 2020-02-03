#include "PRNG.h"
#include <cassert>

byte PRNG::getByte() {
    /*************************************************************************
     * Aufgabe 6d.
     *************************************************************************/

    byte ret = 0;

    // zufälliges byte besteht aus 8 zufälligen Bits
    for (int i = 0; i < 8; i++) {
        // für jedes Bit im Byte wird ein zufälliges Bit erzeugt
        ret = ret << 1;
        ret += getBit();
    }

    return ret;
}

void PRNG::getBlock(byte *buffer, unsigned int n) {
    /*************************************************************************
     * Aufgabe 6e.
     *************************************************************************/

    // Buffer besteht aus n Bytes. Für jedes Byte wird ein
    // zufälliges Byte erzeugt und im buffer an der jeweiligen
    // Stelle abgespeichert
    for(int i = 0; i < n; i++) {
        buffer[i] = getByte();
    }
}

Integer PRNG::getInteger(unsigned int size) {
    /*************************************************************************
     * Aufgabe 6g.
     *************************************************************************/

    // Wie getBlock aber der Rückgabewert ist ein Integer der CryptoPP Klasse
    byte arr[size];
    getBlock(arr, size);

    // Integer Klasse hat Konstruktor für byte-Array
    return Integer(arr, size);
}

Integer PRNG::getInteger(const Integer &m) {
    /*************************************************************************
     * Aufgabe 6i.
     *************************************************************************/

    // Der Parameter m muss laut Aufgabenstellung eine ganze Zahl > 0 sein.
    assert(m > 0);

    Integer ret;

    do {
        // Nach jedem Schleifendurchlauf muss ret zurückgesetzt werden.
        // Sonst wird ret immer größer und ret >= m wird nie false.
        ret = 0;
        // caching der Anzahl von Bits um Integer m darzustellen
        int bitCount = m.BitCount();
        for (int i = 0; i < bitCount; i++) {
            ret <<= 1;
            ret += getBit();
        }
    } while (ret >= m);     // für die geforderte gleichverteilung muss solange
                            // erzeugt werden, bis ret < m.

    return ret;
}
