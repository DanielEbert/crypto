#include <cassert>
#include "VigenereCipher.h"

int VigenereCipher::decrypt(
        const vector<byte> &cipher_text,
        const vector<byte> &key,
        vector<byte> &plain_text) {
    /*************************************************************************
     * Aufgabe 2.
     *************************************************************************/
    // workaround because mod for negative numbers doesnt convert to positive
    for (int i = 0; i < cipher_text.size(); i++) {
        int val = cipher_text.at(i) - key.at(i % key.size());
        if (val < 0) val += 26;
        plain_text.push_back(byte(val));
    }

    return plain_text.size();
}

int VigenereCipher::encrypt(
        const vector<byte> &plain_text,
        const vector<byte> &key,
        vector<byte> &cipher_text) {
    /*************************************************************************
     * Aufgabe 2.
     *************************************************************************/
    // bytes get converted to int and back to byte
    for (int i = 0; i < plain_text.size(); i++) {
        cipher_text.push_back(
            (plain_text.at(i) + key.at(i % key.size())) % 26
        );
    }

    return cipher_text.size();
}


