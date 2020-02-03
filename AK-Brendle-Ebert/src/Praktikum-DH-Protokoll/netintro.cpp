#include <iostream>
#include <socketft.h>
#include "MessageEncoder.h"

using namespace std;
using namespace CryptoPP;

// Maximum size of the receive buffer
#define MAXSIZE 100000

void alice(string host, unsigned int port) {
	// Socket for connecting the server Bob
	Socket client;
	// Buffer for reading data from the socket
	byte* buffer = new byte[MAXSIZE];
	// length of the received data
	int len=0;
	// helper string
	string data;
	// The message which Alice want to send to Bob
	string msg("Hello Bob! How are you?");

	cout << "Acting as Alice." << endl;

	try {
		// Creating a socket ...
		client.Create();
		// ... and connecting to the server
		client.Connect(host.c_str(), port);

		/*
		 *  Note: if no connection can be established, an
		 *  exception is thrown. Otherwise, the socket is ready
		 *  to use.
		 */

		// Send message to the server
		client.Send((byte*)msg.c_str(), msg.length());

		// Receive data form the server
		len = client.Receive(buffer, MAXSIZE);
		data.assign((char*) buffer, len);
		cout << data << endl;

		//Aufgabe 3c
		//Erweitern von netintro, sodass Alice zwei Integer an Bob sendet und die Summe dieser beiden Integer von Bob
		//zurückgesendet bekommt

		//Erzeugen eines Objekts der Klasse MessageEncoder
		MessageEncoder me;
		//Die beiden Integer 10232 und 8934 zum MessageEncoder hinzufügen
		me.append(Integer("10232"));
		me.append(Integer("8934"));
		//Die kodierte Nachricht, die die beiden Integer-Zahlen enthält mit Hilfe des MessageEncoders kodieren
		string message = me.encode();

		//Senden der Nachricht an Bob. Die Nachricht muss als C-String übergeben werden, da Send ein Byte-Array als
		//Parameter fordert, was äquivalent zu einem C-String ist
		client.Send((byte*)message.c_str(),message.length());

		//Die Antwort von Bob mit der Recieve()-Methode erhalten. Die Antwort wird im Byte-Array buffer abgelegt
		len = client.Receive(buffer,MAXSIZE);
		//Im String summe soll die von Bob erhaltene kodierte Nachricht aus dem Byte-Array buffer als String gespeichert
		//werden
        string summe;
		summe.assign((char*)buffer,len);
		//Leeren des Inhalts des MessageEncoders und anschließendes Dekodieren des Strings summe
		me.clear();
		me.decode(summe);
		//wenn weniger als ein Wert im MessageEncoder enthalten ist, dann wurde von Bob zu wenig erhalten, dann wird im
		//else-Zweig der Verzweigung eine Fehlermeldung ausgegeben
		if(me.size() >= 1) {
		    //Ergebnis der Addition von Bob aus dem MessageEncoder auslesen
            Integer result = me.getInteger(0);
            //Ausgabe der Addition auf dem Bildschirm
            cout << "Received sum of addition: 10232 + 8934 = " << result << endl;
		}
		else {
		    //Ausgabe einer Fehlermeldung, wenn von Bob zu wenig Daten erhalten wurden
		    cout << "Did not receive enough encoded data from Bob" << endl;
		}

	}
	catch (Socket::Err& err) {
		// Dump the error message, of an exception is thrown
		cout << "Error: " << err.GetWhat() << endl;
	}

	// Don't forget to release the allocated memory
	delete [] buffer;
}

bool bob(Socket alice) {
	byte* buffer = new byte[MAXSIZE];
	int len;
	string msg="Hi Alice. I am fine!";
	string data;

	try {
		cout << "Accepting incoming connection..." << endl;

		len = alice.Receive(buffer, MAXSIZE);
		data.assign((char*) buffer, len);
		cout << data << endl;

		alice.Send((byte*) msg.c_str(), msg.length());

		//Aufgabe 3c
		//Erweitern der von netintro, sodass Bob zwei Zahlen von Alice erhält, die Summe von diesen berechnet und das
		//Ergebnis an Alice zurücksendet

		//Empfangen der kodierten Nachricht von Alice, die die beiden zu addierenden Zahlen enthält. Die Länge dieser
		//Nachricht wird in der Variable len gespeichert
		len = alice.Receive(buffer,MAXSIZE);
		//Das empfangene char-Array in einen String umwandeln und der Variable data zuweisen. Hierfür wird die Methode
		//assign() eingesetzt, die den aktuellen Inhalt des Strings durch den Inhalt des übergebenen Byte-Arrays
		//ersetzt
		data.assign((char*) buffer, len);

		//Erzeugen eines Objekts der Klasse MessageEncoder, mit dem die von Alice empfangenen Daten dekodierte werden
		MessageEncoder me;
		me.decode(data);
		//Wenn die Anzahl der dekodierten Werte größer gleich 2 ist, dann hat Alice mindestens zwei Zahlen gesendet
		//und die Summe der Zahlen kann berechnet werden
		if(me.size() >= 2) {
		    //Auslesen der beiden zu addierenden Integer-Werte aus dem MessageEncoder mit der getInteger()-Methode
            Integer summand1 = me.getInteger(0);
            Integer summand2 = me.getInteger(1);
            //berechnen der Summe der beiden von Alice erhaltenen Summanden und speichern der Ergebnisses in der
            //Variable summe
            Integer summe = summand1 + summand2;
            //Leeren des MessageEncoders
            me.clear();
            //Hinzufügen der berechneten Summe zum MessageEncoder, um den Wert zu kodieren und an Alice zurückzuschicken
            me.append(summe);
            //Speichern des kodierten Strings in der Variable summeMessage
            string summeMessage = me.encode();
            //Zuletzt wird die kodierte Summe an Alice gesendet
            alice.Send((byte*)summeMessage.c_str(),summeMessage.length());
		}
		else {
		    //Ausgabe einer Fehlermeldung, wenn Bob zu wenig kodierte Werte von Alice erhalten hat
		    cout << "Did not receive enough encoded data from Alice" << endl;
		}

	} catch (Socket::Err& err) {
		cout << "Error: " << err.GetWhat() << endl;
	}

	delete [] buffer;
	return true;
}

void serverBob(int port) {
	// Socket for the server
	Socket server;
	// Socket for the connection client
	Socket client;

	cout << "Acting as Bob." << endl;

	// Create the server socket
	server.Create();
	// Bind to the given port
	server.Bind(port);
	// Listen on the port and wait for an incoming connection
	server.Listen();

	/*
	 * The server does not stop. This is, after the communication
	 * with Alice is finished, the server resets its data and waits
	 * for another connection.
	 */
	while (true) {

		try {
			/*
			 * On a connection, the client socket is initialized
			 * and given the function which performs the data exchange
			 */
			if (server.Accept(client) == true) {
				bob(client);
			}

		} catch (Socket::Err& err) {
			cout << "Error: " << err.GetWhat() << endl;
		}
	}

	// Close the socket (actually this will never happen!
	server.CloseSocket();
}

void help(string name) {
	cout << "Usage: " << name << " <mode>" << endl << endl;
	cout << "Modes:" << endl;
	cout << "  Alice <hostname> <port>" << endl;
	cout << "  Bob <port>" << endl;
}

int main(int argc, char **argv) {
	/*
	 * Depending on the command line arguments, the program
	 * acts as Alice or Bob
	 */
	if (argc < 2) {
		help(argv[0]);
		return 1;
	}

	string mode = argv[1];

	/*
	 * Act as Alice.
	 */
	if (mode == "Alice") {
		if (argc<3) {
			help(argv[0]);
			return 1;
		} else {
			string host(argv[2]);
			int port = atoi(argv[3]);
			alice(host, port);
			return 0;
		}
	}

	/*
	 * Act as Bob.
	 */
	if (mode == "Bob") {
		if (argc<2) {
			help(argv[0]);
			return 1;
		} else {
			int port = atoi(argv[2]);
			serverBob(port);
			return 0;
		}
	}

	help(argv[0]);
	return 0;
}
