/*
 * RabinAttack.cpp
 */

#include "PublicKeyAlgorithmBox.h"
#include "RabinDecryptor.h"
#include "RabinAttack.h"
#include <iostream>

using namespace std;

RabinAttack::RabinAttack() {
}

RabinAttack::~RabinAttack() {
}

int RabinAttack::factorize(const Integer& n, Integer& f, int max_tries,
		RabinDecryptor& rabin_dec) {

    // Das Rabin-System ist anfällig gegen eine Attacke mit frei
    // wählbarem Geheimtext.

    // Nach max_tries Versuchen wird die Attacke abgebrochen.
    // In jedem Schleifendurchlauf wird einmal versucht einen Faktor von n zu
    // berechnen.
    for (int i = 1; i <= max_tries; i++) {
        PublicKeyAlgorithmBox pb;
        // Berechne y = r² mod n für ein zufälliges r in {1,2,..., n-1)
        Integer r = pb.randomInteger(n-2) + 1;
        Integer y = pb.modularExponentation(r, 2, n);
        Integer x;
        // y wird entschlüsselt und in x gespeichert. Der Zugriff auf
        // die Entschlüsselungsfunktion wird über die Instanz rabin_dec
        // der Klasse RabinDecryptor ermöglicht.
        // Bei der Entschlüsselung wird x auf eines der 4 Möglichkeiten für
        // √y mod n gesetzt.
        rabin_dec.compute(y, x);
        // Unter Einsatz des Chinesischen Restsatzes kann man 4 Fälle
        // unterscheiden:
        // (1) x ≡ r (mod p) und x ≡ r (mod q)
        // (2) x ≡ −r (mod p) und x ≡ −r (mod q)
        // (3) x ≡ r (mod p) und x ≡ −r (mod q)
        // (4) x ≡ −r (mod p) und x ≡ r (mod q)
        // Die Fälle (1) und (2) sind nicht verwertbar, weil
        // der Faktor von n sonst nicht im Ergebnis des Chinesischen
        // Restsatzes enthalten ist. Zum Beispiel bei Fall (1) folgt, dass
        // x-r ≡ 0 (mod p) und x-r ≡ 0 (mod q)
        // Anwendung des CRT:
        // x-r ≡ 0 * q * mp - 0 * p * mq ≡ 0 (mod n)
        // Also ist keiner der Faktoren von n ist enthalten.
        // Wird Fall 1 oder 2 erkannt, dann kann mit diesem x nicht faktorisiert
        // werden und der Versuch wird abgebrochen.
        if (x == r || x-n == -r) continue;
        // Anhand von Fall (3) oder (4) kann man n faktorisieren.
        // Das beudetet, dass die Erfolgswahrscheinlichkeit des Angriffs
        // pro versuch bei 50% ist. Durch wiederholtes Ausführen kann die
        // Erfolgswahrscheinlichkeit erhöht werden.
        // Betrachten wir Fall 3, aus dem folgt, dass:
        // x - r ≡ 0 (mod p) und x-r ≡ -2r (mod q)
        // Mit dem CRT:
        // x -r ≡ 0 * q * mp - 2 * r * p * mq ≡ -2 * r * p * mq (mod n)
        // Nachdem das (mod n) herausgerechnet ist, ist die Gleichung:
        // x - r = l * n - 2 * r * p * mq für ein l ∈ N
        //       = p * (l * q - 2 * r * mq)
        // Dieses Mal ist p, ein Faktor von n und ein Faktor von x-r.
        // Weil n nur durch p und q teilbar ist, und p ein Faktor von x-r ist,
        // ist der größte gemeinsame Teiler von n und x-r = p.
        // Fall (4) kann auf analoge Weise analysiert werden.
        f = Integer::Gcd(x-r, n);
        // Falls die Attacke erfolgreich war, dann liefert die Methode die
        // Anzahl der Versuche zurück
        return i;
    }

    // War die Attacke nicht erfolgreich, dann ist der Rückgabewert gleich −1.
    return -1;
}
