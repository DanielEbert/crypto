/*
 * RSADecryptor.h
 */

#ifndef RSADECRYPTOR_H_
#define RSADECRYPTOR_H_

#include "integer.h"

using namespace CryptoPP;

/**
 * Die Klasse RSADecryptor bietet Methoden an, um einen mit RSA verschlüsselten
 * Geheimtext zu entschlüsseln. Der Geheimtext muss mit dem Öffentlichen
 * Schlüssel n und e verschlüsselt worden sein, wobei gelten muss, dass
 * n = p * q und das e * d ≡ 1 (mod ϕ(n)), d also das multiplikative Inverse 
 * von e (mod ϕ(n)) ist. Die Sicherheit von RSA
 * beruht auf dem Faktorisierungsproblem und dem Invertieren der modularen
 * Potenzfunktion. Für keines der beiden Probleme ist ein Polynomialzeit
 * Algorithmus bekannt. Könnten diese Probleme in effizienter Laufzeit gelöst
 * werden, dann ist RSA nicht mehr sicher.
 * Beim Faktorisierungsproblem ist eine zusammengesetzte Zahl n gegeben.
 * Gesucht ist ein Faktor f von n, d.h eine Zahl f mit den Eigenschaften:
 * 1 < f < n und f | n.
 * Die modulare Potenzfunktion ist eine Einwegfunktion und ist deshalb
 * schwierig zu invertieren.
 * Zusätzlich hängt die Sicherheit von der Qualität der Schlüssel und der
 * Geheimhaltung des privaten Schlüssels ab. Mit dem aktuellen Stand der
 * Technik werden mindestens 2048 Bit pro p und q vorgeschlagen.
 * RSA ist ein Public Key Kryptosystem. Da die zur Verschlüsselung benötigten
 * Parameter öffentlich sind, kann jeder eine verschlüsselte Nachricht
 * versenden. Bei hinreichend langer Schlüssellänge und guter Wahl der
 * Parameter gilt RSA als sicheres Kryptosystem.
 */
class RSADecryptor {
private:
    /**
     * p ist ein Teil des privaten Schlüssels. p muss eine Primzahl sein,
     * dies wird von dieser Klasse allerdings nicht überprüft. Es muss p != q
     * gelten, das wird allerdings ebenfalls nicht überprüft.
     */
	Integer p;
    /**
     * q ist ein Teil des privaten Schlüssels. q muss eine Primzahl sein,
     * dies wird von dieser Klasse allerdings nicht überprüft. Es muss
     * p != q gelten, das wird allerdings ebenfalls nicht überprüft.
     */
	Integer q;
    /**
     * d ist ein Teil des privaten Schlüssels. d muss das multiplikative Inverse
     * von e mod ϕ(n) sein. Das wird von dieser Klasse allerdings nicht
     * überprüft. e ist Teil des Öffentlichen Schlüssels.
     */
	Integer d;
	/**
	 * n ist ein Teil des öffentlichen Schlüssels. n wird durch p * q im
	 * Konstruktor berechnet. n wird sowohl bei der Ver- als auch bei der
	 * Entschlüsselung verwendet und gibt die Größe des Klartextraums und
	 * Geheimtextraums an, der Z_n ist.
	 */
	Integer n;

public:
    /***
     * Konstruktor der RSADecryptor Klasse, welche den privaten Teil
     * des Schlüssels, p, q und d setzt. Die Übergabeargumente werden den
     * Klassenvariablen p, q und d zugewiesen. Das n wird ebenfalls gemäß 
     * der RSA-Spezifikation durch
     * p * q berechnet. n ist ein Teil des öffentlichen Schlüssels und
     * gibt mit Z_n den Klartext- und Geheimtextraum vor. Die Parameter
     * werden nicht auf Korrektheit hin überprüft, also wird nicht überprüft,
     * ob p und q tatsächlich mit einer ausreichenden Wahrscheinlichkeit
     * Primzahlen sind und ebenfalls nicht, ob p == q ist.
     *
     * @param p p ist ein Teil des privaten Schlüssels. p muss eine Primzahl
     * sein, das wird allerdings nicht überprüft. p ist ein Integer.
     * @param q q ist ein Teil des privaten Schlüssels. q muss eine Primzahl
     * sein, das wird allerdings nicht überprüft. q ist ein Integer.
     * @param d
     */
	RSADecryptor(const Integer& p, const Integer& q, const Integer& d);
	virtual ~RSADecryptor();

	/***
	 * compute entschlüsselt den Integer y mit dem n des öffentlichen Schlüssels und
	 * dem Privaten Teil d. Bei dieser Implementation der Entschlüsselung
	 * wird die zu entschlüsselnde Zahl y mit y^d mod n entschlüsselt.
	 * Das Ergebnis ist das entschlüsselte y, welches im Parameter x
	 * gespeichert wird. Die Zahl y muss < n sein, weil y sonst nicht Teil
	 * des Geheimtextraums ist.
	 * Sei x der originale Klartext. Wenn x mit x^e mod n = y verschlüsselt
     * wurde, dann kann der daraus resultierende Ciphertext y mit
     * y^d mod n entschlüsselt werden. Der Grund dafür ist, dass laut
     * Vorgabe e * d ≡ 1 (mod ϕ(n)) gelten muss. y = (x^e)^d = x^(e*d)
     * Mit dem Satz von Fermat kann
     * gezeigt werden, dass x^(e*d) ≡ x (mod p) und x^(e*d) ≡ x (mod q)
     * für alle x ∈ Z_n ist. Somit ist der entschlüsselte Text der gleiche wie
     * der originale Klartext x.
     * Da n = p*q kann mit dem Chinesischen Restsatz gezeigt werden, dass
     * x^(e*d) ≡ x (mod n). Der Chinesischen Restsatz kann verwendet werden,
     * weil gcd(p,q) = 1 ist.
	 * @param y Die zu entschlüsselnde Zahl als Integer. y muss ein Element
	 * des Geheimtextraums sein.
	 * @param x Wenn entschlüsselt werden kann, dann wird das entschlüsselte
	 * y in x als Integer gespeichert. x wird ein Element des Klartextraums
	 * sein.
	 * @return True, wenn entschlüsselt werden konnte, also y < n gilt,
	 * False wenn nicht.
	 */
	bool compute(const Integer& y, Integer& x) const;

    /***
     * compute entschlüsselt den Integer y mit dem Öffentlichen Teil n und
     * dem Privaten Teil d. Bei der Implementierung der Entschlüsselung in
     * dieser Methode wird die zu entschlüsselnde Zahl y mit dem Chinesischen Restsatz
     * entschlüsselt.
     * Alternativ zu x ≡ y^d % n
     * kann mit:
     * x ≡ y^d % p
     * x ≡ y^d % q
     * und dem Chinesischen Restsatz entschlüsselt weden, weil gcd(p,q) = 1 ist.
     * Das ist möglich, weil die Faktoren p und q für n zum
     * einen bekannt sind, und zum anderen weil gcd(p,q) = 1 ist, was die
     * Vorbedingung für den Einsatz des CRT ist.
     * Die Anwendung des CRT wird im .cpp Teil dieser Methode erläutert.
     * Das Ergebnis ist das entschlüsselte y, welches im Parameter x
     * gespeichert wird. Die Zahl y muss < n sein, weil sonst y nicht innerhalb
     * des Geheimtextraums liegt.
     * @param y Die zu entschlüsselnde Zahl als Integer. y muss ein Element des
     * Geheimtextraums sein.
     * @param x Wenn entschlüsselt werden kann, dann wird das entschlüsselte
     * y in x als Integer gespeichert. x wird ein Element des Klartextraums
	 * sein.
     * @return True, wenn entschlüsselt werden konnte, also y < n gilt,
     * False wenn nicht.
     */
	bool crt(const Integer& y, Integer& x) const;

    /***
     * compute entschlüsselt den Integer y mit dem Öffentlichen Teil n und
     * dem Privaten Teil d. Bei der Implementierung der Entschlüsselung in dieser
     * Methode wird die zu entschlüsselnde Zahl y mit Garners Verfahren
     * entschlüsselt, welches eine effiziente Entschlüsselung des Geheimtexts
     * erlaubt.
     * Das Ergebnis ist das entschlüsselte y, welches im Parameter x
     * gespeichert wird. Die Zahl y muss < n sein, weil sonst y nicht Teil
     * des Geheimtextraums ist.
     * @param y Die zu entschlüsselnde Zahl als Integer. y muss ein Element
     * des Geheimtextraums sein.
     * @param x Wenn entschlüsselt werden kann, dann wird das entschlüsselte
     * y in x als Integer gespeichert. x ist ein Element innerhalb des 
     * Klartextraums Zn.
     * @return True, wenn entschlüsselt werden konnte, also y < n gilt,
     * False wenn nicht.
     */
	bool garner(const Integer& y, Integer& x) const;
};

#endif /* RSADECRYPTOR_H_ */
