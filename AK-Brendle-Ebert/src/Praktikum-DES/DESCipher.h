#ifndef _DESCipher_h_
#define _DESCipher_h_

#include "BlockCipher.h"

class DESCipher : public BlockCipher {
private:
    /**
     * ip wird am Anfang der processBlock Funktion für die
     * Permutation des in_block verwendet.
     */
    static byte ip[64];

    /**
     * fp wird am Ende der processBlock Funktion für die
     * Permutation des Ergebnis der Rundendurchläufe verwendet.
     */
    static byte fp[64];

    /**
     * ev wird am Anfang der functionF Funktion für die Expansion
     * und Permutation des r_in verwendet.
     */
    static byte ev[48];

    /**
     * pc1 wird in der computeKeySchedule Funktion verwendet, um
     * für jedes Bit von jedem Rundenschlüssel zu berechnen, welches
     * Bit des Rundenschlüssels auf welches Bit des key gesetzt werden
     * soll.
     */
    static byte pc1[];

    /**
     * pc2 wird wie pc1 in der computeKeySchedule Funktion verwendet, um
     * für jedes Bit von jedem Rundenschlüssel zu berechnen, welches
     * Bit des Rundenschlüssels auf welches Bit des key gesetzt werden
     * soll.
     */
    static byte pc2[];

    /**
     * Bei sbox handelt es sich um 8 64-Bit Felder bzw. 8 4x16 Matrizen.
     * Das sind die S-Boxen, welche über die computeSBox Funktion für
     * die Permutationen in der f Funktion (functionF) verwendet werden.
     */
    static byte sbox[8][64];

    /**
     * pp wird abschließend in der f Funktion für eine Permutation mit
     * dem Ergebnis der S-Boxen verwendet. Das Ergebnis dieser
     * Permutation wird dann über r_out zurückgeliefert.
     */
    static byte pp[32];

    /**
     * total_rot gibt die Anzahl der zyklischen Linksverschiebungen
     * für jeden Rundenschlüssel an. Das wird in der computeKeySchedule
     * Funktion verwendet.
     */
    static byte total_rot[16];

    /**
     * In key_schedule werden die 16 6-Byte großen Rundenschlüssel
     * gespeichert. Die Felder werden von der computeKeySchedule
     * gesetzt. Jeder Rundenschlüssel wird der Reihe nach über die
     * feistel Funktion an die f Funktion gegeben, um dort zusammen mit dem
     * r_in Zustand verarbeitet zu werden.
     *
     * Das 2D Array kann mit der getKeySchedule Funktion ausgegeben werden.
     */
    byte key_schedule[16][6];

public:
    /**
     * Leerer Konstruktor für die DESCipher Klasse
     */
    DESCipher();

    /**
     * Leerer Destruktor für die DESCipher Klasse
     */
    ~DESCipher();

    /**
     * Berechnet die Rundenschlüssel und speichert diese in key_schedule
     * ab. Es werden 16 Rundenschlüsselt berechnet, wobei jeder
     * Rundenschlüssel 48 Bit groß ist.
     *
     * @param key ein konstanter byte Zeiger. Dabei handelt es sich um
     * den 64 Bit großen Schlüssel.
     *
     * @param encmode gibt an, ob die Rundenschlüssel für das
     * Verschlüsseln (encmode=true) oder das Entschlüsseln (encmode=false)
     * erstellt werden sollen. encmode ist standardmäßig auf true gesetzt.
     */
    void computeKeySchedule(const byte *key, bool encmode=true);

    /**
     * computeSBox liefert ein Arrayelement zurück. Bei dem
     * Array handelt es sich um das sbox 2D Array. Das Element wird über
     * id, line und col bestimmt.
     *
     * DES verwendet 8 S-Boxen für Permutationen. Jede S-Box ist eine
     * nicht lineare Abbildung von {0,1}⁶ nach {0,1}⁴. Das bedeutet, dass
     * die S-Box einen 6 Bit Wert auf einen 4 Bit Wert abbildet/zuweist.
     * Der param id gibt an, welche der 8 S-Boxen verwendet werden soll.
     *
     * Es gibt 64 verschiedene Werte für {0,1}⁶. Jede S-Box ist deshalb
     * ein 64 Elemente großes Array. Jedes Element ist ein Byte. Da auf
     * {0,1}⁴ abgebildet wird, werden nur die unteren 4 Bits des Bytes
     * gesetzt. Die S-Box kann auch als 4x16 Matrix dargestellt werden.
     * Die ersten 16 Elemente des 64 Elementearrays bilden die erste
     * Zeile der 4x16 Matrix, die darauffolgenden 16 Elemente an
     * Index 16 bis Index 31 des 64 Elementearrays bilden die zweite
     * Zeile der 4x16 Matrix und so weiter.
     * Die param line, die Zeile, und param col, die Spalte, bestimmen
     * das Element Matrix bzw. des 64 Elementearrays.
     *
     * @param id gibt an, welche SBox bzw. welches Array für die Berechnung
     * verwendet werden soll. Es gibt 8 verschiedene SBoxen/Arrays in sbox.
     * Valide Werte von id sind also 0,1,...,7.
     *
     * @param line gibt die Zeile der 4x16 S-Box Matrix an. Da es sich
     * intern um Arrays handelt fängt man hier ab Index 0 an. Valide
     * Werte sind deshalb 0,1,2 und 3.
     *
     * @param col gibt die Spalte der 4x16 S-Box Matrix an. Da es sich
     * intern um Arrays handelt fängt man hier ab Index 0 an. Valide
     * Werte sind deshalb 0,1,2,...,15.
     *
     * @return Der Wert eines Elements der S-Boxen. Der Rückgabewert ist
     * vom Typ byte, allerdings werden nur die unteren 4 Bits gesetzt sein.
     */
    byte computeSBox(byte id, byte line, byte col);

    /**
     * decrypt entschlüsselt cipher_len Bytes ab cipher_text
     * mit dem key_len großen Schlüssel key und speichert den
     * entschlüsselten Text im plain_len großen plain_text
     * Speicherblock ab.
     *
     * @param cipher_text zeigt auf einen Speicherblock, in dem der
     * mindestens cipher_len Byte große verschlüsselte zu entschlüsselnde Text
     * gespeichert ist.
     *
     * @param cipher_len gibt die Größe des cipher_text in Byte an. cipher_len
     * muss ein vielfaches von der durch BlockCipher spezifizierten
     * block_len sein.
     *
     * @param key zeigt auf einen Speicherblock, in dem der 8 Byte
     * große Key gespeichert ist. Dieser Key wird für das entschlüsseln
     * verwendet. Speziell werden aus diesem Key die 16 Rundenschlüssel
     * berechnet.
     *
     * @param key_len gibt die Größe des key in Byte an. key_len muss
     * gleich 8 sein.
     *
     * @param plain_text zeigt auf einen mindestens plain_len großen
     * Speicherblock, in dem der entschlüsselte cipher_text gespeichert
     * wird.
     *
     * @param plain_len gibt die Größe des plain_text in Byte an.
     * Um das komplette Ergebnis zu speichern muss plain_len
     * größer als cipher_len sein.
     *
     * @return den Wert in (param) plain_len
     */
    virtual int decrypt(const byte* cipher_text, int cipher_len,
                      const byte* key, int key_len,
                      byte* plain_text, int plain_len);

    /**
     * encrypt verschlüsselt plain_len Bytes ab plain_text
     * mit dem key_len großen Schlüssel key und speichert den
     * verschlüsselten Text im cipher_len großen cipher_text
     * Speicherblock ab.
     *
     * @param plain_text zeigt auf einen Speicherblock, in dem der
     * mindestens plain_len Byte große zu verschlüsselnde Text
     * gespeichert ist.
     *
     * @param plain_len gibt die Größe des plain_text in Byte an. plain_len
     * muss ein vielfaches von der durch BlockCipher spezifizierten
     * block_len sein.
     *
     * @param key zeigt auf einen Speicherblock, in dem der 8 Byte
     * große Key gespeichert ist. Dieser Key wird für das verschlüsseln
     * verwendet. Mit diesem key kann der resultierende cipher_text
     * auch wieder entschlüsselt werden.
     * Speziell werden aus diesem Key die 16 Rundenschlüssel berechnet.
     *
     * @param key_len gibt die Größe des key in Byte an. key_len muss
     * gleich 8 sein.
     *
     * @param cipher_text zeigt auf einen mindestens cipher_len großen
     * Speicherblock, in dem der verschlüsselte plain_text gespeichert
     * wird.
     *
     * @param cipher_len gibt die Größe des cipher_text in Byte an.
     * Um das komplette Ergebnis zu speichern muss cipher_len größer
     * als plain_len sein.
     *
     * @return den Wert in (param) plain_len
     */
    virtual int encrypt(const byte* plain_text, int plain_len,
                      const byte* key, int key_len,
                      byte* cipher_text, int cipher_len);


    /**
     * Chiffriert block_len bytes ab in_block unter Verwendung der
     * Rundenschlüssel in der Schlüsseltabelle key_schedule mittels DES
     * Das Ergebnis wird in den block_len bytes ab out_block gespeichert.
     * Als Betriebsmodus wird der ECB-Modus verwendet.
     *
     * @param in_block zeigt auf die block_len Bytes, welche chiffriert
     * werden sollen
     *
     * @param out_block zeigt auf einen Speicherbereich, der mindestens
     * out_block Bytes groß sein muss und in welchen das chiffrierte
     * Ergebnis geschrieben wird.
     */
    void processBlock(const byte* in_block, byte* out_block);

    /**
     * Implementation der Feistel-Chiffre.
     * Dabei wird l_out auf r_in gesetzt. Weiter wird die
     * f Funktion mit r_in und key aufgerufen und die
     * Ausgabe der Funktion wird mit l_in XOR'd und in
     * r_out geschrieben.
     *
     * @param l_in zeigt auf den 32 Bit großen linken Teil.
     *
     * @param r_in zeigt auf den 32 Bit großen rechten Teil.
     *
     * @param key zeigt auf einen 48 Bit großen Rundenschlüssel.
     *
     * @param l_out In l_out wird der berechnete linke Teil
     * geschrieben.
     *
     * @param r_out In r_out wird der berechnete rechte Teil
     * geschrieben.
     */
    void feistel(const byte* l_in,
               const byte* r_in,
               const byte* key,
               byte* l_out,
               byte* r_out,
               int rnd=0);

    /**
     * Implementation der DES-Funktion f. Sie ist eine
     * beliebige injektive Abbildung von
     * {0, 1}^(32) × {0, 1}^(48) nach {0, 1}^(32).
     * Dabei sind r_in und key die Eingaben und r_out die
     * Ausgabe der Funktion.
     *
     * @param r_in zeigt auf einen 32 Bit großen Speicherblock.
     * Diese 32 Bit werden mit dem Rundenschlüssel key verarbeitet.
     *
     * @param key zeigt auf einen 48 Bit großen Rundenschlüssel.
     *
     * @param r_out In r_out wird die 32 Bit Ausgabe der f Funktion
     * geschrieben.
     *
     * @param rnd ist optional, gibt die aktuelle DES Runde an und
     * wird in der aktuellen Implementation nicht verwendet. Sie kann
     * in der Zukunft für Debug Ausgaben verwendet werden.
     */
    void functionF(const byte* r_in,
                 const byte* key,
                 byte* r_out,
                 int rnd=0);

    /**
     * Liefert das pos-te Bit des array zurück.
     *
     * @param array verweist auf einen Speicherblock.
     *
     * @param array_len gibt die Größe des array in Byte an.
     *
     * @param pos gibt an, dass das pos-te Bit zurückgegeben werden soll.
     *
     * @return den Wert des pos-ten Bits des array.
     */
    bool getBit(const byte* array, int array_len, int pos) const;

    /**
     * p ist ein p_len großes Array mit Integern. Die Bits in in_array
     * werden an die durch den Wert der Elemente in p bestimmen Position
     * ins out_array geschrieben.
     * Das bedeutet, dass das i-te Bit in in_array an das (p[i]-1)-te Bit
     * in out_array geschrieben, wobei i = 0,1,...,p_len-1.
     * Die Indices in p starten bei 1 und nicht bei 0.
     *
     * @param p zeigt auf ein p_len byte großes Array, wobei jedes Element
     * des Arrays eine Zuordnung eines Bits auf in_array zu out_array
     * bestimmt.
     *
     * @param p_len gibt die Größe in byte des p Arrays an.
     *
     * @param in_array zeigt auf ein in_len byte großes Array. Bits aus
     * in_array werden später an durch p beschriebene Positionen in das
     * out_array geschrieben. Es gilt zu beachten, dass möglicherweise
     * keine, manche oder alle Bits aus in_array an Positionen in
     * out_array kopiert werden und dass auch manche Bits mehrmals
     * in out_array kopiert werden können.
     *
     * @param in_len gibt die Größe in bytes des in_array an.
     *
     * @param out_array zeigt auf ein out_len byte großes Array.
     * Die out_len*8 Bits ab out_array werden auf den Wert von Bits
     * des in_array gesetzt.
     *
     * @param out_len gibt die Größe in bytes des out_array an.
     */
    void permutate(const byte* p, int p_len,
                 const byte* in_array, int in_len,
                 byte* out_array, int out_len) const;

    /**
     * printBitField gibt 8*len Bit ab bytefield als Bitfolge auf der
     * Standardausgabe aus.
     * Nach jeweils block_len bits wird ein Leerzeichen ausgegeben, außer
     * auf das Leerzeichen folgt kein weiteres Bit (no trailing whitespace).
     *
     * @param bytefield zeigt auf eine Position im Speicher, ab der len
     * Bytes auf der Standardausgabe ausgegeben werden.
     *
     * @param len gibt die Anzahl der Byte an
     *
     * @param block_len gibt an, nach jeweils wievielen ausgegebenen Bits
     * ein Leerzeichen ausgegeben werden soll. block_len ist standardmäßig
     * auf 8 gesetzt.
     */
    void printBitField(const byte* bytefield, int len, int block_len=8) const;

    /**
     * setBit setzt das pos-te Bit in array auf den Wert von value.
     *
     * @param array zeigt auf ein array_len Byte großes Feld.
     *
     * @param array_len gibt die Größe des array in Bytes an. Damit das
     * Bit gesetzt werden kann, muss pos < 8*array_len sein.
     *
     * @param pos gibt an, dass das pos-te Bit ab array gesetzt werden soll.
     *
     * @param value Das pos-te Bit wird auf value gesetzt.
     */
    void setBit(byte* array, int array_len, int pos, bool value) const;

    /**
     * getKeySchedule speichert die 16 6-Byte großen Rundenschlüssel
     * hintereinander in der durch keySchedule gezeigten Position ab.
     * Ab keySchedule muss darum 16*6=96 Byte Speicher reserviert sein.
     *
     * @keySchedule Zeigt auf einen mindestens 96 Byte großen Speicherblock.
     * In diesen Speicherblock werden die Rundenschlüssel geschrieben.
     */
    void getKeySchedule( byte* keySchedule );

};

#endif
