/*
 * RSAAttack.cpp
 */

#include <vector>
#include "PublicKeyAlgorithmBox.h"
#include "RSAAttack.h"

#include <iostream>
using namespace std;

RSAAttack::RSAAttack() {
}

RSAAttack::~RSAAttack() {
}

// #factorizeN()
bool RSAAttack::factorizeN(const Integer& n, const Integer& phi_n, Integer& p,
		Integer& q) const {
    PublicKeyAlgorithmBox pb;

    // Berechnen von b für die Mitternachtsformel
    Integer b = phi_n - n - 1;

    // Berechnen der Wurzel der Mitternachtsformel
    Integer s;
    pb.sqrt(b * b - 4 * n, s);

    // Berechne Mitternachtsformel für die Gleichung
    // 0 = p² + (ϕ(n) − n − 1)p + n um p und q zu berechnen.
    // Bei der Mitternachtsformel ist a = 1, b = ϕ(n) - n - 1 und c = n
    Integer pTemp = (-b + s) / 2;
    Integer qTemp = (-b - s) / 2;

    // Überprüfen, ob die Faktorisierung Erfolgreich war.
    // Das ist zum Beispiel nicht der Fall, falls das ϕ(n) falsch ist.
    if (pTemp * qTemp == n) {
        // pTemp und qTemp sind die Faktoren von n
        // Bei Erfolg werden die beiden Faktoren von n in p und q gespeichert.
        p = pTemp;
        q = qTemp;
        return true;
    }

    // n konnte nicht faktorisiert werden. Das wird durch das zurückgeben
    // von False signalisiert.
    return false;
}

// #wienerAttack()
bool RSAAttack::wienerAttack(const Integer& n, const Integer& e, Integer& p,
		Integer& q) const {
    //Wiener-Attacke auf RSA

    //Erzeugen eines Objekts der Klasse PublicKeyAlgorithmBox
    PublicKeyAlgorithmBox pb;

    //Erzeugen der Integer-Vektoren c und d, in denen der Zähler und der Nenner
    //der Konvergenten des Kettenbruchs von e/n gespeichert werden
    vector<Integer> c, d;
    //Berechnen der Konvergenten des Kettenbruchs von e/n durch die in er Klasse
    //PublicKeyAlgorithmBox bereitgestellte Methode
    pb.computeConvergents(e, n, c, d);

    //Überprüfe für jede Konvergente c[i]/d[i] des Kettenbruchs e/n ab
    // i = 2...
    for (int i = 2; i < c.size(); i++) {
        //ob (e*d[i]-1)/c[i] eine ganze Zahl ist. Ist dies nicht der Fall,
	//wenn (e*d[i]-1) % c[i] ungleich 0 ist, also ein Rest bei der Division
	//Division entstehen würde, dann mache mit dem nächsten Schleifendurchlauf
	//weiter
        if ((e * d[i] - 1) % c[i] != 0) continue;
	//Anschließend wird Phi von n berechned durch (e*d[i]-1)/c[i]
        Integer phiN = (e * d[i] - 1) / c[i];
	//erzeugen von Temporären Variablen, die zum Aufruf von factorizeN
	//eingesetzt werden
        Integer pTemp, qTemp;
	//wenn das Faktorieren von n mit Hilfe des erzeugten phiN erfolgreich
	//ablief, dann befinden sich in pTemp und qTemp die beiden Primzahlen,
	//durch deren Multiplikation n dargestellt werden kann
        if (factorizeN(n, phiN, pTemp, qTemp)) {
	    //Abspeichern der beiden Werte von pTemp und qTemp in den per Referenz
	    //übergebenen Parametern p und q
            p = pTemp;
            q = qTemp;
	    //Es wird True zurückgegeben, um dem Aufrufer zu signalisieren, dass die
	    //Wiener-Attacke erfolgreich ablief
            return true;
        }
    }
    //wenn diese Überprüfungen für alle betrachteten Konvergenten fehlschlagen,
    //dann schlug der Angriff fehl, da die Parameter des RSA-Schlüssels nicht den
    //geforderten Eigenschaften entsprachen. Dann wird False zurückgegeben, um den
    //den Fehlschlag des Angriffs zu signalisieren.
    return false;
}

// #halfAttack()
Integer RSAAttack::halfAttack(const Integer& n, const Integer& e, const Integer& y,
		RSAOracle& rsa_oracle) const {
    //Implementiert die Half-Attack auf RSA

    //Erzeugen eines Objekts der Klasse PublicKeyAlgorithmBox
    PublicKeyAlgorithmBox pb;
    //Bool-Vector h, in dem die höchstwertigsten Bits abgespeichert werden, die durch
    //die Half-Funktion des RSA-Orakels berechnet
    vector<bool> h;

    //setze den int l auf die Bitlänge von n, um die Anzahl der Bits von n abzuspeichern
    int l = n.BitCount();
    //Setze y2 auf den Wert von y, um das y während der Berechnung verändern zu können
    Integer y2 = y;
    //Führe die nachfolgende Schleife für jedes Bit von n durch
    for (int i = 0; i < l; i++) {
	//Berechne den Wert des höchstwertigsten Bits, des zu y2 gehörenden Klartext mit
	//Hilfe der half-Funktion des RSA-Orakels und füge den Wert dieses Bits an das
	//Ende des Vektors h an
        h.push_back(rsa_oracle.half(y2));
	//Multipliziere den aktuellen Wert von y2 mit dem Ergebnis der Modularen Exponentation
	//von 2^e (mod n) und rechne das Ergebnis modulo n. Hier wird die Eigenschaft von RSA
	//ausgenutzt, dass enc(K,x1*x2) = enc(K,x1)*enc(K,x2) und explizit enc(K,2) * y2
	//berechnet. Dies entspricht einem linksshifts des Klartexts um 1 und so kann im zweiten 
	//Schleifendurchlauf das zweithöchste Bit von y mit der Half-Methode berechnet werden, usw.
        y2 = (y2 * pb.modularExponentation(2,e,n)) % n;
    }

    //Setze den Integer lo auf 0 und hi auf n. Diese Werte dienen als Grenzen 
    //für die anschließende Binäre Suche
    Integer lo = 0;
    Integer hi = n;
    //Multipliziere den Wert von hi l mal mit 10, damit dieser Größer wird und auf
    //diese Weise keine ungenauigkeiten bei der Binären Suche auftreten
    for (int i = 0; i < l; i++) hi *= 10;

    //Führe eine Binäre Suche anhand der in der obigen For-Schleife berechneten Bits durch
    for (int i = 0; i < l; i++) {
        Integer mid = (hi + lo) / 2;
        if (h[i]) {
            lo = mid;
        } else {
            hi = mid;
        }
    }

    //Dividiere den Inhalt von hi l-mal durch 10, um den vor der Binären Suche vergrößerten
    //Wert wieder auf den Ursprünglichen zu verkleinern
    for (int i = 0; i < l; i++) hi /= 10;

    //Rückgabe des Werts hi, in dem sich der zum Geheimtext y gehörende Klartext x befindet.
    //Dieser Klartext ist richtig, falls das Orakel die richtigen Werte für die half-Funktion
    //errrechnet hat.
    return hi;
}
