/*
 * rabin.cpp
 */

#include <iostream>
#include <vector>
#include "osrng.h"
#include "PublicKeyAlgorithmBox.h"
#include "RabinDecryptor.h"
#include "RabinEncryptor.h"
#include "RabinAttack.h"

using namespace std;

// #rabinParameters()
void rabinParameters() {
	/*********************************************************************
	 * Aufgabe 9.
	 *********************************************************************/

	PublicKeyAlgorithmBox pb;
	Integer p, q;
	// unter Verwendung der Methode randomRabinPrime() werden zwei geeignete
	// 256-Bit Rabin Primzahlen p und q als Parameter für das Rabin-Kryptosystem
	//  generiert. Für das Rabin-Kryptosystem muss jede der beiden Primzahlen
	// eine Rabin-Primzahl, ð, sein, bei der gilt, dass x ≡ 3 (mod 4), wobei x
	// p bzw. q ist.
    pb.randomRabinPrime(p, 256, 50);
    pb.randomRabinPrime(q, 256, 50);
	cout << "Erste  256 Bit Rabin Primzahl p: " << p << endl;
    cout << "Zweite 256 Bit Rabin Primzahl q: " << q << endl;
}

// #sqrtModPrimeExercise()
void sqrtModPrimeExercise() {
	/*********************************************************************
	 * Aufgabe 10.
	 *********************************************************************/

	PublicKeyAlgorithmBox pb;
	vector<Integer> v;
	// Berechnet die zwei Quadratwurzeln von 400040001 mod 884249923643 und
	// speichert diese als Integer im vector v ab.
	pb.modPrimeSqrt(Integer("400040001"), Integer("884249923643"), v);
	cout << "Quadratwurzeln von 400040001 mod 884249923643" << v[0] << ' '
	     << v[1] << endl;
	// Überprüfung und Ausgabe des Ergebnisses.
	if (v[0] != Integer("884249903642") || v[1] !=  Integer("20001")) {
        cerr << "ERROR: Quadratwurzeln von 400040001 mod 884249923643" << v[0] << ' '
             << v[1] << ", allerdings wurde 884249903642 und 20001 erwartet." << endl;
	}

    vector<Integer> v2;
    // Berechnet die zwei Quadratwurzeln von 644640535034 % 868380007367 und
    // speichert diese im vector v2 ab.
    pb.modPrimeSqrt(Integer("644640535034"), Integer("868380007367"), v2);
    cout << "Quadratwurzeln von 644640535034 mod 868380007367" << v2[0] << ' '
         << v2[1] << endl;
    if (v2[0] != Integer("868378777318") || v2[1] !=  Integer("1230049")) {
        cerr << "ERROR: Quadratwurzeln von 644640535034 mod 868380007367" << v2[0] << ' '
             << v2[1] << ", allerdings wurde 868378777318 und 1230049 erwartet." << endl;
    }
}

// #rabinDemo()
void rabinDemo() {
	/*********************************************************************
	 * Aufgaben 11 und 12.
	 *********************************************************************/

    Integer p = Integer("728768879148869666628372866383");
    Integer q = Integer("1178365175275537416785439551531");
    // Der RabinEncryptor wird mit n = p * q initialisiert.
    RabinEncryptor rabinEncryptor(p * q, 0);
    Integer y;
    // Verschlüssle 234131892323212 mit dem Rabin Kryptosystem und speichere
    // den verschlüsselten Klartext in y als Integer ab.
    rabinEncryptor.compute(Integer("234131892323212"), y);
    if (y != Integer("54817743002848138258673996944")) {
        cerr << "ERROR: Die Zahl 234131892323212 wurde mit dem rabin Kryptosystem "
             << "zu " << y << " verschlüsselt. Allerdings wurde 54817743002848138258673996944"
             << " als verschlüsselte Zahl erwartet." << endl;
    }

    // Der RabinDecryptor entschlüsselt Zahlen, die mit n = p*q verschlüsselt
    // worden sind.
    RabinDecryptor rabinDecryptor(p, q, 0);

    vector<Integer> v;
    // Entschlüssle y und speichere die 4 möglichen Klartexte im vector v ab.
    rabinDecryptor.compute(y, v);
    cout <<"Die 4 möglichen Klartexte: " << v[0] << ' ' << v[1] << v[2] << ' ' << v[3] << endl;
    if (v[0] != Integer("858755868013614750163033981532656431854843747105980613759161")) {
        cerr << "ERROR: Als erster möglicher Klartext wurde " << v[0] << "zurückgegeben."
             << " Allerdings wurde 858755868013614750163033981532656431854843747105980613759161 erwartet." << endl;
    }
    if (v[1] != Integer("468253878381255984906252283528370901639136709523733710358252")) {
        cerr << "ERROR: Als zweiter möglicher Klartext wurde " << v[1] << "zurückgegeben."
             << " Allerdings wurde 468253878381255984906252283528370901639136709523733710358252 erwartet." << endl;
    }
    if (v[2] != Integer("390501989632358765256781698004285530215707037816378795724121")) {
        cerr << "ERROR: Als dritter möglicher Klartext wurde " << v[2] << "zurückgegeben."
             << " Allerdings wurde 390501989632358765256781698004285530215707037816378795724121 erwartet." << endl;
    }
    if (v[3] != Integer("234131892323212")) {
        cerr << "ERROR: Als vierter möglicher Klartext wurde " << v[3] << "zurückgegeben."
             << " Allerdings wurde 234131892323212 erwartet." << endl;
    }

    Integer x1;
    // Wird als zweites Argument ein Integer und kein vector an compute übergeben,
    // dann wird nur eines der 4 möglichen Ergebnisse über x1 zurückgelifert.
    rabinDecryptor.compute(y, x1);
    if (x1 != Integer("858755868013614750163033981532656431854843747105980613759161")) {
        cerr << "ERROR: Als erster möglicher Klartext wurde " << x1 << "zurückgegeben."
             << " Allerdings wurde 858755868013614750163033981532656431854843747105980613759161 erwartet." << endl;
    }

    // With Padding
    // In der Implementierung wurde die Möglichkeit hinzugefügt, Klartexte
    // mit einem Padding zu markieren. Dadurch kann der Dekodierer einen
    // Hinweis darauf bekommen, welches der 4 möglichen Klartexte der
    // tatsächliche Klartext ist. Das Padding muss beiden Parteien bekannt sein.
    // Als Padding wird 987654321 festgelegt. Das beudetet, dass an einen
    // zu verschlüsselnden Klartext x das padding angehangen wird. Zum Beispiel
    // wenn x = 234131892323212, dann wird 234131892323212987654321
    // verschlüsselt.
    RabinEncryptor rabinEncryptorPadded(p * q, Integer("987654321"));
    Integer yPadded;
    rabinEncryptorPadded.compute2(Integer("234131892323212"), yPadded);

    RabinDecryptor rabinDecryptorPadded(p, q, Integer("987654321"));
    Integer x;
    // Über die compute2 Methode wird spezifiziert, dass nach der Markierung
    // in den 4 möglichen Klartexten gesucht werden soll. Der Klartext ohne
    // Padding/Markierung wird über x zurückgelifert.
    rabinDecryptorPadded.compute2(yPadded, x);
    if (x != Integer("234131892323212")) {
        cerr << "ERROR: Als entschlüsselter Text wurde " << x << "zurückgegeben."
             << " Allerdings wurde 234131892323212 erwartet." << endl;
    }
}

// #rabinAttack()
void rabinAttack() {
	/*********************************************************************
	 * Aufgabe 13.
	 *********************************************************************/

	// Beispiel einer Chosen Ciphertext Attacke auf das Rabin Kryptosystem
	// Dabei darf ein beliebiger Ciphertext mit dem Rabin Kryptosystem
	// entschlüsselt werden. Das Ziel ist es, n zu faktorisieren, das heißt
	// p oder q ohne Kentniss von p und q zu bekommen.

	// Um einen Ciphertext zu verschlüsseln wird eine RabinDecryptor Klasse
	// erzeugt
    Integer p = Integer("728768879148869666628372866383");
    Integer q = Integer("1178365175275537416785439551531");
    RabinDecryptor rabinDecryptor(p, q, 0);

    //  Die Klasse RabinAttack implementiert die Chosen Ciphertext Attacke
    RabinAttack rabinAttack;
    // In f wird bei Erfolg ein Faktor von n, wobei n = p * q, gespeichert
    Integer f;
    // factorize liefert bei Erfolg die Anzahl der Versuche zurück und bei
    // keinem Erfolg wird -1 zurückgelifert.
    // factorize bekommt das öffentliche n (=p*q) als erstes Übergabeargument.
    // Die maximale Anzahl der Versuche wird im dritten Argument auf 5 gesetzt.
    // Die Erfolgswahrscheinlichkeit liegt dadurch bei 1-0.5⁵.
    // Dem zu verschlüsselnden Klartext wurde keine Markierung/Padding
    // hinzugefügt. Mit Padding könnte der Angriff verhindert werden.
    int tries = rabinAttack.factorize(p * q, f, 5, rabinDecryptor);
    if (tries != -1) {
        // Nach tries Versuchen wurde ein Faktor f gefunden.
        cout << "Found a factor f: " << f << endl;
        if (f != p && f != q) {
            cerr << "ERROR: Von rabinAttack wurde " << f << " als Primfaktor "
                 << "zurückgegeben. Allerdings wurde " << p << " oder " << q
                 << " erwartet." << endl;
        }
    } else {
        // Nach 5 Versuchen wurde kein Faktor von n gefunden.
        cout << "Did not find f after 5 tries." << endl;
    }
}

// #main()
int main() {

	rabinParameters();
	sqrtModPrimeExercise();
	rabinDemo();
	rabinAttack();

	return 0;
}
