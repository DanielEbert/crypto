/*
 * FortunaGenerator.h
 */

#ifndef FORTUNAGENERATOR_H_
#define FORTUNAGENERATOR_H_

#include <sys/time.h>
#include <stdint.h>
#include <time.h>
#include "PRNG.h"

/**
 *
 * Diese Klasse implementiert den von Niels Ferguson und Bruce Schneier entwickelten Pseudo Random Number Generator
 * Fortuna. Dieser Zufallsgenerator generiert beliebig lange Byte-Folgen, indem er eine Block-Chiffre, in einem zum
 * Counter-Mode ähnlichen Modus, verwendet. Dafür werden die Blockchiffre AES und die Hash-Funktion SHA-256 eingesetzt.
 * Der interne Zustand des Fortuna-Generators besteht aus einem 256-Bit langen Schlüssel und einem 128-Bit langen
 * Zähler.
 */
class FortunaGenerator: public PRNG {

private:
    /**
     * 32-Byte langer Schlüssel der zum Verschlüsseln des Counters mit der Blockchiffre AES zum Erzeugen von
     * Zufallsbytes verwendet wird
     */
	byte key[32];
	/**
	 * 16-Byte langer Zähler, der zum erzeugen von Zufallsbytes mit key verschlüsselt und anschließend inkrementiert
	 * wird. Dieses Byte-Array wird dazu als Zahl in Least-Significant-Byte-First Darstellung interpretiert
	 */
	byte cntr[16];

	/**
	 * Das Flag seeded gibt an, ob bereits ein Reseed des Fortuna Generators stattgefunden hat. Nach Erzeugung eines
	 * Fortuna-Generator-Objekts besitzt es den Wert False, nach dem ersten Reseeding den Wert true
	 */
	bool seeded;

	/**
	 * Zeitstempel des letzten Reseedings in Millisekunden ab dem Unix epoch modulo 2^32
	 */
	uint32_t last_reseed;

protected:

	uint32_t getTimeStamp() const;

    /**
     * Die Methode generateBlocks() erzeugt n 16-Byte Blöcke. Dazu prüft der Fortuna-Generator zunächst, ob bereits ein
     * Reseeding stattgefunden hat, oder ob das letzte Reseeding länger als 500 Millisekunden zurückliegt. Ist dies der
     * Fall wird ein Reseeding durchgeführt. Als Seed hierfür werden 32 Byte aus /dev/urandom gelesen und damit die
     * Methode reseed() aufgerufen.
     * Das Erzeugen von n 16-Byte großen Blocks findet folgendermaßen statt:
     * Zunächst wird der Buffer, in dem die Pseudozufallsbytes gespeichert werden sollen, vom
     * Funktionsaufrufenden  mit mindestens 16*n Bytes reserviert. Anschließend wird eine
     * Schleife n-mal durchlaufen. Innerhalb eines Schleifendurchlaufs wird der
     * Counter mit dem AES verschlüsselt und der auf diese Weise erzeugte Block an den bisher erzeugten Buffer
     * angehängt. Am Ende eines Schleifendurchlaufs wird der Wert des Counters um 1 erhöht. Diese Abfolge von Schritten
     * findet n mal statt. Der Fortuna-Generator erzeugt immer 16 Bytes mit einer AES-Verschlüsselung auf einmal, da es
     * sich dabei um die Blocklänge des AES handelt und die Erzeugung der Bytes durch die Verschlüsselung mit dem AES
     * stattfindet. Das aufrufende Programm muss beim Aufruf dieser Methode sicherstellen, dass für das Byte-Array
     * buffer ausreichend Speicher allokiert wurde.
     * @param buffer In diesem Byte-Array werden die erzeugten Blöcke gespeichert
     * @param n Anzahl der zu erzeugenden 16 Byte großen Blöcke
     */
	void generateBlocks(byte* buffer, unsigned int n);

public:
    /**
     * Konstruktor der Klasse FortunaGenerator. Initialisiert den internen Zustand (key und cntr) mit 0 und setzt den
     * Boolean seeded auf false. In diesem initialisierten Zustand kann der Generator noch nicht eingesetzt werden. Dafür
     * muss zuerst ein Reseeding stattgefunden haben.
     */
	FortunaGenerator();

	/**
	 * Inkrementiert den Counter des internen Zustandes um eins. Dabei wird vom Least-Significant-Byte in Richtung des
	 * Most-Significant-Byte vorgegangen. Das erste Byte wird inkrementiert und falls es dabei zu einem Überlauf
	 * gekommen ist wird auch das nächste Byte inkrementiert. Dieses Vorgehen wird solange wiederholt, bis es bei
	 * einem Byte nicht zum Overflow gekommen ist oder es einen Überlauf am Most-Significant-Byte gegeben hat.
	 * @return false, wenn es beim inkrementieren des Counters zu einem Überlauf gekommen ist, ansonsten true
	 */
	bool incCntr();

	/**
	 * Führt ein Reseeding für den Fortuna Generator durch. Während einem Reseedings wird der interne Zustand des
	 * PRNGs verändert. Dazu wird der neue Key gleichgesetzt mit der SHA-256 Hashsumme der Byte-Array-Konkatenation
	 * des alten Schlüssels und des übergebenen Seeds. Anschließend wird der Wert des Counters um 1 erhöht. Zuletzt wird
	 * das Flag reseeded auf true gesetzt, um zu markieren, dass der Pseudozufallsgenerator einsatzbereit ist, und der
	 * Zeitstempel last_reseed wird auf die aktuelle Zeit in Millisekunden ab dem Unix epoch modulo 2^32 gesetzt.
	 * @param seed Byte-Array, welches den Seed enthält, der zum Reseeding eingesetzt werden soll
	 * @param size Größe des Byte-Arrays seed
	 */
	void reseed(byte* seed, unsigned int size);

	/**
	 * getBit() liefert ein mit dem Fortuna Generator zufällig erzeugtes Bit zurück
	 * @return Das erzeugte Bit als boolscher Wert
	 */
	virtual bool getBit();

	/**
	 * getByte() liefert ein mit dem Fortuna Generator zufällig erzeugtes Byte zurück
	 * @return Wert des erzeugten Bytes
	 */
	virtual byte getByte();

	/**
	 * Erzeugt n Zufallsbytes und speichert diese im Byte-Array buffer. Die Anzahl der generierten Bytes muss dabei
	 * zwischen 0 (inklusiv) und 2^20 (exklusiv) liegen. Ansonsten werden keine Bytes im buffer zurückgelifert. Der
	 * Fortuna Generator liefert auf einmal maximal diese Menge an Bytes, um sicherstellen zu können, dass die erzeugte
	 * Byte-Folge zufällig aussieht und keine statistischen Besonderheiten aufweist. Die Anzahl an zu erzeugenden Bytes
	 * muss hierbei kein vielfaches von 16 sein. Anschließend wird key auf den Wert
	 * von zwei weiteren zufällig erzeugten Blöcken gesetzt
	 *
	 * @param buffer Byte-Array, in dem die erzeugten Zufallsbytes gespeichert werden sollen
	 * @param n Anzahl der zu erzeugenden Bytes
	 */
	virtual void getBlock(byte* buffer, unsigned int n);

};

#endif /* FORTUNAGENERATOR_H_ */
