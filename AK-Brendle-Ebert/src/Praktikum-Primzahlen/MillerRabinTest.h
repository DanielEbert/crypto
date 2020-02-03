/*
 * MillerRabinTest.h
 */

#ifndef MILLERRABINTEST_H_
#define MILLERRABINTEST_H_

#include "integer.h"
#include "PRNG.h"

using namespace CryptoPP;
/**
 * Diese Klasse implementiert den Miller-Rabin Primzahltest, einen Monte-Carlo Algorithmus, der mit einer bestimmten
 * Fehlerwahrscheinlichkeit entscheiden kann, ob eine Zahl eine Primzahl ist.
 */
class MillerRabinTest {
public:

    /**
     * Die Methode boundedExponentation() berechnet die Ganzzahlpotenz b^e und arbeitet dafür auf einer abgewandelten
     * Form des Alogrithmus für Modulare Exponentiation. Während der Berechnung der Ganzzahlpotenz prüft der
     * Algorithmus, ob dass Zwischenergebnis größer als die angegebene Obergrenze bound ist. Ist dies der Fall, bricht
     * die Berechnung ab und bound+1 wird zurückgegeben.
     * @param b Basis der zu berechnenden Ganzzahlpotenz
     * @param e Exponent der zu berechnenden Ganzzahlpotenz
     * @param bound Obergrenze für die die zu berechnende Ganzzahlpotenz
     * @return bound+1, wenn die Ganzzahlpotenz größer als bound ist, b^e, wenn die Ganzzahlpotenz kleiner oder gleich
     * bound ist
     */
	Integer boundedExponentation(const Integer& b, const Integer& e,
			const Integer& bound) const;
    /**
     * Diese Methode implementiert den Algorithmus Modulare Exponentiation. Mit diesem Verfahren kann der Term
     * a^b mod n effizient berechnet werden. Dafür wird b in seine Binärdarstellung zerlegt und das Ergebnis der
     * Potenz durch Multiplikation und Quadratur berechnet. Eine Variable d startet mit Wert 1 und wird für jedes
     * Bit von b quadriert. Falls das Bit gleich 1 ist, dann wird d noch zusätzlich mit dem Wert von a multipliziert.
     * Alle diese Berechnungen finden modulo n statt, damit die Zwischenergebnisse nicht zu groß werden und der
     * Alogrithmus das Ergebnis in effizienter Zeit berechnen kann.
     * @param a Basis der Exponentiation vom Crypto++ Typ Integer
     * @param b Exponent der Exponentiation vom Crypto++ Typ Integer
     * @param n Modulus der Exponentiation vom Crypto++ Typ Integer
     * @return Ergebnis der Berechnung a^b (mod n)
     */
	Integer modularExponentation(const Integer& a, const Integer& b,
			const Integer& n) const;

	/**
	 * Der Algorithmus witness ist eine abgewandelte Form der modularen Exponentiation, die für die übergebenen
	 * Parameter a und n den Term  a^(n-1) (mod n) berechnet. Diese Berechnung soll den Satz von Fermat überprüfen.
	 * Der Satz
	 * von Fermat sagt aus: Ist n eine Primzahl, dann gilt für alle a in Z Stern n, dass: a^(n-1) kongruent zu 1
	 * (mod n). Wenn das Ergebnis dieser Berechnung ungleich 1 ist, dann ist die Zahl definitiv keine Primzahl und a
	 * kann als Solider Beweis dafür angesehen werden, dass n keine Primzahl ist. Ein solider Beweis ist eine Zahl a
	 * in Z Stern n, die beweist, dass n eine zusammengesetzte Zahl ist. Sei n also eine beliebige ungerade natürliche
	 * Zahl, die größer 1 ist. Dann werden r und u so gewählt, dass n-1 = 2^r * u ist. Dann ist a ein solider Beweis,
	 * falls: 1. a^u nicht kongruent zu +1 oder -1 (modulo n) und 2. Für alle i=1 bis r-1 gilt: a ^(2^i * u) ist
	 * nicht kongruent zu -1 (modulo n). Wenn n eine beliebige, zusammengesetzte Zahl ist, die keine Primzahlpotenz ist,
	 * dann sind mindestens die Hälfte aller Zahlen in Z Stern n solide Beweise dafür, dass n eine zusammengesetzte
	 * Zahl ist. Ist das Ergebnis der Berechung des Fermat-Kriteriums a^(n-1) (mod n) jedoch gleich 1,
	 * dann ist es möglich, dass die Zahl eine Primzahl ist. Es existieren jedoch auch Carmichael Zahlen. Für diese
	 * seltenen, zusammengesetzten Zahlen gilt: Sei c eine Charmichael Zahl, dann gilt für alle a in Z Stern c, dass
	 * a^(n-1) kongruent zu 1 (modulo n). Deshalb reicht der Satz von Fermat allein nicht aus, um eine Primzahl
	 * zu erkennen.
	 * Deshalb wird während der Berechnung von a^(n-1) parallel dabei nach einer Lösung für die Gleichung x^2 kongruent
	 * zu 1 (mod n) gesucht, die verschieden zu 1 und -1 (also n-1) ist. Wird eine solche Zahl gefunden, dann ist a ein
	 * Solider Beweis dafür, dass n keine Primzahl ist. Diese Schlussfolgerung ist möglich, da wenn n eine
	 * Primzahl ungleich 2 ist, dann sind 1 und -1 die einzigen Quadratwurzeln von 1 (mod n) in Z Stern n.
	 * @param a Zahl in Z Stern n, für die überprüft wird, ob sie ein soldier Beweis dafür ist, dass n eine
	 *  zusammengesetzte Zahl ist
	 * @param n Zahl, für die getestet werden soll, ob sie eine Primzahl ist.
	 * @return True, wenn n keine Primzahl ist (a also ein Solider Beweis ist), ansonsten False
	 */
	bool witness(const Integer& a, const Integer& n) const;

	bool witness2(const Integer& a, const Integer& n) const;

	/**
	 * Die Methode isPerfectPower kann zur Lösung des Entscheidungsproblems Ganzzahlpotenz verwendet werden. In diesem
	 * Entscheidungsproblem ist für eine natürliche Zahl n > 2 gefragt, ob es sich dabei um eine Ganzzahlpotenz handelt.
	 * Der Algorithmus überprüft für alle möglichen Exponenten e, ob ein b existiert, sodass b^e = n. Dafür wird für
	 * jedes mögliche e zwischen 2 und log₂(n)+1 die Methode searchBase() aufgerufen und anschließend überprüft, ob
	 * dieser eine Basis gefunden hat, sodass b^e = n. Falls eine Basis gefunden wurde, werden diese in den Parametern
	 * b und e gespeichert.
	 * @param n Zahl, für die festgestellt werden soll, ob es sich um eine Ganzzahlpotenz handelt oder nicht
	 * @param b Im Parameter b wird die Basis der Ganzzahlpotenz gespeichert, wenn es sich bei n um eine Ganzzahlpotenz
	 * handelt
	 * @param e Im Parameter e wird der Exponent der Ganzzahlpotenz gespeichert, wenn es sich bei n um eine
	 * Ganzzahlpotenz handelt
	 * @return True, wenn es sich bei n um eine Ganzzahlpotenz handelt. In diesem Fall wird die Basis und der Exponent
	 * dieser Ganzzahlpotenz in den Parametern b und e gespeichert. Wenn es sich um keine Ganzzahlpotenz handelt, dann
	 * gibt die Methode False zurück.
	 */
	bool isPerfectPower(const Integer& n, Integer& b, Integer& e) const;

	/**
	 * Dieser Algorithmus berechnet für natürliche Zahlen n >= 2 und e (mit e: 2 <= e <= log₂(n)+1), ob eine Basis b
	 * existiert, damit b^e = n. Dafür setzt der Algorithmus eine Binäre Suche ein, um b schneller zu finden bzw.
	 * effizienter festzustellen, dass kein solcher Wert b existiert. Der Algorithmus berechnet für den aktuell
	 * zu überprüfenden Wert b b^e mit dem Algorithmus für Bounded Exponentation (implementiert in Methode
	 * boundedExponentation()), welcher nicht mehr weiterrechnet, wenn ein Zwischenergebnis der Berechnung b^e größer
	 * als n ist. Ist das Ergebnis der Berechnung b^e = n, dann wird b zurückgegeben. Die Laufzeit des Algorithmus ist
	 * polynomial in der Größe der Binärdarstellung von n
	 * @param n Zahl, für welche festgestellt werden soll, ob es sich dabei um eine Ganzzahlpotenz handelt
	 * @param e Exponent der Ganzzahlpotenz
	 * @return b, falls ein b exisitiert, sodass b^e = n, ansonsten 0
	 */
	Integer searchBase(const Integer& n, const Integer& e) const;

	bool extendedEuklid(const Integer& a, const Integer& b, Integer& d,
			Integer& x, Integer& y) const;
    /**
     * Die Methode isPrime() implementiert den Miller-Rabin Primzahltest. Dieser Algorithmus ist ein Monte Carlo
     * Algorithmus. Ein Monte Carlo Algorithmus ist ein Algorithmus, der folgende Eigenschaften besitzt: Der
     * Algorithmus bekommt als Eingabe eine beliebig lange Bitfolge und gibt als Ergebnis 0 oder 1 zurück; der
     * Algorithmus hat Zugriff auf einen Zufallsgenerator, der Zufallsbits zurückliefert, die gleichverteilt sind; der
     * Algorithmus berechnet das korrekte Ergebnis, mit der Wahrscheinlichkeit von (1/2) + Ɛ, wobei Ɛ > 0.
     * Der Miller-Rabin Primzahltest besitzt einen einseitigen Fehler. Wird eine Zahl von ihm als eine zusammengesetzte
     * Zahl eingestuft, dann ist das Ergebnis zu 100 Prozent korrekt. Wird eine Zahl jedoch als Primzahl erkannt, dann
     * ist das Ergebnis mit einer Wahrscheinlichkeit von 1-2^(-s) korrekt. Diese Fehlerwahrscheinlichkeit wird durch
     * den Qualitätsparameter s beeiflusst, der angibt, wie viele zufällige Zahlen zwischen 1 und n-1 erzeugt werden
     * und daraufhin überprüft werden, ob sie ein Solider Beweis sind, dass n eine zusammengesetzte Zahl ist.Die
     * Laufzeit des Miller-Rabin Primzahltests beträgt O(log₂(n)³ * s). Eine Vorraussetzung für den Einsatz des
     * Miller-Rabin Primzahltests sind kryptographisch gute Zufallszahlen.
     * @param rng Zufallsgenerator mit dem die benötigten Zufallszahlen erzeugt werden
     * @param n Integer, für den untersucht werden soll, ob es sich um eine Primzahl oder um eine zusammengesetze
     * Zahl handelt
     * @param s Qualitätsparameter s, der angibt, wie oft die Schleife durchlaufen werden soll
     * @return True, wenn die Zahl eine Primzahl ist, False wenn die Zahl eine zusammengesetze Zahl ist
     */
	bool isPrime(PRNG* rng, const Integer& n, unsigned int s) const;

};

#endif /* MILLERRABINTEST_H_ */
