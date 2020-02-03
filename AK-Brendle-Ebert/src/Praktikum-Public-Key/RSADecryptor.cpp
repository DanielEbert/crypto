/*
 * RSADecryptor.cpp
 */

#include "PublicKeyAlgorithmBox.h"
#include "RSADecryptor.h"

#include <iostream>

using namespace std;

RSADecryptor::RSADecryptor(const Integer& p, const Integer& q,
		const Integer& d) {
    // Setzen des privaten Teils (p,q,d) und des öffentlichen Teils n des
    // RSA Schlüssels. n ist laut Definition p * q, wobei p und q prim sind,
    // und p != q ist, aber das wird nicht überprüft.
    this->p = p;
    this->q = q;
    this->d = d;
    n = p * q;
}

RSADecryptor::~RSADecryptor() {
}

// #compute()
bool RSADecryptor::compute(const Integer& y, Integer& x) const {
    // Wenn x >= n wahr ist, dann ist x nicht im Klartextraum und kann deshalb
    // nicht verschlüsselt werden.
    if (y >= n) return false;
    PublicKeyAlgorithmBox pb;
    // Sei x der originale Klartext. Wenn x mit x^e mod n = y verschlüsselt
    // wurde, dann kann der daraus resultierende Ciphertext y mit
    // y^d mod n entschlüsselt werden. Der Grund dafür ist, dass laut
    // Vorgabe e * d ≡ 1 (mod ϕ(n)) gelten muss. Mit dem Satz von Fermat kann
    // gezeigt werden, dass x^(e*d) ≡ x (mod p) und x^(e*d) ≡ x (mod q)
    // für alle x ∈ Z_n ist.
    // Da n = p*q kann mit dem Chinesischen Restsatz gezeigt werden, dass
    // x^(e*d) ≡ x (mod n). Der Chinesischen Restsatz kann verwendet werden,
    // weil gcd(p,q) = 1 ist.
    x = pb.modularExponentation(y, d, n);

    // y ist ein Element des Geheimtextraums und konnte deshalb
    // entschlüsselt werden. Darum wird True zurückgegeben.
    return true;
}

// #crt()
bool RSADecryptor::crt(const Integer& y, Integer& x) const {
    // Wenn x >= n wahr ist, dann ist x nicht im Klartextraum und kann deshalb
    // nicht verschlüsselt werden.
    if (y >= n) return false;
    PublicKeyAlgorithmBox pb;

    // Alternativ zu x ≡ y^d % n
    // kann mit:
    // x ≡ y^d % p
    // x ≡ y^d % q
    // und dem Chinesischen Restsatz entschlüsselt weden, weil gcd(p,q) = 1 ist.

    // Für den CRT müssen die b1 und b2 in
    // x ≡ b1 (mod p) und
    // x ≡ b2 (mod q) berechnet werden.
    // b1 ist y^d % p und y^d % p kann mit der modularen Exponentation
    // berechnet werden. Das selbe ist für y^d % q der Fall.
    Integer b1 = pb.modularExponentation(y, d, p);
    Integer b2 = pb.modularExponentation(y, d, q);

    // Ebenfalls benötigt werden die multiplikativen Inversen von p mod q und
    // q mod p.
    Integer mp, mq;
    pb.multInverse(p, q, mp);
    pb.multInverse(q, p, mq);

    // Nach dem CRT liefert der CRT die eindeutlige Lösung
    // x = (b1 * M1 * n1 + b2 * M2 * n2) mod n.
    // b1 und b2 sind bereits bekannt.
    // M1 ist M / p. M ist p * q = n. Also ist p * q / p = q.
    // M2 ist M / q. M ist p * q = n. Also ist p * q / q = p.
    // n1 ist das mulitplikative Inverse von M1 mod p.
    // n2 ist das mulitplikative Inverse von M2 mod q.
    // Die Werte in die Formel einsetzten und x kann berechnet werden.
    x = (b1 * q * mq + b2 * p * mp) % n;

    // y ist ein Element des Geheimtextraums und konnte deshalb
    // entschlüsselt werden. Darum wird True zurückgegeben.
    return true;
}

// #garner()
bool RSADecryptor::garner(const Integer& y, Integer& x) const {
    // Wenn x >= n wahr ist, dann ist x nicht im Klartextraum und kann deshalb
    // nicht verschlüsselt werden.
    if (y >= n) return false;

    PublicKeyAlgorithmBox pb;

    // Für Garners Verfahren wird
    // a = y^d mod p
    Integer a = pb.modularExponentation(y, d, p);
    // und  b = y^d mod q berechnet.
    Integer b = pb.modularExponentation(y, d, q);

    // Ebenfalls benötigt wird das multiplikative Inverse von q mod p.
    Integer qInv;
    pb.multInverse(q, p, qInv);

    // Ansonsten werden die Werte in Garner's Formel eingesetzt.
    x = (((a - b) * (qInv % p)) % p) * q + b;

    // y ist ein Element des Geheimtextraums und konnte deshalb
    // entschlüsselt werden. Darum wird True zurückgegeben.
    return true;
}
