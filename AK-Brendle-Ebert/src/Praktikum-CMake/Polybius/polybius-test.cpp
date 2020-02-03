/*
 * Für Testzwecke während der Entwicklung der Polybius Chiffre.
 */
#include <iostream>
#include <string>
#include "PolybiusCipher.h"
#include <cassert>

using namespace std;


void testSetKey() {
    cout << "---\ntestSetKey" << endl;

    PolybiusCipher p;

    bool b = p.setKey("nx94ew1qcav5ujh3df7soipm26ytkbrgzl08");
    bool expectedOutcome = true;
    cout << (b == expectedOutcome ? "T" : "FFF") << endl;

    b = p.setKey("");
    expectedOutcome = false;
    cout << (b == expectedOutcome ? "T" : "FFF") << endl;

    b = p.setKey("423");
    expectedOutcome = false;
    cout << (b == expectedOutcome ? "T" : "FFF") << endl;

    b = p.setKey("nx94ew1qcav5ujh3df7soipm26ytkbrgzl08n");
    expectedOutcome = false;
    cout << (b == expectedOutcome ? "T" : "FFF") << endl;

    b = p.setKey("nx94ew1qcjh3d08f7soipmav5u26ytkbrgzl");
    expectedOutcome = true;
    cout << (b == expectedOutcome ? "T" : "FFF") << endl;

    b = p.setKey("nx94ew1qcav5ujh3df7soipm26ytkbrgzl08nxs");
    expectedOutcome = false;
    cout << (b == expectedOutcome ? "T" : "FFF") << endl;
}

void testPrintKey() {
    cout << "---\ntestPrintKey" << endl;

    PolybiusCipher p;

    p.setKey("nx94ew1qcav5ujh3df7soipm26ytkbrgzl08");
    p.printKey();
}

void testencdec() {
    cout << "---\ntestencdec" << endl;

    PolybiusCipher p;

    p.setKey("nx94ew1qcav5ujh3df7soipm26ytkbrgzl08");
    p.printKey();

    string res1 = p.encrypt("1234");
    string res2 = p.decrypt(res1);

    cout << "1234 -> " << res1 << " -> " << res2 << endl;
}

int main() {

    testSetKey();

    testPrintKey();

    testencdec();

    return 0;
}
