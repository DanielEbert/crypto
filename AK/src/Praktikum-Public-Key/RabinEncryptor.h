/*
 * RabinEncryptor.h
 */

#ifndef RABINENCRYPTOR_H_
#define RABINENCRYPTOR_H_

#include "integer.h"

using namespace CryptoPP;

/**
 * RabinEncryptor stellt Methoden bereit, um eine Zahl in einem Integer
 * mit dem Rabin Kryptosystem zu verschlüsseln. Das Rabin-Kryptosystem wurde im Jahre
 * 1979 von Michael O. Rabin entwicket. In dieser Implementierung gibt es zusätzlich die
 * Möglichkeit, einen zu verschlüsselnden Klartext zu markieren.
 * Markiert bedeutet, dass das padding an das Ende der zu verschlüsselnden
 * Zahl angehangen wird und dann erst wird "x||padding" verschlüsselt.
 * Der Grund dafür ist, dass es bei der Entschlüsselung einer verschlüsselten
 * Zahl 4 verschiedene Möglichkeiten gibt und nur eine davon ist der
 * ursprüngliche Klartext. Es gibt genau 4 mögliche Klartexte, da gilt, dass wenn n
 * das Produkt zweier Rabin-Primzahlen ist, für Gleichung x² ≡ a (mod n) genau
 * 4 Lösungen existieren. Da bei Markierung das padding beiden Parteien bekannt ist, erhält
 * die entschlüsselnde Partei durch das Padding einen Hinweis darauf,
 * welches der 4 entschlüsselten Möglichkeiten der originale Klartext sein
 * könnte.
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
class RabinEncryptor {
private:
    /**
     * Der Integer n soll das Produkt zweier Rabin-Primzahlen sein.
     * Dies wird jedoch nicht überprüft.
     * n ist ein Teil des öffentlichen Schlüssels und repräsentiert den Modul
     * der Ver- und Entschlüsselungsfunktion.
     * n wird sowohl bei der Ver- als auch bei der
     * Entschlüsselung verwendet und gibt den Klartextraum und Geheimtextraum
     * an, der Z_n ist.
     */
	Integer n;
	/**
	 * offset stellt die Zahl dar, mit der ein zu verschlüsselnder Text/Zahl
     * multipliziert werden muss, damit das padding auf dieses Produkt addiert
     * werden kann und in Dezimaldarstellung die letzten x Ziffern belegt,
     * wobei x die Anzahl der Ziffern des paddings in Dezimaldarstellung ist.
     * Zum Beispiel, wenn padding = 987 ist, dann ist offset = 1000.
     * Ein zu verschlüsselnder Geheimtext a = 123 kann dann mit
     * a * offset + padding = 123987 markiert werden.
	 */
	Integer offset;
    /**
     * offset stellt die Zahl dar, mit der ein zu verschlüsselnder Text/Zahl
     * multipliziert werden muss, damit das padding auf dieses Produkt addiert
     * werden kann und in Dezimaldarstellung die letzten x Ziffern belegt,
     * wobei x die Anzahl der Ziffern des paddings in Dezimaldarstellung ist.
     * Zum Beispiel, wenn padding = 987 ist, dann ist offset = 1000.
     * Ein zu verschlüsselnder Geheimtext a = 123 kann dann mit
     * a * offset + padding = 123987 markiert werden.
     * Wenn das Padding zum markieren von Klartexten verwendet werden soll,
     * dann muss das Padding beim Ver- und Entschlüsseln bekannt sein.
     */
	Integer padding;

public:
    /**
     * Konstruktor der RabinEncryptor Klasse, welcher die Klassenvariablen
     * n, offset, und padding setzt.
     *
     * @param n n soll das Produkt zweier Rabin Primzahlen als Crypto++ Integer
     * sein. n ist Teil des öffentlichen Schlüssels und repräsentiert den Modul.
     * @param padding Ist der Wert, mit dem der zu verschlüsselnde Text markiert
     * wird, wenn mit der compute2 Methode verschlüsselt wird. Markiert
     * bedeutet, dass das padding an das Ende des zu verschlüsselten Texts
     * angehangen wird.
     */
	RabinEncryptor(const Integer& n, const Integer& padding=Integer("987654321"));
	virtual ~RabinEncryptor();

	/**
	 * compute verschlüsselt die Zahl im Parameter x mit dem Rabin Kryptosystem
	 * und speichert die verschlüsselte Zahl ohne padding/Markierung in y ab.
	 * Bei der Verschlüsselung mit dem Rabin-Kryptosystem wird der Geheimtext
	 * y durch y = x² (mod n) berechnet. x muss ein Element des Klartextraums
	 * sein. Der Klartextraum ist Z_n, das heißt, dass 0 <= x < n sein muss. y wird
	 * ein Element aus dem Geheimtextraum Z_n.
	 *
	 * @param x Zu verschlüsselnde Zahl als Integer. x muss kleiner als n sein.
	 * x ist ein Element des Klartextraums Z_n.
	 * @param y In y wird die verschlüsselte Zahl als Integer geschrieben. y ist
	 * ein Element des Geheimtextraums Z_n und ein Quadratischer Rest modulo n.
	 * @return True, wenn erfolgreich verschlüsselt wurde, also wenn x ein passender Klartext ist, 
	 * False wenn nicht.
	 */
	bool compute(const Integer& x, Integer& y);

    /**
     * compute2 verschlüsselt die Zahl in x mit dem Rabin-Kryptosystem und
     * speichert die verschlüsselte Zahl mit padding/Markierung in y ab.
     * Markiert bedeutet, dass das padding an das Ende der zu verschlüsselnden
     * Zahl angehangen wird und dann erst wird "x||padding" verschlüsselt. Die 
     * Verschlüsselung findet durch y = (x||padding)² (mod n) statt.
     * Der Grund dafür ist, dass die Verschlüsselungsfunktion des Rabin-Kryptosystems
     * nicht injektiv ist und es so bei der Entschlüsselung einer verschlüsselten
     * Zahl 4 verschiedene Möglichkeiten gibt und nur eine davon ist der
     * ursprüngliche Klartext. Es gibt genau 4 mögliche Klartexte, da gilt, dass wenn n
     * das Produkt zweier Rabin-Primzahlen ist, für Gleichung x² ≡ a (mod n) genau
     * 4 Lösungen existieren. Da bei Markierung das padding beiden Parteien bekannt ist, erhält
     * die entschlüsselnde Partei durch das Padding einen Hinweis darauf,
     * welches der 4 entschlüsselten Möglichkeiten der originale Klartext sein
     * könnte.
     * Es ist möglich, dass mehr als einer der 4 entschlüsselten Möglichkeiten
     * mit dem Padding endet. Die Wahrscheinlichkeit dafür sinkt bei größerem
     * Padding.
     * Die aus x||padding resultierende Zahl muss ein Element des Klartextraums
     * sein. Das bedeutet, dass es nicht mehr wie bei compute ausreicht, dass
     * nur x ein Element aus Z_n ist.
     *
	 * @param x Zu verschlüsselnde Zahl als Integer. x muss kleiner als n sein.
     * Die aus x||padding resultierende Zahl muss ein Element Z_n, also des
     * Klartextraums sein.
	 * @param y In y wird die zu verschlüsselnde Zahl mit padding/Markierung als
     * Integer geschrieben. y ist Teil des Geheimtextraums Z_n und ein
     * Quadratischer Rest modulo n.
	 * @return True, wenn erfolgreich verschlüsselt wurde, also x||padding < n
     * gilt, False wenn nicht.
     */
	bool compute2(const Integer&x, Integer& y);
};

#endif /* RABINENCRYPTOR_H_ */
