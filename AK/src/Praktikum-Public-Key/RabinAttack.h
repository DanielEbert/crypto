/*
 * RabinAttack.h
 */

#ifndef RABINATTACK_H_
#define RABINATTACK_H_

#include "integer.h"
#include "RabinDecryptor.h"

using namespace CryptoPP;

/**
 * Das Rabin-System ist anfällig gegen eine Attacke mit frei wählbarem
 * Geheimtext.
 * Die Klasse RabinAttack bietet eine Methode an, mit der das Rabin
 * Kryptosystem gebrochen werden kann, unter der Voraussetzung, dass ein
 * frei zu wählender Geheimtext mit dem zu brechenden Kryptosystem bzw. deren
 * Schlüsseln/Parametern p und q entschlüsselt werden darf.
 */
class RabinAttack {
public:
	RabinAttack();
	virtual ~RabinAttack();

	/**
	 * Das Rabin-System ist anfällig gegen eine Attacke mit frei
     * wählbarem Geheimtext. Anfällig bedeutet, dass ohne Kenntniss von p und q
     * und nur durch Kenntniss von n ein p oder q bestimmt werden kann. Mit
     * anderen Worten, dass n faktorisierbar ist. Kann man n faktorisieren,
     * dann kann man das Rabin Kryptosystem brechen und verschlüsselte
     * Nachrichten brechen/entschlüsseln, weil p und q Teil des privaten Schlüssels
     * sind. Pro Versuch liegt die Erfolgswahrscheinlichkeit bei 50%. Durch
     * wiederholte Versuche wird die Erfolgswahrscheinlichkeit erhöht. Mit
     * max_tries kann die Anzahl der Versuche bestimmt werden. Die
     * Erfolgswahrscheinlichkeit aller Versuche zusammen ist dann
     * 1-2^-max_tries. Das Vorgehen des Versuchs wird im Source Code erläutert.
     * Die Attacke darf einen frei wählbaren Geheimtext mit dem zu brechenden
     * RabinDecryptor entschlüsseln. Der zu brechende RabinDecryptor wird
     * im 4. Parameter übergeben. Im ersten Parameter wird das n, der
     * öffentliche Teil des Schlüssels des RabinEncryptors des 4. Parameters
     * spezifiziert. Bei Erfolg wird ein Faktor von n in das f, also den
     * zweiten Parameter, geschrieben und die Anzahl der Versuche, die
     * benötigt wurden, um n zu Faktorisieren, wird als int zurückgegeben.
     * Wenn n nicht faktorisiert worden ist, dann wird -1 zurückgegeben.
     * Der Angriff wird detailliert im .cpp Code erläutert.
     * In dieser Implementierung wird davon ausgegangen, dass nur einer der
     * 4 möglichen Klartexte des frei wählbarem Geheimtext bekannt sind. Wären
     * alternativ alle 4 möglichen Klartexte bekannt, dann würde der Angriff
     * direkt zum Erfolg führen. Der Angriff kann verhindert werden, indem man
     * den Klartext mit einer eindeutigen, wiedererkennbaren Markierung
     * versieht, weil dann nicht nur der Klartext x, sondern x||padding
     * verschlüsselt wird.
     *
	 * @param n Das zu faktorisierende n als Integer.
	 * @param f In f wird bei erfolgreicher Faktorisierung von n ein Faktor
	 * von f als Integer geschrieben.
	 * @param max_tries Die maximale Anzahl der Versuche als int. Mit anderen
	 * Worten: Konnte nach max_tries Versuchen nicht faktorisiert werden, dann
	 * wird die Attacke abgebrochen. max_tries gibt auch ebenfalls die
	 * Erfolgswahrscheinlichkeit für die Attacke an, die bei 1-2^-max_tries
	 * liegt.
	 * @param rabin_dec Eine Instanz der Klasse RabinEncryptor, die als
	 * öffentlichen Schlüssel ebenfalls n hat. Mit anderen Worten, mussten die
	 * zwei Faktoren von n als p und q an den Konstruktor dieser RabinDecryptor
	 * Instanz übergeben worden sein.
	 * @return Bei Erfolg die Anzahl der Versuche als int, die benötigt wurden,
	 * um n zu Faktorisieren. Wenn n nicht faktorisiert worden ist, dann wird -1
	 * zurückgegeben.
	 */
	int factorize(const Integer& n, Integer& f, int max_tries, RabinDecryptor& rabin_dec);
};

#endif /* RABINATTACK_H_ */
