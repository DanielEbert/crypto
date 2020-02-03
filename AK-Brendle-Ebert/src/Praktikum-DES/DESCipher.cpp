#include <cassert>
#include <iomanip>
#include "DESCipher.h"

DESCipher::DESCipher() {
}

DESCipher::~DESCipher() {
}

void DESCipher::computeKeySchedule(const byte *key, bool encmode) {
/******************************************************************************
 * Aufgabe 5
 ******************************************************************************/
    // Wir berechnen und speichern das Mapping von Bits aus key zu den
    // Rundenschlüsseln in roundKeySchedule ab.
    // Das wird für jeden der 16 Rundenschlüssel gemacht.
    // Jeder Rundenschlüssel ist 48 Bit groß. Für jedes Bit X des
    // Rundenschlüssels wird berechnet und in roundKeySchedule
    // abgespeichert, welches Bit aus *key für Bit X des
    // Rundenschlüssels verwendet werden soll.
    // Zum Beispiel ist roundKeySchedule[0][0] = 10. Das bedeutet,
    // dass das erste Bit des ersten Rundenschlüssels auf den Wert
    // des zehnten Bit des key gesetzt wird.
    byte roundKeySchedule[16][48];

    // Der Rundenschlüssel werden in meherern Schritten berechnet.
    // Als Erstes wird der 64 Bit key mit der pc1 Permutation
    // (bzw. des Arrays pc1, welches bei der Permutation verwendet wird)
    // permutiert. Permutation mit pc1 bedeutet, dass das i-te Bit
    // in key an das (pc1[i]-1)-te Bit eines Felds geschrieben wird,
    // wobei i = 0,1,...,55.
    // Bei dieser Permutation werden 8 Bits des 64 Bit
    // key nicht zugewiesen. Bei diesen 8 Bits handelt es sich um
    // die Parity-Bits. Diese werden bei der Generierung der
    // Rundenschlüssel nicht benötigt und werden durch die
    // Permutation ignoriert bzw. herausgefiltert.

    // Anschließend werden die aus der Permutation resultierenden
    // 56 Bits in zwei Teile geteilt. Jedes Teil ist 28 Bit groß.
    // Der Inhalt des ersten Teils entspricht den ersten 28 Bits
    // der aus der Permutation pc1 resultierenden 56 Bits.
    // Der Inhalt des zweiten Teils entspricht den letzten
    // 28 Bits der aus der Permutation pc1 resultierenden 56 Bits.
    // Auf beide Teile werden zyklische Linksverschiebungen
    // angewendet. Zyklisch bedeutet, dass Bits die links
    // herausgeschoben werden auf der rechten Seite wieder
    // reingeschoben werden. Die Anzahl der zyklischen
    // Linksverschiebungen hängt davon ab, um welchen
    // Rundenschlüssel es sich handelt. Zum Beispiel
    // wird für den ersten Rundenschlüssel ein mal,
    // für den zweiten zwei mal, für den dritten vier mal
    // und so weiter verschoben. Das total_rot Array gibt
    // diese Anzahl für jeden Rundenschlüssel an.

    // Abschließend werden die zwei Teile wieder zusammengesetzt,
    // sind also wieder 56 Bits groß, und werden mit einer weiteren
    // Permutation pc2 permutiert. Dabei werden wieder
    // 8 Bits herausgefiltert. Die Permutation weist nur
    // 48 der 56 Bits zu. Die 8 nicht zugewiesenen Bits
    // werden wie bei Permutation pc1 nicht verwendet und
    // ignoriert. Die aus dieser Permutation resultierenden
    // 48 Bits bilden einen Rundenschlüssel.

    // Um für roundKeySchedule jedes j-te Bit jedes i-ten
    // Rundenschlüssel zu berechnen, muss rückwärtsgerechnet
    // werden.
    // Speziell wird für jeden Rundenschlüssel i...
    for (int i = 0; i < 16; i++) {
        // ...und jedes Bit j des Rundenschlüssel i...
        for (int j = 0; j < 48; j++) {
            // ...als Erstes die pc2 Permutation zurückgerechnet.
            // jPC2 gibt an, welches Bit nach den Linksverschiebungen
            // am j-ten Bit des Rundenschlüssels steht. Dabei gilt
            // zu beachten, dass hier ab 1 und nich von 0 an gezählt
            // wird.
            byte jPC2 = pc2[j];
            // Danach wird berechnet und in jLS abgespeichert,
            // welches Bit vor den Linksverschiebungen an der
            // durch jPC2 spezifizierten Position steht.
            byte jLS;
            // Hier wird berechnet, wieviele Linksverschiebungen
            // angewendet worden sind.
            // Dabei muss auch der Teil, in dem sich jPC2
            // befindet berücksichtigt werden.
            // Wegen den zyklischen Linksverschiebungen und
            // der Größe der Teile von 28 Bits wird modulo 28
            // gerechnet.
            if (jPC2 <= 28) {
                jLS = (jPC2 + total_rot[i]-1) % 28;
            } else {
                jLS = 28 + ((jPC2 - 28 + total_rot[i]-1) % 28);
            }
            // Abschließend wird, wie bei der pc2 Permutation,
            // die pc1 Permutation zurückgerechnet.
            // Der Wert von pc1[jLS] gibt an, dass das j-te Bit
            // des i-ten Rundenschlüssels auf den Wert
            // des pc1[jLS]-ten Bit des key gesetzt wird.
            roundKeySchedule[i][j] = pc1[jLS];
        }
    }

    // roundKeySchedule kann dann mit einer Permutation zur Berechnung der
    // Rundenschlüssel verwendet werden.
    for( size_t i = 0; i < 16; i++ ) {
        for( size_t j = 0; j < 48; j++ ) {
            setBit(key_schedule[i], 6, j, getBit(key, 8, roundKeySchedule[i][j]-1));
        }
    }

    // Beim Entschlüsseln wird die Reihenfolge der Rundenschlüssel
    // vertauscht. Speziell wird bei der ersten Entschlüsselungsrunde
    // der 16-te berechnete Rundenschlüssel verwendet. Bei der
    // zweiten Entschlüsselungsrunde der 15-te berechnete Rundenschlüssel
    // und so weiter.
    if (!encmode) {
        for (int i = 0; i < 8; i++) {
            // Vertauscht 6 bytes, also die Größe eines Rundenschlüssels
            // ab key_schedule[i] und key_schedule[15-i]
            for(int j = 0; j < 6; j++) {
                byte tmp = key_schedule[i][j];
                key_schedule[i][j] = key_schedule[15-i][j];
                key_schedule[15-i][j] = tmp;
            }
        }
    }
}


byte DESCipher::computeSBox(byte id, byte line, byte col) {
    /******************************************************************************
     * Aufgabe 6a
     ******************************************************************************/
     // Es gibt 8 S-Boxen und jede S-Box ist eine 4x16 Matrix.
     // Checks for out of bounds access.
    assert(id < 8);
    assert(line < 4);
    assert(col < 16);

    // line (Zeile) und col (Spalte) spezifizieren die Position eines Elements
    // in einer 4x16 Matrix.
    // Die Matrix wird hier, wie im Doxygen beschrieben,
    // 'ausgerollt' als Array abgespeichert.
    // Über 'line*16' kann die Startposition der Matrixzeile im Array bestimmt
    // werden.
    return sbox[id][line * 16 + col];
}


int DESCipher::decrypt
        (
                const byte *cipher_text,
                int cipher_len,
                const byte *key,
                int key_len,
                byte *plain_text,
                int plain_len
        ) {
    /******************************************************************************
     * Aufgabe 8
     ******************************************************************************/

    assert(cipher_len % block_len == 0);
    // Um das komplette Ergebnis zu speichern muss plain_len größer
    // als cipher_len sein.
    assert(plain_len >= cipher_len);
    assert(key_len == 8);

    // Aus dem key werden 16 Rundenschlüssel erzeugt.
    // encmode ist auf false gesetzt, damit Rundenschlüssel
    // für das Entschlüsseln erzeugt werden bzw. dass die
    // Rundenschlüssel in der richtigen Reihenfolge
    // in key_schedule abgespeichert werden.
    computeKeySchedule(key, /*encmode=*/false);

    // Unter processBlock wird dann auf diese in key_schedule
    // abgespeicherten Rundenschlüssel zugewiesen.

    // DES ist eine Blockchiffre. Deshalb wird in block_len großen Byte
    // Blöcken entschlüsselt. Das Ent- und Verschlüsseln eines Blocks wird
    // durch processBlock implementiert.
    // processBlock bearbeitet jeweils block_len Bytes.
    // Deshalb muss processBlock gegebenenfalls mehrmals mit
    // jeweils block_len Bytes des cipher_text aufgerufen werden.
    for (unsigned int i = 0; i < cipher_len; i += block_len) {
        processBlock(cipher_text+i, plain_text+i);
    }

    return plain_len;
}


int DESCipher::encrypt
        (
                const byte *plain_text,
                int plain_len,
                const byte *key,
                int key_len,
                byte *cipher_text,
                int cipher_len) {
    /******************************************************************************
     * Aufgabe 8
     ******************************************************************************/

    assert(plain_len % block_len == 0);
    // Um das komplette Ergebnis zu speichern muss cipher_len größer
    // als plain_len sein.
    assert(cipher_len >= plain_len);
    assert(key_len == 8);

    // Aus dem key werden 16 Rundenschlüssel erzeugt.
    // encmode ist auf true gesetzt, damit Rundenschlüssel
    // für das Verschlüsseln erzeugt werden bzw. dass die
    // Rundenschlüssel in der richtigen Reihenfolge
    // in key_schedule abgespeichert werden.
    computeKeySchedule(key, /*encmode=*/true);

    // Unter processBlock wird dann auf diese in key_schedule
    // abgespeicherten Rundenschlüssel zugewiesen.

    // DES ist eine Blockchiffre. Deshalb wird in block_len großen Byte
    // Blöcken verschlüsselt. Das Ent- und Verschlüsseln eines Blocks wird
    // durch processBlock implementiert.
    // processBlock bearbeitet jeweils block_len Bytes.
    // Deshalb muss processBlock gegebenenfalls mehrmals mit
    // jeweils block_len Bytes des cipher_text aufgerufen werden.
    for (unsigned int i = 0; i < plain_len; i += block_len) {
        processBlock(plain_text+i, cipher_text+i);
    }

    return plain_len;
}


void DESCipher::feistel
        (
                const byte *l_in,
                const byte *r_in,
                const byte *key,
                byte *l_out,
                byte *r_out,
                int rnd
        ) {
    /******************************************************************************
     * Aufgabe 6c
     ******************************************************************************/
    // Der zu Ver- oder Entschlüsselnde Text besteht aus zwei 32 Bit großen
    // Teilen. Der Inhalt beider Teile wird verarbeiten und wieder als
    // zwei 32 Bit Teile in l_out und r_out gespeichert.
    // In jeder Runde wird der Inhalt des linken l_out Teils auf den Inhalt des
    // rechten r_in Teils gesetzt.
    // In jedem Schleifendurchlauf werden 8 Bit gesetzt.
    for (int i = 0; i < 4; i++) {
        l_out[i] = r_in[i];
    }

    // Der Inhalt des rechten r_out Teils wird berechnet, indem
    // die F Funktion mit r_in und key verarbeitet, und abschließend
    // dieses Ergebnis mit l_in XOR wird.
    byte z[4];
    functionF(r_in, key, z, rnd);
    for (int i = 0; i < 4; i++) {
        r_out[i] = z[i] ^ l_in[i];
    }
}


void DESCipher::functionF
        (
                const byte *r_in,
                const byte *key,
                byte *r_out,
                int rnd
        ) {
    /******************************************************************************
     * Aufgabe 6b
     ******************************************************************************/

    // Im ersten Schritt werden die 32 Bit in r_in zu einem
    // 48 Bit expandiert, indem manche Bits in r_in mehrmals
    // für das resultierende 48 Bit große Ergebnis verwendet
    // werden. Das Ergebnis wird im expand array gespeichert.
    // Für die Expansion gibt das ev Array an, welche Bits
    // von r_in welchen Bits in expand zugewiesen werden.
    // Diese Funktionalität implementiert die permutate
    // Funktion.
    // Das bedeutet, dass das i = 0,1,...,48-te Bit in expand auf das
    // (ev[i]-1)-te Bit des r_in gesetzt wird.
    byte expand[6];
    permutate(ev, 48, r_in, 4, expand, 6);

    // Im zweiten Schritt wird expand XOR key berechnet.
    // Das Ergebnis wird im expand array gespeichert.
    for (int i = 0; i < 6; i++) {
        expand[i] ^= key[i];
    }

    // Danach werden die 48 Bits in expand in 6 Bit
    // Blöcke zerlegt und in b abgespeichert.
    // b ist ein 2D Array. Jedes Element 'bi' in b
    // repräsentiert einen 6 Bit Block und ist ein Array mit
    // 6 byte Elementen, wobei in jedem Byte ein Bit des
    // zerlegten 6 Bit 'bi' gespeichert wird.
    // Das bedeutet, dass in der zweiten Dimension von b
    // jedes byte entweder 0 oder 1 ist.
    byte b[8][6];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 6; j++) {
            b[i][j] = getBit(expand, 8, i * 6 + j);
        }
    }
    // Jeder 6 Bit Block wird Permutiert. Die Permutation
    // wird durch S-Boxen implementiert. In dieser
    // DES Implementation sind die durch die Permutation
    // resultierenden Ergebnisse bereits in Tabellen
    // berechnet. Diese Tabellen können dann als
    // Lookup Tabellen verwendet werden.
    // Wie in der Doxygen unter der Funktion computeSBox
    // beschrieben, werden die Ergebnisse eines Blocks in
    // einer 4x16 Matrix gespeichert. Das erste und sechste
    // Bit jedes Blocks bestimmt eindeutig die Zeile, die
    // Bits 2,3,4 und 5 bestimmen eindeutig die Spalte der
    // Matrix.
    // Das Ergebnis eines Blocks ist ein 4 Bitwort. Die Ergebnisse
    // aller 8 Blöche werden als byte im s Array gespeichert.
    // Da es sich um 4 Bitwörter handelt, sind die oberen 4 Bits
    // der Bytes immer 0.
    byte s[8];
    for (int i = 0; i < 8; i++) {
        byte line = 2 * b[i][0] + b[i][5];
        assert(line >= 0 && line <= 3);
        byte col = 8 * b[i][1] + 4 * b[i][2] + 2 * b[i][3] + b[i][4];
        assert(col >= 0 && col <= 15);
        s[i] = computeSBox(i, line, col);
    }

    // Die acht 4-Bitwörter hintereinander
    // im z Speicherblock gespeichert.
    byte z[4];
    z[0] = (s[0] << 4) + s[1];
    z[1] = (s[2] << 4) + s[3];
    z[2] = (s[4] << 4) + s[5];
    z[3] = (s[6] << 4) + s[7];
    // Abschließend werden die 32 Bits ab z mit der pp Tabelle
    // permutiert. Das Ergebnis wird im r_out Array gespeichert.
    // Das bedeutet, dass das i = 0,1,...,63-te Bit in r_out auf das
    // (pp[i]-1)-te Bit des z gesetzt wird.
    permutate(pp, 32, z, 4, r_out, 4);
}


bool DESCipher::getBit(const byte *array, int array_len, int pos) const {
    int bytepos, bitpos;
    byte b;

    assert(array_len > 0);
    assert(pos >= 0);

    bytepos = pos / 8;
    bitpos = 7 - (pos % 8);
    if (bytepos < array_len) {
        b = 0x01;
        b = b << bitpos;
        // out << "(" << dec << bytepos << "," << bitpos << "," << hex << (short)b << ") ";
        return ((array[bytepos] & b) == b);
    } else {
        return false;
    }

}


void DESCipher::permutate
        (
                const byte *p,
                int p_len,
                const byte *in_array,
                int in_len,
                byte *out_array,
                int out_len
        ) const {
    int i;

    assert(in_len > 0);
    // p_len gibt an, dass 0,1,...,p_len-1 Bits ab out_array
    // gesetzt werden. Dafür müssen mindestens out_len*8 Bit
    // ab out_array reserviert worden sein.
    assert(p_len <= out_len * 8);

    // setzt das 0,1,...,(p_len-1)-te Bit in out_array auf das
    // (p[i]-1)-te Bit des in_array
    for (i = 0; i < p_len; i++) {
        setBit(out_array, out_len, i, getBit(in_array, in_len, p[i] - 1));
    }
}


void DESCipher::printBitField(const byte *bytefield, int len, int block_len) const {
    int i, bit_len;

    // bit_len Bits werden ausgegeben
    bit_len = 8 * len;
    for (i = 0; i < bit_len; i++) {
        // getBit liefert das i-te Bit ab Position bytefield zurück
        // Dieses Bit wird auf der Standardausgabe ausgegeben
        cout << getBit(bytefield, len, i);
        // Überprüft, ob noch weitere Bits ausgegeben werden. Falls das
        // der Fall ist und falls seit dem letzten Leerzeichen block_len
        // Bits ausgegeben worden sind, dann wird ein Leerzeichen ausgegeben
        if ((i + 1 < bit_len) && ((i + 1) % block_len == 0)) {
            cout << " ";
        }
    }
}


void DESCipher::processBlock(const byte *in_block, byte *out_block) {
/******************************************************************************
 * Aufgabe 7
 ******************************************************************************/

    // Als Erstes werden die 64 Bit ab in_block mit ip permutiert.
    // Das Ergebnis dieser Permutation wird in tmp gespeichert.
    // Das bedeutet, dass das i = 0,1,...,63-te Bit in tmp auf das
    // (ip[i]-1)-te Bit des in_block gesetzt wird.
    byte tmp[8];
    permutate(ip, 64, in_block, 8, tmp, 8);

    // Danach werden in jedem Rundendurchlauf die 64 bit in zwei 32 Bit Teile eingeteilt.
    // Die ersten 32 Bit in ab *tmp bilden den ersten und rechten Teil,
    // die darauffolgenden 32 Bit den zweiten und linken Teil.
    // Mit diesen zwei Teilen wird die Feistel Funktion aufgerufen.
    // Das Ergebnis der Feistel Funktion wird in *tmp geschrieben und
    // wird dadurch im nächsten Rundendurchlauf weiterarbeitet.
    for (int i = 0; i < 16; i++) {
        // Die Feistelausgabe wird in einem neuen Speicherblock
        // gespeichert
        byte t[8];
        feistel(tmp, tmp+4, key_schedule[i], t, t+4, i);
        // und wird nach Feistel für den nächsten Schleifendurchlauf
        // wieder in tmp geschrieben
        for (int j = 0; j < 8; j++) {
            tmp[j] = t[j];
        }
    }

    // Im letzten Schleifendurchlauf wird der rechte und linke Teil
    // vertauscht. In DES findet dieser Tausch nicht statt. Aus
    // diesem Grund machen wir den Tausch des letzten Schleifendurchlaufs
    // wieder rückgängig. Dieses Ergebnis wird in roundResult geschrieben.
    byte roundResult[8];
    // Der Reihe nach werden die zwei 32 Bit Teile in einen 64 Bit Block
    // geschrieben
    for(int i = 0; i < 4; i++) {
        roundResult[i] = tmp[i+4];
        roundResult[i+4] = tmp[i];
    }

    // Abschließend werden die 64 Bit in roundResult mit der fp Tabelle
    // permutiert.
    // Das bedeutet, dass das i = 0,1,...,63-te Bit in out_block auf das
    // (fp[i]-1)-te Bit des roundResult gesetzt wird.
    permutate(fp, 64, roundResult, 8, out_block, 8);
}


void DESCipher::setBit
        (
                byte *array,
                int array_len,
                int pos,
                bool value
        ) const {
    int bytepos, bitpos;
    byte b;

    assert(array_len > 0);
    assert(pos >= 0);
    assert(pos < 8 * array_len);

    bytepos = pos / 8;
    bitpos = 7 - (pos % 8);
    b = 0x01;
    b = b << bitpos;
    //cout << "(" << dec << bytepos << "," << bitpos << "," << hex << (short)b << ") ";
    if (value == true) {
        array[bytepos] |= b;
    } else {
        b = b ^ 0xff;
        array[bytepos] &= b;
    }

}

// unrolls key_schedule into (param) key_schedule2
// Die 16 6-Byte großen Rundenschlüssel in der key_schedule der DESCipher Klasse
// werden hintereinander in der durch key_schedule2 spezifizierten Position abgespeichert.
void DESCipher::getKeySchedule(byte *key_schedule2) {
    for (size_t i = 0; i < 16; i++) {
        for (size_t j = 0; j < 6; j++) {
            // Ab keySchedule muss darum 16*6=96 Byte Speicher reserviert sein.
            key_schedule2[i * 6 + j] = key_schedule[i][j];
        }
    }
}


// #ip
byte DESCipher::ip[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
};


// #fp
byte DESCipher::fp[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
};


// #ev
byte DESCipher::ev[48] = {
        32, 1, 2, 3, 4, 5,
        4, 5, 6, 7, 8, 9,
        8, 9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32, 1
};


// #pc1
byte DESCipher::pc1[] = {
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,

        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4
};

// #pc2
byte DESCipher::pc2[] = {
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
};

// #sbox
byte DESCipher::sbox[8][64] = {
        /* S1 */
        14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,

        /* S2 */
        15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
        3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,

        /* S3 */
        10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
        1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,

        /* S4 */
        7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
        3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,

        /* S5 */
        2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,

        /* S6 */
        12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
        4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,

        /* S7 */
        4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
        6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,

        /* S8 */
        13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
        1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
        2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
};


// #pp
byte DESCipher::pp[32] = {
        16, 7, 20, 21,
        29, 12, 28, 17,
        1, 15, 23, 26,
        5, 18, 31, 10,
        2, 8, 24, 14,
        32, 27, 3, 9,
        19, 13, 30, 6,
        22, 11, 4, 25
};


// #total_rot
byte DESCipher::total_rot[16] = {
        1, 2, 4, 6, 8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 28
};
