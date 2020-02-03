#ifndef AES_AESSTATE_H
#define AES_AESSTATE_H

#include <cassert>
#include <string>
#include "datatypes.h"
#include "AESMath.h"

using namespace std;

/**
 * AESState stellt den internen Zustand von AES dar.
 * Der interne Zustand wird state genannt. Im Standard ist state ein
 * Zweidimensionales Array. In dieser Implementierung wird dieser
 * Zweidimensionales Array in einem eindimensionales Array gespeichert, wobei
 * die Spalten hintereinander konkateniert abgespeichert werden.
 * Wie in den anderen Klassen für AES dieser Implementierung
 * wird davon ausgegangen, dass nb = 4 ist. Der state ist deshalb 16 Byte groß.
 * Für das Ver- oder Entschlüsseln wird der state auf die Eingabe gesetzt.
 * Nach einer Reihe von Transformationen enthält der State das Resultat der
 * Ver- oder Entschlüsselung.
 *
 * Weiter werden Funktionen angeboten, welche den State
 * verändern/transformieren.
 *
 * Wie in den anderen Klassen für AES dieser Implementierung
 * wird davon ausgegangen, dass nb = 4 ist.
 */
class AESState {
private:
    /**
     * Flag zum Aktivieren von Debug-Ausgaben. Wenn True, dann werden
     * Debug Ausgaben über stdout ausgegeben.
     */
    bool debug_mode;
    /**
     * Zeiger auf ein Objekt der Klasse AESMath, welche Mathematische
     * Operationen auf dem endlichen Körper GF(256) implementiert.
     */
    const AESMath *math;

protected:
    /**
     * Interner Zustand (Array mit 16 Byte) des AES
     *
     * Die state kann als rechteckiges array von bytes interpretiert werden.
     * Dieses Array hat 4 Reihen und weil nb auf 4 gesetzt ist auch 4 Zeilen
     * (also eine Matrix mit Dimension 4x4). Gespeichert ist es allerdings
     * in einem 1 dimensionalen Array, wobei  die Spalten hintereinander
     * konkateniert abgespeichert werden.
     *
     * Auf diesen 16 Bytes werden die in dieser Klasse implementiereten
     * Transformationen angewandt, um eine 16 Byte große Eingabe in state
     * zu ver- oder entschlüsseln.
     */
    byte state[16];

    byte getCell(size_t row, size_t col);
    void setCell(size_t row, size_t col, byte b);

    /**
     * shiftRow führt eine zyklische Linksverschiebung der Zeile row der state
     * um shift Positionen durch.
     *
     * @param row Zeile der State, die verschoben werden soll.
     * @param shift Anzahl der Positionen, um die die row nach links
     * zyklisch verschoben werden wird.
     */
    void shiftRow(size_t row, size_t shift);

public:
    AESState(const AESMath& aesmath, bool debug_mode=false);

    /**
     * Setzt die 16 Byte in state auf die 16 Bytes ab *in.
     *
     * @param in Zeiger auf einen mindestens 16 Byte großen Speicherbereich.
     * Diese 16 Bytes werden nach state kopiert.
     */
    void set(const byte* in);

    /**
     * Setzt die 16 Bytes ab *out auf die 16 Bytes von state.
     *
     * @param out Zeiger auf einen mindestens 16 Byte großen Speicherbereich,
     * an welchen das 16 Byte große state Array kopiert wird.
     */
    void get(byte* out);

    /**
     *  Führt die Basisoperation ShiftRows auf den internen Zustand aus.
     *  ShiftRows shiftet die Reihen der state zyklisch um verschiedene offsets
     *  nach links.
     *  Reihe 0 wird nicht verschoben. Reihe 1 wird um 1 Byte, Reihe 2 um 2 und
     *  Reihe 3 um 3 Byte nach links zyklisch verschiebt. Diese Offsets können
     *  bei anderen Werten für nb auch unterschiedlich sein. 
     */
    void shiftRows();

    /**
     * Führt die Basisoperation SubBytes auf den internen Zustand aus.
     * SubBytes transformiert den Zustand mit Hilfe einer Substritution mit
     * der SBox, wobei jedes Byte des state über die sBox Funktion einen neuen
     * Wert zugewiesen bekommt.
     * sBox wendet zwei bijektive Abbildungen auf jedes Byte b in state an.
     * Als erstes wird dabei b auf sein multiplikatives
     * Inverses in GF(256) abgebildet. Zweitens wird das im ersten Schritt
     * berechnete multiplikative Inverse von b mit einer
     * affinen Abbildung abgebildet.
     */
    void subBytes();

    /**
     * Führt die Basisoperation MixColumns auf den internen Zustand aus.
     * Dabei wird der Zustand spaltenweise transformiert, wobei eine Spalte
     * als Vektor interpretiert wird und mit einer vorbestimmten 4x4 Matrix in GF(256)
     * multipliziert wird.
     */
    void mixColumns();

    /**
     * invShiftRows macht die shiftRows Funktion wieder rückgänig.
     * Dafür wird die Reihe 1 wird um 3 Byte, Reihe 2 um 2 und
     * Reihe 3 um 1 Byte zyklisch nach links verschoben. Da das Shiften
     * zyklisch ist, kann an dieser Stelle ein Linkssshift anstelle eines
     * Rechtsshifts verwendet werden.
     * Wird für eine State shiftRows und danach invShiftRows aufgerufen, dann ist
     * die daraus resultierende State die Gleiche wie vor dem Aufruf von shiftRows().
     *
     * Jede Basisoperation auf den state kann invertiert werden. Diese
     * Eigenschaft wird hier benötigt, um einen verschlüsselten Text wieder
     * entschlüsseln zu können. Für das Entschlüsseln werden die Basisoperationen durch
     * ihre inversen Paare ersetzt und in umgekehrter Reihenfolge auf den
     * state, also des cipher_texts, angewandt.
     */
    void invShiftRows();

    /**
     * invSubBytes macht die subBytes Substitution wieder rückgängig.
     *
     * Jede Basisoperation auf den state kann invertiert werden. Diese
     * Eigenschaft wird hier benötigt, um einen verschlüsselten Text wieder
     * zu entschlüsseln. Für das Entschlüsseln werden die Basisoperationen durch
     * ihre inversen Paare ersetzt und in umgekehrter Reihenfolge auf den
     * state, also des cipher_texts, angewandt.
     */
    void invSubBytes();

    /**
     * invMixColumns macht die mixColumns Transformation wieder rückgänig.
     * Dabei wird der Zustand spaltenweise transformiert, wobei eine Spalte
     * als Vektor interpretiert wird und mit einer vorbestimmten 4x4 Matrix in GF(256)
     * multipliziert wird. Diese 4x4 Matrix ist so gesetzt, damit die
     * mixColumns Transformation invertiert bzw. rückgängig gemacht wird.
     *
     * Jede Basisoperation auf den state kann invertiert werden. Diese
     * Eigenschaft wird hier benötigt, um einen verschlüsselten Text wieder
     * zu entschlüsseln. Für das Entschlüsseln werden die Basisoperationen durch
     * ihre inversen Paare ersetzt und in umgekehrter Reihenfolge auf den
     * state, also des cipher_texts, angewandt.
     */
    void invMixColumns();

    /**
     * addKey implementiert die AddRoundKey Transformation, wobei der
     * Rundenschlüssel in key auf den internen Zustand addiert wird. Bei dieser
     * Addition wird jedes Byte des Zustand mit dem entsprechenden Byte des
     * Rundenschlüssels mit XOR verknüpft. Der Rundenschlüssel muss dabei
     * die Größe der state haben. Da diese in unserer Implementierung fest
     * gesetzt ist, muss jeder Rundenschlüssel eine Größe von 16 Byte haben.
     *
     * Jeder Rundenschlüssel besteht aus 16 Bytes der key_schedule.
     *
     * @param key Zeiger auf den key, welcher der Rundenschlüssel ist,
     * der für die AddRoundKey Transformation verwendet wird. Dieser
     * Rundenschlüssel muss eine Größe von 16 Byte, also 4 words haben.
     */
    void addKey(const word* key);

    string format() const;
};


#endif //AES_AESSTATE_H
