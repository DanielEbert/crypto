/*
 * DHProtocol.h
 */

#ifndef DHPROTOCOL_H_
#define DHPROTOCOL_H_

#include <iostream>
#include <integer.h>
#include <socketft.h>

using namespace std;
using namespace CryptoPP;
/**
 * Diese Klasse implementiert das im Jahre 1976 von Whitfield Diffie und Martin Hellman vorgestellte Diffie-Hellman
 * Schlüsselaustausch Protokoll für die beiden Kommunikationspartner Alice und Bob, sowie eine Man-In-The-Middle
 * Attacke eines Angreifers Oscar.
 */
class DHProtocol {
protected:
	/**
	 *  Modul des Diffie-Hellman Key Exchanges. Die Primzahl p wird dafür so gewählt, dass sie eine sichere Primzahl
	 *  ist. Eine sichere Primzahl ist eine Primzahl mit ausreichender Größe, die erzeugt wird durch 2*q+1 = p, wobei
	 *  q selbst eine Primzahl ist. Wenn
	 *  die Primzahl p eine sichere Primzahl ist, dann besitzt Z Stern p folgende Untergruppen: die Untergruppe {1},
	 *  die Untergruppe {1,p-1}, die Untegruppe mit q Elementen und Z Stern p selbst mit 2*q Elementen, da für alle
	 *  Primzahlen  x gilt, dass Z Stern x eine zyklische Gruppe ist.
	 */
	Integer p;
	/**
	 * Die Primzahl q wird dazu verwendet, um zu Überprüfen, ob p tatsächlich eine sichere Primzahl ist. 
	 * Dabei muss gelten: p = 2*q+1
	 */
	Integer q;
	/**
	 * Das Element alpha ist ein erzeugendes Element der Gruppe Z Stern p. Da Z Stern p  für alle Primzahlen p eine
	 * zyklische Gruppe ist, muss ein erzeugendes Element existieren. Ein erzeugendes Element einer Gruppe G ist ein
	 * a ∈ G, für dessen durch wiederholtes Verknüpfen erzeugte Untergruppe gilt, dass diese gleich G ist. Es ist
	 * wichtig, dass alpha als erzeugendes Element gewählt wird, da sonst die Mächtigkeit der von alpha erzeugten Gruppe
	 * kleiner als p ist und das eine Brute-Force-Attacke erfolgreicher macht.
	 */
	Integer alpha;

	/**
	 * Diese Methode bekommt den Socket zum Client übergeben und führt das Diffie-Hellman Key-Exchange Protokoll mit
	 * diesem Client durch. In dieser Methode wird die Rolle von Bob übernommen. Bei diesem Protokoll einigen sich
	 * Alice und Bob zuerst auf eine ausreichend große sichere Primzahl p und ein α, welches ein erzeugendes Element von
	 * Z Stern p ist. In dieser Implementierung werden diese benötigten Parameter im Konstruktor aus einer an den Konstruktor übergebenen
	 * Datei ausgelesen. Bob erhält das von Alice berechnete ya, welches aus α ^ a (mod p) berechnet wird. Anschließend
	 * wählt Bob eine zufällige Zahl b aus Z Stern p und berechnet anschließend yb = α ^ b (mod p). Im Anschluss daran
	 * kodiert Bob yb mit dem MessageEncoder und sendet sie an Alice. Zuletzt wird der geheime Schlüssel K berechnet,
	 * indem Kb = ya ^ b (mod p). Bob berechnet hier den selben Schlüssel wie Alice, da Kb = ya ^ b (mod p) =
	 * (α ^ a) ^ b (mod p) = (α ^ b) ^ a (mod p) = yb ^ a (mod p) = Ka. Somit ist Kb = Ka und sowohl Alice als auch
	 * Bob berechnen denselben Geheimen Schlüssel, der aus den bekannten Werten, welche über das Netzwerk versendet
	 * werden, nicht berechnet werden kann.
	 *
	 * @param c Geöffneter Socket zur Verbindung mit Alice
	 *
	 * @returns True, wenn der Diffie-Hellman Schlüsselaustausch erfolgreich ablief, ansonsten False
	 */
	bool serverBob(Socket c);

    /**
     * Diese Methode implementiert die Man-In-The-Middle Attacke auf das Diffie-Hellman Protokoll, bei der ein Angreifer
     * Oskar zwischen der Verbindung von Alice und Bob sitzt. Der Angreifer Oscar handelt sowohl mit Alice als auch Bob
     * unabhängig voneinander einen geheimen Schlüssel aus. Auf diese Weise können Alice und Bob nur noch verschlüsselt
     * über Oscar kommunizieren, der die gesendeten Daten dann entsprechend an an den anderen Kommunikationspartner
     * weiterleitet. Alice und Bob können ihre verschlüsselten Nachrichten gegenseitig nicht mehr entschlüsseln. Diese
     * Methode ruft die Methode serverBob() auf, um sich gegenüber Alice als Bob auszugeben und die Methode alice(), um
     * sich gegenüber Bob als Alice auszugeben. Der Man-In-The-Middle Angriff könnte verhindert werden, wenn im
     * Protokoll zusätzlich Signaturen verwendet werden würden.
     *
     * @param c Socket, über den Alice eine Verbindung zu Oscar initiiert hat
     * @param host Host, an dem Bob erreichbar ist
     * @param port Nummer des Ports, an dem Bob auf eine Verbindung zur Initiierung des Diffie-Hellman Protokolls wartet
     *
     * @returns True, wenn der Man-In-The-Middle Angriff erfolgreich war, ansonsten False
     */
	bool serverOscar(Socket c, const string& host, unsigned int port);

public:
    /**
     * Der Konstruktor der Klasse DHProtocol liest die für das Diffie-Hellman Protokoll benötigten Parameter aus der
     * übergebenen Datei aus, die diese mit dem MessageEncoder kodiert enthält und speichert die dekodierten Werte
     * innerhalb der Integer-Klassenvariablen p, q und alpha.
     * @param param_file Pfad zur Datei, in welcher sich die kodierten Parameter befinden
     */
	DHProtocol(const string& param_file);

	/**
	 * Die Methode alice() übernimmt den Teil von Alice beim Diffie-Hellman Key Exchange Protokoll. Die Methode bekommt
	 * den Host und den Port übergebenen, mit welchem eine Verbindung gestartet werden soll. Vor Beginn des Protokolls
	 * müssen sich Alice und Bob auf eine ausreichend große Primzahl p und ein erzeugendes Element in Z Stern p
	 * α einigen. Bei dieser Implementierung werden diese Parameter aus einer Datei im Konstruktor DHProtocol()
	 * ausgelesen. Zu Beginn des Protokolls erzeugt Alice eine zufällige Zahl a in Z Stern p. Diese zufällige Zahl wird
	 * mit dem Fortuna-Pseudozufallsgenerator erzeugt. Anschließend berechnet Alice ya = α ^ a (mod p), kodiert ya mit
	 * dem MessageEncoder und sendet diese Nachricht an Bob. Bob arbeitet Analog zu Alice und erzeugt auf selbe Art und
	 * Weiße ein yb und sendet dies an Alice. Alice dekodiert die von Bob erhaltenene Nachricht über den MessageEncoder
	 * und erhält auf diese Weise yb. Dann berechnet sie den geheimen Schlüssel Ka = yb ^ a (mod p). Beide Teilnehmer
	 * berechnen den selben geheimen Schlüssel, da Ka = yb ^ a (mod p) = (α ^ b) ^ a (mod p) = (α ^ a) ^ b (mod p) =
	 * ya ^ b (mod p) = Kb. Die Sicherheit des Diffie-Hellman Protokolls basiert auf dem Diskreten Logarithmus Problem.
	 * Bei diesem Problem ist eine Primzahl p, ein erzeugendes Element α von Z Stern p und ein β ∈ Z Stern p gegeben.
	 * Gesucht wird die ganze Zahl a mit 0 <= a <= p-2, sodass α ^ a kongruent zu β (mod p). Dieses gesuchte a wird
	 * diskreter Logarithmus β zur Basis α genannt. Für jedes β in Z Stern p existiert ein solches a. Das Protokoll gilt
	 * als sicher, da zurzeit kein Algorithmus bekannt ist, der das Diskrete Logarithmus Problem in effizienter Laufzeit
	 * lösen kann. Wenn die Primzahl p ausreichend groß gewählt wird, dann hat jeder bekannte Algorithmus zur
	 * Lösung des Problems mindestens exponentielle Laufzeit.
	 *
	 * @param host Host, zu dem eine Verbindung initiiert werden soll
	 * @param port Nummer des Ports, auf dem der Kommunikationspartner auf eingehende Verbindung wartet
	 */
	void alice(const string& host, unsigned int port);

	/**
	 * Diese Methode implementiert den Server Bob. Es wird ein Socket auf dem als Parameter übergeben Port geöffnet,
	 * der in einer Endlosschleife auf eingehende Verbindungen wartet. Beginnt ein Client eine Verbindung mit dem
	 * Server, dann wird für die weitere Kommunikation und dem Diffie-Hellman Schlüsselaustausch die Methode serverBob()
	 * mit dem geöffnetenen Socket aufgerufen.
	 * @param port Nummer des Ports, auf dem auf eingehende Verbindungen gewartet werden soll
	 */
	void bob(unsigned int port);

    /**
     * Diese Methode implementiert den Server für den Angreifer Oscar, der auf die eingehende Verbindung von Alice
     * wartet. Dabei wird in einer Endlosschleife auf die eingehende Verbindung gewartet und anschließend die Methode
     * serverOscar() aufgerufen, die die Man-In-The-Middle Attacke durchführt.
     * @param inport Nummer des Ports, auf dem Oscar auf eine eingehende Verbindung von Alice wartet
     * @param host Host, auf dem Bob erreichbar ist
     * @param port Nummer des Ports, auf dem Bob auf eingehende Verbindungen wartet
     */
	void oscar(unsigned int inport, const string& host, unsigned int port);
};

#endif /* DHPROTOCOL_H_ */
