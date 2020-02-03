#include <cassert>
#include <iostream>
#include <iomanip>
#include "AESCipher.h"

using namespace std;

AESCipher::AESCipher(bool debug_mode) : debug_mode(debug_mode), aesmath(),
    key_schedule(aesmath, debug_mode), state(aesmath, debug_mode) {
}

bool AESCipher::encrypt(const vector<byte>& plain_text, vector<byte>& cipher_text) {
    /*
     * Aufgabe 24b
     */
    // ruft process im mode Encryption auf.
    // in plain_text steht der zu verschlüsselnde Klartext, welcher in cipher_text
    // gespeichert wird.
    return process(plain_text, cipher_text, Encryption);
}

bool AESCipher::decrypt(const vector<byte>& cipher_text, vector<byte>& plain_text) {
    /*
     * Aufgabe 24c
     */
    // ruft process im mode Decryption auf.
    // in cipher_text steht der zu entschlüsselnde Ciphertext, welcher in plain_text
    // gespeichert wird.
    return process(cipher_text, plain_text, Decryption);
}

bool AESCipher::process(const vector<byte>& in, vector<byte>& out, bool mode) {
    /*
     * Aufgabe 24a
     */
    
    // AES ist eine Blockchiffre mit Blocklänge 16 Bytes. Der zu ver- oder entschlüsselnde
    // Text muss als Größe ein Vielfaches von 16 besitzen. Ansonsten wird
    // false zurückgegeben, was einen Fehler signalisiert.
    if (in.size() % 16 != 0) {
        return false;
    }
    
    // Der Vector ist im Grunde ein Array. Dieses Array wird auf die Größe des
    // in Arrays gesetzt, damit das Ergebnis dort gespeichert
    // werden kann.
    out.resize(in.size());
    
    // In jedem Schleifendurchlauf wird ein Block (16 Bytes) verschlüsselt.
    for(int i = 0; i < in.size(); i+= 16) {
        // Der mode gibt an, ob in verschlüsselt oder entschlüsselt werden soll.
        // Das Ergebnis wird in beiden Fällen in out geschrieben.
        if (mode == Encryption) {
            // An encryptBlock werden der Reihe nach 16 Bytes des plain_texts
            // übergeben. Auch die Position, &out[i], an dem die 16 verschlüsselten Bytes
            // geschrieben werden sollen, werden übergeben.
            encryptBlock(&in[i], &out[i]);
        } else {
            // an decryptBlock werden der Reihe nach 16 Bytes des cipher_text
            // übergeben. Auch die Position, &out[i], an dem die 16 entschlüsselten Bytes
            // geschrieben werden sollen, werden übergeben.
            decryptBlock(&in[i], &out[i]);
        }
    }
    return true;
}

bool AESCipher::setKey(const vector<byte>& key) {
    return key_schedule.setKey(key);
}

void AESCipher::debugMessage(size_t round, string msg) {
    if (debug_mode) {
        cout << "round[" << dec << setw(2) << setfill(' ') << round
            << "]." << msg << endl;
    }
}

void AESCipher::decryptBlock(const byte *cipher_text, byte *plain_text) {
    /*
     * Aufgabe 23
     */

    // Am Anfang wird die state auf den zu entschlüsselnden cipher_text gesetzt.
    state.set(cipher_text);
    debugMessage(0, "iinput " + state.format());

    // Da jede der Basisoperationen für sich invertierbar ist, besteht  die
    // Entschlüsselung in der Anwendung der inversen  Transformationen in
    // umgekehrter Reihenfolge.

    // Inverse Final Round
    // In der letzten Runde der Verschlüsselung wurde die SubBytes gefolgt von
    // der ShiftRows und abschließend die addKey Transformation mit dem letzten
    // Rundenschlüssel in key_schedule ausgeführt.
    // In der letzten Runde der verschlüsselung wurde keine mixColumns
    // Transformation auf die State angewandt.
    // Um die letzte Runde zu invertieren müssen die inversen Varianten dieser
    // Transformationen in umgekehrter Reihenfolge auf die state bzw. den
    // cipher_text angewandt werden.
    // addKey XOR verknüpft den Rundenschlüssel mit der State. Für XOR wird
    // keine separate inverse Funktion benötigt, weil XOR selbst das XOR der
    // Verschlüsselung invertiert.
    state.addKey(key_schedule.getRoundKey(key_schedule.getNrOfRounds()));
    debugMessage(0, "ik_sch " + key_schedule.formatRoundKey(key_schedule.getNrOfRounds()));
    debugMessage(key_schedule.getNrOfRounds(), "istart " + state.format());
    state.invShiftRows();
    debugMessage(key_schedule.getNrOfRounds(), "is_row " + state.format());
    state.invSubBytes();
    debugMessage(key_schedule.getNrOfRounds(), "is_box " + state.format());

    // Nach der inversen letzten Runde der Verschlüsselung müssen noch die
    // anderen Runden der Verschlüsselung invertiert werden. Da bei der
    // Verschlüsselung ebenfalls die MixColumns verwendet worden ist, muss hier
    // bei der Entschlüsselung ebenfalls die inverse MixColumns Funktion
    // verwendet werden.
    for (int i = 1; i < key_schedule.getNrOfRounds(); i++) {
        state.addKey(key_schedule.getRoundKey(key_schedule.getNrOfRounds()-i));
        debugMessage(i, "ik_sch " + key_schedule.formatRoundKey(key_schedule.getNrOfRounds()-i));
        debugMessage(i, "ik_add " + state.format());
        state.invMixColumns();
        debugMessage(i, "istart " + state.format());
        state.invShiftRows();
        debugMessage(i, "is_row " + state.format());
        state.invSubBytes();
        debugMessage(i, "is_box " + state.format());
    }

    // Am Anfang der Verschlüsselung wurde der initiale Cipher Key mit der State
    // XOR verknüpft. Diese Transformation muss ebenfalls rückgängig gemacht
    // werden.
    state.addKey(key_schedule.getRoundKey(0));
    debugMessage(key_schedule.getNrOfRounds(), "ik_sch " + key_schedule.formatRoundKey(0));

    // Abschließend steht der entschlüsselte cipher_text in state. Dieser kann
    // mit state.get an *plain_text kopiert werden.
    state.get(plain_text);
    debugMessage(key_schedule.getNrOfRounds(), "ioutput " + state.format());
}

void AESCipher::encryptBlock(const byte *plain_text, byte *cipher_text) {
    /*
     * Aufgabe 22
     */
    // 16 Byte des state wird auf die 16 Bytes ab *plain_text gesetzt.
    state.set(plain_text);
    // Nach jeder state Transformation wird durch den Aufruf der debugMessage
    // Funktion die state menschenlesbar über stdout ausgegeben, vorausgesetzt
    // das debug_mode flag ist auf true gesetzt.
    debugMessage(0, "input " + state.format());
    // Der initiale Rundenschlüssel K wird der State hinzugefügt.
    // Dabei wird der Rundenschlüssel mit der state XOR verknüpft.
    state.addKey(key_schedule.getRoundKey(0));
    // Bei der addKey Transformation wird ebenfalls der verwendete
    // Rundenschlüssel ausgegeben.
    debugMessage(0, "k_sch " + key_schedule.formatRoundKey(0));

    // key_schedule.getNrOfRounds() gibt die Anzahl der AES Runden an. Dabei gilt
    // zu beachten, dass in der letzten Runde keine mixColumns Transformation
    // angewendet wird. Die Schleife bearbeitet deshalb die ersten 
    // getNrOfRounds()-1 Runden.
    for(int i = 1; i < key_schedule.getNrOfRounds(); i++) {
        // Dabei werden der Reihe nach die subBytes, shiftRows und mixColumns
        // Transformationen auf die State angewandt.
        debugMessage(i, "start " + state.format());
        state.subBytes();
        debugMessage(i, "s_box " + state.format());
        state.shiftRows();
        debugMessage(i, "s_row " + state.format());
        state.mixColumns();
        debugMessage(i, "m_col " + state.format());
        // Abschließend wird ein Rundenschlüssel auf den state XOR'd.
        // Alle Rundenschlüssel wurden im Vorfeld in key_schedule erzeugt.
        state.addKey(key_schedule.getRoundKey(i));
        debugMessage(i, "k_sch " + key_schedule.formatRoundKey(i));
    }

    // Final Round
    // In der letzten Runde wird keine mixColumns Transformation auf die State
    // angewandt. Ansonsten werden die Transformationen wie in den bisherigen
    // Runden angewandt.
    debugMessage(key_schedule.getNrOfRounds(), "start " + state.format());
    state.subBytes();
    debugMessage(key_schedule.getNrOfRounds(), "s_box " + state.format());
    state.shiftRows();
    debugMessage(key_schedule.getNrOfRounds(), "s_row " + state.format());
    state.addKey(key_schedule.getRoundKey(key_schedule.getNrOfRounds()));
    debugMessage(key_schedule.getNrOfRounds(), "k_sch " + key_schedule.formatRoundKey(key_schedule.getNrOfRounds()));
    
    // An dieser Stelle steht in state der verschlüsselte plain_text.
    // die 16 Byte des State werden an die 16 Bytes ab *cipher_text kopiert.
    state.get(cipher_text);
    debugMessage(key_schedule.getNrOfRounds(),"output " + state.format());
}

vector<byte> AESCipher::toVector(const string& msg, size_t block_len) {
    assert((block_len>0) && (block_len<=32) );

    vector<byte> v;

    copy(msg.begin(), msg.end(), back_inserter(v));

    v.push_back(0);
    while (v.size() % block_len != 0) {
        v.push_back(1);
    }

    return v;
}

string AESCipher::toString(const vector<byte>& vec) {
    auto it = vec.begin();
    string r;

    while ((*it!=0) and (it!=vec.end())) {
        r.push_back(*it);
        it++;
    }
    return r;
}
