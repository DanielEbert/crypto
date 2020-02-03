/*
 * encoder.cpp
 */
#include <iostream>
#include "MessageEncoder.h"
#include <integer.h>

using namespace std;
using namespace CryptoPP;

void encoderTest() {
	cout << "MessageEncoderTest" << endl <<
			"==================" << endl;

	//Aufgabe 2b
	//Implementieren des Beispiels
	MessageEncoder me;
	cout << "\nAufgabe 2b\n----------" << endl;
	//Hinzufügen der Werte zum MessageEncoder
	me.append(Integer("10892"));
	me.append("Hallo Welt!");
	me.append(Integer("459111231"));
	//Kodieren der zuvor hinzugefügten Werte mit der encode()-Methode des MessageEncoders
	string encodedString = me.encode();
	//Ausgabe des mit dem MessageEncoder kodierten String
	cout << "Kodierter String: " << encodedString << endl << endl;
	//Aufruf der Methode decode() des MessageEncoders mit dem vorher kodierten String, die den kodierten String wieder
	//dekodiert
	me.decode(encodedString);
	//die dekodierten Werte in den entsprechenden Variablen speichern
	Integer i = me.getInteger(0);
	string s = me.getString(1);
	Integer j = me.getInteger(2);
	//Ausgabe der dekodierten Variablen auf dem Bildschirm
	cout << "Dekodierte Werte:" << endl;
	cout << "Integer i: " << i << endl;
	cout << "String s:  " << s << endl;
	cout << "Integer j: " << j << endl;

	//Aufgabe 2c
	//kodieren des Datensatzes aus der Aufgabenstellung
	cout << "\nAufgabe 2c\n----------" << endl << endl;
	//Inhalt des MessageEncoders leeren
	me.clear();
	//Hinzufügen der Werte in der Tabelle zum MessageEncoder
	me.append(Integer("98921323668113423232"));
	me.append("Kryptografie macht Spass!");
	me.append("Crypto++ ist eine Kryptografie-Bibliothek");
	me.append(Integer("456291260721096"));
	//Kodieren der Werte innerhalb des MessageEncoders über den Aufruf der Methode encode().
    //Der kodierte String wird in der Variable encodedString abgelegt
	encodedString = me.encode();
	//Ausgabe des kodierten Strings in encodedString
	cout << "Kodierter String: " << encodedString << endl;

	//Aufgabe 2d
	//Dekodieren des Datensatzes aus Aufgabe 2c
	cout << "\nAufgabe 2d\n----------" << endl << endl;
	//Aufruf der Methode decode() des MessageEncoders mit dem kodierten String zur Dekodierung
    me.decode(encodedString);
    //Auslesen der dekodierten Werte aus dem MessageEncoder
	Integer decoded0 = me.getInteger(0);
	string decoded1 = me.getString(1);
	string decoded2 = me.getString(2);
	Integer decoded3 = me.getInteger(3);
	//Ausgabe der dekodierten Werte auf dem Bildschirm
	cout << "Dekodierter Wert 0: " << decoded0 << endl;
	cout << "Dekodierter Wert 1: " << decoded1 << endl;
	cout << "Dekodierter Wert 2: " << decoded2 << endl;
	cout << "Dekodierter Wert 3: " << decoded3 << endl;

	//Aufgabe 2e
	//dekodieren des Datensatzes aus der Aufgabenstellung
	cout << "\nAufgabe 2e\n----------" << endl << endl;
	//kodierter Datensatz der Aufgabenstellung, der einen Integer und zwei Strings enthält
	encodedString = "6DB03BF70E#4963682062696E2065696E65205A65696368656E666F6C67652C20646965#207369636820617573207A"
                 "776569205465696C656E207A7573616D6D656E7365747A74";
	cout << "Kodierter String: " << encodedString << endl;
	//leeren des Inhalts des MessageEncoders
	me.clear();
	//den kodierten String dekodieren
	me.decode(encodedString);
	//Auslesen der dekodierten Werte. Es wurde zuerst ein Integer gefolgt von zwei Strings kodiert
	Integer value0 = me.getInteger(0);
	string value1 = me.getString(1);
	string value2 = me.getString(2);
	//Ausgabe der dekodierten Werte auf dem Bildschirm
	cout << "Dekodierter Wert 0: " << value0 << endl;
	cout << "Dekodierter Wert 1: " << value1 << endl;
	cout << "Dekodierter Wert 2: " << value2 << endl;

	//Aufgabe 2f
	//überprüfen des Message Authentication Codes (MAC)

	cout << "\nAufgabe 2f\n----------" << endl << endl;
	//Variablen für den MAC und den den Schlüssel initialisieren
	string mac = "937A6984EE71C06E808AB996BF76C246AC29E198B6637B02C37ABDE92D6A46C8";
	string key = "ahVoh4naghie";
	//Den MAC und den Schlüssel auf dem Bildschirm ausgeben
	cout << "MAC: " << mac << endl;
	cout << "Key: " << key << endl;
	//Ausgabe des Rückgabewerts der Methode verify des MessageEncoders auf dem Bildschirm.
	//Die Methode verify bekommt einen Schlüssel und einen Message Authentication Code übergeben und überprüft ob der
	//übergebene MAC mit der SHA-256 Hashsumme der Konkatenation des übergebenen Keys und dem kodierten String, der
	//innerhalb des MessageEncoders verwaltet wird, übereinstimmt. Es wird eine 1 ausgegeben, wenn dies übereinstimmt
	//und eine 0 wenn sie nicht übereinstimmten
	cout << "Verifikation der Daten aus Aufgabe 2e liefert: " << me.verify(key,mac) << endl;


}

int main(int argc, char **argv) {
	encoderTest();
	return 0;
}
