#include "byte.h"
#include "DESCipher.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    DESCipher desCipher;

    byte key[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    desCipher.computeKeySchedule(key);

    byte roundKeys[16*6];

    desCipher.getKeySchedule(roundKeys);

    for (int i = 0; i < 16; i++) {
        cout << "Rundenschlüssel " << i << ": ";
        for (int j = 0; j < 6; j++) {
            printf("%02x", roundKeys[i]);
        }
        cout << endl;
    }

    return 0;
}