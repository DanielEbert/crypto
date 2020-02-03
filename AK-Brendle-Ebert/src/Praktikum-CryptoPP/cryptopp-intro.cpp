#include <cassert>
#include <iostream>
#include <string>
#include "filters.h" // StringSink
#include "hex.h" // HexEncoder
#include "sha.h"
#include "aes.h"
#include "modes.h"
#include "base64.h"
#include "files.h" // FileSink
#include "osrng.h" // BlockingRNG
#include "integer.h" // Integer
#include "nbtheory.h" // RabinMillerTest
#include "BlumBlumShubGenerator.h"
#include "../Praktikum-CMake/Polybius/Toolbox.h"

#include <math.h>

using namespace std;
using namespace CryptoPP;

// Convertiert/Encodiert Bytes in (param) src und gibt 
// den zur Basis 16 encodierten Hexstring zurück.
// Als Hexstring wird eine Zeichenkette bezeichnet, in der die Bytes/Characters
// zur Basis 16 kodiert gespeichert sind.
string toHex(string src) {
    string dst;
    StringSink* sink = new StringSink(dst);
    HexEncoder* encoder = new HexEncoder(sink);
    StringSource(src, true, encoder);
    return dst;
}

// Convertiert/Decodiert den Hexstring in (param) src und gibt 
// den decodierten String zurück.
string fromHex(string src) {
    string dst;
    StringSource(src, true,
        new HexDecoder(
            new StringSink(dst)
        )
    );
    return dst;
}
 
void DataFlowExercise() {
    /*************************************************************************
     * Aufgabe 2.
     *************************************************************************/
    cout << endl
         << "Data Flow Exercise:" << endl
         << "===================" << endl << endl;
 
    string src = "Kryptographie macht Spaß!!!";

    try {
        // Convertiert die Bytes in src zu Base 16 encodiertem Hex.
        string dst = toHex(src);
        cout << src << " >  HexEncoder > " << dst << endl;
        // Der Hexstring dst dekodiert sollte der gleiche
        // wie das Original src sein.
        assert(src == fromHex(dst));

        src = "4963682062696E2065696E20537472696E6720696E204865786164657A696D616C6B6F64696572756E6721";
        // Convertiert einen auf Basis 16 encodierten Hexstring zu Bytes.
        string decoderDst = fromHex(src);
        cout << src.substr(0, 20) << "... > HexDecoder > " << decoderDst << endl;
        // Das Encodieren des Strings decoderDst sollte wieder wie src sein.
        assert(src == toHex(decoderDst));

        // Die Funktion readFromFile aus der Toolbox wurde geändert. 
        // Zuerst haben wir die Funktionalität entfernt, dass alle 
        // Zeichen zu Kleinbuchstaben umgewandelt werden, indem 
        // wir den Aufruf von 'tolower(c)' in readFromFile() entfernt haben.
        // Zweitens haben wir als drittes Argument einen neuen bool 
        // hinzugefügt, welcher skipNewlines heißt. Dieses Argument ist 
        // standardmäßig auf 'false' voreingestellt. Das Setzen von 
        // skipNewlines auf 'true' entfernt Newlines ('\n') in 
        // der Funktionsausgabe. Aufgrund des voreingestellten 'false' wird
        // die Funktionsweise für vorherigen und zukünftigen Code, welche
        // die readFromFile Funktion nutzen, nicht verändert. Falls zukünftige
        // Funktionen das Verwenden von tolower(c) bei readFromFile() erwarten, 
        // müsste readFromFile abgeändert werden.
        if (!Toolbox::readFromFile("data/base64data.txt", src, /*skipNewlines=*/true)) {
            // Das Öffnen der Datei ist fehlgeschlagen. Wir fahren Sie trotzdem 
            // fort, um die anderen Aufgaben auszuführen.
            cerr << "Auslesen der File data/base64data.txt fehlgeschlagen. Continuing." << endl;
        }

        cout << "File data/base64data.txt Inhalt: " << src << endl;

        dst = "";
        // Dekodiert Base64 kodierte Daten in src. Die kodierten Daten 
        // werden in 'dst' als String abgespeichert..
        StringSource(src, true,
            new Base64Decoder(
                new StringSink(dst)
            )
        );

        cout << "Decoded File Inhalt: " << dst << endl;
    } catch(const Exception& e) {
        cerr << e.what() << endl;
        assert(false);
    }
}

void EncryptionExercise_Part1() {
    byte key[16];
    string encoded_key = "08a8cbfe7a3d1262c8abc3d1197dfefe";
    // Konvertiert den Hexstring 'encoded_key' zu Bytes.
    // Das Ergebnis wird in einem 'byte' (bzw. char) Array abgespeichert.
    // Da ein Character des Hexstrings in 4 Bits abgespeichert werden kann,
    // können die 32 Characters des Hexstrings in einem 16 byte großen 
    // Array abgespeichert werden. Diese Umwandlung wird benötigt,
    // da wir den key als bytes dekodiert an das AES Objekt übergeben müssen.
    StringSource(encoded_key, true,
        new HexDecoder(
            new ArraySink(key, 16)
        )
    );

    string plain_text = "Streng geheime Botschaft";
    string cypher_text;

    // AES Implementierung für die Verschlüsselung aus der CryptoPP-Bibliothek
    // im ECB Modus
    ECB_Mode<AES>::Encryption aes_enc;

    // Verwendung der 16 Bytes des 'key'-Arrays für zukünftige Verschlüsselung
    // mit dem AES Objekt
    aes_enc.SetKey(key, sizeof(key));

    // Verschlüsselt den Inhalt von plain_text mit AES im ECB-Modus
    // Der HexEncoder wird verwendet, um die Bytes der AES Ausgabe in 
    // menschenlesbaren Hex-Code zu konvertieren.
    StringSource(plain_text, true,
        new StreamTransformationFilter(aes_enc,
            new HexEncoder(
                new StringSink(cypher_text)
            )
        )
    );

    cout << plain_text << " > AES Encrypt > " << cypher_text << endl;

    // AES Implementierung für die Entschlüsselung aus der CryptoPP-Bibliothek
    // mit dem ECB Modus.
    ECB_Mode<AES>::Decryption aes_dec;

    // Derselbe Key aus der Verschlüsselung wird bei AES ebenfalls
    // für die Entschlüsselung verwendet.
    aes_dec.SetKey(key, sizeof(key));

    string p;

    // Während des Verschlüsselns haben wir die AES Ausgabe in einen HexCode
    // konvertiert. Für die Entschlüsselung benötigt das AES Objekt ebenfalls
    // wieder bytes. Deshalb wird der HexDecoder wird verwendet, um den 
    // Hexstring zurück in Bytes zu konvertieren.
    StringSource(cypher_text, true,
        new HexDecoder(
            new StreamTransformationFilter(aes_dec,
                new StringSink(p)
            )
        )
    );

    cout << cypher_text << " > AES Decrypt > " << p << endl;
    assert(plain_text == p);
}

void EncryptionExercise_Part2() {
    string cypher_text;

    string src;
    if (!Toolbox::readFromFile("data/aescipher.txt", src, true)) {
        cerr << "Auslesen der File data/aescipher.txt fehlgeschlagen. Continuing." << endl;
    }

    cout << "File data/aescipher.txt Inhalt: " << src << endl;

    // Die Funktion SetKeyWithIV benötigt den Schlüssel und IV als Bytes 
    // dekodiert und die beiden Argumente werden über ein Byte-Array übergeben.
    // Das Konvertieren und Abspeichern läuft wie in der letzten Aufgabe
    // beim Dekodieren von 'encoded_key' ab.
    byte key[16];
    string encoded_key = "47656865696D65725363686CC3BC7373";
    StringSource(encoded_key, true,
        new HexDecoder(
            new ArraySink(key, 16)
        )
    );

    byte iv[16];
    string encoded_iv = "496E697469616C6973696572756E6773";
    StringSource(encoded_iv, true,
         new HexDecoder(
             new ArraySink(iv, 16)
         )
    );

    // AES Implementierung für die Entschlüsselung aus der CryptoPP-Bibliothek
    // mit dem CTR Modus.
    CTR_Mode<AES>::Decryption aes_crt_dec;

    // Die Bytes im Schlüssel-Array und IV-Array werden in der 
    // zukünftigen Dekodierung verwendet.
    aes_crt_dec.SetKeyWithIV(key, sizeof(key), iv);

    string dst;
    // Der String in src ist eine base64-kodierte Zeichenkette. 
    // Der Base64Decoder wird verwendet, um die kodierte Zeichenkette in Bytes 
    // zu konvertieren. Dies ist für den AES StreamTransformationFilter 
    // erforderlich.
    StringSource(src, true,
        new Base64Decoder(
            new StreamTransformationFilter(aes_crt_dec,
                 new StringSink(dst)
            )
        )
    );

    cout << src.substr(0, 20) << "... > AES Decrypt > " << dst << endl;
}
 
void EncryptionExercise() {
    /*************************************************************************
     * Aufgabe 3.
     *************************************************************************/
    cout << endl
         << "Encryption Exercise:" << endl
         << "====================" << endl
         << endl;

    try {
        // Aufgabe 3a) und 3b)
        EncryptionExercise_Part1();

        // Aufgabe 3c)
        EncryptionExercise_Part2();
    } catch(const Exception& e) {
        // Dieser Zweig wird ausgeführt, wenn eine Exception im Try-Block 
        // ausgelöst wird, z.B. wenn CryptoPP während der AES-Kodierung oder 
        // -Dekodierung einen Error/Exception auslöst. Dabei wird der Grund für 
        // die Exception ausgegeben und das Programm wird beendet
        cerr << e.what() << endl;
        assert(false);
    }
}
 
 
void HashExercise() {
    /*************************************************************************
     * Aufgabe 4.
     *************************************************************************/
    cout << endl
         << "Hash Exercise:" << endl
         << "==============" << endl
         << endl;
 
    string message = "Kryptographie macht immer noch Spaß!!!";
 
    string digest;
    // SHA256 Implementierung aus der CryptoPP-Bibliothek
    SHA256 hash;
    // Der String in message wird mit CryptoPP's implementation von SHA256
    // gehasht. Der Hash wird im 'digest' String abgespeichert.
    StringSource(message, true,
        new HashFilter(hash,
            new StringSink(digest)
        )
    );

    // Der SHA256 Hash in 'hash' wird zu einem Hexstring umgewandelt,
    // damit er menschenlesbar ausgegeben werden kann.
    string dst;
    StringSink* sink = new StringSink(dst);
    HexEncoder* encoder = new HexEncoder(sink);
    StringSource(message, true, encoder);

    cout << "Message Hash: " << dst << endl;

    // HASH_AT_BEGIN: Gibt an, dass der SHA256 Hash in 'digest' am Anfang der 
    // zu übergebenen Zeichenkette stehen soll, die 'message' wird dann direkt 
    // nach dem 'digest' angehängt. 
    // PUT_RESULT: Gibt an, dass das Ergebnis der Verifikation in der Variable 
    // 'result' gespeichert werden soll.
    word32 flags = HashVerificationFilter::HASH_AT_BEGIN
                   | HashVerificationFilter::PUT_RESULT;

    // Es wird überprüft, ob es sich bei dem Digest um den SHA256 Hash von 
    // 'message' handelt. Davor muss mit hash.Restart() der den internen Zustand 
    // des SHA256 Hash Objekts neu gestartet werden. 
    // Das Ergebnis wird als Boolean in der Variable 'result' gespeichert.
    bool result;
    hash.Restart();
    StringSource(digest + message, true,
        new HashVerificationFilter(hash,
            new ArraySink((byte*)&result, sizeof(result)),
                flags
        )
    );

    cout << "Verification " << (result ? "successful" : "failed") << endl;

    assert(result);

    // Aufgabe b: Mit *sehr* hoher Wahrscheinlichkeit wird nun die Nachricht
    // gehasht nicht gleich 'digest' sein. Dies führt dazu, dass die
    // Verifikation 'false' in der 'result' Variable zurückliefert.

    // Aufgabe c: Ersetze 'HashVerificationFilter::HASH_AT_BEGIN'
    //              durch 'HashVerificationFilter::HASH_AT_END'.
}

// Berechnet (a^b) % n und gibt das Ergebnis dieser Berechnung als Integer 
// Objekt aus der CryptoPP lib zurück. Diese Berechnung erfolgt nach dem 
// Pseudocode für Modulare Exponentiation vom Zahlentheorie-Vorkurs.
Integer modexp(const Integer &a, const Integer &b, const Integer &n) {
    /*************************************************************************
     * Aufgabe 5a.
     *************************************************************************/
    Integer d("1");

    for (int i = b.BitCount()-1; i >= 0; i--) {
        d = (d * d) % n;
        if (b.GetBit(i) == 1) {
            d = (d * a) % n;
        }
    }
 
    return d;
}
 
 
void IntegerExercise() {
    /*************************************************************************
     * Aufgabe 5bc.
     *************************************************************************/
    cout << endl
         << "IntegerExercise:" << endl
         << "================" << endl << endl;

    // Berechnet (a^b) % n mit unserer Implementation
    // a = 1. argument; b = 2. arg; n = 3. arg
    Integer res = modexp(Integer(371), Integer(18961551), Integer(2371));

    // Berechnet (a^b) % n mit der Implementation der CryptoPP Bibliothek
    // a = 1. argument; b = 2. arg; n = 3. arg
    Integer res2 = a_exp_b_mod_c(Integer(371), Integer(18961551), Integer(2371));

    // Überprüfung, ob das Ergebnis der Crypto++ Funktion a_exp_b_mod_c mit
    // der von uns implementierten modexp Funktion übereinstimmt.
    assert(res == res2);

    Integer p(95957);
    Integer a(58788);
    // Da p eine ungerade Primzahl ist, gilt nach dem Euler Kriterium auf
    // Seite 43 der 'Zufallszahlengeneratoren' Folien, dass a ein
    // quadratischer Rest modulo p genau dann ist, wenn a^((p-1)/2) kongruent zu
    // 1 (mod p) ist.
    bool isQR = modexp(a, Integer((p-1) / 2), p) == 1;
    cout << "Die Zahl " << a << " ist " << (isQR ? "ein" : "kein") << " QR mod " << p << endl;
}

void RNGExercise() {
    /*************************************************************************
     * Aufgabe 6cfhj.
     *************************************************************************/
    cout << endl
         << "RNGExercise:" << endl
         << "============" << endl << endl;

    // Das n für den BBS wird aus dem Produkt von zwei Primzahlen gebildet.
    Integer n = Integer("1252910265243849922375596598575099209083498535192739493227403") *
                Integer("1476222059624949757818143837507324048590620075519516306265283");

    // Initialisierung des BlumBlumShubGenerators mit n für "mod n" und Seed 42.
    BlumBlumShubGenerator bbs(n, Integer(42));

    cout << "Generierung von 20 Zufallsbits: ";
    for (int i = 0; i < 20; i++) {
        cout << int(bbs.getBit());
    }
    cout << endl << endl;

    cout << "Generierung von 5 Zufallsbytes: ";
    for (int i = 0; i < 5; i++) {
        cout << int(bbs.getByte()) << " ";
    }
    cout << endl << endl;

    byte array[100];
    bbs.getBlock(array, 100);
    cout << "Generierung von 100 Zufallsbytes: ";
    for (int i = 0; i < 100; i++) {
        cout << int(array[i]) << " ";
    }
    cout << endl << endl;

    cout << "Generierung einer Zufallszahl mit 200 Bytes: " << bbs.getInteger(200);
    cout << endl << endl;


    Integer sum;
    cout << "Generierung von 1000 gleichverteilten Integern aus der Menge {0,1,...,24}. ";
    for (int i = 0; i < 1000; i++)
        sum += bbs.getInteger(Integer(25));
    cout << "Generierte Zahlen summiert: " << sum
         << " Summiert sollte das irgendwo in der Nähe von " << 12 * 1000 << " sein." << endl << endl;

    cout << "Generierung einer Zufallszahl aus der Menge  {0, 1, . . . , 2^30 − 1}: "
         << bbs.getInteger(Integer(1 << 30)-1) << endl;
}
 
 
int main() {
    DataFlowExercise();
    EncryptionExercise();
    HashExercise();
    IntegerExercise();
    RNGExercise();
 
    return 0;
}
