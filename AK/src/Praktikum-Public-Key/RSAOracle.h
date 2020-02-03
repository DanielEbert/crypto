/*
 * RSAOracle.h
 */

#ifndef RSAORACLE_H_
#define RSAORACLE_H_

#include "integer.h"

using namespace std;
using namespace CryptoPP;

/**
 * Diese Klasse implementiert das RSA-Orakel, welches für die Half-Attack benötigt wird.
 * Das Orakel besitzt kenntnis über den geheimen RSA-Schlüssel und ist so in der Lage,
 * Aussagen über das höchstwertigste Bit eines Klartexts mit dem dazugehörigen
 * Geheimtexts zu treffen.
 */
class RSAOracle {
private:
	Integer n;
	Integer d;

public:
	/***
	* Der Konstruktor der Klasse weißt die übergebenen Parametern den entsprechenden
	* Attributen des erzeugten Objekts zu. Dabei wird n = p*q und this->d = d zugewiesen.
	*
	* @param p Primzahl p, die zum Berechnen von n des RSA-Schlüssels eingesetzt wird
	* @param q Primzahl q, welche zur Berechnung von n des RSA-Schlüssels eingesetzt wird
	* @param d Parameter d des geheimen Teils des RSA-Schlüssels, über den der Klartext
	* berechnet wird
	*/
	RSAOracle(Integer& p, Integer& q, Integer& d);
	virtual ~RSAOracle();

	/***
	* Die Methode half() wird zur Durchführung einer Half-Attack auf RSA verwendet. Diese Methode
	* bekommt einen Geheimtext übergeben und liefert den Wert des höchstwertigen Bits des
	* entschlüsselten Geheimtext in y zurück. Dabei wird angenommen, dass der entschlüsselte Geheimtext die
	* gleiche Bitlänge wie n besitzt. Er wird also wenn nötig links mit Nullen aufgefüllt/gepadded.
	* Da keine solche Funktion existiert, welche diese
	* Funktionalität in effizienter Zeit mit dem öffentlichen Schlüssel berechnet, besitzt das Orakel
	* in dieser Implementierung Wissen über den privaten Teil des RSA-Schlüssels, um den Geheimtext
	* entschlüsseln zu können und auf diese Weise das höchstwertigste Bit zu bestimmen.
	*
	* @param y Geheimtext, für den der Wert des höchstwertigsten Bits des zugehörigen Klartextes
	* bestimmt werden soll
	*
	* @return True, falls das höchstwertigste Bit des zugehörigen Klartexts 1 ist, ansonsten False.
	*/
	bool half(const Integer& y) const;

	bool parity(const Integer& y) const;

};

#endif /* RSAORACLE_H_ */
