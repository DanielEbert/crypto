/*
 * RabinDecryptor.h
 */

#ifndef RABINDECRYPTOR_H_
#define RABINDECRYPTOR_H_

#include <vector>
#include "integer.h"

using namespace CryptoPP;
using namespace std;

/**
 * RabinDecryptor stellt Methoden bereit, um eine Zahl in einem Integer
 * mit dem Rabin Kryptosystem zu entschlüsseln. Es gibt zusätzlich die
 * Möglichkeit, über einen mit dem padding endenden verschlüsselten Klartext
 * aus den 4 möglichen, aus der Entschlüsselung resultierenden, Klartexten
 * zu bestimmen. Wurde am Ende des Klartexts ein Padding hinzugefügt, dann
 * wird das in dieser Dokumentation als markieren des Klartextes bezeichnet.
 * Markiert bedeutet, dass das padding an das Ende der zu verschlüsselnden
 * Zahl angehangen wird und dann erst wird "x||padding" verschlüsselt.
 * Der Grund dafür ist, dass es bei der Entschlüsselung einer verschlüsselten
 * Zahl 4 verschiedene Möglichkeiten gibt und nur eine davon ist der
 * ursprüngliche Klartext. Es gibt genau 4 mögliche Klartexte, da gilt, dass
 * wenn n das Produkt zweier Rabin-Primzahlen ist, für Gleichung x² ≡ a (mod n)
 * genau 4 Lösungen existieren. Da bei Markierung das padding beiden Parteien
 * bekannt ist, erhält die entschlüsselnde Partei durch das Padding einen
 * Hinweis darauf, welches der 4 entschlüsselten Möglichkeiten der originale
 * Klartext sein könnte.
 * Das Rabin Kryptosystem ist anfällig gegen eine Chosen-Ciphertext-Attacke.
 * Durch das Padding kann ebenfalls ein Angriff dieser Art allerdings abgewehrt
 * werden.
 * Es ist möglich, dass mehr als einer der 4 entschlüsselten Möglichkeiten
 * mit dem Padding endet. Die Wahrscheinlichkeit dafür sinkt bei größerem
 * Padding.
 * Das Rabin Kryptosystem ist ein Public Key Kryptosystem, dessen Sicherheit
 * vom Faktorisierungsproblem abhängt.
 * Beim Faktorisierungsproblem ist eine zusammengesetzte Zahl n gegeben.
 * Gesucht ist ein Faktor f von n, d.h eine Zahl f mit den Eigenschaften:
 * 1 < f < n und f | n.
 * Das Rabin Kryptosystem ist beweisbar sicher. Es hat eine geringe
 * Bedeutung für die Praxis, da zum Beispiel ein erhöhter Aufwand bei der
 * Entschlüsselung aufgrund der nicht injektiven Verschlüsselungsfunktion
 * entsteht.
 */
class RabinDecryptor {
private:
    /**
     * p ist eine Rabin Primzahl. Dabei gilt, dass p ≡ 3 (mod 4) ist.
     * p ist Teil des privaten Schlüssels. Es muss p * q = n und p != q gelten.
     */
	Integer p;
    /**
     * q ist eine Rabin Primzahl. Dabei gilt, dass q ≡ 3 (mod 4) ist.
     * q ist Teil des privaten Schlüssels. p * q = n und p != q müssen gelten.
     */
	Integer q;
    /**
     * offset stellt die Zahl dar, mit der ein zu verschlüsselnder Text/Zahl
     * multipliziert werden muss, damit das padding auf dieses Produkt addiert
     * werden kann und in Dezimaldarstellung die letzten x Ziffern belegt,
     * wobei x die Anzahl der Ziffern des paddings in Dezimaldarstellung ist.
     * Zum Beispiel, wenn padding = 987 ist, dann ist offset = 1000.
     * Ein zu verschlüsselnder Geheimtext a = 123 kann dann mit
     * a * offset + padding = 123987 markiert werden. Weiter kann mit
     * y % offset == padding, wobei y ein möglicher Klartext ist,
     * erkannt werden, ob dieser mögliche Klartext mit dem padding endet.
     * Das kann ein Hinweis darauf sein, dass der Klartext markiert worden ist.
     * Es kann aber auch sein, dass einer der möglichen Klartexte nur zufällig
     * mit dem padding endet.
     */
	Integer padding;
    /**
     * offset stellt die Zahl dar, mit der ein zu verschlüsselnder Text/Zahl
     * multipliziert werden muss, damit das padding auf dieses Produkt addiert
     * werden kann und in Dezimaldarstellung die letzten x Ziffern belegt,
     * wobei x die Anzahl der Ziffern des paddings in Dezimaldarstellung ist.
     * Zum Beispiel, wenn padding = 987 ist, dann ist offset = 1000.
     * Ein zu verschlüsselnder Geheimtext a = 123 kann dann mit
     * a * offset + padding = 123987 markiert werden. Weiter kann mit
     * y % offset == padding, wobei y ein möglicher Klartext ist,
     * erkannt werden, ob dieser mögliche Klartext mit dem padding endet.
     * Das kann ein Hinweis darauf sein, dass der Klartext markiert worden ist.
     * Es kann aber auch sein, dass einer der möglichen Klartexte nur zufällig
     * mit dem padding endet.
     * Wenn das Padding zum markieren von Klartexten verwendet werden soll,
     * dann muss das Padding beim Ver- und Entschlüsseln bekannt sein.
     */
	Integer offset;

public:
    /**
     * Konstruktor der RabinDecryptor Klasse, welcher die Klassenvariablen
     * p, q, offset, und padding setzt.
     *
     * @param p p soll eine Rabin Primzahl (p ≡ 3 (mod 4)) und verschieden zu q
     * sein. p wird Teil des privaten Schlüssels. Der Konstruktor überprüft jedoch
     * nicht, ob es sich bei p tatsächlich um eine Rabin-Primzahl handelt.
     * @param q q soll eine Rabin Primzahl (q ≡ 3 (mod 4)) und verschieden zu p
     * sein. q wird Teil des privaten Schlüssels. Es muss jedoch vom aufrufenden
     * Programm sichergestellt werden, dass es sich um eine Rabin-Primzahl handelt.
     * @param padding Ist ein Wert, mit dem ein zu verschlüsselnder Text markiert
     * wurde. Markiert bedeutet, dass das padding an das Ende des zu
     * verschlüsselten Texts angehangen worden ist.
     */
	RabinDecryptor(const Integer& p, const Integer& q, const Integer& padding=Integer("987654321"));
	virtual ~RabinDecryptor();

	/**
	 * compute Entschlüsselt den Geheimtext y und speichert die 4 möglichen
	 * Klartexte im Vektor xv ab. Es wird nicht auf das padding am Ende der
	 * möglichen Klartexte überprüft. Zu beachten gilt es, dass 4
	 * mögliche Klartexte zurückgeliefert werden und diese Methode nicht
	 * bestimmt, welches der 4 möglichen Klartexte der tatsächliche Klartext
	 * ist. Zur Entschlüsselung muss die Wurzel von y modulo n berechnet werden.
	 * Diese Berechnung ist durchführbar, wenn die beiden Rabin-Primzahlen p und q
	 * des privaten Schlüssels bekannt sind. Grundlage der Entschlüsselung ist der 
	 * Satz, dass die beiden Lösungen der Gleichung x² ≡ a (mod p), wenn p eine 
	 * eine Rabin-Primzahl ist, durch die Formel: x1/2 = (+/-) a^((p-1)/4) mod p
	 * berechnet werden kann. Die Entschlüsselung des Rabin-Kryptosystems findet dann
	 * folgendermaßen statt: Zuerst werden die Lösungen der Gleichungen a² ≡ y (mod p)
	 * und b² ≡ y (mod q) berechnet. Anschließend werden mit Hilfe des Chinesischen Restsatzes
	 * die vier möglichen Lösungen der Gleichung v² ≡ y (mod p*q) berechnet. Der Chinesische
	 * Restsatz kann angewandt werden, da gcd(p,q) = 1. Das funktioniert, weil
	 * y aus x² mod n entstanden ist und somit ist y ein quadratischer Rest
	 * modulo n.
	 * @param y y ist der zu entschlüsselnde Geheimtext bzw. die zu
	 * entschlüsselnde Zahl als Integer. y muss ein Element des Geheimtextraums
	 * sein.
	 * @param xv xv ist ein Vektor von Integern, in welchen bei erfolgreicher
	 * Entschlüsselung die 4 möglichen Klartexte abgespeichert werden. Jeder
	 * dieser Integer ist ein Element des Klartextraums.
	 * @return True, wenn das Entschlüsseln erfolgreich war, false wenn nicht.
	 */
	bool compute(const Integer& y, vector<Integer>& xv);
    /**
     * compute Entschlüsselt den Geheimtext y. Zu beachten gilt es, dass 4
	 * mögliche Klartexte resultieren werden und diese Methode nicht
	 * bestimmt, welches der 4 möglichen Klartexte der tatsächliche Klartext
	 * ist. Stattdessen wird einer dieser 4 möglichen Klartexte zurückgegeben.
     * Zur Entschlüsselung muss die Wurzel von y modulo n berechnet werden.
	 * Diese Berechnung ist durchführbar, wenn die beiden Rabin-Primzahlen p und q
	 * des privaten Schlüssels bekannt sind. Grundlage der Entschlüsselung ist der
	 * Satz, dass die beiden Lösungen der Gleichung x² ≡ a (mod p), wenn p eine
	 * eine Rabin-Primzahl ist, durch die Formel: x1/2 = (+/-) a^((p-1)/4) mod p
	 * berechnet werden kann. Die Entschlüsselung des Rabin-Kryptosystems findet dann
	 * folgendermaßen statt: Zuerst werden die Lösungen der Gleichungen a² ≡ y (mod p)
	 * und b² ≡ y (mod q) berechnet. Anschließend werden mit Hilfe des Chinesischen Restsatzes
	 * die vier möglichen Lösungen der Gleichung v² ≡ y (mod p*q) berechnet. Der Chinesische
	 * Restsatz kann angewandt werden, da gcd(p,q) = 1. Das funktioniert, weil
	 * y aus x² mod n entstanden ist und somit ist y ein quadratischer Rest
	 * modulo n.
     * Es wird nicht nach einem padding in
     * den möglichen Klartexten hin überprüft.
     * @param y y ist der zu entschlüsselnde Geheimtext bzw. die zu
     * entschlüsselnde Zahl als Integer. y muss ein Element des Geheimtextraums
	 * sein.
     * @param x x ist ein Integern, in welchem bei erfolgreicher
     * entschlüsselung eines der 4 möglichen Klartexte abgespeichert wird. x
     * ist ein Element des Klartextraums.
     * @return True, wenn das entschlüsseln erfolgreich war, false wenn nicht.
     */
	bool compute(const Integer& y, Integer& x);

    /**
     * compute2 Entschlüsselt den Geheimtext y. Zu beachten gilt es, dass 4
	 * mögliche Klartexte aus der Entschlüsselung resultieren.
     * Zur Entschlüsselung muss die Wurzel von y modulo n berechnet werden.
	 * Diese Berechnung ist durchführbar, wenn die beiden Rabin-Primzahlen p und q
	 * des privaten Schlüssels bekannt sind. Grundlage der Entschlüsselung ist der
	 * Satz, dass die beiden Lösungen der Gleichung x² ≡ a (mod p), wenn p eine
	 * eine Rabin-Primzahl ist, durch die Formel: x1/2 = (+/-) a^((p-1)/4) mod p
	 * berechnet werden kann. Die Entschlüsselung des Rabin-Kryptosystems findet dann
	 * folgendermaßen statt: Zuerst werden die Lösungen der Gleichungen a² ≡ y (mod p)
	 * und b² ≡ y (mod q) berechnet. Anschließend werden mit Hilfe des Chinesischen Restsatzes
	 * die vier möglichen Lösungen der Gleichung v² ≡ y (mod p*q) berechnet. Der Chinesische
	 * Restsatz kann angewandt werden, da gcd(p,q) = 1. Das funktioniert, weil
	 * y aus x² mod n entstanden ist und somit ist y ein quadratischer Rest
	 * modulo n.
     * Zusätzlich zu compute überprüft compute2, ob einer dieser
     * 4 Klartexte in Dezimaldarstellung mit dem padding endet. Das padding
     * wird als Markierung gesehen, welches den Klartext markiert. compute2
     * speichert bei Erfolg den Klartext ohne padding im Integer x.
     * @param y y ist der zu entschlüsselnde Geheimtext bzw. die zu
     * entschlüsselnde Zahl als Integer. y muss ein Element des Klartextraums
     * sein.
     * @param x x ist ein Integer, in welchem bei Erfolg der entschlüsselte
     * Klartext ohne padding gespeichert wird. x wird ein Element des
     * Klartextraums sein.
     * @return True, wenn das Entschlüsseln erfolgreich war, false wenn nicht.
     */
	bool compute2(const Integer& y, Integer& x);
};

#endif /* RABINSYSTEM_H_ */
