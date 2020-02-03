/*
 * MillerRabinTest.cpp
 */
#include <iostream>
#include <cassert>
#include "MillerRabinTest.h"

#include <math.h>

using namespace std;

Integer MillerRabinTest::boundedExponentation(const Integer& b, const Integer& e, const Integer& bound) const {
    //Aufgabe 5a

    //Modifizierter Algorithmus für modulare Exponentiation
    //Es wurden bei den Multiplikationen auf die modulo Operation verzichtet. Dafür wurde die Obergrenze bound
    //hinzugefügt. Am Ende jedes Schleifendurchlaufs wird überprüft ob d > bound ist
    Integer d = 1;
    //In der Variable eBits wird die Länge der Zahl e in Bits abgespeichert, da diese Anzahl für die nachfolgende
    //for-Schleife benötigt wird, um die Anzahl der Schleifendurchläufe festzulegen
    ssize_t  eBits = e.BitCount();
    //Iteriere über die Binärdarstellung von e vom Most-Significant-Bit zum Least-Significant-Bit
    for(ssize_t i = eBits-1; i >=0; i--) {
        d = (d*d);
        //checke nach der Quadratur von d, ob d bereits größer als bound ist, wenn ja, dann gib bound+1 zurück
        if(d > bound) {
            return bound+1;
        }
        //wenn das Bit an der Stelle i eine 1 war, dann multipliziere d noch zusätzlich mit b
        if(e.GetBit(i)) {
            d = (d*b);
        }
        //checke am Ende erneut, ob das aktuelle Zwischenergebnis größer als bound ist, ist dies der Fall, dann
        // gib bound+1 zurück
        if(d > bound) {
            return bound+1;
        }
    }
    //ist das berechnete Ergebnis kleiner gleich bound, gib das berechnete Ergebniss für b^e zurück.
    //Erreicht das Programm diese Stelle, dann ist dies der Fall, da am Ende jedes Schleifendurchlaufs überprüft wird,
    //ob d > bound ist
    return d;
}

bool MillerRabinTest::isPerfectPower(const Integer& n, Integer& b, Integer& e) const {
    //Aufgabe 7
    //Implementierung des Algorithmus PerfectPower

    //wenn n < 2 ist, dann kann die Zahl n keine Ganzzahlpotenz sein. In diesem Fall, gib direkt False zurück
    if(n < 2) {
        return false;
    }

    //Über die Variable found wird in der nachfolgenden while-Schleife festgestellt ob bereits ein Exponent e gefunden
    //wurde, für den eine Basis b existiert
    //Sie wird zu Beginn mit False initialisiert
    bool found = false;

    //Setze e zum Beginn auf den Wert 2
    //Wenn n keine Ganzzahlpotenz ist, dann sollen die Übergabeparameter b und e nicht verändert werden.
    //Aus diesem Grund werden curE und curB anstelle von e und b für das Speichern von Zwischenergebnissen verwendet.
    Integer curE = 2;
    Integer curB;

    //Führe die Schleife aus, solange e <= log₂(n)+1 und noch kein Ergebnis gefunden wurde
    //Der log₂ von n wird über die Anzahl der Bits von n berechnet, da diese Anzahl dem log₂(n) abgerundet zur nächsten
    //Ganzen Zahl entspricht
    while(curE < (n.BitCount()+1) && found == false) {
        //Setze b auf das Ergebnis des Algorithmus von SearchBase()
        curB = searchBase(n,curE);
        //Wenn b größer als 0 ist, dann hat searchBase eine Basis gefunden, so dass b^(aktuelles e) = n
        //Dann wird found auf True gesetzt und die while-Schleife bricht bei der nächsten Auswertung der
        //Schleifenbedingung ab
        if(curB > 0) {
            b = curB;
            e = curE;
            found = true;
        }
        //Ansonsten existiert keine Basis b für das aktuelle e und n. Dann wird e um 1 erhöht.
        else {
            curE++;
        }
    }

    //Rückgabe des Werts found, der angibt, ob ein b und e gefunden wurden/ n eine Ganzzahlpotenz ist
    return found;
}

Integer MillerRabinTest::modularExponentation
(
 const Integer& a,
 const Integer& b,
 const Integer& n
) const
{
    //Aufgabe 3b
    //Berechnung von a ^ b mod n mit Algorithmus für modulare Exponentiation

    //Weise d zu Beginn den Wert 1 zu
    Integer d = 1;
    ssize_t bBits = b.BitCount();
    //Verarbeite in jedem Schleifendurchlauf ein Bit von b. Durchlaufe b in Richtung vom Most-Significant Bit zum
    //Least-Significant Bit
    for(ssize_t i = bBits-1; i >= 0; i--) {
        //Quadriere d (modulo n)
        d = (d*d) % n;
        //Wenn das Bit an Position i den Wert 1 besitzt: dann multipliziere d mit a (modulo n)
        if(b.GetBit(i)) {
            d = (d*a) % n;
        }
    }

    //Rückgabe der Variable d, die das Ergebnis von a^b (mod n) enthält
    return d;

}

bool MillerRabinTest::witness(const Integer& a, const Integer& n) const {
    //Aufgabe 3c
    //Implementierung des Witness-Algorithmus, der True zurückliefert, wenn n keine Primzahl ist

    //sicherstellen, dass n ungerade Zahl ist
    assert((n%2)==1);

    //Berechne u und r so dass n-1 = u * 2^r
    Integer nMinus1 = n-1;
    Integer u,r;
    Integer powerof2;

    //Für jedes Bit in der Binärdarstellung von n-1
    for(int i = 0; i < nMinus1.BitCount(); i++) {
        //wenn das Bit an der i-ten Position 1 ist, dann Berechne u = (n-1)/ 2^i
        if(nMinus1.GetBit(i)) {
            powerof2 = Integer::Power2(i);
            r = i;
            u = nMinus1 / powerof2;
            //wenn alle Kriterien für u und r erfüllt sind und die Division in der obigen Zeile ohne Rest
            //durchgeführt werden konnte, dann brich die Schleife ab, da r und u gefunden wurde, sodass die Bedingung
            //erfüllt ist
            if(((nMinus1 % powerof2) == 0) && ((u % 2) == 1)) {
                break;
            }
        }
    }

    Integer d = modularExponentation(a,u,n);
    //Führe in jedem Schleifendurchlauf eine Quadratur von d durch, um a^(n-1) (mod n) zu berechnen
    for(Integer i = 0; i < r; i++) {
        Integer x = d;
        d = (d*d) % n;
        //wenn d=1 und x !=1 und x != -1 ist, dann gib True zurück, da n keine Primzahl sein kann, wenn n eine
        //Quadratwurzel in Z Stern n besitzt, die ungleich 1 und -1 (also n-1) ist
        if((d==1) && (x != 1) && (x != (n-1))) {
            return true;
        }
    }
    //wenn das Ergebnis der Berechnung a^(n-1) % n ungleich 1 ist (das Fermat-Kriterium also nicht erfüllt ist), dann gib
    //True zurück, da n dann keine Primzahl ist
    if(d != 1) {
        return true;
    }

    return false;
}

bool MillerRabinTest::isPrime(PRNG* rng, const Integer& n, unsigned int s) const {
    //Aufgabe 4a
    //Impementiert den Miller-Rabin Primzahltest, der feststellen kann, ob es sich bei n um eine Primzahl oder eine
    //Zusammengesetze Zahl handelt

    //Sicherstellen, dass die Zahl n > 2 und der Qualitätsparameter s größer gleich 0 ist
    assert(n > 2);
    assert(s >= 0);

    //Wenn die Zahl gerade ist (also n % 2 = 0), dann gib False zurück. Die Zahl 2 ist die einzige gerade Primzahl.
    //Da aber sichergestellt wird, dass die zu testende Zahl größer als 2 ist, kann geschlussfolgert werden,
    //dass es sich sicher um keine Primzahl handelt, wenn n gerade ist
    if((n % 2) == 0) {
        return false;
    }
    //Durchlaufe die Schleife s-mal
    for(int i = 0; i < s; i++) {
        //Die zufällig erzeugte Zahl a muss in range 1 bis n-1 liegen. Da getInteger() eine Zahl im Raum von 0 bis n-1
        //liefert, wird der Raum der erzeugten Zahlen um 1 reduziert und am Ende eins aufaddiert. So ist
        //die kleinstmöglich erzeugbare Zahl 0+1 = 1 und die größtmöglich erzeugbare Zahl n-2+1 = n-1. Somit befindet
        //sich die zufällig erzeugte Zahl in dem vom Algorithmus benötigten Bereich von {1,...,n-1}
        Integer a = rng->getInteger(n-1)+1;

        //Wenn der größte Gemeinsame Teiler der zufällig erzeugten Zahl und n größer als 1 ist, dann ist n
        //keine Primzahl, da für eine Primzahl p gilt, dass der ggT aller Zahlen in Z Stern p (also alle Zahlen
        //zwischen 1 und p-1 (inklusiv)) gleich 1 ist
        if(Integer::Gcd(a,n) > 1) {
            return false;
        }
        //wenn die Funktion Witness true zurückgibt -> Gib false zurück, da n dann keine Primzahl ist, da hierfür ein
        //solider Beweis gefunden wurde
        if(witness(a,n)) {
            return false;
        }
    }

    Integer b,e;
    //Wenn die Zahl eine Ganzzahlpotenz ist, gib false zurück, da eine Ganzzahlpotenz keine Primzahl ist
      if(isPerfectPower(n,b,e)) {
        return false;
    }
    return true;
}


Integer MillerRabinTest::searchBase(const Integer& n, const Integer& e) const {
    //Aufgabe 6a
    //Implementierung des Alogritmus searchBase, der für ein n und e feststellt, ob ein b existiert, sodass b^e = n
    assert(n > 2);

    //Linke und Rechte Grenze für die Binäre Suche festlegen
    //Bei dem Integer left handelt es sich um die untere Grenze des Suchintervalls und beim Integer right handelt es
    //sich um die obere Grenze des Suchintervalls
    Integer left = 2;
    Integer right = n;
    //Im Integer b, wird in jedem Schleifendurchlauf die Basis gespeichert, für die die Bounded Exponentation mit
    //e durchgeführt werden soll
    //Im Integer x wird in jedem Schleifendurchlauf das Ergebnis der Bounded Exponentation gespeichert
    Integer b, x;
    //do-while Schleife, die so lange durchgeführt wird, bis ein b gefunden wurde, sodass b^e = n oder der rechte Wert
    //des binären Suchintervalls kleiner gleich der linken Grenze ist
    do {
        //Berechne b so, dass es in der Mitte des Intervalls der linken und rechten Grenze liegt
        b = left + ((right-left)/2);
        //berechne das Ergebnis von b^e mit dem Algorithmus für Bounded Exponentation
        x = boundedExponentation(b,e,n);
        //wenn das Ergebnis der Bounded Exponentation größer als n war, dann kann geschlussfolgert werden, dass der
        //Wert für b (falls er denn existiert) zwischen der linken Grenze und b liegt. Setze deshalb die rechte
        //Grenze auf b
        if(x > n) {
            right = b;
        }
        //Ansonsten liegt der Wert für b (falls er existiert) zwischen b und der oberen Grenze. Setze also die linke
        //Grenze auf b
        else {
            left = b;
        }
    } while(x != n && (( (right-left) / 2) > 0 ) );

    //Überprüfen ob ein x gefunden wurde, dass gleich n ist, also ob die do-while Schleife beendet wurde, weil ein
    //geeigneter Wert für b gefunden wurde. In diesem Fall wird b zurückgegeben
    if(x == n) {
        return b;
    }
    //Ansonsten war die Binäre Suche erfolglos und 0 wird zurückgegeben
    else {
        return 0;
    }
}
