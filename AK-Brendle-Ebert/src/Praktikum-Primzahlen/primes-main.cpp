/*
 * primes.cpp
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include "FortunaGenerator.h"
#include "MillerRabinTest.h"

#include "integer.h"


using namespace std;

Integer randomPrime(unsigned int bytes) {
    //Aufgabe 8
    //Die Funktion randomPrime erzeugt unter Einsatz des Fortuna-Generators und des Miller-Rabin Tests eine Primzahl
    //der Länge bytes
    //Erzeugen der FortunaGenerator und MillerRabinTest Objekte
    FortunaGenerator f;
    MillerRabinTest mrt;
    //Im Integer p werden die erzeugten Zufallszahlen gespeichert
    Integer p;
    //Festlegung des Qualitätsparameters s auf 50
    unsigned int s = 50;
    do {
        //Erzeugen einer zufälligen Zahl mit dem FortunaGenerator
        p = f.getInteger(bytes);
        //Durchlaufe die While-Schleife solange, bis der Miller-Rabin Test die zufällig erzeugte Zahl als eine
        //Primzahl einstuft
    } while(!mrt.isPrime(&f,p,s));
    //Rückgabe von p. Diese Zahl ist mit der Wahrscheinlichkeit von 1-(2^-s) eine Primzahl
    return p;
}

void millerRabin() {
	cout << "Miller-Rabin Test" << endl
		 << "================="
		 << endl << endl;
	MillerRabinTest m;
	FortunaGenerator f;


	//Aufgabe 3d
	//Finden eines soliden Beweises, der beweist, dass 121399589920585397786647347761633 eine zusammengesetzte Zahl
	//ist
	//Dafür wird solange eine zufällige Zahl zwischen 1 und 121399589920585397786647347761632 erzeugt, bis ein solider
	//Beweis gefunden wurde
	cout << "Aufgabe 3d\n----------\n\n";
	cout << "Siehe AK/primzahlenAufgaben.txt" << endl;
	bool beweisFound = false;
	Integer zahl3d("121399589920585397786647347761633");
	//While-Schleife, die erst abbricht, wenn ein solider Beweis gefunden wurde
	while(!beweisFound) {
	    //erzeuge eine zufällige Zahl mit dem Fortuna Generator im Bererich 1 bis 121399589920585397786647347761633-1
	    Integer test = f.getInteger(zahl3d);
	    //wenn witness true zurück gibt, dann gib aus dass die generierte Zahl test ein Solider Beweis ist und
	    //beende die While-Schleife ab
	    if(m.witness(test,zahl3d) == true) {
	        beweisFound = true;
	        cout << test << " ist ein solider Beweis, dass " << zahl3d <<" eine zusammengesetzte Zahl ist" << endl;
	    }
	}
	cout << endl << endl;

	//Aufgabe 4b
	// Überprüfe für die Zahlen in der Tabelle, ob es sich bei ihnen um Primzahlen handelt
	cout << "Aufgabe 4b\n----------\n\n";
	//Der Qualitätsparameter s wird auf 50 festgelegt. Er gibt beim Aufruf des Miller-Rabin-Tests an, wie oft die
	//Schleife durchlaufen werden soll
	unsigned int s = 50;
	//Initialisieren der vier Integervariablen s1 bis s4
	//Für jede Variable wird ausgegeben, ob die Zahl eine Primzahl ist oder nicht. Dafür wird der ternäre Operator
	//eingesetzt und der Text "ist eine Primzahl" ausgegeben, wenn isPrime True zurückgegeben hat und "ist keine
	// Primzahl" ausgegeben, wenn False zurückgegeben wurde
	Integer z1 = Integer("249236727971798483921811058804041930204535828339");
	cout << z1 << " " << (m.isPrime(&f,z1,s) ? "ist eine Primzahl": "ist keine Primzahl") << endl;

    Integer z2 = Integer("167376539261987877304745111417636335723391598799");
    cout << z2 << " " << (m.isPrime(&f,z2,s)? "ist eine Primzahl": "ist keine Primzahl") << endl;

    Integer z3 = Integer("207241657224647904979925877995751997643166331621");
    cout << z3 << " " << (m.isPrime(&f,z3,s) ? "ist eine Primzahl": "ist keine Primzahl") << endl;

    Integer z4 = Integer("203681457354349143050092586190992078578082388049");
    cout << z4 << " " <<  (m.isPrime(&f,z4,s)? "ist eine Primzahl": "ist keine Primzahl") << endl;

    cout << endl << endl;

    //Aufgabe 5b
    // Test der Implementierung der boundedExponentation Methode der KLasse MillerRabinTest
    cout << "Aufgabe 5b\n----------\n\n";
    //Initialisieren der Variablen für die Basis, den Exponenten und die Grenze
    Integer base = 7;
    Integer exponent = 43;
    Integer bound = 2;
    //berechnen von 2 ^ 121 für den Wert von Bound
    for(int i = 1; i < 121; i++) {
        bound *= 2;
    }
    cout << "Bounded Exponentation für " << base << " ^ " << exponent << " mit:" << endl;
    //boundedExponentation mit bound = 2 ^ 121
    cout << "Bound = 2 ^ 121: " << m.boundedExponentation(base,exponent,bound) << endl;

    //bound durch 2 teilen, damit der Integer den Wert 2 ^ 120 enthält
    bound /= 2;
    //boundedExponentation mit bound = 2 ^ 120
    cout << "Bound = 2 ^ 120: " << m.boundedExponentation(base,exponent,bound) << endl;
    cout << endl << endl;

    //Aufgabe 6b
    //Überprüfen ob die Zahl 346158389003024161132823470747012309343112001349179903402673 eine Ganzahlpotenz mit dem
    //Exponent 29 ist
    cout << "Aufgabe 6b\n----------\n\n";
    Integer n = Integer("346158389003024161132823470747012309343112001349179903402673");
    exponent = 29;
    cout << "Teste ob die Zahl " << n << " eine Ganzzahlpotenz mit Exponent " << exponent << " ist:" << endl;
    cout << "Basis b=" << m.searchBase(n,exponent) << endl;
    cout << endl << endl;

    //Aufgabe 7b
    //testen der Methode isPerfectPower() der Klasse MillerRabinTest mit den Werten aus Aufgabe 5 und 6
    //Dafür wird für beide Ganzzahlpotenzen die Methode isPerfectPower() aufgerufen und die errechnete Basis und der
    //Exponent in den Variablen basis und exponent abgespeichert
    //Nach der Berechnung werden beide WErte auf dem Bildschirm ausgegeben
    cout << "Aufgabe 7b\n----------\n\n";
    Integer basis;
    exponent = 0;
    Integer zahl = Integer("2183814375991796599109312252753832343");
    cout << "Test aus Aufgabe 5b: \nBerechnung von Basis und Exponent der Ganzzahlpotenz " << zahl << endl;
    m.isPerfectPower(zahl,basis,exponent);
    cout << "Basis " << basis << " Exponent " << exponent << endl << endl;
    basis = 0;
    exponent = 0;
    cout << "Test aus Aufgabe 6b: \nBerechnung von Basis und Exponent der Ganzzahlpotenz " << n << endl;
    m.isPerfectPower(n,basis,exponent);
    cout << "Basis " << basis << " Exponent: " << exponent << endl << endl;
}

void fortuna() {
	cout << "Fortuna PRNG" << endl
		 << "============"
		 << endl << endl;
    //Aufgabe 2f
    //generieren von 1000 Zufallsbytes mit Fortuna Generator mit Seed 0x7ab2af1a4561abf8 und anschließende
    //Ausgabe auf dem Bildschirm
    //Dazu wird ein Byte-Array der Größe 8 angelegt, das die Bytes des Seeds enthält und damit die Methode reseed()
    //des Fortuna Genrators aufgerufen
	FortunaGenerator f;
	byte seed[8] = {0x7a, 0xb2, 0xaf, 0x1a, 0x45, 0x61, 0xab, 0xf8};
	f.reseed(seed,8);
    // Anschließend wird die Methode getBlock() des Fortuna-Generators mit dem Byte-Array block der Länge 1000
    //aufgerufen, wodurch 1000 Zufallsbytes im Array gespeichert werden
	byte block[1000];
	f.getBlock(block,1000);
	cout << "Aufgabe 2f\n----------\n\n";
    cout << "Erzeugen von 1000 Zufallsbytes mit dem Fortuna Generator mit Seed 0x7ab2af1a4561abf" << endl << endl;
    //Danach werden die 1000 Bytes durch ein Leerzeichen getrennt auf dem Bildschirm ausgegeben
    //Die Werte der Bytes werden zu einem Integer gecastet, damit sie als eine Dezimalzahl zwischen 0 und 255
    //dargestellt werden können
	for(int i = 0; i < 1000; i++) {
	    cout << (int) block[i] << " ";
	}
	cout << endl << endl << endl;

	//Aufgabe 2h
	//erneutes erzeugen von 100 Zufallsbytes
	//der PRNG muss nicht mehr initialisiert werden, da der PRNG bereits initialisiert wurde und sein letzter Reseed
	//kürzer als 500 Millisekunden zurück liegt
    cout << "Aufgabe 2h\n----------\n\n";
	cout << "Erneutes Erzeugen eines Blocks von 1000 Zufallsbytes" << endl << endl;
	byte block2[1000];
	//Rufe erneut die Methode getBlock() des Fortuna Generators auf. Bei diesem Aufruf wird das Byte Array block2, mit
	//der Länge Tausend an die Methode übergeben
	f.getBlock(block2,1000);
	//Ausgabe der 1000 erzeugten Bytes durch eine Komma getrennt
	for(int i = 0; i < 1000; i++) {
	    cout << (int) block2[i] << " ";
	}
	cout << endl << endl << endl;
}

int main() {
	cout << "Generierung von Primzahlen" << endl
		 << "==========================" << endl;

	fortuna();
	millerRabin();

	//Aufgabe 8
	//erzeugen von Primzahlen über den Fortuna PRNGs und des Miller Rabin Tests
	cout << "Aufgabe 8\n----------\n\n";
	cout << "Erzeugen einer Primzahl der Länge:" << endl << endl;
	//Für jede Länge (512, 1024, 2048, 4096) wird die Funktion randomPrime mit der Anzahl der zu erzeugenden Länge in
	//Bytes (anzahl Bits / 8) aufgerufen und auf dem Bildschirm ausgegeben
	cout << "512 Bit: " << randomPrime(512/8) << endl;
    cout << "1024 Bit: " << randomPrime(1024/8) << endl;
    cout << "2048 Bit: " << randomPrime(2048/8) << endl;
    cout << "4096 Bit: " << randomPrime(4096/8) << endl;
	return 0;
}
