/*
 * RSAEncryptor.h
 */

#ifndef RSAENCRYPTOR_H_
#define RSAENCRYPTOR_H_

#include "integer.h"

using namespace CryptoPP;

/**
 * RSAEncryptor bietet die Funktion, eine Zahl mit dem RSA Kryptosystem
 * zu verschlüsseln. Bei der Verschlüsselung wird n, ein Teil des öffentlichen
 * Schlüssels und e, ebenfalls ein Teil des öffentlichen Schlüssels, verwendet.
 * Die Sicherheit von RSA
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
 * geheimhaltung des privaten Schlüssels ab. Mit dem aktuellen Stand der
 * Technik werden mindestens 2048 Bit pro p und q vorgeschlagen.
 * RSA ist ein Public Key Kryptosystem. Da die zur Verschlüsselung benötigten
 * Parameter öffentlich sind, kann jeder eine verschlüsselte Nachricht
 * versenden. Bei hinreichend langer Schlüssellänge und guter Wahl der
 * Parameter gilt RSA als sicheres Kryptosystem.
 */
class RSAEncryptor {
private:
    /**
    * n ist ein Teil des öffentlichen Schlüssels.
    * n wird sowohl bei der Ver- als auch bei der
    * Entschlüsselung verwendet und gibt die Größe des Klartextraums und
    * Geheimtextraums an, der Z_n ist.
    */
	Integer n;
	/**
	 * Der öffentliche Teil e des Schlüsseln, mit dem ein zu verschlüsselnder
	 * Text verschlüsselt wird. e wird im Konstruktor dieser Klasse gesetzt.
	 * e muss das multiplikative Inverse
     * von d mod ϕ(n) sein. Das wird von dieser Klasse allerdings nicht
     * überprüft.
	 */
	Integer e;

public:
    /**
     * Konstruktor der RSAEncryptor Klasse, welche den öffentlichen Teil
     * des Schlüssels, n und e, setzt. Die Übergabeargumente werden den
     * Klassenvariablen n und e zugewiesen.
     * @param n Der bei der Ver- und Entschlüsselung verwendete Modul n,
     * welcher das Produkt von zwei Primzahlen p und q ist, wobei p != q. Das
     * wird allerdings im Konstruktor nicht überprüft. n ist ein Integer und
     * Teil des öffentlichen Schlüssels.
     * @param e Der bei der Verschlüsselung verwendete Exponent e als Integer.
     * e ist ebenfalls Teil des öffentlichen Schlüssels. Weiter sollte gelten,
     * dass gcd(e,ϕ(n)) = 1, e und ϕ(n) also teilerfremd sind, sodass ein multiplikatives
     * Inverses d mod(ϕ(n)) für e existiert, sodass e * d ≡ 1 (mod ϕ(n)), wobei d 
     * ein Teil des privaten Schlüssels ist
     * und in der Klasse RSADecryptor gesetzt wird und somit zum Entschlüsseln
     * verwendet wird.
     */
	RSAEncryptor(const Integer& n, const Integer& e);
	virtual ~RSAEncryptor();

	/**
	 * compute verschlüsselt die Zahl x mit dem RSA Kryptosystem und dem
	 * öffentlichen Schlüssel n und e. Dabei wird x^e mod n berechnet.
	 * Das Ergebnis ist das verschlüsselte x, welches im Parameter y
	 * gespeichert wird. Die Zahl x muss < n sein, weil sonst x nicht Teil
	 * des Klartextraums ist.
	 * @param x Die zu verschlüsselnde Zahl als Integer. x muss ein Element
	 * des Klartextraums Z_n sein.
	 * @param y In y wird bei Erfolg die verschlüsselte Zahl x als Integer
	 * gespeichert. y wird ein Element des Geheimtextraums.
	 * @return True, wenn x verschlüsselt worden ist und somit x < n gilt,
	 * false wenn nicht.
	 */
	bool compute(const Integer& x, Integer& y) const;
};

#endif /* RSAENCRYPTOR_H_ */
