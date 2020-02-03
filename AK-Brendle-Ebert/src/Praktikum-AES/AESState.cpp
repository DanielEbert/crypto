#include <string>
#include "AESState.h"

#include <cstring>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

AESState::AESState(const AESMath& aesmath, bool debug_mode)
    : debug_mode(debug_mode), math((const AESMath *) &aesmath)
{
    for (size_t i=0; i<16; i++) {
        state[i] = 0;
    }
}

void AESState::addKey(const word* key) {
    /*
     * Aufgabe 19
     */

    // Der Reihe nach wird jede Cell, also jedes Byte in state mit dem
    // Rundenschlüssel der aktuellen Runde XOR verknüpft.
    for (int col = 0; col < 4; col++) {
        byte keyWordAsByte[4];
        // Da der Rundenschlüssel als word übergeben wurde und die state ein
        // Byte Array ist, wird der Rundenschlüssel als 4 Byte großes Array
        // in keyWordAsByte gespeichert.
        for (int j = 0; j < 4; j++) {
            keyWordAsByte[3-j] = (key[col] >> (8 * j));
        }
        // Anschließend können die 4 Cells der aktuell verarbeitenden Spalte
        // Bitweise XOR verknüpft werden.
        for (int row = 0; row < 4; row++) {
            byte newB = getCell(row, col) ^ keyWordAsByte[row];
            setCell(row, col, newB);
        }
    }
}

string AESState::format() const {
    ostringstream os;

    for (size_t i=0; i<16; i++) {
        os << setw(2) << setfill('0') << hex << (unsigned)state[i] << " ";
    }
    return os.str();
}


void AESState::get(byte* out) {
    /*
     * Aufgabe 14b
     */
    assert(out!=NULL);
    memcpy(out, state, 16);
}

byte AESState::getCell(size_t row, size_t col) {
    return state[row + 4*col];
}

void AESState::setCell(size_t row, size_t col, byte b) {
    state[row + 4*col] = b;
}


void AESState::subBytes() {
    /*
     * Aufgabe 17a
     */

    // Auf jedes Byte des state
    for (int i = 0; i < 16; i++) {
        // wird die Substitution mit der S-Box angewandt.
        state[i] = math->sBox(state[i]);
    }
}


void AESState::invSubBytes() {
    /*
     * Aufgabe 17b
     */
    // Für jedes Byte des state
    for (int i = 0; i < 16; i++) {
        // wird die Substitution mit der inversen S-Box rückgängig gemacht.
        state[i] = math->invSBox(state[i]);
    }
}

void AESState::set(const byte* in) {
    /*
     * Aufgabe 14a
     */

    // kopiert die 16 Bytes ab *in an state
    for (int i = 0; i < 16; i++) {
        state[i] = in[i];
    }
}


void AESState::mixColumns() {
    /*
     * Aufgabe 18a
     */

    // m ist die 4x4 Matrix, mit der die als Vektor interpretierte Spalte
    // multipliziert wird
    byte m[4][4] = {{2,3,1,1},
                    {1,2,3,1},
                    {1,1,2,3},
                    {3,1,1,2}};

    // Jede Spalte des state wird transformiert
    for (int col = 0; col < 4; col++) {
        // Dabei müssen die Zwischenergebnisse in einem separaten Speicher
        // gespeichert werden, da sonst in der Transformation benötigte
        // Daten von Zwischenergebnissen überschrieben werden.
        byte tmp[4] = {0,0,0,0};
        // Jede Spalte des state wird als Vektor interpretiert. Die Spalte des state besteht aus
        // 4 Reihen. Dieser Vektor wird mit Hilfe der Matrixmultiplikation
        // über GF(256) mit der 4x4 Matrix in m multipliziert.
        // Dafür muss für jede Zeile der Matrix m, also 4 mal, die jeweilige
        // Zeile der Matrix mit dem Vektor der aktuell zu transformierenden Spalte des state
        // multipliziert werden und die Ergebnisse aller Zeilen der Matrix m müssen zusammen
        // addiert werden. Da über GF(256) addiert und multipliziert wird, können
        // die Methoden der AESMath Klasse verwendet werden.
        for (int row = 0; row < 4; row++) {
            byte cur = 0;
            for (int colMult = 0; colMult < 4; colMult++) {
                cur = math->add(cur, math->mul(m[row][colMult], getCell(colMult, col)) );
            }
            tmp[row] = cur;
        }
        // Abschließend kann das Ergebnis in tmp zurück in den state kopiert
        // werden.
        for(int i = 0; i < 4; i++){
            setCell(i, col, tmp[i]);
        }
    }
}

void AESState::invMixColumns() {
    /*
     * Aufgabe 18b
     */

    // m ist die 4x4 Matrix, mit der die als Vektor interpretierte Spalte
    // multipliziert wird
    byte m[4][4] = {{14,11,13,9},
                    {9,14,11,13},
                    {13,9,14,11},
                    {11,13,9,14}};

    // Diese Berechnungen sind die selben wie bei mixColumns, einzig die 4x4
    // Matrix m, mit der multipliziert wird, wurde so abgeändert, damit die
    // mixColumns Funktion invertiert wird.

    // Jede spalte des state wird transformiert
    for (int col = 0; col < 4; col++) {
        // Dabei müssen die Zwischenergebnisse in einem separaten Speicher
        // gespeichert werden, da sonst in der Transformation benötigte
        // Daten von Zwischenergebnissen überschrieben werden.
        byte tmp[4] = {0,0,0,0};
        // Jede Spalte des state wird als Vektor interpretiert. Die Spalte des state besteht aus
        // 4 Reihen. Dieser Vektor wird mit Hilfe der Matrixmultiplikation
        // über GF(256) mit der 4x4 Matrix in m multipliziert.
        // Dafür muss für jede Zeile der Matrix m, also 4 mal, die jeweilige
        // Zeile der Matrix mit dem Vektor der aktuell zu transformierenden Spalte der state
        // multipliziert werden und die Ergebnisse aller Zeilen der Matrix m müssen zusammen
        // addiert werden. Da über GF(256) addiert und multipliziert wird, können
        // die Methoden der AESMath Klasse verwendet werden.
        for (int row = 0; row < 4; row++) {
            byte cur = 0;
            for (int colMult = 0; colMult < 4; colMult++) {
                cur = math->add(cur, math->mul(m[row][colMult], getCell(colMult, col)) );
            }
            tmp[row] = cur;
        }
        // Abschließend kann das Ergebnis in tmp zurück in die state kopiert
        // werden.
        for(int i = 0; i < 4; i++){
            setCell(i, col, tmp[i]);
        }
    }
}

void AESState::shiftRow(size_t row, size_t shift) {
    /*
     * Aufgabe 15
     */

    // in jedem Schleifendurchlauf wird die row des state um
    // 1 Byte nach links verschoben
    for (int i = 0; i < shift; i++) {
        // Das Byte der Spalte 0 muss zwischengespeichert werden, damit es
        // nicht überschrieben wird.
        byte tmp = getCell(row, 0);
        // Anschließend wird das Byte in Spalte 0 (und Reihe row) auf das Byte
        // der ersten Spalte gesetzt, das Byte der Spalte 1 auf das Byte der
        // Spalte 2 und das Byte der Spalte 2 auf das Byte der Spalte 3.
        for (int j = 0; j < 3; j++) {
            setCell(row, j, getCell(row, j+1));
        }
        // Abschließend muss nurnoch das Byte der Spalte 3 auf das
        // zwischengespeicherte Byte in tmp, also der ursprünglichen Spalte 0,
        // gesetzt werden.
        setCell(row, 3, tmp);
    }
}


void AESState::shiftRows() {
    /*
     * Aufgabe 16a
     */
    // 0-te Spalte wird nicht geshiftet
    // 1-te Spalte wird um 1 nach links geshiftet
    shiftRow(1, 1);
    // 2-te Spalte wird um 2 nach links geshiftet
    shiftRow(2, 2);
    // 3-te Spalte wird um 3 nach links geshiftet
    shiftRow(3, 3);
}


void AESState::invShiftRows() {
    /*
     * Aufgabe 16b
     */
    // 0-te Spalte wird nicht geshiftet
    // 1-te Spalte wird um 3 nach links geshiftet. Die Basisoperation shiftRows
    // hatte die Spalte um 1 nach links geshiftet. Drei weitere Linksshifts bringen die
    // Werte der Reihe wieder in die ursprüngliche Position.
    shiftRow(1, 3);
    // Die Basisoperation shiftRows
    // hatte die Spalte um 2 nach links geshiftet. 2 weitere Linksshifts bringen die
    // Werte der Reihe wieder in die ursprüngliche Position.
    shiftRow(2, 2);
    // Die Basisoperation shiftRows
    // hatte die Spalte um 3 nach links geshiftet. 1 weiterer Linksshifts bringt die
    // Werte der Reihe wieder in die ursprüngliche Position.
    shiftRow(3, 1);
}
