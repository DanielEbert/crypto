#ifndef AES_AESMATH_H
#define AES_AESMATH_H

#include <string>
#include <vector>
#include "datatypes.h"

using namespace std;

/**
 * Diese Klasse bietet Hilfsfunktionen für AES an.
 *
 * Im AES Algorithmus basiert die Mathematik auf dem endlichen Körper GF(256).
 * Diese Klasse implementiert Verfahren, um mit diesem Körper zu arbeiten.
 * Elemente von endlichen Körpern können addiert und multipliziert werden.
 * Allerdings unterscheiden sich diese Verfahren von denen für Zahlen.
 * Die Elemente haben ebenfalls ein inverses Element.
 *
 * Ein Körper ist eine algebraische Struktur mit (A, '⊕', '°').
 * (In den Folien ist das Symbole für die Division ° anders, aber das in der
 * Folie verwendete Symbole kann nicht richtig abebildet werden. In dieser
 * Dokumentation wird aus diesem Grund ° für die endlichen Körper Division
 * verwendet.) Der Körper hat die Eigenschaft, dass (A, ⊕) und (A\{0}, °)
 * kommutative Gruppen sind, dass das Distributivgesetz gilt und dass
 * ||A|| < ∞.
 *
 * Die Elemente im Körper werden in einem Byte als 8-Tupel über {0,1} dargestellt.
 * Sie werden als Polynome mit Grad 7 und Koeffizienten in {0,1}
 * interpretiert. Das Element (h,g,f,e,d,c,b,a) ∈ {0,1}^8 steht für das Polynom
 * hx^7 + gx^6 + fx^5 + ex^4 + dx^3 + cx^2 + xb + a. Dabei ist a das
 * niedrigwertigste Bit des Bytes, b das zweit niedrigwertigste, ..., und h
 * das höchstwertigste bit des Bytes, in dem das Polynom gespeichert ist.
 * Das Byte mit Hexadezimalwert 57 (in binary 01010111) steht für das Polynom
 * x^6 + x^4 + x^2 + x + 1. An die Funktionen dieser Klasse werden bytes
 * übergeben. Diese bytes werden wie die hier beschriebenen Polynome
 * interpretiert.
 */
class AESMath {
private:
    /**
    * Lookup Tabelle für log3(i) (über GF(256)). i ist der Index in die Tabelle.
    * Macht die Exponentation exp() mit exp_table wieder rückgängig.
    */
    vector<byte> log_table;
    /**
    * Lookup Tabelle für 3^i (über GF(256)). i ist der Index in die Tabelle.
    */
    vector<byte> exp_table;
    /**
    * Lookup Tabelle für die zwei bijektiven Abbildungen der SBox.
    */
    vector<byte> sbox;
    /**
    * Lookup Tabelle für das invertieren der zwei bijektiven Abbildungen der SBox.
    * Macht die Substitution der sbox wieder rückgängig.
    */
    vector<byte> inv_sbox;


public:
    /**
     * Dieser Konstrukor setzt die Lookup-Tabellen exp_table, log_table, sbox
     * und inv_sbox. Die möglichen Eingabewerte für exp, log, sbox und insbox
     * sind jeweils 256 Elemente groß. Dadurch kann das Ergebnis für jede
     * mögliche Eingabe an exp_table, log_table, sbox und inv_sbox  bereits im
     * Vorfeld berechnet werden.
     *
     * Nachdem diese Lookup Tabellen einmal berechnet wurden, können sie weiter
     * dazu verwendet werden, um Berechnungen in anderen Funktionen dieser
     * Klasse schneller abzuarbeiten.
     */
    AESMath();
    /**
     * Polynomaddition auf dem endlichen Körper GF(256).
     *
     * In der hier verwendeten Polynomialrepräsentation ist die Summe zweier
     * Elemente in GF(256) das Polynom mit den Koeffizienten, die durch die
     * Summe mod 2 der Koeffizienten der Elemente a und b gegeben sind.
     * Dabei ist für jedes i = 0,1,..,7: i-te Bit in a + i-te Bit in b
     * mod 2 ergibt das i-te Bit des Ergebnisses. Diese Rechnung kann als a XOR b
     * implementiert werden.
     *
     * @param a Ein Byte in Polynomialrepräsentation. a ist einer der Summanden.
     * @param b Ein Byte in Polynomialrepräsentation. b ist einer der Summanden.
     * @return Das Ergebnis der Polynomaddition von a ⊕ b als Byte in
     * Polynomialrepräsentation.
     */
    static byte add(byte a, byte b);
    /**
    * exp berechnet 3^i (über GF(256)).
    * 3 ist ein Generator von GF(256).
    *
    * @param i Exponent als Byte in Polynomialrepräsentation.
    * @param return Das Ergebnis von 3^i, 
    * als Byte in Polynomialrepräsentation.
    */
    byte exp(byte i) const;
    /**
    * inv berechnet das multiplikative Inverse von b.
    * b ist ein Element in GF(256). Das Ergebnis ist ebenfalls ein Element
    * in GF(256).
    * Bei der Eingabe 0 wird 0 als Byte zurückgegeben.
    *
    * @param b Zu invertierendes Byte in Polynomialrepräsentation.
    * @param return Das Inverse Element von b als Byte in Polynomialrepräsentation.
    */
    byte inv(byte b) const;
    /**
    * log berechnet log3(i) (über GF(256)).
    * 3 ist ein Generator von GF(256).
    *
    * @param i Zu berechnendes Byte in Polynomialrepräsentation.
    * @param return Das Ergebnis von 3^i, 
    * als Byte in Polynomialrepräsentation.
    */
    byte log(byte i) const;
    /**
     * Polynommultiplikation auf dem endlichen Körper GF(256).
     *
     * Da in AES der endliche Körpert GF(256) fest gewählt ist, kann die
     * Multiplikation effizient durch die im Konstruktur berechneten
     * Lookup Tabellen implementiert werden. Da (GF(256)\{0}, °) eine zyklische
     * Gruppe ist, besitzt sie ein erzeugendes Element g. In diesem Fall ist
     * g = 3. Um a°b zu berechnen, verwendet man den Generator g.
     * Falls a=g^i und b=g^j ist, dann ist a°b = g^i ° g^j = g^((i+j) mod 255).
     *
     * @param a Ein Byte in Polynomialrepräsentation. a ist der Multiplikator.
     * @param b Ein Byte in Polynomialrepräsentation. b ist der Multiplikand.
     * @return Das Ergebnis der Polynommultiplikation von a ° b als Byte in
     * Polynomialrepräsentation.
     */
    byte mul(byte a, byte b) const;

    /**
     * sBox für die Substitution.
     *
     * sBox wendet zwei bijektive Abbildungen auf b an und liefert das Ergebnis
     * als byte zurück. Als erstes wird dabei b auf sein multiplikatives
     * Inverses abgebildet. Zweitens wird das im ersten Schritt
     * berechnete multiplikative Inverse von b mit einer in atrans berechneten
     * affinen Abbildung über GF(2) abgebildet. In dieser Implemenation werden
     * Lookup Tabellen verwendet, um dieses Prozess zu beschläunigen.
     *
     * @param b Eingabebyte, auf welches die zwei bijektive Abbildungen
     * angewendet werden.
     * @return Ergebnis als byte der zwei auf b angewendeten bijektive
     * Abbildungen.
     */
    byte sBox(byte b) const;

    /**
     * invSBox um die Substitution der sBox rückgängig zu machen.
     * Zum Beispiel: Angenommen sBox(1) = 10, dann ist invSBox(10) = 1.
     * Dafür wird zuerst die affine Transformation atrans invertiert und
     * danach das multiplikative Inverse in GF(256) des Ergebnisses der
     * invertierten affinen Transformation berechnet und zurückgegeben.
     * In dieser Implementation sind diese Werte bereits im inv_sbox Table
     * gecached, wo sie über den sbox Table berechnet worden sind.
     *
     * @param b Eingabebyte, auf welches die zwei bijektive Abbildungen
     * angewendet werden.
     * @return Ergebnis als byte der zwei auf b angewendeten bijektive
     * Abbildungen.
     */
    byte invSBox(byte b) const;

    /**
     * atrans implementiert eine affine Transformation über GF(2).
     * Diese Transformation wird als Substitution in ByteSub von AES verwendet
     * und ist umkehrbar. Eine vorgegebene 8x8 Matrix wird wird mit Hilfe der
     * Matrixmultiplikation mit dem Übergabeparameter x multipliziert und
     * anschließend mit einem weiteren 8 Elemente großen Vektor additiert.
     * Wie erläutert werden diese Operationen über dem Körper GF(2) ausgeführt.
     *
     * Diese Funktion wird bei der Berechnung der S-box im Konstruktor dieser
     * Klasse verwendet. Die S-Box wird wiederum bei der ByteSub Transformation
     * in AES verwendet.
     *
     * @param x Eingabebyte, auf welches die affine Transformation angewendet
     * wird.
     * @return Ergebnis als Byte der affinen Transformation mit x
     */
    static byte atrans(byte x);

    /**
     * Russische Bauernmultiplikation über GF(256).
     * 
     * Die Russische Bauernmultiplikation berechnet das Produkt zweier
     * ganzer Zahlen a und b über GF(256).
     * 
     * @param a Ein Byte in Polynomialrepräsentation. a ist der Multiplikator.
     * @param b Ein Byte in Polynomialrepräsentation. b ist der Multiplikand.
     * @return Das Ergebnis der Polynommultiplikation von a ° b als Byte in
     * Polynomialrepräsentation.
     */
    static byte rpmul(byte a, byte b);
    /**
     * Die Parity-Funktion berechnet die Anzahl der Einsen in der
     * Binärdarstellung eines Bytes und gibt an, ob diese gerade
     * oder ungerade ist. Ist sie ungerade, dann wird 1 zurückgegeben. Andernfalls
     * wird 0 zurückgegeben.
     *
     * Diese Funktion wird als Hilfsfunktion in der Berechnung der
     * linearen Transformation verwendet.
     *
     * @param b Das Byte, welches nach einer geraden Anzahl von Einsen
     * überprüft wird.
     * @return 1, wenn die Anzahl der Eines in b ungerade ist. 0 wenn nicht.
     */
    static byte parity(byte b);
    static void printTable(const vector<byte>& table);
    /**
    * xtime multipliziert ein Element aus GF(256) mit 2 (über GF(256)) und
    * liefert das Ergebnis, welches ebenfalls ein Element aus GF(256) ist,
    * als Byte in Polynomialrepräsentation zurück.
    * 
    * @param b Das zu multiplizierende Element aus GF(256) als Byte in Polynomialrepräsentation.
    * @param return 2°b, ein Element aus GF(256) als Byte in Polynomialrepräsentation.
    */ 
    static byte xtime(byte b);

    static string format(byte b);

};


#endif //AES_AESMATH_H
