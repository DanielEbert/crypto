#ifndef _PublicKeyAlgorithmBox_h_
#define _PublicKeyAlgorithmBox_h_

#include <vector>
#include "integer.h"

using namespace std;
using namespace CryptoPP;

/**
 * Die Klasse PublicKeyAlgorithmBox stellt diverse zahlentheoretische Algorithmen bereit.
 * Die Methoden dieser Klasse werden von anderen Klassen des Praktikum-Public-Key als
 * Hilfsfunktionen verwendet.
 */
class PublicKeyAlgorithmBox {
public:
	/**
	* In der Methode computeConvergents werden die Konvergenten des Kettenbruchs 
	* von a/b berechnet. Die Konvergenten eines Kettenbruchs können auch als 
	* Näherungsbruch bezeichnet werden. Sei [q1,...,qm] ein Kettenbruch. Für alle
	* j mit 1 <= j <= m gilt, dass der Kettenbruch Cj = [q1,...,qj] die j-te Konvergente
	* des ursprünglichen Kettenbruch ist. Cj kann durch den Bruch c[j]/d[j] dargestellt werden.
	* Dabei besitzt der Zähler c[j] die Werte: 1, wenn j = 0, q1, falls j=  1 und qj *c[j-1] +c[j-2], 
	* für alle j >= 2. Der Nenner d[j] besitzt hingegen die Werte: 0, wenn j = 0, 1, falls j = 1 und
	* qj * d[j-1] + d[j-2] für alle j >= 2.
	*
	* @param a Integer Variable, die den Nenner des Bruchs enthält
	* @param b Integer Variable, die den Zähler des Bruchs enthält
	* @param c Vector, in dem die von der Methode berechneten c[j] für 0 <= j <= m gespeichert
	* werden, welche jeweils den Zähler des j-ten Näherungsbruch repräsentieren.
	* @param d Vector, in dem die von der Methode berecheten d[j] für 0 <= j <= m gespeichert 
	* werden, welche jeweils den Nenner des j-ten Näherungsbruch repräsentieren.
	*
	* @return Die Länge des errechneten Vectors c
	*/
	unsigned int computeConvergents(const Integer& a, const Integer& b,
			vector<Integer>& c, vector<Integer>& d);

	/**
	 * Die Methode euklid berechnet den endlichen Kettenbruch a/b mit Hilfe des 
	 * euklidschen Algorithmus. Ein endlicher Kettenbruch ist ein Term, der
	 * durch q1 + (1/(q2 + (1/(q3 + ... + (1(qm))))) dargestellt werden kann,
	 * wobei q1 bis qm nicht-negative ganze Zahlen sind. Ein endlicher Kettenbruch
	 * kann auch als [q1,...,qm] dargestellt werden. Für alle Brüche a/b, mit 
	 * a,b ∈ natürliche Zahlen, mit b > 0, gilt, dass sie als endlicher Kettenbruch
	 * [q1,...,qm] dargestellt werden können. Dieser Kettenbruch kann mit Hilfe des
	 * Algorithmus von Euklid berechnet werden. Dabei repräsentiert jede errechnete
	 * Gleichung den Term r{m-1} = q{m}*r{m} + r{m+1}. In dieser Implementierung starten
	 * die Indexe der Zahlen eines Kettenbruchs mit 0, da diese innerhalb eines Vectors
	 * gespeichert werden. Die Berechnung des endlichen
	 * Kettenbruchs wird benötigt, um die Wiener-Attacke auf RSA durchzuführen.
	 *
	 * @param a Integer Variable, die den Zähler des Bruchs enthält
	 * @param b Integer Variable (b > 0), die den Nenner des Bruchs enthält
	 * @param q Vector in dem die Integer-Werte des Kettenbruchs [q1,...,qm] abgespeichert
	 * werden
	 * @return Berechneter gcd(a,b), welches das letzte Element des Kettenbruchs ist
	 */
	Integer euklid(const Integer& a, const Integer& b, vector<Integer>& q);

	/**
	 * EEA realisiert den Erweiterten Algorithmus von Euklid. Dabei wird der
	 * größte gemeinsame Teiler von a und b berechnet und in d gespeichert.
	 * Zusätzlich gilt, dass der größte gemeinsame Teiler von a und b das
	 * kleinste positive Element der Menge {ax + by | x, y ∈ Z} aller
	 * Linearkombinationen von a und b ist. Mit dem EEA wird zusätzlich dieses
	 * x und y berechnet und in (param) x und (param) y abgespeichert.
	 * Die Methode liefert True zurück, wenn a und b teilerfremd sind, also
	 * wenn gcd(a, b) gleich 1 ist.
	 *
	 * @param a Eine ganze Zahl > 0 als Integer
	 * @param b Eine ganze Zahl > 0 als Integer
	 * @param d Diese Methode setzt d auf den größten gemeinsamen Teiler
	 * von a und b.
	 * @param x Diese Methode setzt x auf das x der oben erläuterten
	 * Linearkombination von  {ax + by | x, y ∈ Z}.
	 * @param y Diese Methode setzt y auf das y der oben erläuterten
	 * Linearkombination von {ax + by | x, y ∈ Z}.
	 * @return True, wenn d = 1, also wenn gcd(a,b)=1 (und die beiden
	 * Integer teilerfremd sind), sonst False.
	 */
	bool EEA(const Integer& a, const Integer& b, Integer& d,
			Integer& x, Integer& y);

	/**
	 * modularExponentation implementiert den Algorithmus zur modularen Exponentation. Dabei
	 * wird die Funktion a^b mod n effizient berechnet und das Ergebnis wird über
	 * den Rückgabewert als Integer zurückgeliefert.
	 *
	 * @param a a für a^b mod n als Integer.
	 * @param b b für a^b mod n als Integer.
	 * @param n c für a^b mod n als Integer.
	 * @return Das Ergebnis aus a^b mod n als Integer.
	 */
	Integer modularExponentation(const Integer& a, const Integer& b,
			const Integer& n);

	/**
	 * multInverse berechnet das multiplikative Inverse von a modulo n,
	 * vorausgesetzt ein solches existiert. Für ein multiplikatives Inverses a_inv 
	 * von a gilt, dass a * a_inv ≡ 1 (mod n), da 1 dem neutralen Element der 
	 * Multiplikation (mod n) entspricht. Die Methode liefert True
	 * zurück, wenn ein solches multiplikatives Inverses existiert und
	 * False wenn nicht.
	 * Ein a ist invertierbar modulo n, wenn gcd(a,n) = 1, also a und n teilerfremd
	 * zu einander sind.
	 *  gcd(a,n) kann auch mit
     * dem Erweiterten Algorithmus von Euklid berechnet werden.
     * Wegen ZTK Satz 3.4 gibt es x,y ∈ Z, so dass gcd(a, n) = 1 = ax + ny.
     * Somit gilt 1 ist kongruent zu ax+ny (mod n). Wegen mod n kürzt sich das
     * ny raus und ist somit das gleiche wie 1 ist kongruent zu ax (mod n).
     * Dadurch ist x das multiplikative Inverse von a mod n und dieses x kann
     * ebenfalls durch den Erweiterten Euklidschen Algorithmus berechnet werden.
     * Es werden keine negativen Inversen zurückgegeben.
	 *
	 * @param a Das a, für welches das multiplikative Inverse a mod n berechnet
	 * wird. a ist ein Integer.
	 * @param n Das n, für welches das multiplikative Inverse a mod n berechnet
	 * wird. n ist ein Integer.
	 * @param a_inv a_inv ist ein Integer, in welchem diese Funktion das
	 * multiplikative Inverse von a modulo n speichert, wenn ein solches
	 * multiplikatives Inverses existiert.
	 * @return True, wenn ein multiplikatives Inverses von a mod n  existiert.
	 * False, wenn nicht.
	 */
	bool multInverse(const Integer& a, const Integer& n, Integer& a_inv);

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
	bool witness(const Integer& a, const Integer& n);

    /**
     * Die Methode millerRabinTest() implementiert den Miller-Rabin Primzahltest. Dieser Algorithmus ist ein Monte Carlo
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
     * @param n Integer, für den untersucht werden soll, ob es sich um eine Primzahl oder um eine zusammengesetze
     * Zahl handelt
     * @param s Qualitätsparameter s, der angibt, wie oft die Schleife durchlaufen werden soll
     * @return True, wenn die Zahl eine Primzahl ist, False wenn die Zahl eine zusammengesetze Zahl ist
     */
	bool millerRabinTest(Integer& n, unsigned int s);

	/**
	 * randomInteger liefert einen zufälligen Integer
	 * im Bereich 0,1,...,n-1 zurück.
	 *
	 * @param n n gibt die Obergrenze des zu generierenden Integers an.
	 * n ist exklusiv, das heißt der generierte Integer ist < n.
	 * n ist vom Typ Integer und muss größer als 0 sein.
	 * @return Einen zufälligen Integer im Bereich 0,1,...,n-1
	 */
	Integer randomInteger(const Integer& n);

	/**
	 * randomPrime generiert eine Zahl, die mit Wahrscheinlichkeit 1-2^-s eine Primzahl ist
	 * und speichert diese Zahl im Parameter p ab. Die generierte Zahl hat ebenfalls eine
	 * maximale Größe in Binardarstellung von bitlen Ziffern/Bits. Die generierte Zahl wird
	 * mit einem Nicht Blockierenden Pseudozufallszahlengenerator generiert und mit
	 * dem Rabin Miller Test überprüft, ob es sich dabei mit Wahrscheinlichkeit 1-2^-s um 
	 * eine Primzahl handelt.
	 *
	 * @param p In p wird die generierte Zahl als Integer abgespeichert.
	 * @param bitlen bitlen gibt die Größe der zu erzeugenden Zahl in Bits an.
	 * @param s s ist das s in 1-2^-s, welches die Wahrscheinlichkeit dafür angibt,
	 * dass p tatsächlich eine Primzahl ist.
	 * @return randomPrime gibt die Anzahl der generierten Zufallszahlen zurück,
	 * die überprüft worden sind, bis eine dieser Zufallszahlen als wahrscheinlich
	 * Prime evaluiert wurde.
	 */
	unsigned int randomPrime(Integer &p, unsigned int bitlen, unsigned int s);

	/**
	 * randomRabinPrime generiert mit Fehlerwahrscheinlichkeit 2^-s eine
	 * Zufallsprimzahl mit folgenden Eigenschaften:
	 * - Die Binärdarstellung von p hat höchstens bitlen Bits.
	 * - p ist eine Rabin-Primzahl, ð, für die gilt p ≡ 3 (mod 4).
	 * Die Methode speichert die erzeugte Rabin-Primzahl in p ab.
	 *
	 * @param p In Variable p wird die generierte Rabin-Primzahl abgespeichert.
	 * @param bitlen bitlen gibt die maximale Größe der zu erzeugenden Primzahl
	 * in Bits an.
	 * @param s s ist das s in 2^-s, welches die Fehlerwahrscheinlichkeit
	 * angibt, dass der zurückgelieferte Wert in p keine Primzahl ist.
	 * @return Die Anzahl der Versuche, bis eine Rabin Primzahl gewürfelt
	 * worden ist.
	 */
	unsigned int randomRabinPrime(Integer &p, unsigned int bitlen,
			unsigned int s);

	/**
	 * modPrimeSqrt berechnet die zwei Quadratwurzeln von y mod p, falls p
	 * kongruent zu 3 (mod 4) ist. Denn ist p eine Primzahl mit dieser
	 * Eigenschaft, dann ist y ein quadratischer Rest modulo p und die
	 * Gleichung y² = a (mod p) besitzt 2 Lösungen. Diese können durch
	 * (+-)a^((p-1)/4) mod p berechnet werden. Das berechnete Ergebnis wird im
	 * im Vektor v gespeichert.
	 *
	 * @param y Das y bei y mod p, für welches die Quadratwurzeln berechnet
	 * werden. y ist ein Integer.
	 * @param p Das p bei y mod p, für welches die Quadratwurzeln berechnet
	 * werden. p ist ein Integer. Es werden nur Quadratwurzeln berechnet, wenn
	 * gilt, dass p kongruent zu 3 (mod 4) ist. Für ein richtiges Ergebnis muss
	 * p eine Primzahl sein.
	 * @param v v ist ein Vector aus Integern, in welchem die zwei
	 * Quadratwurzeln von y mod p gespeichert werden, falls p
	 * kongruent zu 3 (mod 4) ist, da dann genau zwei Quadratwurzeln für y
	 * existieren.
	 *
	 * @return True, falls die Quadratwurzeln berechnet wurden, ansonsten False.
	 * Die Quadratwurzeln können von dieser Funktion berechnet werden, falls
	 * p kongruent zu 3 (mod 4) ist.
	 */
	bool modPrimeSqrt(const Integer& y, const Integer& p, vector<Integer>& v);

	/**
	 * sqrt versucht die kleinste nicht-negativen ganzzahligen Quadratwurzel s
	 * von x zu berechnen. Wird diese gefunden, dann wird true zurückgegeben.
	 * Andernfalls wird false zurückgegeben. Der Algorithmus sucht in
	 * Logarithmischer Zeit mit der binären Suche nach s.
	 *
	 * @param x Die Zahl als Integer, für welche die kleinste nicht negative
	 * ganzzahlige Quadratwurzel gesucht werden soll.
	 * @param s Wird die kleinste nicht negative ganzzahlige Quadratwurzel von x
	 * gefunden, dann wird diese von dieser Methode in s geschrieben.
	 * @return True, falls die kleinste nicht negative ganzzahlige Quadratwurzel
	 * gefunden wurde, false wenn nicht.
	 */
	bool sqrt(const Integer& x, Integer& s) const;

	/**
	 * generateRSAParams generiert Werte für einen RSA-Schlüssel. Speziell
	 * besteht der RSA Schlüssel K aus n, p, q, e, d. n ist das Produkt
	 * aus den Primzahlen p * q und kann somit ebenfalls aus den Werten p und q berechnet
	 * werden. n und e bilden den öffentlichen Schlüssel, p, q und d den
	 * privaten Teil des Schlüssels. Es wird ebenfalls sichergestellt, dass
	 * p != q, da sonst n einfach durch die Wurzel faktorisierbar ist.
	 * Für die Sicherheit von n darf n nicht einfach faktorisierbar sein.
	 *
	 * @param p p ist eine bitlen-Bit Zufallszahl, die mit einer
	 * Wahrscheinlichkeit von 1-2^-s eine Primzahl ist.
	 * p wird von dieser Methode gesetzt.
	 * @param q q ist eine bitlen-Bit Zufallszahl, die mit einer
	 * Wahrscheinlichkeit von 1-2^-s eine Primzahl ist.
	 * q wird von dieser Methode gesetzt.
	 * @param e e ist eine Zufallszahl mit der Eigenschaft,
	 * dass der größte gemeinsame Teiler von e und ϕ(n) = 1 ist.
	 * e wird von dieser Methode gesetzt.
	 * @param d d ist das multiplikative Inverse von e mod ϕ(n)
	 * d wird von dieser Methode gesetzt.
	 * @param bitlen bitlen gibt die maximale Größe der zu generierenden Zufallszahlen
	 * p und q in Binärdarstellung an. bitlen kann vom Caller gesetzt werden und ist standardmäßig auf 256 gesetzt.
	 * @param s s ist das s in 2^-s, welches die Fehlerwahrscheinlichkeit dafür
	 * angibt, dass die generierte Zahl p keine Primzahl ist. Das selbe ist für
	 * q der Fall. s kann vom Caller gesetzt werden und ist standardmäßig auf 30 gesetzt.
	 */
	void generateRSAParams(Integer& p, Integer& q, Integer& e, Integer& d,
			unsigned int bitlen = 256, unsigned int s = 30);
};

#endif
