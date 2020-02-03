/*
 * DHProtocol.cpp
 */
#include <cassert>
#include <fstream>
#include <socketft.h>
#include "FortunaGenerator.h"
#include "MessageEncoder.h"
#include "DHProtocol.h"

using namespace std;
using namespace CryptoPP;

#define BUFFERSIZE 20000

DHProtocol::DHProtocol(const string& param_file) {
	ifstream file;
	string data;
	MessageEncoder encoder;

	file.open(param_file.c_str());
	if (file.is_open() == false) {
		cout << "Could not open file: " << param_file << endl;
		assert(false);
	}

	file >> data;
	file.close();

	if (encoder.decode(data) == false) {
		cout << "Could not decode DH parameters." << endl;
		assert(false);
	}

	if (encoder.size() != 3) {
		cout << "Wrong number of DH parameters." << endl;
		assert(false);
	}

	q = encoder.getInteger(0);
	p = encoder.getInteger(1);
	alpha = encoder.getInteger(2);


    /* cout << "q = " << q << endl;
	 cout << "p = " << p << endl;
	 cout << "alpha = " << alpha << endl; */


}

void DHProtocol::alice(const string& host, unsigned int port) {
    //Aufgabe 4c
    //Implementieren der Methode alice(), in der das Diffie-Hellman Protokoll zwischen Alice und Bob abgearbeitet wird
    cout << "Acting as Alice." << endl;
    //Assertion die sicherstellen soll, das p = (2*q)+1, damit p eine sichere Primzahl ist und die Werte, die aus
    //der Konfigurationsdatei ausgelesen und dekodiert wurden richtig sind
    assert(p == ((2*q)+1));
    cout << "I'm Alice host: " << host << " port " << port << endl;
    //Erstellen eines Objekts der Klasse Fortuna-Generator zum erzeugen von Pseudozufallszahlen
    FortunaGenerator fg;
    //Mit dem PRNG soll eine zufällige Zahl a in Z Stern p erzeugt werden, die von Alice geheim gehalten werden soll.
    //Dazu wird die Methode getInteger() des Fortuna-Generators mit p aufgerufen
    Integer a = fg.getInteger(p);

    //berechne alpha hoch a modulo b
    Integer alphaHocha = a_exp_b_mod_c(alpha,a,p);
    //kodiere das berechnete alpha hoch a modulo b als hexstring und lege den kodierten String in message ab
    MessageEncoder me;
    me.append(alphaHocha);
    string message = me.encode();
    //Netzwerk-Kommunikation mit Bob (oder Oscar) innerhalb eines Try-Catch-Blocks, damit ein Fehler des Sockets
    //abgefangen werden kann
    try {
        //Erstellen eines Sockets alice, der sich mit dem als Parameter angegebenen Host und Port zu Bob verbindet
        Socket alice;
        alice.Create();
        alice.Connect(host.c_str(), port);

        //Sende die Nachricht message an Bob, die das kodierte alpha hoch a modulo b enthält
        // Der Integer wurde zu einem (Hex)String kodiert, weil die zu sendenden Daten über einen Byte Pointer
        // bei Send() spezifiziert werden. Der String kann als c string zu einem byte Pointer gecastet werden.
        // Durch das Wählen der Länge message.size() wurd das terminierende Nullbyte des c strings nicht
        // mit versendet.
        alice.Send((byte*)message.c_str(), message.size());

        //Erstellen eines Byte-Arrays buffer der Länge BUFFERSIZE
        byte buffer[BUFFERSIZE];

        //Empfange das von Bob berechnete und kodierte alpha hoch b modulo p und speichere es im Byte-Array buffer.
        //In len wird die Anzahl der erhaltenen Bytes abgespeichert.
        int len = alice.Receive(buffer,BUFFERSIZE);
        //Den Inhalt des Byte-Arrays buffer im String recievedMessage ablegen
        string recievedMessage;
        recievedMessage.assign((char*)buffer,len);

        //Inahlt des MessageEncoders vor der neuen Verwendung leeren
        me.clear();
        //Aufruf der Methode decode() des MessageEncoders mit dem String recievedMessage, um diesen zu dekodieren
        me.decode(recievedMessage);

        //Wenn sich weniger als eine dekodierte Zahl innerhalb des MessageEncoders befindet -> Abbruch
        if(me.size() < 1) {
            return;
        }
        //Dekodiertes Alpha hoch b aus dem MessageEncoder auslesen
        Integer alphaHochb = me.getInteger(0);

        cout << "Berechnung des geheimen Schlüssels" << endl;
        //Berechnung des geheimen Schlüssels durch (alpha ^ b) ^ a mod p. Dieser berechnete Schlüssel ist identisch mit
        //dem von Bob berechneten geheimen Schlüssel, da (alpha ^ b) ^ a mod p = (alpha ^ a) ^ b mod p aufgrund der
        //Potenzgesetze
        Integer secretKey = a_exp_b_mod_c(alphaHochb,a,p);
        //Ausgabe des berechneten geheimen Schlüssels auf dem Bildschirm
        cout << "Secret Key: " << secretKey << endl;
    }
    catch(Socket::Err& err) {
        //Ausgabe einer Fehlermeldung, wenn im obigen try-catch-Block eine Exception geworfen wurde
        cout << "Fehler bei der Verbindung: " << err.GetWhat() << endl;
    }
}

void DHProtocol::bob(unsigned int port) {
	cout << "Acting as Bob." << endl;

	Socket server, client;
	int len;

	server.Create();
	server.Bind(port);
	server.Listen();

	while (true) {
		try {

			if (server.Accept(client) == true) {
				serverBob(client);
			}
		} catch (Socket::Err& err) {
            cout << "Fehler: " << err.GetWhat() << endl;
        }
	}
	server.CloseSocket();
}

bool DHProtocol::serverBob(Socket client) {
    //Aufgabe 4c
    //Implementieren der Methode serverBob() zur Abarbeitung des Diffie-Hellman Protokolls mit dem Client Alice

    //Assertion die sicherstellen soll, das p = (2*q)+1, damit p eine sichere Primzahl ist und die Werte, die aus
    //der Konfigurationsdatei ausgelesen und dekodiert wurden richtig sind
    assert(p == ((2*q)+1));
    // buffer für erhaltenen Daten
    byte* buffer = new byte[BUFFERSIZE];
    // In len wird später die Länge der zuletzt über den socket client erhaltenen Nachricht gespeichert.
    int len;
    string recievedMessage;

    try {
        cout << "Accepting incoming connection..." << endl;
        //Empfangen der von Alice kodierten Nachrichten. Innerhalb der Variable len wird die Länge der erhaltenen
        //Nachricht gespeichert
        len = client.Receive(buffer, BUFFERSIZE);
        //Die erhaltenen Nachricht im String recievedMessage speichern
        recievedMessage.assign((char*) buffer, len);
        //Die kodierte Nachricht in recievedMessage mit einem Objekt der Klasse MessageEncoder dekodieren
        MessageEncoder me;
        me.decode(recievedMessage);
        //Gib False zurück, wenn die von Alice erhaltene Nachricht keinen kodierten Wert enthielt. Dadurch wird
        //signalisiert, dass das Diffie-Hellman Protokoll nicht erfolgreich ablief
        if(me.size() < 1) {
            return false;
        }
        //Das von Alice gesendete alpha hoch a aus dem MessageEncoder an Position 0 auslesen
        Integer alphaHocha = me.getInteger(0);

        //Mithilfe des Pseudo-Zufallsgenerators Fortuna wird eine zufällige Zahl b in Z Stern p berechnet
        FortunaGenerator fg;
        Integer b = fg.getInteger(p);
        //Berechne alpha hoch b mod p
        Integer alphaHochb = a_exp_b_mod_c(alpha,b,p);

        //Leeren des Inhalts des MessageEncoders zur anschliessenden weiternutzung
        me.clear();
        //Kodieren des eben erzeugten Integers alphaHochb mit dem MessageEncoder und abspeichern der Nachricht in der
        //Variable message
        me.append(alphaHochb);
        string message = me.encode();
        //Sende das berechnete und kodierte alpha ^ b mod p an Alice
        client.Send((byte*)message.c_str(),message.length());

        cout << "Berechnung des geheimen Schlüssels" << endl;
        //Berechnen des geheimen Schlüssels durch (alpha ^ a) ^ b mod p. Alice berechnet den selben geheimen Schlüssel,
        //da (alpha ^ a) ^ b mod p = (alpha ^ b) ^ a mod p aufgrund der Potenzgesetze
        Integer secretKey = a_exp_b_mod_c(alphaHocha,b,p);
        //Ausgabe des berechneten geheimen Schlüssels auf dem Bildschirm
        cout << "Secret Key: " << secretKey << endl;
        return true;

    }
    catch (Socket::Err& err) {
        //Abfangen einer geworfenen Socket-Exception und Ausgabe eienr Fehlermeldung auf dem Bildschirm
        cout << "Error: " << err.GetWhat() << endl;
        return false;
    }

}

void DHProtocol::oscar(unsigned int inport, const string& host, unsigned int port) {
	cout << "Acting as Oscar." << endl;

	Socket server, client;
	int len;

	server.Create();
	server.Bind(inport);
	server.Listen();

	while (true) {
		try {

			if (server.Accept(client) == true) {
				serverOscar(client, host, port);
			}
		} catch (Socket::Err& err) {
			cout << "Error: " << err.GetWhat() << endl;
		}
	}
	server.CloseSocket();
}

bool DHProtocol::serverOscar(Socket alice, const string& host, unsigned int port) {
    //Aufgabe 4e
    //Implementierung der Methode serverOscar(), in der die Man-In-The-Middle Attacke durchgeführt wird
    //Der Man-In-The-Middle Oscar sitzt zwischen Alice und Bob und führt das Diffie-Hellman Protocol mit Alice und mit
    //Bob durch. Wenn Alice und Bob ihre Daten mit dem ausgehandelten Schlüssel verschlüsseln, kann der andere
    //Kommunikationspartner diese Nachricht nicht entschlüsseln, da dieser mit Oskar einen anderen Schlüssel
    //ausgehandelt hat. Oskar kann jedoch alle Daten von Alice und Bob entschlüsseln und entsprechend an den anderen
    //der beiden Kommunikationspartner weiterleiten.

    cout << "Aushandeln eines Secret Keys mit Alice:" << endl;
    //Um einen Schlüssel mit dem Client Alice auszuhandeln kann die Methode serverBob() erneut verwendet werden, da
    //auch Oscar hier als Server agieren muss
    bool ret = serverBob(alice);
    //liefere False zurück, wenn das Aushandeln eines Schlüssels mit Alice nicht erfolgreich ablief
    if(ret == false) {
        return false;
    }
    cout << "Aushandeln eines Secret Keys mit Bob:" << endl;
    //Um einen Schlüssel mit dem Server Bob auszuhandeln kann die Methode alice() wiederverwendet werden, da Oscar in
    //diesem Fall ebenfalls als Client Alice gegenüber Bob agieren muss
    this->alice(host,port);
    return true;
}

