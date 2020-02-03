/*
 * FortunaGenerator.cpp
 *
 *  Created on: 20.04.2011
 *      Author: chkarg
 */

#include <iostream>
#include <fstream>
#include "filters.h"
#include "sha.h"
#include "aes.h"
#include "modes.h"
#include "hex.h"
#include "files.h"
#include "FortunaGenerator.h"

using namespace std;

FortunaGenerator::FortunaGenerator() {
	for (int i = 0; i < sizeof(key); i++) {
		key[i] = 0;
	}

	for (int i = 0; i < sizeof(cntr); i++) {
		cntr[i] = 0;
	}

	seeded = false;
	last_reseed=0;
}

bool FortunaGenerator::incCntr() {
    // Aufgabe 2a

    //Variable i wird für das Iterieren über cntr verwendet
    int i = 0;
    //Iteriere in jedem Schleifendurchlauf über das Array cntr in Richtung vom Least-Significant Byte
    //zum Most-Significant-Byte. Das Least-Significant Byte befindet sich an cntr[0], da cntr als eine Zahl in
    //Least-Significant-First Darstellung interpretiert wird.
    //Inkrementiere den Wert des Bytes um 1. Wenn es dabei zu keinem Überlauf gekommen ist, dann beende die Methode
    //und gib true zurück, ansonsten (wenn es zu einem Überlauf geommen ist) wird auf gleiche Art und Weise mit dem
    //nächsten Byte weiter gemacht. Wenn die Schleife beim Most-Significant angekommen ist und es auch dort zu einem
    // Überlauf des Bytes kommt, dann wird false zurück gegeben und der Counter besitzt wieder den Wert 0
    while(i < 16) {
        //Inkrementiere den Wert des i-ten Bytes um 1
        cntr[i]++;
        //wenn der Wert nach dem Inkrementieren ungleich 0 ist, dann kam es zu keinem Überlauf und es kann True zurück
        //gegeben werden
        if(cntr[i] != 0) {
            return true;
        }
        //Wenn der Wert des i-ten Bytes gleich 0 ist und i gleich 15 ist (also es einen Überlauf am Most-Significant
        // Byte gegeben hat), dann gib false zurück
        if(cntr[i] == 0 && i == 15) {
            return false;
        }
        //ansonsten kam beim Inktrementieren zu einem Überlauf im Byte an der Position i, welches nicht das letzte Byte
        //des Counters war. Somit muss das nächste Byte noch um diese Überlaufene 1 inkrementiert werden
        i++;
    }
    return true;
}

void FortunaGenerator::reseed(byte* seed, unsigned int size) {
    //Aufgabe 2b

    //konkateniertes Byte-Array aus Key und Seed erstellen.
    //Dieses Array (mit dem Namen arr) besitzt die Länge 32 (Länge des Keys) + size (Länge des übergebenen Seeds)
    int byteArraySize = 32 + size;
    byte arr[byteArraySize];
    //In den ersten 32 Elementen des konkatenierten Arrays werden die 32 Bytes des Keys gespeichert
    for(int i = 0; i < 32; i++) {
        arr[i] = key[i];
    }
    //In den restlichen Positionen des Arrays werden die Bytes des Seeds abgelegt
    for(int i = 0; i < size; i++) {
        arr[32+i] = seed[i];
    }

    //SHA256-Hash von Key||Seed mit den entsprechenden Methoden der Crypto++-Library berechnen und
    //als neuen Key abspeichern
    SHA256 hash;
    new ArraySource( arr, byteArraySize, true,
            new HashFilter(hash,
                    new ArraySink(key, 32)
                    )
            );

    //Wert des Counters um 1 inkrementieren
    incCntr();

    //seeded auf true und last_reseed auf die aktuelle Zeit setzen, um zu kennzeichnen, dass ein Reseed durchgefüht
    //wurde
    seeded = true;
    last_reseed = getTimeStamp();

}

bool FortunaGenerator::getBit() {
    //Aufgabe 2e

    //Rückgabe eines zufällig erzeugten Bits

    //Rückgabe des letzten Bits des zufällig generierten Bytes
    //Um dies zu Berechnen wird der Rückgabewert der Funktion getByte() modulo 2 berechnet
    return getByte() % 2;
}

byte FortunaGenerator::getByte() {
    //Aufgabe 2e
    //Rückgabe eines Zufälligen Bytes

    //Byte Array buffer, in dem der erzeugte 16-Byte große Block gespeichert werden soll
    byte buffer[16];

    //Aufruf der Methode getBlock() mit dem Array buffer der Größe 16 Bytes.
    //Es ist notwendig einen 16-Byte Block zu generieren, da der Fortuna PRNG auf einmal immer nur Blöcke
    //der Größe 16 Bytes generieren kann
    getBlock(buffer,1);
    //Rückgabe des ersten Bytes des erzeugten 16 Bytes Block
    return buffer[0];
}

void FortunaGenerator::generateBlocks(byte* buffer, unsigned int n) {
    //Aufgabe 2c & Aufgabe 2g

    //wenn der PRNG noch nicht gereseedet wurde oder der letzte Reseed mehr als 500 Millisekunden zurück liegt:
    //Führe eine Reseeding mit 32 Byte aus /dev/urandom durch
    if(!seeded || ( (getTimeStamp() -last_reseed) > 500 )) {
       //lese 32 Bytes aus /dev/urandom
       byte seed[32];
       string file = "/dev/urandom";
       ifstream urandStream(file, ios::binary);
       urandStream.read((char*)seed,32);
       urandStream.close();
       //Führe das Reseeding mit dem eben ausgelesenen Seed der Länge 32 durch
       reseed(seed,32);
    }

    //Generieren von n 16-Byte großen Blöcken
    for(int i = 0; i < n; i++) {
        //Zur Generierung wird der cntr mit dem AES verschlüsselt
        //Hierfür werden die entsprechenden Methoden der Crypto++-Library verwendet.
        //Als Modus für die Blockchiffre wird lediglich der ECB-Modus verwendet, da sowieso nur ein Block auf einmal
        //verschlüsselt wird und der Counter-Modus quasi manuell durch das anschließende Inkrementieren des Counters
        //erzeugt wird
        ECB_Mode<AES>::Encryption aes;
        aes.SetKey(key,32);
        //Verschlüsseln des Counters mit AES im ECB-Modus
        ArraySource(cntr,16,true,
                new StreamTransformationFilter(aes,
                        new ArraySink(buffer+(i*16),16)
                        )
                );
        //inkrementieren des Counters
        incCntr();
    }
}

void FortunaGenerator::getBlock(byte* buffer, unsigned int n) {

    //Wenn n null ist, dann müssen keine Bytes erzeugt werden
    if (n == 0) {
        return;
    }

    //Wenn n größer 2^20 = 1048576, dann wird die Ausführung abgebrochen, damit sichergestellt wird, dass die
    //erzeugte Byte-Folge statistisch zufällig aussieht
    if(n > 1048576) {
        return;
    }

    //Berechne die Anzahl an zu erzeugenden Blöcken, indem n-1 durch 16 geteilt wird (abgerundet) und um 1 addiert wird
    //Es muss ein Block mehr generiert werden, falls n nicht ohne rest durch 16 dividiert werden kann, damit genügend
    //Bytes erzeugt werden
    //Der Fall n == 0 ist bereits abgefangen und durch unsigned int ist n an dieser Stelle immer >= 1
    int blocksToGenerate = (n-1)/16 + 1;
    //Erzeuge ein temporäres Byte-Array mit dieser Länge, in welchem die zufälligen Blöcke gespeichert werden
    byte b[blocksToGenerate*16];

    //Erzeuge die zufälligen Blöcke
    generateBlocks(b, blocksToGenerate);

    //kopiere die ersten n Bytes in das Array buffer
    for(int i = 0; i < n; i++) {
        buffer[i] = b[i];
    }

    //setze key auf 2 erzeugte 16-Byte Zufallsblöcke
    generateBlocks(key,2);
    //setze den Zeitstempel für den letzten Reseed auf die aktuelle Zeit in Millisekunden ab dem Unix epoch modulo 2^32,
    //da durch verändern des Schlüssels ein Reseeding stattgefunden hat
    last_reseed = getTimeStamp();

}

uint32_t FortunaGenerator::getTimeStamp() const {
	struct timeval  tv;

	gettimeofday(&tv,NULL);
	return tv.tv_sec*1000 + tv.tv_usec/1000;
}

