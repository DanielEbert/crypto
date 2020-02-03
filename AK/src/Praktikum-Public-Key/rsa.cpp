/*
 * rsa.cpp
 */

#include <iostream>
#include "PublicKeyAlgorithmBox.h"
#include "RSAEncryptor.h"
#include "RSADecryptor.h"
#include "RSAAttack.h"
#include "RSAOracle.h"

using namespace std;

// #rsaParameters()
void rsaParameters() {
	/*********************************************************************
	 * Aufgabe 15.
	 *********************************************************************/

	PublicKeyAlgorithmBox pb;
	// p q e und d werden von der Methode berechnet.
	// p q und d bilden den privaten Schlüssel. e ist Teil des öffentlichen
	// Schlüssels. Das n, der zweite Teil des öffentlichen Schlüssels, kann
	// effizient mit p * q berechnet werden.
	Integer p, q, e, d;
	// 256 ist die bitlen, die maximale Bitgröße der zu generierenden
	// Primzahlen p und q.
	// 30 ist der Qualitätsparameter s. s ist das s in 1-2^-s, welches die
	// Fehlerwahrscheinlichkeit dafür angibt, dass p keine Primzahl ist. Das
	// gleiche gilt für q.
	pb.generateRSAParams(p, q, e, d, 256, 30);
	// e ∈ {1, 2, . . . , ϕ(n) − 1} muss wahr sein.
	Integer phiN = (p-1)*(q-1);
	if (e < 1 || e > phiN-1) {
	    cerr << "ERROR: e ∈ {1, 2, . . . , ϕ(n) − 1} ist nicht wahr."
             << " e for RSA Key out of Range" << endl;
	}
	// e und ϕ(n) müssen teilerfremd sein
	if (Integer::Gcd(e, phiN) != 1) {
	    cerr << "ERROR: e und ϕ(n) sind nicht teilerfremd. Es wurde erwartet,"
             << " dass diese teilerfremd sind." << endl;
	}
	// d = e^−1 mod ϕ(n) muss wahr sein.
    pb.multInverse(e, phiN, d);
	if ((e * d) % phiN != 1) {
	    cerr << "d = e^−1 mod ϕ(n) ist nicht wahr. Es wurde erwartet,"
             << "dass dies jedoch wahr ist." << endl;
	}
	cout << "Ausgabe der RSA Parameter: " << endl;
    cout << "p: " << p << " q: " << q << " e: " << e << " d: " << d << endl;
    cout << "-------------" << endl;
}

// #rsaDemo()
void rsaDemo() {
	/*********************************************************************
	 * Aufgabe 16.
	 *********************************************************************/

	// Der RSA Schlüssel für das Beispiel ist vorgegeben.
    // p q und d bilden den privaten Schlüssel. e ist Teil des öffentlichen
    // Schlüssels. Das n, der zweite Teil des öffentlichen Schlüssels, kann
    // effizient mit p * q berechnet werden.
	Integer p = Integer("15192846618168946907");
    Integer q = Integer("10041530829891794273");
    Integer e = Integer("141290156426204318982571851806193576543");
    Integer d = Integer("73707354481439936713886319521045114527");

    // x ist der zu verschlüsselnde Integer.
    Integer x = Integer("79372353861768787619084471254314002875");
    // Es wird erwartet, dass x mit dem obigen Schlüssel zu y verschlüsselt
    // wird. Es wird ebenfalls erwartet, dass y entschlüsselt wieder x ergibt.
    Integer y = Integer("47915958473033255778832465116435774510");

    // Die gleichen Checks aus der rsaParameters Funktion von oben.
    Integer phiN = (p-1)*(q-1);
    if (e < 1 || e > phiN-1) {
        cerr << "ERROR: e ∈ {1, 2, . . . , ϕ(n) − 1} ist nicht wahr."
             << " e for RSA Key out of Range" << endl;
    }
    if (Integer::Gcd(e, phiN) != 1) {
        cerr << "ERROR: e und ϕ(n) sind nicht teilerfremd. Es wurde erwartet,"
             << " dass diese teilerfremd sind." << endl;
    }
    PublicKeyAlgorithmBox pb;
    pb.multInverse(e, phiN, d);
    if ((e * d) % phiN != 1) {
        cerr << "d = e^−1 mod ϕ(n) ist nicht wahr. Es wurde erwartet,"
             << "dass dies jedoch wahr ist." << endl;
    }

    // RSAEncryptor bietet Methoden an, um eine Zahl mit dem RSA Kryptosystem
    // zu verschlüsseln.
    RSAEncryptor rsaEncryptor(p * q, e);
    Integer outy;
    // Verschlüsselt die Zahl x mit dem öffentlichen Schlüssel (n, e) und
    // speichert das Ergebnis in outy ab.
    rsaEncryptor.compute(x, outy);
    // Es wird erwartet, dass x mit dem obigen Schlüssel zu y verschlüsselt
    // wird.
    cout << "RSA Verschlüsselung: " << x << " > " << outy << endl;
    if (outy != y) {
        cerr << "ERROR: Es wurde erwartet, dass " << x << " zu " << y
             << "verschlüsselt wird. Allerdings wurde zu " << outy
             << "verschlüsselt." << endl;
    }

    // RSADecryptor bietet Methoden an, um eine Zahl mit dem RSA Kryptosystem
    // zu entschlüsseln. Es wird erwartet, dass alle drei Methoden die
    // verschlüsselte Zahl zur ursprünglichen, entschlüsselten Zahl x
    // entschlüsseln.
    RSADecryptor rsaDecryptor(p, q, d);
    Integer outx, outgarner, outcrt;
    rsaDecryptor.compute(outy, outx);
    cout << "RSA Entschlüsselung: " << outy << " --compute--> " << outx << endl;
    if (outx != x) {
        cerr << "ERROR: Es wurde erwartet, dass " << outy << " zu " << x
             << "entschlüsselt wird. Allerdings wurde zu " << outx
             << "entschlüsselt." << endl;
    }
    // Alternative Entschlüsselung mit dem Chinesischen Restsatz.
    rsaDecryptor.crt(outy, outcrt);
    cout << "RSA Entschlüsselung: " << outy << " --CRT--> " << outcrt << endl;
    if (outcrt != x) {
        cerr << "ERROR: Es wurde erwartet, dass " << outy << " zu " << x
             << "entschlüsselt wird. Allerdings wurde zu " << outcrt
             << "entschlüsselt." << endl;
    }
    rsaDecryptor.garner(outy, outgarner);
    // Alternative Entschlüsselung mit Garners Verfahren.
    cout << "RSA Entschlüsselung: " << outy << " --Garner--> " << outgarner << endl;
    if (outgarner != x) {
        cerr << "ERROR: Es wurde erwartet, dass " << outy << " zu " << x
             << "entschlüsselt wird. Allerdings wurde zu " << outgarner
             << "entschlüsselt." << endl;
    }
}

//#sqrtExercise()
void sqrtExercise() {
	/*********************************************************************
	 * Aufgabe 17.
	 *********************************************************************/

	PublicKeyAlgorithmBox pb;
	Integer s;
	// Suche Kleinste nicht negative Quadratwurzel s mit
	// s² = 3157242151326374471752634944
	pb.sqrt(Integer("3157242151326374471752634944"), s);
	cout << "Kleinste nicht negative Quadratwurzel von 3157242151326374471752634944: "
	     << s << endl;
	if (s != Integer("56189341972712")) {
	    cerr << "Es wurde erwartet, dass die kleinste nicht negative "
             << "Quadratwurzel von 3157242151326374471752634944 gleich "
             << "56189341972712 ist. Allerdings wurde " << s << " zurückgeliefert." << endl;
	}
    // Suche Kleinste nicht negative Quadratwurzel s mit
    // s² = 11175843681943819792704729
    pb.sqrt(Integer("11175843681943819792704729"), s);
    cout << "Kleinste nicht negative Quadratwurzel von 11175843681943819792704729: "
         << s << endl;
    if (s != Integer("3343029117723")) {
        cerr << "Es wurde erwartet, dass die kleinste nicht negative "
             << "Quadratwurzel von 11175843681943819792704729 gleich "
             << "3343029117723 ist. Allerdings wurde " << s << " zurückgeliefert." << endl;
    }
    // Suche Kleinste nicht negative Quadratwurzel s mit
    // s² = 3343229819990029117723. Es wird erwartet, dass eine solche Zahl s
    // nicht existiert.
    Integer returned = pb.sqrt(Integer("3343229819990029117723"), s);
    if (!returned) {
        cout << "Es wird erwartet, dass es keine Kleinste nicht negative "
             << "Quadratwurzel von 3343229819990029117723 gibt." << endl;
    } else {
        cerr << "Es wurde erwartet, dass es keine Kleinste nicht negative "
             << "Quadratwurzel von 3343229819990029117723 gibt. Allerdings "
             << "wurde zurückgegeben, dass es eine gibt und dass diese "
             << s << " ist." << endl;
    }
}

// #factorizingAttack()
void factorizingAttack() {
	/*********************************************************************
	 * Aufgabe 18.
	 *********************************************************************/

	// Beispiel für die Attacke auf RSA, bei der das ϕ(n) bekannt ist.
	RSAAttack rsaAttack;
    Integer p, q;
    // n ist das erste Argument, ϕ(n) das zweite.
    // Bei Erfolg liefert factorizeN die Faktoren p und q von n im 3. und 4.
    // Parameter zurück.
    rsaAttack.factorizeN(Integer("127869459623070904125109742803085324131"),
                         Integer("127869459623070904102412837477002840200"),
                         p, q);
    // Es wird erwartet, dass n erfolgreich faktorisiert werden konnte.
    cout << "Faktoren von n: " << p << " " << q << endl;
    // 127869459623070904125109742803085324131 ist n
    if (p * q != Integer("127869459623070904125109742803085324131")) {
        cerr << "ERROR: rsaAttack liefert die Faktoren " << p << " und " << q
             << " als Faktoren von n zurück. Allerdings ist p * q nicht n." << endl;
    }
}

// #euklidExercise()
void euklidExercise() {
	/*********************************************************************
	 * Aufgabe 19.
	 *********************************************************************/
    PublicKeyAlgorithmBox pb;
    // Berechnet die Werte q1 bis qm des Kettenbruchs 39/112 und speichert diese im
    // Vektor q ab
    vector<Integer> q;
    Integer d = pb.euklid(39, 112, q);
    // eukild() liefert gcd(39,112) zurück, es wird erwartet dass dieser 1 ist,
    // also 39 und 112 teilerfremd sind.
    cout << "gcd(39, 112) = " << d << endl;
    if (d != 1) {
        cerr << "Es wurde erwartet, dass 39 und 112 teilerfremd sind. "
             << "Allerdings liefert euklid " << d << " zurück." << endl;
    }
    vector<Integer> expectedResult = {Integer("0"), Integer("2"), Integer("1"),
                                   Integer("6"), Integer("1"), Integer("4")};
    cout << "q1 bis qm des Kettenbruchs 39/112: " << endl;
    for (auto i : q) {
        cout << i << ' ';
    }
    cout << endl;
    // Überprüfen, ob die Werte q1 bis qm in q mit den erwarteten Werten
    // übereinstimmen.
    if (q != expectedResult) {
        cerr << "Der berechnete Kettenbruch von 39/112 stimmt nicht mit dem "
             << "erwarteten Ergebnis überein." << endl;
    }
}

// #convergentsExercise()
void convergentsExercise() {
	/*********************************************************************
	 * Aufgabe 20.
	 *********************************************************************/

    PublicKeyAlgorithmBox pb;
    // Berechnung der Konvergenten des Kettenbruchs von 39/112 und abspeichern der Zähler in
    // Vector c und der Nenner in Vector d
    vector<Integer> c, d;
    pb.computeConvergents(Integer(39), Integer(112), c, d);
    vector<Integer> expectedC = {1, 0, 1, 1, 7, 8, 39};
    vector<Integer> expectedD = {0, 1, 2, 3, 20, 23, 112};
    // Überprüfen, ob die Zähler und Nenner mit den erwarteten Werten übereinstimmen
    cout << "Zähler der Konvergenten des Kettenbruchs 39/112: " << endl;
    for (auto i : c) {
        cout << i << ' ';
    }
    cout << endl;
    cout << "Nenner der Konvergenten des Kettenbruchs 39/112: " << endl;
    for (auto i : d) {
        cout << i << ' ';
    }
    cout << endl;
    if (c != expectedC || d != expectedD) {
        cerr << "Die berechneten Konvergenten von 39/112 stimmen nicht mit dem "
             << "erwarteten Ergebnis überein." << endl;
    }
}

// #wienerAttack()
void wienerAttack() {
	/*********************************************************************
	 * Aufgabe 21.
	 *********************************************************************/
    // Beispiel der Wiener Attacke auf RSA
	RSAAttack rsaAttack;
	Integer p, q;
	// Die wienerAttack kann, wenn die Parameter von RSA ungünstig gewählt
	// sind, mit dem Öffentlichen Schlüssel den privaten Schlüssel p und q
	// berechnen. An wienerAttack wird das n des öffentlichen Schlüssels
	// im ersten Argument und das e des öffentlichen Schlüssels im zweiten
	// Argument übergeben. Bei Erfolg liefert wienerAttack True zurück
	// und speichert die Faktoren von n im dritten und vierten Parameter
	// ab.
	Integer n = Integer("224497286580947090363360894377508023561");
	rsaAttack.wienerAttack(n, Integer("163745652718951887142293581189022709093"),
	                       p, q);
	// Es wird erwartet, dass die wienerAttack erfolgreich war und die Faktoren
	// von n korrekt von wienerAttack gesetzt wurden.
	cout << "Wiener Attack: Faktoren von n: " << p << ' ' << q << endl;
    if (p * q != n) {
        cerr << "ERROR: Wiener Attack liefert die Faktoren " << p << " und " << q
             << " als Faktoren von n zurück. Allerdings ist p * q nicht n." << endl;
    }
}

// #oracleExercise()
void oracleExercise() {
	/*********************************************************************
	 * Aufgabe 22.
	 *********************************************************************/

	Integer p = Integer("16015510136412338011");
	Integer q = Integer("12177032305856164321");
	Integer d = Integer("946975621");

	RSAOracle rsaOracle(p, q, d);
	// Die Methode half() liefert den Wert des höchstwertigen Bits des
    // entschlüsselten Geheimtext in y zurück. y wird an half als Integer übergeben.
    // Dabei hat der entschlüsselte Geheimtext die
    // gleiche Bitlänge wie n, wird also wenn nötig mit Nullen links aufgefüllt/gepadded.
    // Da keine solche Funktion existiert, welche diese
    // Funktionalität in effizienter Zeit mit dem öffentlichen Schlüssel berechnet, muss dem Orakel
    // Wissen über den privaten Teil des RSA-Schlüssels übergegeben werden, um den Geheimtext
    // entschlüsseln zu können und auf diese Weise das höchstwertigste Bit zu bestimmen.
    bool result = rsaOracle.half(Integer("116415012259126332853105614449093205668"));
    cout << "Half Funktion auf 116415012259126332853105614449093205668 liefert " << result << endl;
    // Es wird erwartet, dass das höchstwertigste Bit von
    // 116415012259126332853105614449093205668 entschlüsselt 1 ist.
	if (!result) {
	    cerr << "ERROR: Es wurde erwartet, dass das höchstwertigste Bit von "
             << "116415012259126332853105614449093205668 entschlüsselt 1 ist. "
             << "Allerdings wurde 0 zurückgeliefert." << endl;
	}
	result = rsaOracle.half(Integer("74304303162215663057995326922844871006"));
	cout << "Half Funktion auf 74304303162215663057995326922844871006 liefert " << result << endl;
    // Es wird erwartet, dass das höchstwertigste Bit von
    // 74304303162215663057995326922844871006 entschlüsselt 0 ist.
    if (result) {
        cerr << "ERROR: Es wurde erwartet, dass das höchstwertigste Bit von "
             << "74304303162215663057995326922844871006 entschlüsselt 0 ist. "
             << "Allerdings wurde 1 zurückgeliefert." << endl;
    }
    result = rsaOracle.half(Integer("102949691974634609941445904667722882083"));
    cout << "Half Funktion auf 102949691974634609941445904667722882083 liefert " << result << endl;
    // Es wird erwartet, dass das höchstwertigste Bit von
    // 102949691974634609941445904667722882083 entschlüsselt 0 ist.
    if (result) {
        cerr << "ERROR: Es wurde erwartet, dass das höchstwertigste Bit von "
             << "102949691974634609941445904667722882083 entschlüsselt 0 ist. "
             << "Allerdings wurde 1 zurückgeliefert." << endl;
    }
    result = rsaOracle.half(Integer("42549620926959222864355800078420537413"));
    cout << "Half Funktion auf 42549620926959222864355800078420537413 liefert " << result << endl;
    // Es wird erwartet, dass das höchstwertigste Bit von
    // 42549620926959222864355800078420537413 entschlüsselt 1 ist.
    if (!result) {
        cerr << "ERROR: Es wurde erwartet, dass das höchstwertigste Bit von "
             << "42549620926959222864355800078420537413 entschlüsselt 1 ist. "
             << "Allerdings wurde 0 zurückgeliefert." << endl;
    }
}

// #halfAttack()
void halfAttack() {
	/*********************************************************************
	 * Aufgabe 23.
	 *********************************************************************/
    // Beispiel Angriff auf RSA mit half Attack, bei der ein verschlüsselter
    // Geheimtext entschlüsselt wird
	Integer p = Integer("12889769717276679053");
	Integer q = Integer("17322528238664264177");
	Integer e = Integer("55051594731967684255289987977028610689");
    Integer d = Integer("149154082258429024247010774747829057473");
    Integer x = Integer("167092961114842952923160287194683529938");

    //Die Klasse RSAAttack implementiert die halfAttack Funktion, welche
    // diesen Angriff implementiert
	RSAAttack rsaAttack;
	// halfAttack benötigt die in RSAOracle implementierte Methode half(), welche
	// den Wert des höchstwertigen Bits des entschlüsselten Geheimtext in y zurückliefert.
	RSAOracle rsaOracle(p, q, d);
	// halfAttack erhält das öffentliche n des RSA Schlüsels, also p*q, im ersten
	// Argument.
	// Das e des öffentlichen Schlüssels wird im zweiten Parameter übergeben
	// Das dritte Argument enhält den zu entschlüsselnden Geheimtext und
	// das vierte Argument eine Referenz auf eine RSAOracle Klasse.
	// halfAttack liefert den entschlüsselten Geheimtext zurück.
	Integer result = rsaAttack.halfAttack(p * q, e, x, rsaOracle);

	// Abschließend wird überprüft, ob der laut halfAttack entschlüsselte
	// Geheimtext tatsächlich der Klartext ist. Dafür wird
	// RSADecryptor mit dem privaten Schlüssel initialisiert und
	// das mit halfAttack entschlüsselte x wird ebenfalls mit
	// RSADecryptor entschlüsselt.
	RSADecryptor rsaDecryptor(p, q, d);
	Integer y;
	rsaDecryptor.compute(x, y);
    cout << "Decrypted Message: " << result << endl;
    // Wenn halfAttack erfolgreich war, dann sind beide entschlüsselten
    // Zahlen gleich.
    if (y != result) {
        cerr << "ERROR: halfAttack liefert " << result << " als "
             << "entschlüsselten Text zurück. Allerdings wurde " << y
             << "erwartet." << endl;
    }
}

// #main()
int main() {
	rsaParameters();
	rsaDemo();
	sqrtExercise();
	factorizingAttack();
	euklidExercise();
	convergentsExercise();
	wienerAttack();
	oracleExercise();
	halfAttack();
	return 0;
}

