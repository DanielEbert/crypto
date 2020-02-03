/*
 * RabinDecryptor.cpp
 */

#include <vector>
#include "RabinDecryptor.h"
#include "PublicKeyAlgorithmBox.h"

RabinDecryptor::RabinDecryptor(const Integer& p, const Integer& q,
		const Integer& padding) {
    // Initialisieren der Klassenattribute mit den Übergabeparametern n und
    // padding.
    this->p = p;
    this->q = q;
    this->padding = padding;
    // offset stellt die Zahl dar, mit der ein zu verschlüsselnder Text/Zahl
    // multipliziert werden muss, damit das padding auf dieses Produkt addiert
    // werden kann und in Dezimaldarstellung die letzten x Ziffern belegt,
    // wobei x die Anzahl der Ziffern des paddings in Dezimaldarstellung ist.
    // Zum Beispiel, wenn padding = 987 ist, dann ist offset = 1000.
    // Ein zu verschlüsselnder Geheimtext a = 123 kann dann mit
    // a * offset + padding = 123987 markiert werden.
    // padding = 0 ist ein Spezialfall, in dem der offset <= padding
    // Algorithmus nicht funktioniert. Er wird deshalb separat
    // behandelt, wobei das offset auf 10 gesetzt und zurückgegeben wird.
    // offset ist 10, weil für das Padding mit der Ziffer 0 eine Ziffer
    // für das Abspeichern des Paddings benötigt wird.
    if (padding == 0) {
        offset = 10;
        return;
    }
    offset = 1;
    // In jedem Schleifendurchlauf wird offset sozusagen in Dezimaldarstellung
    // um 1 nach links verschoben. Wenn offset > padding ist, dann ist das
    // offset ausreichend groß, um das padding in der Anzahl der Ziffern von
    // offset in Dezimaldarstellung zu speichern.
    while (offset <= padding) {
        offset *= 10;
    }
}

// #compute()
bool RabinDecryptor::compute(const Integer& y, vector<Integer>& xv) {

    // Es wird überprüft, ob y ein Element des Geheimtextraums Z_n ist.
    // Ist das nicht der Fall, dann kann y nicht entschlüsselt werden und es
    // wird false zurückgegeben.
    Integer n = p * q;
    if (y >= n) return false;

    // Gesucht ist die Wurzel von y mod n.
    // Wenn können die Wurzeln sowohl von p als auch für q berechnet werden,
    // weil diese laut Vorgabe Rabin Primzahlen sind.
    vector<Integer> bp, bq;
    PublicKeyAlgorithmBox pb;
    // Berechne die Lösungen bp1,2 der Gleichung bp^2 ≡ y (mod p)
    // bzw. bp = √y (mod p)
    pb.modPrimeSqrt(y, p, bp);
    // Berechne die Lösungen bq1,2 der Gleichung bq^2 ≡ y (mod q)
    // bzw. bq = √y (mod q)
    pb.modPrimeSqrt(y, q, bq);

    // und nach dem Chinesischen Restsatz gibt
    // bp * mp * q + bq * mq * p mod (p * q) die Lösung für √y mod (p * q) an.
    // Zu beachten gilt, dass bp und bq jeweils 2 verschiedene Zahlen sind.
    // Dadurch gibt es 4 verschiedene Möglichkeiten, je nachdem welche
    // Kombination der Zahlen in bp und bq man auswählt. Es gibt somit keine
    // eindeutige Lösung.

    // Berechne mittels des Chinesischen Restsatzes die vier Lösungen
    // <v1, v2, v3, v4> der Gleichung v^2 ≡ y (mod p * q)
    // Dabei ist:
    //   v^2 ≡  y (mod p * q)
    // = v   ≡ √y (mod p * q)

    // In bp * mp * q + bq * mq * p mod (p * q) ist mp das multiplikative
    // Inverse von q mod p und mq das multiplikative Inverse von p mod q.
    // mp und mq können mit dem Erweiterten Algorithmus von Euklid berechnet
    // werden. Dafür muss der gcd(p, q) = 1 sein. Da p != q und p und q
    // Primzahlen sind, bzw. das erwartet wird, gibt es ein solches mp und mq.
    // Wegen ZTK Satz 3.4 gibt es x,y ∈ Z, so dass gcd(p, q) = 1 = p*x + q*y.
    // x und y werden über den EEA berechnet. Ist also x und y gegeben, dann ist
    // p*x + q*y ≡ 1 (mod q) = p*x ≡ 1 (mod q), das heißt das x ist bekannt
    // und x ist das multiplikative Inverse von p mod q. Ebenfalls kann mit
    // p*x + q*y ≡ 1 (mod p) = q*y ≡ 1 (mod p) das y als multiplikative Inverse
    // von q mod p festgelegt werden.
    // Also ist mp = q^-1 (mod p)
    // und      mq = p^-1 (mod q)
    Integer d, mq, mp;
    pb.EEA(p, q, d, mq, mp);
    // Check ob p und q tatsächlich wie erwartet teilerfremd sind,
    // also gcd(p, q) == 1 gilt. Ist das nicht der Fall, dann wurden für
    // p und q keine Primzahlen ausgewählt oder p == q.
    if (d != 1) return false;

    // Berechnet bp * mp * q + bq * mq * p mod (p * q) für die 4 Kombinationen
    // mit bp und bq und speichert diese im vector xv ab.
    // Oder mit anderen Worten:
    // ( √y (mod p) * q * q^-1 (mod p)
    //   +
    //   √y (mod q) * p * p^-1 (mod q) ) mod (p * q)
    xv.push_back((bp[0] * q * mp + bq[0] * p * mq) % n);
    xv.push_back((bp[0] * q * mp + bq[1] * p * mq) % n);
    xv.push_back((bp[1] * q * mp + bq[0] * p * mq) % n);
    xv.push_back((bp[1] * q * mp + bq[1] * p * mq) % n);

    // y konnte entschlüsselt werden. Darum wird True zurückgegeben.
    return true;
}

bool RabinDecryptor::compute(const Integer& y, Integer& x) {
    vector<Integer> xv;
    // Berechne die 4 Ergebnisse für √y (mod n)
    if (compute(y, xv)) {
        // Wenn erfolgreich, dann wird das erste der 4 Ergebnisse in x
        // gespeichert und True zurückgegeben.
        x = xv[0];
        // y konnte entschlüsselt werden. Darum wird True zurückgegeben.
        return true;
    }
    // Wenn √y (mod n) nicht berechnet werden konnte, dann wird False
    // zurückgegeben.
    return false;
}

// #compute2()
bool RabinDecryptor::compute2(const Integer& y, Integer& x) {
    vector<Integer> xv;
    // Berechne die 4 Ergebnisse für √y (mod n)
    compute(y, xv);

    // yWithPaddingzählt die Anzahl der möglichen Klartexte, die mit dem padding
    // enden (in Dezimaldarstellung)
    unsigned int yWithPadding = 0;

    // Überprüft in jedem der 4 Ergebnisse...
    for (auto xPadded : xv) {
        // ...ob der entschlüsselte Klartext mit dem padding in
        // Dezimaldarstellung aufhört.
        if (xPadded % offset == padding) {
            // Wenn das der Fall ist, dann wird x dem Klartext ohne das
            // padding zugewiesen.
            x = xPadded / offset;
            // yWithPadding wird um 1 erhöht, da ein möglichen Klartext mit
            // dem padding endet.
            yWithPadding++;
        }
    }

    // Wenn nicht Entschlüsselt werden konnte oder wenn mehr als 1 der
    // möglichen Klartexte mit dem padding enden, dann kann trotz Padding
    // nicht bestimmt werden, welches der möglichen Klartexte der
    // tatsächliche Klartext ist. Es gibt immer die Wahrscheinlichkeit > 0, dass
    // zufällig ein möglicher Klartext mit dem padding endet.
    return yWithPadding == 1;
}

RabinDecryptor::~RabinDecryptor() {
}

