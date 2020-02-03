/*
 * PRNG.h
 *
 *  Created on: 13.04.2011
 *      Author: chkarg
 */

#ifndef PRNG_H_
#define PRNG_H_

#include "integer.h" // byte

using namespace CryptoPP;

class PRNG {
public:
    /**
	 * Generiert ein (pseudo) zufälliges, gleichverteiltes
	 * Bit und gibt dieses in Form eines bools zurück.
	 *
	 * @return Gleichverteilt ein zufälliges Bit in Form eines booleans.
	 */
	virtual bool getBit() = 0;

    /**
     * Generiert ein (pseudo) zufälliges Byte und gibt dieses in Form
     * eines byte Wertes zurück. Die möglichen erzeugten Werte des Bytes
     * sind gleichverteilt.
     *
     * @return Gleichverteilt ein zufälliges Byte in Form eines char
     * beziehungsweise eines 'byte' alias.
     */
	virtual byte getByte();

    /**
     * Generiert ein (pseudo) zufälliges Array von n (param) Bytes.
     * Die möglichen erzeugten Werte der Bytes sind gleichverteilt
     * und werden an die von 'buffer' (param) verweisende Stelle geschrieben.
     *
     * @param buffer Ein Buffer mit einer Größe von (mindestens) (param) n
     * Bytes.
     *
     * @param n Die Anzahl der zu generierenden Bytes, welche in (param) buffer
     * abgespeichert werden. (param) n sollte nicht größer als der für
     * (param) buffer reservierte Speicherplatz in Bytes sein.
     */
	virtual void getBlock(byte* buffer, unsigned int n);

    /**
     * Generiert eine Zufallszahl, die eine Größe von (param) size Bytes hat.
     * Die möglichen erzeugten Werte der Zufallszahl sind gleichverteilt
     * und werden in Form eines Integers zurückgegeben.
     *
     * @param size Eine Positive Ganze Zahl in Form eines unsigned int
     *
     * @return Eine (param) size Byte große Zufallszahl in Form eines
     * "Integer" Objekts der CryptoPP Bibliothek
     */
	virtual Integer getInteger(unsigned int size);

    /**
     * Generiert eine Zufallszahl aus der Menge {0, 1,..., m-1}.
     * Der Übergabeparameter muss ein Integer sein, der größer als 0 ist.
     * Die möglichen erzeugten Werte der Zufallszahl sind gleichverteilt
     * und werden in Form eines Integers zurückgegeben.
     *
     * @param m Eine positive ganze Zahl in Form eines "Integer" Objekts
     * der CryptoPP Bibliothek
     *
     * @return Eine Zufallszahl aus der Menge {0, 1,..., m-1} in Form
     * eines "Integer" Objekts der CryptoPP Bibliothek
     */
	virtual Integer getInteger(const Integer& m);
};

#endif /* PRNG_H_ */
