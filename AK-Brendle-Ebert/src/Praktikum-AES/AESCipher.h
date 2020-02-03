#ifndef _AESCIPHER_H
#define _AESCIPHER_H

#include <string>
#include <vector>
#include "AESMath.h"
#include "AESState.h"
#include "AESKeySchedule.h"
#include "BlockCipher.h"

using namespace std;

/**
 * Die Klasse AESCipher implementiert den Algorithmus AES.
 *
 * AES ist eine Block Cipher von Joan Daemen und Vincent Rijmen.
 * AES verwendet das Rijndael Kryptosystem. Dieses ist ein symmetrisches
 * Kryptosystem.
 * Die Klasse bietet Methoden an, um 16 Byte Blöcke mit AES zu ver- und entschlüsseln.
 * Für diese Operationen wird ein Schlüssel benötigt. In dieser Implementation
 * werden Schlüssel der Länge 128, 192 und 256 Bit unterstützt.
 * Wird ein 16 Byte plaintext mit einem Schlüssel k verschlüsselt, dann kann
 * der aus der Verschlüsselung resultierende ciphertext mit demselben Schlüssel
 * k entschlüsselt werden. Der entschlüsselte Text ist wieder derselbe wie der
 * originale plaintext.
 */
class AESCipher : public BlockCipher {
private:
    /**
     * Flag zum Aktivieren von Debug-Ausgaben. Wenn True, dann werden
     * Debug Ausgaben über stdout ausgegeben.
     */
    bool debug_mode;
    /**
     * Zeiger auf ein Objekt der Klasse AESMath, welche Mathematische
     * Operationen auf dem endlichen Körper GF(256) implementiert.
     */
    AESMath aesmath;
    /**
     * Ein Objekt der Klasse AESKeySchedule für die Verwaltung der Rundenschlüssel.
     */
    AESKeySchedule key_schedule;
    /**
     * Ein Objekt der Klasse AESState für die 16 Byte große state des AES.
     * Weiter sind dort die Basisoperationen, die für jede Runde auf den state
     * angewendet werden, implementiert.
     */
    AESState state;

    void debugMessage(size_t round, string msg);

public:

    static const bool Encryption=false;
    static const bool Decryption=true;

    AESCipher(bool debug_mode=false);

    /**
    * Die Methode setKey() ruft die Methode setKey des key_schedules auf, der den
    * key_schedule für den übergebenen Schlüssel berechnet.
    *
    * @param key Schlüssel als Byte-Vektor, für den der Key-Schedule berechnet werden soll
    * @return True, wenn die Berechnung des Key-Schedules erfolgreich ablief. False, wenn
    * die Berechnung des Key-Schedules nicht erfolgreich war
    */
    bool setKey(const vector<byte>& key);

    /**
    * Die Methode encryptBlock() verschlüsselt einen 16-Byte großen Plaintextblock
    * mit dem AES. Dafür werden für jede Runde der Reihe nach die Basisoperationen
    * subBytes, shiftRows, mixColumns und addKey mehrmals auf den state angewandt.
    * Bei der Operation addKey wird darüber hinaus der Rundenschlüssel für die akuelle
    * Runde aus dem key_schedule benötigt. In der letzten Runde wird auf die
    * Basisoperation mixColumns verzichtet.
    *
    * @param plain_text Zeiger auf ein Byte-Array der Größe 16, welches mit dem AES
    * verschlüsselt werden soll.
    * @param cipher_text Zeiger auf ein Byte-Array der Größe 16 in dem das Ergebnis der
    * Verschlüsselung abgelegt wird.
    */
    void encryptBlock(const byte *plain_text, byte *cipher_text);

    /**
    * In decryptBlock() wird ein 16 Byte großer Geheimtextblock entschlüsselt. Dafür werden
    * die inversen Basisoperationen in umgekehrter Reihenfolge zur Verschlüsselung eines
    * Blocks aufgerufen. Für eine Runde ist das: addKey, invMixColumns, invShiftRows, invSubbytes.
    * Für die erste Runde, in welcher die letzte Runde der Verschlüsselung invertiert wird,
    * wird auf die invMixColumns-Operation verzichtet.
    *
    * @param cipher_text Zu entschlüsselnder Geheimtextblock der Länge 16 Bytes
    * @param plain_text In plain_text wird der entschlüsselte Block abgelegt
    */
    void decryptBlock(const byte *cipher_text, byte *plain_text);

    /**
     * Überprüft, ob in die korrekte Länge hat und reserviert Speicher in out.
     * out hat daraufhin (mindestens) die gleiche Länge wie die Eingabe in in.
     * Wenn mode auf Encryption gesetzt ist, dann wird die Methode encrypt()
     * mit in als Plaintext aufgerufen. Danach steht in out der Ciphertext.
     * Andernfalls, wenn mode auf Decryption gesetzt ist, wird die
     * Methode decrypt mit in als Ciphertext aufgerufen. Danach steht in out
     * der entschlüsselte Ciphetext.
     * Für die Dokumentation zur Ver- und Entschlüsselung siehe die encrypt() und
     * decrypt() Methoden.
     * Die Länge des Texts in in muss ein Vielfaches von 16 Byte sein.
     *
     * @param in Je nach mode der Plain- oder Ciphertext, der Ver- oder
     * Entschlüsselt werden soll.
     * @param out In out wird der Ver- oder Entschlüsselte Text von in
     * gespeichert. Es muss kein Speicher für out reserviert werden.
     * @param mode Gibt an, ob der Text in ver- oder entschlüsselt werden
     * soll. Ist mode gleich Encryption oder false, dann wird verschlüsselt.
     * Andernfalls wenn mode gleich Decryption oder true ist, wird entschlüsselt.
     * @return True, wenn der Text in in erfolgreich ver- oder entschlüsselt
     * werden konnte. False, wenn ein Fehler aufgetreten ist, was zum Beispiel
     * der Fall ist, wenn die Eingabe 'in' eine nicht unterstützte Länge hat.
     * Die Länge des zu Texts in 'in' muss ein Vielfaches von 16 Byte sein.
     */
    bool process(const vector<byte>& in, vector<byte>& out, bool mode);

    /**
    * Diese Methode verschlüsselt einen Klartext plain_text mit dem AES und
    * speichert das Ergebnis in cipher_text. Dabei wird jeder 16-Byte große 
    * Block über die Methode encryptBlock() einzeln verschlüsselt. Die Länge
    * des zu verschlüsselnden Klartexts muss ein Vielfaches von 16 sein.
    *
    * @param plain_text Byte-Vektor, der den zu verschlüsselnden Klartext enthält
    * @param cipher_text Byte-Vektor, in dem der verschlüsselte Geheimtext abgelegt
    * wird
    *
    * @return True, falls die Verschlüsselung erfolgreich war, ansonsten False
    */
    bool encrypt(const vector<byte>& plain_text, vector<byte>& cipher_text);

    /**
    * Die Methode decrypt() entschlüsselt den übergebenen Geheimtext cipher_text und
    * legt den entschlüsselnden Klartext in plain_text ab. Die Länge des zu entschlüsselnden
    * Geheimtexts muss ein Vielfaches von 16 Byte sein.
    *
    * @param cipher_text Byte-Vektor, der den zu entschlüsselnden Geheimtext enthält
    * @param plain_text in dem der entschlüsselte Geheimtext abgelegt wird
    *
    * @return True, falls die Entschlüsselung erfolgreich war, ansonsten False
    */
    bool decrypt(const vector<byte>& cipher_text, vector<byte>& plain_text);

    static vector<byte> toVector(const string& msg, size_t block_len=16);

    static string toString(const vector<byte>& vec);
};


#endif
