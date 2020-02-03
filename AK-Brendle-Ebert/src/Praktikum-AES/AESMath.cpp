#include <sstream>
#include <iostream>
#include <iomanip>
#include "AESMath.h"

using namespace std;

AESMath::AESMath() : exp_table(), log_table(256, 0), sbox(), inv_sbox(256, 0) {

    /*
     * Aufgabe 5a
     */
    
    // g = 3 ist ein erzeugendes Element aus GF(256)
    // g steht für dieses erzeugendes Element 3 bzw. den Generator 3

    // Lookup Tabelle für g^i für alle Elemente i aus GF(256).
    // i ist die Position in der Tabelle. Für jeden key i ist
    // exp_table[i] = g^i
    byte a = 1;
    byte b;
    
    // Der Fall 3^0 = 1 wird an die Position 0 des exp_table gepushed.
    exp_table.push_back(a);

    // Die Fälle 3^1 bis 3^255 werden der Lookup Tabelle hinzugefügt.
    // Für jedes i ist 3^i = 3^(i-1)*3. Dadurch kann das Ergebnis 
    // des letzten Schleifendurchlaufs weiterverwendet werden.
    for (int i = 1; i < 256; i++) {
        // b = g^i
        b = rpmul(a,3);
        // b wird an Position i im exp_table gesetzt
        exp_table.push_back(b);
        // im nächsten Schleifendurchlauf ist a = 3^(i-1).
        a = b;
    }
    // Da der Generator 3 als Basis für exp verwendet wird, sind die
    // 256 Werte in exp_table unique.

    /*
     * Aufgabe 5b
     */

    // Lookup Tabelle für log3(i) für alle Elemente i aus GF(256).
    // i ist die Position in der Tabelle. Für jeden Key i ist
    // log_table[i] = log3(i)
    for (int i = 0; i < 256; i++) {
        // log3(3^i) ist gleich i. log ist die inverse Operation von exp.
        // Da der Generator 3 für exp und log verwendet wird, sind die
        // 256 Werte in exp_table unique. Dadurch werden auch die 256 Werte
        // in log_table unique.
        log_table[exp_table[i]] = i;
    }


    /*
     * Aufgabe 9a
     */

    // i steht für den key der sbox. Die value der sbox für key i ist eine
    // Komposition aus zwei bijektiven Abbildungen. Die sbox implementiert
    // eine Substitution.
    // Als erstes wird dabei i auf sein multiplikatives Inverses abgebildet.
    // Das wird von inv(i) implementiert. Zweitens wird das im ersten Schritt
    // berechnete multiplikative Inverse von i mit einer in atrans implementierten
    // affinen Abbildung über GF(2) abgebildet.
    // Da der key im Wertebereich 0 bis 255 liegt, können die 256 Ergebnisse
    // der 256 möglichen Eingaben an die sbox im Vorfeld berechnet, in einer
    // Lookup Tabelle sbox gespeichert und über die sBox() Funktion im weiteren
    // Programmverlauf verwendet werden.
    // Die sbox wird als Substitution in ByteSub von AES bei der Verschlüsselung
    // verwendet.
    for(int i = 0; i < 256; i++) {
        sbox.push_back(atrans(inv(i)));
    }

    /*
     * Aufgabe 9b
     */

    // inv_sbox macht die Substitution der sbox rückgängig. inv_sbox wird
    // bei der Entschlüsselung verwendet.
    // Zum Beispiel: Angenommen sbox[1] = 10, dann ist inv_sbox[10] = 1.
    // Wie bei der sbox können die 256 Ergebnisse
    // der 256 möglichen Eingaben an die inv_sbox im vorfeld berechnet, in einer
    // Lookup Tabelle inv_sbox gespeichert und über die invSBox() Funktion im weiteren
    // Programmverlauf verwendet werden.
    for (int i = 0; i < 256; i++) {
        inv_sbox[sbox[i]] = i;
    }
}

byte AESMath::add(byte a, byte b) {
    /*
     * Aufgabe 2
     */
    // Für jedes i = 0,1,..,7: das i-te Bit in a plus das i-te Bit in b
    // mod 2 ergibt das i-te Bit des Rückgabebytes.
    // Diese Rechnung wird als a XOR b implementiert.
    return a ^ b;
}

byte AESMath::atrans(byte x) {
    /*
     * Aufgabe 8
     */

    /*
     * atrans berechnet folgende affine transformation über GF(2):
     *  [y0]   [1 1 1 1 1 0 0 0]   [x0]   [1]
     *  [y1]   [0 1 1 1 1 1 0 0]   [x1]   [1]
     *  [y2]   [0 0 1 1 1 1 1 0]   [x2]   [0]
     *  [y3] = [0 0 0 1 1 1 1 1] ° [x3] ⊕ [0]
     *  [y4]   [1 0 0 0 1 1 1 1]   [x4]   [0]
     *  [y5]   [1 1 0 0 0 1 1 1]   [x5]   [1]
     *  [y6]   [1 1 1 0 0 0 1 1]   [x6]   [1]
     *  [y7]   [1 1 1 1 0 0 0 1]   [x7]   [0]
     *
     *  wobei x0 bis x7 die 8 Bits der Parameters x sind und y0 bis y7 die
     *  8 Bits des Byte Rückgabewerts sind.
     */

    // v ist eine Version der 8x8 Matrix:
    // 1 1 1 1 1 0 0 0
    // 0 1 1 1 1 1 0 0
    // 0 0 1 1 1 1 1 0
    // 0 0 0 1 1 1 1 1
    // 1 0 0 0 1 1 1 1
    // 1 1 0 0 0 1 1 1
    // 1 1 1 0 0 0 1 1
    // 1 1 1 1 0 0 0 1
    //, wobei das erste Element in v die erste Zeile darstellt. Dabei werden
    // die Spalten dieser Zeile als Bits interpretiert. Die Spalten einer Zeile
    // zusammen ergeben ein Byte. Für jede Zeile i in 0,1,2,..,7 steht v[i]
    // für dieses Byte.
    // Diese Matrix ist im Standard vorgegeben.
    vector<byte> v = { 248, 124, 62, 31, 143, 199, 227, 241 };

    // v2 ist der 8 Elemente großer Vektor, welcher auf das Ergebnis der
    // Matrixmultiplikation von x und v addiert wird und
    // danach zurückgegeben wird.
    // Dieser Vektor ist im Standard vorgegeben.
    vector<byte> v2 = { 0, 1, 1, 0, 0, 0, 1, 1 };
    // In ret wird das Byte gespeichert, welches zurückgegeben wird.
    byte ret = 0;

    // Die nachfolgenden Schleifen implementieren die Matrixmultiplikation von
    // x mit v und die darauffolgende Addition, wobei diese Berechnungen über
    // GF(2) sind.

    // Für jede i-te Zeile in i = 7,6,5,...,0
    for (int i = 7; i >= 0; i--) {
        // parity(x & v[i]) implementiert einen Teil der Matrixmultiplikation,
        // einer Zeile über GF(2). Speziell wird in jedem Schleifendurchlauf
        // ein Bit des Outputbytes berechnet.
        // Bei der Matrixmultiplikation einer Zeile mit x wird jedes Bit der
        // Zeile mit jedem Bit in x multipliziert. Das wird von x & v[i]
        // implementiert. Weiter werden diese 8 Zwischenergebnisse über GF(2)
        // addiert. Dabei wird der Reihe nach jedes Bit addiert und mod 2 (wegen der Berechnung über
        // GF(2)) gerechnet. Das ist das selbe wie zu überprüfen, ob die Anzahl
        // der Eines im Ergebnis gerade oder ungerade ist.
        // Das wird von parity() implementiert.
        // Dieses (Ergebnis + v2[i]) % 2 implementiert die Addition über GF(2)
        if ( (parity(x & v[i]) + v2[i]) % 2 ) {
            // Das jeweilige Bit des Outputbytes wird je nach i, also je nachdem
            // welche Zeile aktuell in der Berechnung bearbeitet wird, gesetzt,
            // wenn die Matrixmultiplikation mit x und Addition mit v2[i] der
            // aktuellen Zeile gleich 1 ist.
            ret += 1 << (7-i);
        }
    }

    return ret;
}

byte AESMath::exp(byte i) const {
    /*
     * Aufgabe 5c
     */
    // exp_table ist eine Lookup Tabelle für 3^i. An Index i in der Tabelle steht
    // das Ergebnis für 3^i. Die Lookup Table wurde im Konstruktor dieser Klasse
    // berechnet.
    return exp_table[i];
}

byte AESMath::inv(byte b) const {
    /*
     * Aufgabe 6
     */
    // Bei Eingabe 0 wird ebenfalls 0 zurückgeliefert.
    if (b == 0) return 0;

    // Gesucht ist das ? in: 3^i ° ? = 3^0 (über GF(256)), wobei b=3^i.
    // log(b) liefert das i.
    // Da '?' ein Element aus GF(256) ist, kann es auch durch den Generator 
    // 3 mit 3^j erzeugt werden.
    // Für das Inverse muss gelten, dass i+j = 0 (mod 255) ist. 
    // Umgestellt ist das j = -i (mod 255). Das kann zu 255-i umgeschrieben werden.
    // Dadurch ist 3^(255-i) das Ergebnis.
    return exp(255-log(b));
}

byte AESMath::log(byte b) const {
    /*
     * Aufgabe 5d
     */
    // log_table ist eine Lookup Tabelle für log3(i). An Index i in der Tabelle steht
    // das Ergebnis für log3(i). Die Lookup Table wurde im Konstruktor dieser Klasse
    // berechnet.
  return log_table[b];
}

byte AESMath::rpmul(byte a, byte b) {
    /*
     * Aufgabe 4
     */
    byte p = 0;
    while (a > 0) {
        // wenn aktuell niedrigstes Bit in a auf 1 ist
        if (a % 2 == 1) {
            // wird p mit b XOR gerechnet.
            // das XOR ist die Addition von p und b über GF(256)
            p = p ^ b;
        }
        // in jedem Schleifendurchlauf wird b mit xtime verdoppelt.
        b = xtime(b);
        // und a wird um 1 Bit nach rechts geshiftet.
        a = a >> 1;
    }
    return p;
}

byte AESMath::mul(byte a, byte b) const {
    /*
     * Aufgabe 5e
     */
    // Die Polynommultiplikation mit 0 ergibt ebenfalls 0.
    if (a == 0 || b == 0) return 0;

    /*
     * g steht für den Generator des endlichen Körpers GF(256) und ist gleich 3.
     * Um a°b = g^i ° g^j = g^((i+j) mod 255) zu berechnen, muss als erstes
     * das i in g^i für a und das j in g^j für b berechnet werden. Dafür wird
     * die Funktionn log() verwendet, welche die im Konstruktor berechnete
     * Lookup Tabelle log verwendet. log liefert das i für log(a) und das j
     * für log(b) zurück. Danach kann g^((i+j) mod 255) berechnet werden.
     * Sowohl g, als auch das Ergebnis von ((i+j) mod 255) ist bekannt. Jetzt
     * muss nur noch die "Polynomexponentation" angewendet werden, welches von
     * der Funktion exp() implementiert wird. exp() liefert das Ergebnis von
     * g^((i+j) mod 255) zurück und verwendet die im Konstruktor berechnete
     * LookupTabelle exp.
     */
    return exp( (log(a) + log(b)) % 255 );
}

byte AESMath::parity(byte b) {
    /*
     * Aufgabe 7
     */
    // cnt zählt die Anzahl der bisher gefundenen Einsen im Byte
    byte cnt = 0;
    // In jedem Schleifendurchlauf wird 1 Bit des Bytes überprüft
    for (int i = 0; i < 8; i++) {
        // b wird mit einer Bitmask nach != 0 überprüft. Die Bitmask hat 1 Bit
        // auf 1 gesetzt, die anderen Bits des Bytes sind 0. Wenn das Bit in b
        // an der gleichen Position wie in der Bitmask auf 1 ist, dann ist
        // != 0 Wahr und der counter cnt wird um 1 erhöht.
        if ((b & (1 << i)) != 0) cnt++;
    }
    // cnt % 2 liefert 0 zurück, wenn die Anzahl der Eines in b ungerade ist.
    // Und 0 wenn die Anzahl gerade ist.
    return cnt % 2;
}

void AESMath::printTable(const vector<byte> &table) {
    cout << "         +----------------------------------------";
    cout << "---------------------------------------+" << endl;
    cout << "         |                                       y";
    cout << "                                       |" << endl;
    cout << "         +----+----+----+----+----+----+----+----+";
    cout << "----+----+----+----+----+----+----+----+" << endl;
    cout << "        ";
    for (size_t i = 0; i < 16; i++) {
        cout << " | " << setw(2) << setfill(' ') << hex << i;
    }
    cout << " |" << endl;
    cout << "+---+----+----+----+----+----+----+----+----+----+";
    cout << "----+----+----+----+----+----+----+----+" << endl;
    for (size_t i = 0; i < 16; i++) {
        cout << "|   | " << setw(2) << setfill(' ') << hex << i;
        for (size_t j = 0; j < 16; j++) {
            cout << " | " << setw(2) << setfill('0') << (unsigned) table[(i << 4) + j];
        }
        cout << " |" << endl;
        if (i < 15) {
            if (i == 7) {
                cout << "| x ";
                cout << "+----+----+----+----+----+----+----+----+----+";
                cout << "----+----+----+----+----+----+----+----+" << endl;

            } else {
                cout << "|   ";
                cout << "+----+----+----+----+----+----+----+----+----+";
                cout << "----+----+----+----+----+----+----+----+" << endl;
            }
        } else {
            cout << "|---";
            cout << "+----+----+----+----+----+----+----+----+----+";
            cout << "----+----+----+----+----+----+----+----+" << endl;
        }
    }
}

byte AESMath::sBox(byte b) const {
    /*
     * Aufgabe 9c
     */
    // sbox ist eine Lookup Tabelle für die zweifache bijektive Abbildung der S-Box. 
    // An Index b in der Tabelle steht das Ergebnis für die S-Box von b. 
    // Die Lookup Tabelle wurde im Konstruktor dieser Klasse
    // berechnet.
    return sbox[b];
}

byte AESMath::invSBox(byte b) const {
    /*
     * Aufgabe 9d
     */
    // inv_sbox ist eine Lookup Tabelle für das Inverse der zweifachen bijektive Abbildung der S-Box. 
    // An Index b in der Tabelle steht das Inverse für die S-Box von b. Die Lookup Table wurde im 
    // Konstruktor dieser Klasse berechnet.
    return inv_sbox[b];
}

byte AESMath::xtime(byte b) {
    /*
     * Aufgabe 3
     */
    byte r = b << 1;
    // wenn das höchstwertigste Bit gesetzt ist, dann muss durch das irreduzible
    // Polynom m(x) = 0x11b subtrahiert werden.
    // Die Subtraktion über GF(256) ist wie die Addition und ebenfalls über XOR
    // implementierbar.
    // In jedem Fall werden die Bits in b um 1 Bit nach links verschoben. 
    // Der 1-fache Linksshift multipliziert b mit 2.
    if ((b & 0x80) == 0x80) {
        r ^= 0x1b;
    }
    return r;
}

string AESMath::format(byte b) {
    ostringstream os;
    os << "{" << hex << setfill('0') << setw(2) << (unsigned)b <<"}";
    return os.str();
}
