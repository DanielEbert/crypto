/*
 * RabinEncryptor.cpp
 */

#include "RabinEncryptor.h"


RabinEncryptor::RabinEncryptor(const Integer& n, const Integer& padding) {
    // Initialisieren der Klassenattribute mit den Übergabeparametern n und
    // padding. n soll das Produkt zweier Primzahlen sein. Das wird allerdings
    // nicht überprüft.
    this->n = n;
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

RabinEncryptor::~RabinEncryptor() {
}

// #compute()
bool RabinEncryptor::compute(const Integer& x, Integer& y) {
    // Ist die zu verschlüsselnde Zahl x größer als n, dann wird x nicht
    // verschlüsselt, weil x nicht Teil des Klartextraums ist.
    // In einem solchen Fall würde das Entschlüsseln den Wert x (mod n) und
    // und nicht x zurückliefern.
    if (x >= n) return false;
    // Zur Verschlüsselung wird enc((n,p,q),x) = x² mod n berechnet, wobei
    // a_exp_b_mod_c, also die modulare Exponentation a^b mod n, verwendet wird.
    // Dabei ist a = x, b = 2 und n = n.
    // Bei compute wird der Verschlüsselte Text nicht mit dem padding markiert.
    y = a_exp_b_mod_c(x, 2, n);
    // x konnte verschlüsselt werden. Darum wird True zurückgegeben.
    return true;
}

// #compute2()
bool RabinEncryptor::compute2(const Integer& x, Integer& y) {
    // Ist die zu verschlüsselnde Zahl x mit padding größer als n, dann wird x
    // nicht verschlüsselt, weil x||padding nicht Teil des Klartextraums ist.
    // In einem solchen Fall würde das Entschlüsseln den Wert x (mod n) und
    // und nicht x (ohne mod n) zurückliefern.
    if (x * offset + padding >= n) return false;
    // Zur Verschlüsselung wird enc((n,p,q),x) = x² mod n berechnet, wobei
    // a_exp_b_mod_c, also die modulare Exponentation a^b mod n, verwendet wird.
    // Dabei ist a = x, b = 2 und n = n.
    // Bei compute2 wird der Verschlüsselte Text mit dem padding markiert.
    // Dafür wird das padding am Ende von x (in Dezimaldarstellung) angehangen.
    // Das padding ist dem Entschlüssler bekannt und dadurch hat dieser einen
    // Hinweis darauf, welches der 4 Möglichkeiten der tatsächliche Klartext
    // ist.
    y = a_exp_b_mod_c(x * offset + padding, 2, n);
    // x konnte verschlüsselt werden. Darum wird True zurückgegeben.
    return true;
}
