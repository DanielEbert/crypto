#include <iostream>
#include <set>
#include "osrng.h"
#include "PublicKeyAlgorithmBox.h"

using namespace std;

void integerExercise() {
	/*********************************************************************
	 * Aufgabe 2.
	 *********************************************************************/

	// Integer der CryptoPP lib werden erzeugt
    Integer a = Integer(23975);
    // Wenn ein Integer auf eine Zahl gesetzt werden soll, die größer ist
    // als die elementaren Datentypen, dann muss die Zahl als String kodiert
    // an den Konstruktor für Integer übergeben werden. Ansonsten kommt es
    // zu einem Überlauf des elementaren Datentyps.
    Integer b = Integer("12345678900987654321");
    // Berechnet 2^10 und speichert das Ergebnis im Integer c ab.
    Integer c = Integer::Power2(10);

    // mit diesen Integern kann wie mit den elementaren Datentypen int und
    // long gerechnet werden.
    cout << "(a + c) * b % 50001 = " << (a + c) * b % 50001 << endl;
}

void eeaExercise() {
	/*********************************************************************
	 * Aufgabe 3.
	 *********************************************************************/

    PublicKeyAlgorithmBox pb;
    Integer d, x, y;
    // Berechnet das d, x und y für verschiedene a und b mit dem Erweiterten
    // Algorithmus von Euklid. a wird an EEA als erstes Argument übergeben
    // und b an EEA als zweites Argument. d, x und y werden nach jeder der drei
    // Beispiele über stdout ausgegeben. d ist der größte Gemeinsame Teiler
    // von a und b. Das x und y ist für die Linearkombination
    // {ax + by | x, y ∈ Z}, wobei a*x + b*y = gcd(a,b).
    pb.EEA(Integer(2987), Integer(1279865), d, x, y);
    cout << "d: " << d << " x: " << x << " y: " << y << endl;
    pb.EEA(Integer(78845945), Integer(2503417846), d, x, y);
    cout << "d: " << d << " x: " << x << " y: " << y << endl;
    pb.EEA(Integer(57913313), Integer(173739939), d, x, y);
    cout << "d: " << d << " x: " << x << " y: " << y << endl;
}

void invExercise() {
	/*********************************************************************
	 * Aufgabe 4.
	 *********************************************************************/

    PublicKeyAlgorithmBox pb;
    Integer a_inv;
    // multInverse berechnet das multiplikative Inverse
    // von 10353 modulo 820343. Falls ein solches existiert, dann wird es in
    // a_inv gespeichert. multInverse liefert auch True zurück, falls ein
    // solches inverses Element existert und False wenn nicht.
    bool returned = pb.multInverse(Integer(10353), Integer(820343), a_inv);
    cout << "multiplikatives Inverses von 10353 modulo 820343: " << a_inv << endl;
    // Es wird erwartet, dass ein solches multiplikative Inverse existiert und
    // dass dieses 21711 ist.
    if (!returned || a_inv != Integer(21711)) {
        cerr << "ERROR: multiplikatives Inverses von 10353 modulo 820343: "
             << a_inv << ", Erwartet: 21711" << endl;
    }
    // Es wird erwartet, dass kein multiplikatives Inverses von
    // 10353 modulo 820344 existiert. In diesem Fall liefert multInverse()
    // False zurück.
    returned = pb.multInverse(Integer(10353), Integer(820344), a_inv);
    if (returned) {
        cerr << "ERROR: multiplikatives Inverses von 10353 modulo 820343: "
             << a_inv
             << ", Erwartet: Es existiert kein multiplikatives Inverses."
             << endl;
    } else {
        cout << "Kein multiplikatives Inverses von 10353 modulo 820344 existiert." << endl;
    }
    // Berechnet das multiplikatives Inverses von 562312 modulo 57913313.
    // Es wird erwartet, dass dieses 53494466 ist.
    returned = pb.multInverse(Integer(562312), Integer(57913313), a_inv);
    cout << "multiplikatives Inverses von 562312 modulo 57913313: " << a_inv << endl;
    if (!returned || a_inv != Integer(53494466)) {
        cerr << "ERROR: multiplikatives Inverses von 562312 modulo 57913313: "
             << a_inv << ", Erwartet: 53494466" << endl;
    }
}

void modexpExercise() {
	/*********************************************************************
	 * Aufgabe 5.
	 *********************************************************************/

	PublicKeyAlgorithmBox pb;
	// modularExponentation berechnet a^b mod n, wobei a als erstes, b als
	// zweites und n als drittes Argument an modularExponentation übergeben
	// werden.
	// Berechnet 2^100000 % 23 und überprüft ob das Ergebnis gleich 12 ist.
	Integer ergebnis = pb.modularExponentation(Integer(2), Integer(100000), Integer(23));
	cout << "2^100000 % 23 = " << ergebnis << endl;
	if (ergebnis != Integer(12)) {
        cerr << "ERROR: 2^100000 % 23 = " << ergebnis << ", Erwartet: 12" << endl;
	}
    // Berechnet 2343947997^765 % 111 und überprüft ob das Ergebnis gleich 105 ist.
    ergebnis = pb.modularExponentation(Integer(2343947997), Integer(765), Integer(111));
    cout << "2343947997^765 % 111 = " << ergebnis << endl;
    if (ergebnis != Integer(105)) {
        cerr << "ERROR: 2343947997^765 % 111 = " << ergebnis << ", Erwartet: 105" << endl;
    }
}

void randExercise() {
	/*********************************************************************
	 * Aufgabe 6.
	 *********************************************************************/

    // Aufgabe 6 b):
    // Bei den Zufallszahlengeneratoren handelt es sich um Pseudozufallszahlen-
    // generatoren. Diese erzeugen scheinbar zufällig aussehende Bitfolgen
    // basierend auf einem Seed. Damit der generierte Bitstream noch mehr
    // zufällig aussieht, sollte dieser Seed immer wieder mal geändert werden.
    // Die Pseudozufallszahlen dev/random und dev/urandom von Linux verwenden
    // dafür einen 'entropy pool'. Wird ein neuer Seed gebraucht, dann kann
    // dieser aus dem pool entnommen werden. Das Betriebssystem fügt dem
    // pool Seeds aus verschiedenen scheinbar zufälligen Quellen wie zum
    // Beispiel die Verzögerung zwischen dem Drücken von Tasten hinzu. Dieser Pool
    // kann auch leer werden. Wenn dieser Pool leer ist, dann blockt ein
    // BlockingRng wie dev/random, also wartet bis er ein Bit zurückgibt,
    // hält praktisch die Ausführung an, bis zusätzliche Werte dem pool
    // hinzugefügt werden. Das Gerät /dev/random ist eine
    // veraltete Schnittstelle, die auf eine Zeit zurückgeht, in der den
    // kryptographischen Primitiven, die bei der Implementierung von
    // /dev/urandom verwendet wurden, kein großes Vertrauen entgegengebracht
    // wurde. Ein NonblockingRng blockt nicht und gibt auch Bits zurück, auch
    // wenn der pool leer ist und er eigentlich neue seeds haben möchte.
    // Mittlerweise wird, außer vielleicht kurz nach dem ersten Booten,
    // den kryptographischen Primitiven wie denen in dev/urandom mehr Vertrauen
    // entgegengebracht, wodurch auch NonblockingRngs für Dinge wie
    // Kryptographie als ausreichend sicher gelten.

    Integer a;
    BlockingRng blockingRng;
    // Randomize wird eine Referenz auf eine Klasse vom Typ
    // RandomNumberGenerator übergeben. BlockingRng und NonblockingRng
    // erben von RandomNumberGenerator und sind somit für Randomize geeignet.
    // Über das zweite Argument wird die maximale Bitgröße der zu
    // erzeugenden Zufallszahl bestimmt. Hier wird also eine 128 Bit
    // Zufallszahl gewürfelt, welche in a gespeichert wird.
    a.Randomize(blockingRng, 128);
    cout << "Erzeuge 128 Bit Zufallszahl: " << a << endl;

    NonblockingRng nonblockingRng;
    // Erzeugen einer 1025 Bit Zufallszahl mit einem nicht
    // blockierenden Pseudozufallszahlengenerator. Die Zufallszahl
    // wird in a abgespeichert.
    a.Randomize(nonblockingRng, 1024);
    cout << "Erzeuge 1024 Bit Zufallszahl: " << a << endl;
}

void millerRabinExercise() {
	/*********************************************************************
	 * Aufgabe 7.
	 *********************************************************************/

	PublicKeyAlgorithmBox pb;
	Integer a = Integer("279226292160650115722581212551219487007");
	// Mit dem Miller Rabin Test wird überprüft, ob a mit
	// Fehlerwahrscheinlichkeit 2^-s eine Primzahl ist.
	// Die zu testende Zahl a wird als erstes Argument an millerRabinTest()
	// übergeben.
	// Als Fehlerwahrscheinlichkeit wird 2^−100 über s, dem zweiten Argument,
	// gewählt.
	// millerRabinTest liefert True zurück, wenn mit der Fehlerwahrscheinlichkeit
	// angenommen wird, dass es sich bei a um eine Primzahl handelt.
	bool ergebnis = pb.millerRabinTest(a, 100);
	cout << "279226292160650115722581212551219487007 ist laut Rabin Miller Test " << (ergebnis ? "Prim" : "nicht Prim") << endl;
	// Es wird erwartet, dass a eine Primzahl ist.
	Integer returned = pb.millerRabinTest(a, 100);
	if (!returned) {
	    cerr << "Es wurde erwartet, dass " << a << " eine Primzahl ist. Mit "
	         << "Fehlerwahrscheinlichkeit 2^-100 wurde sie aber nicht als "
	         << "Primzahl erkannt." << endl;
	}
	Integer b = Integer("247278711133334795867191516244139839983");
    ergebnis = pb.millerRabinTest(b, 100);
    cout << "247278711133334795867191516244139839983 ist laut Rabin Miller Test " << (ergebnis ? "Prim" : "nicht Prim") << endl;
    // Es wird erwartet, dass b keine Primzahl ist.
    if (ergebnis) {
        cerr << "Es wurde erwartet, dass " << b << " keine Primzahl ist." << endl;
    }
    Integer c = Integer("192172622525902080249109244057747132167");
    ergebnis = pb.millerRabinTest(c, 100);
    cout << "192172622525902080249109244057747132167 ist laut Rabin Miller Test " << (ergebnis ? "Prim" : "nicht Prim") << endl;
    // Es wird erwartet, dass c keine Primzahl ist.
    if (ergebnis) {
        cerr << "Es wurde erwartet, dass " << c << " keine Primzahl ist." << endl;
    }
    Integer d = Integer("177387942943728133030691912202779547031");
    ergebnis = pb.millerRabinTest(d, 100);
    cout << "177387942943728133030691912202779547031 ist laut Rabin Miller Test " << (ergebnis ? "Prim" : "nicht Prim") << endl;
    // Es wird erwartet, dass d eine Primzahl ist.
    if (!ergebnis) {
        cerr << "Es wurde erwartet, dass " << d << " eine Primzahl ist. Mit "
             << "Fehlerwahrscheinlichkeit 2^-100 wurde sie aber nicht als "
             << "Primzahl erkannt." << endl;
    }
}

int main(int argc, char** argv) {

	integerExercise();
	eeaExercise();
	invExercise();
	modexpExercise();
	randExercise();
	millerRabinExercise();

	return 0;

}
