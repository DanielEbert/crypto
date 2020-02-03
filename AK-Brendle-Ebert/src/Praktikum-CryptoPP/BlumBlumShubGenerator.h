#ifndef _BlumBlumShubGenerator_h_
#define _BlumBlumShubGenerator_h_

#include "integer.h" // Integer
#include "PRNG.h"

using namespace CryptoPP;

/**
 * Der BlumBlumShubGenerator ist eine Klasse für das
 * Erzeugen von (pseudo) zufälligen Bits. Die erzeugten Bits
 * hängen von den Parametern n (mod n) und s (seed) ab.
 * Das heißt, dass bei Verwendung der gleichen Parametern die gleiche
 * Zufallszahlenfolge erzeugt wird (die Bits werden deterministisch erzeugt).
*/
class BlumBlumShubGenerator : public PRNG {
private:
	Integer n;
	Integer s;

public:
    /**
     * Einziger Konstruktor der BlumBlumShubGenerator Klasse.
     * Der Übergabeparameter n ist vom Typ Integer und
     * sollte das Produkt von zwei Primzahlen sein.
     * Der Übergabeparameter seed ist vom Typ Integer und
     * sollte nicht 0 oder 1 sein.
     *
     * @param n Ein "Integer" Objekt der CryptoPP Bibliothek
     *
     * @param seed Ein "Integer" Objekt der CryptoPP Bibliothek
     */
	BlumBlumShubGenerator(const Integer& n, const Integer& seed);

	/**
	 * Generiert ein (pseudo) zufälliges, gleichverteiltes
	 * Bit und gibt dieses in Form eines bools zurück.
	 *
	 * @return Gleichverteilt entweder einen bool mit 'True' oder 'False'
	 */
	virtual bool getBit();

};

#endif
