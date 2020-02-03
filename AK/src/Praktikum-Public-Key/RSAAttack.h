/*
 * RSAAttack.h
 */

#ifndef RSAATTACK_H_
#define RSAATTACK_H_

#include "integer.h"
#include "RSAOracle.h"

using namespace CryptoPP;

/**
 * Die Klasse RSAAttack stellt mehrere Methoden zur Verfügung, in denen Angriffe
 * auf das RSA-Kryptosystem implementiert sind. Bei diesen Angriffen handelt es
 * sich um factorize n, einem Angriff der n anhand des bekannten ϕ(n) faktorisiert,
 * die Wiener Attacke, die ebenfalls n faktorisieren kann, wenn die Parameter des
 * öffentlichen Schlüssels ungünstig gewählt sind, sowie die nur theorethisch 
 * durchführbare Half-Attack, über die der zum Geheimtext zugehörige Klartext
 * berechnet werden könnte, falls eine Funktion existieren würde, die für einen
 * Geheimtext das höchstwertigste Bit seiner Binärdarstellung in Zn berechnen
 * kann.
 */
class RSAAttack {
public:
	RSAAttack();
	virtual ~RSAAttack();

	/***
	 * factorizeN berechnet die beiden Primfaktoren p und q von n anhand von ϕ(n). Das
	 * bedeutet, dass wenn ϕ(n) bekannt ist, n gebrochen werden kann. n ist
	 * Teil des öffentlichen Schlüssels.
	 * Für diese Attacke muss n und ϕ(n) bekannt sein, mit
     * (1) n = p * q und (2) ϕ(n) = (p-1) * (q-1). ϕ(n) kann durch diese Formel
     * berechnet werden, da n das Produkt zweier Primzahlen ist. Da zwei Primzahlen
     * auf jeden Fall teilerfremd zueinander sind kann der zahlentheorethische Satz
     * (Zahlentheorie Vorkurs Satz 8.3) ϕ(p*q) = ϕ(p) * ϕ(q) angewendet werden,
     * wobei für Primzahlen gilt ϕ(p) = p - 1. 
     * Mit bekannten p und q kann RSA gebrochen werden, also es können verschlüsselte
     * Nachrichten entschlüsselt werden.
     * Grundlage der Berechnung ist, dass Gleichung (1) in Gleichung (2) mit p = n / q eingesetzt wird:
     * ϕ(n) = (p-1) * (n/q - 1)
     * Diese Gleichung kann in eine Form gebracht werden, um p und q mit der
     * Mitternachtsformel zu berechnen:
     * 0 = p² + (ϕ(n) − n − 1)p + n
     * Bei der Mitternachtsformel ist a = 1, b = ϕ(n) - n - 1 und c = n
	 * @param n n ist ein Integer und Teil des öffentlichen zu brechenden
	 * Schlüssels.
	 * @param phi_n  ϕ(n), wobei  ϕ(n) = (p-1)*(q-1) ist.  ϕ(n) kann ohne
	 * Kenntniss von p und q nicht effizient berechnet werden.
	 * @param p In p wird bei Erfolg ein Faktor von n als Integer geschrieben.
	 * @param q In q wird bei Erfolg der andere Faktor von n als Integer
	 * geschrieben.
	 * @return True, wenn erfolgreich n faktorisiert wurde, false wenn nicht.
	 */
	bool factorizeN(const Integer& n, const Integer& phi_n, Integer& p, Integer& q) const;

	/***
	* Diese Methode implementiert die Wiener-Attacke von Michael J. Wiener, mit der das n
	* eines RSA-Schlüssels faktorisiert werden kann, wenn dessen Parameter ungünstig gewählt
	* sind. Dieser Angriff basiert auf endlichen Kettenbrüchen und einem zahlentheoretischen
	* Satz, der aussagt: Sind a,b,c,d beliebige natürliche Zahlen und gcd(a,b) = 1 und 
	* gcd(c,d) = 1 und |(a/b)-(c/d)| < (1/2d²). Unter diesen Bedingung ist c/d eine der 
	* Konvergenten des Kettenbruchs a/b. Daraus folgt, dass der Exponent d des privaten
	* RSA-Schlüssels berechnet werden kann, wenn q < p < 2q und 3d < n^(1/4). Das bedeutet,
	* dass die Wiener-Attacke erfolgreich ist, wenn die beiden Primzahlen p und q nicht zu
	* sehr von einander entfernt sind und die Binärdarstellung von d kürzer als ein Viertel
	* der Binärdarstellung von n minus 1 groß ist. Für den Angriff werden die beiden Komponenten
	* n und e des öffentlichen RSA-Keys benötigt. Zuerst werden die Konvergenten des Kettenbruchs
	* e/n berechnet. Dies geschieht über die computeConvergents() Methode der Klasse 
	* PublicKeyAlgorithmBox. Anschließend wird für jeden Näherungsbruch c[i]/d[i] der Konvergenten
	* beginnend mit i = 2 folgende beiden Bedingungen überprüft: 1. Ist ϕ(n) = (e*d[i]-1)/c[i] eine
	* Ganze Zahl 2. Ist n anhand vom errechneten ϕ(n) aus Bedingung 1 faktorisierbar. Dazu wird die
	* Methode factorizeN() eingesetzt. Treffen beide Bedingungen zu, dann konnten die beiden Primzahlen
	* p und q errechnet werden. Dieses Vorgehen führt zur Faktorisierung von n, da aus den 
	* Schlussfolgerungen des zahlentheoretischen Satzes ϕ(n) anhand der Konvergenten des Kettenbruchs 
	* von e/n berechnet werden kann. Schlagen die beiden Bedingungen für alle Näherungsbrüche fehl, dann war
	* der Angriff nicht erfolgreich und n konnte nicht faktorisiert werden.
	*
	* @param n n ist eine Integer-Variable und entspricht dem n des öffentlichen Teil des RSA-Schlüssels
	* @param e e ist eine Integer-Variable und muss dem e des öffentlichen Teils des RSA-Schlüssels entsprechen
	* @param p Im Parameter p wird die errechnete Primzahl p (sodass p*q = n) gespeichert, falls die Wiener-Attacke
	* erfolgreich ablief
	* @param q Im Parameter q wird die errechnete Primzahl p (sodass p*q = n) gespeichert, falls die Wiener-Attacke
	* erfolgreich ablief
	*
	* @return Die Methode liefert True zurück, wenn die Wiener-Attacke erfolgreich war und das n faktorisiert werden
	* konnte und False, wenn der Angriff fehlschlug
	*/
	bool wienerAttack(const Integer& n, const Integer& e, Integer& p, Integer& q) const;

	/***
	* Diese Methode implementiert den theoretischen Angriff Half-Attack auf das RSA-Kryptosystems, mit der es
	* möglich wäre über den Öffentlichen Schlüssel und einen damit verschlüsselten Ciphertext den zugehörigen
	* Klartext zu berechnen. Dieser Angriff wäre durchführbar, wenn die Funktion half(y) effizient berechenbar
	* wäre. Sei y = enc(K,x), dann ist half(y) = 0, wenn 0 <= x < n/2 bzw 1, wenn n/2 < x <= n-1. Die Funktion
	* half(y) müsste also den Wert des höchstwertigen Bits des Klartexts berechnen können. Würde eine solche
	* Funktion existieren, dann könnte der zugehörige Klartext zu einem Ciphertext berechnet werden. Dafür wird 
	* sich die Eigenschaft von RSA zunutze gemacht, dass für alle x1, x2 in Zn gilt, dass enc(K,x1*x2) = 
	* enc(K,x1) * enc(K,x2). Somit gilt auch für alle positiven i, dass enc(K,2^i * x) = enc(K,2^i) * enc(k,x).
	* Die Half-Attack läuft dann folgendermaßen ab: Zuerst wird mit der half(y)-Funktion das höchstwertigste
	* Bit des Klartexts errechnet. Dann wird y mit der über den öffentlichen Schlüssel RSA verschlüsselten 2 
	* multipliziert und das Ergebnis als neues y gesetzt. Dieses neue y ist also enc(K,2*x), also wurde der
	* Klartext um ein Bit nach links geshiftet. Für dieses neue y wird dann erneut die Half-Funktion 
	* aufgerufen. Dieses Verfahren wird für die gesamte Bitlänge von n wiederholt und die Bits abgespeichert.
	* Durch diese errechneten Bits, kann der Klartext x dann über eine Binäre Suche systematisch errechnet werden.
	* Da die Funktion half(y) in der Praxis nicht existiert, wurde in dieser Implementierung ein Orakel der
	* Klasse RSAOracle eingesetzt, welches den zur Entschlüsselung benötigten Parameter d besitzt und damit
	* half(y) berechnen kann, da es dazu in der Lage ist y zu entschlüsseln. Die Laufzeit der Half-Attack
	* beträgt O(l³ + l*t(l)), wobei t(l) für die Laufzeit von half(y) für Zahlen mit l-Bit steht.
	*
	* @param n Die Integer-Variable n entspricht dem Parameter n des öffentlichen Teils des RSA-Schlüssels
	* @param e Die Integer-Variable e entspricht dem Parameter e des öffentlichen Teils des RSA-Schlüssels
	* @param rsa_oracle Zeiger auf ein Objekt der Klasse RSAOracle, dass half(y) berechnen kann
	*
	* @return Die Methode gibt den Klartext x zurück, sodass y = x^e (mod n). Die Vorraussetzung hierfür ist
	* jedoch, dass rsa_oracle das richtige Ergebnis auf half(y) liefert.
	*/
	Integer halfAttack(const Integer& n, const Integer& e, const Integer& y, RSAOracle& rsa_oracle) const;

};

#endif /* RSAATTACK_H_ */
