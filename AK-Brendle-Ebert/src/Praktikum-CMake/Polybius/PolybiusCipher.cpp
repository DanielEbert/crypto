#include <cctype>
#include <sstream>
#include <fstream>
#include <random>
#include <ctime>
#include <set>
#include <cassert>
#include "PolybiusCipher.h"

PolybiusCipher::PolybiusCipher() : decrypt_map(), encrypt_map() {
  /****************************************************************************
   * Aufgabe 9.
   ****************************************************************************/

  setKey("0123456789abcdefghijklmnopqrstuvwxyz");
}

PolybiusCipher::~PolybiusCipher() {
}


bool PolybiusCipher::setKey(string key) {
  /****************************************************************************
   * Aufgabe 8.
   ****************************************************************************/
  
  // returns true if key consists of every element in {a,b,...,z,0,1,...9} once
  set<int> s;

  string klartextRaum = "0123456789abcdefghijklmnopqrstuvwxyz";

  int klartextRaumLength = klartextRaum.length();

  for (int i = 0; i < klartextRaumLength; i++) {
    bool containsElement = s.find(klartextRaum[i]) != s.end();
    if (containsElement) {
      s.clear();
      return false;
    }
    s.insert(klartextRaum[i]);
  }

  int keyLength = key.length();

  for (int i = 0; i < keyLength; i++) {
    auto elemPos = s.find(key[i]);
    if (elemPos != s.end()) s.erase(elemPos);
    else {
      s.clear();
      return false;
    }
  }

  bool isEmpty = s.empty();
  s.clear();
  if (!isEmpty) return false;

  int pos = 0;
  for (int i = 1; i <= 6; i++) {
    for (int j = 1; j <= 6; j++) {
      //TODO: can i use s for both? what if 1 is deleted?
      string s(1, key[pos]);
      encrypt_map[s] = to_string(i) + to_string(j);
      decrypt_map[encrypt_map[s]] = s;
      pos += 1;
    }
  }

  return true;
}

void PolybiusCipher::printKey() const {
  /****************************************************************************
   * Aufgabe 10.
   ****************************************************************************/

  for (int i = 1; i <= 6; i++) {
    for (int j = 1; j <= 6; j++) {
      cout << decrypt_map.at(to_string(i) + to_string(j)) << " ";
    }
    cout << endl;
  }
  cout << endl;
}


string PolybiusCipher::randomKey() {
    string key = "0123456789abcdefghijklmnopqrstuvwxyz";
    default_random_engine generator(time(NULL));
    uniform_int_distribution<int> distribution(0,key.size()-1);

    for (size_t i=0; i<key.size(); i++) {
        swap(key[i],key[distribution(generator)]);
    }

    return key;
}

void PolybiusCipher::tenKeys() {
  for (int i = 0; i < 10; i++) {
    cout << randomKey() << endl;
  }
}


string PolybiusCipher::encrypt(string plain_text) const {
  /****************************************************************************
   * Aufgabe 12a.
   ****************************************************************************/
    string cipher_text;

    for (int i = 0; i < plain_text.size(); i++) {
      string cur = string(1, plain_text.at(i));
      // if cur not in map: skip
      if ( (cur >= "0" && cur <= "9") || (cur >= "a" && cur <= "z"))
        cipher_text.append(encrypt_map.at(cur));
    }

    return cipher_text;
}

string PolybiusCipher::decrypt(string cipher_text) const {
  /****************************************************************************
   * Aufgabe 12b.
   ****************************************************************************/
    string plain_text;

    string n;

    for (int i = 0; i < cipher_text.length(); i++) {
      if(cipher_text[i] < '0' || cipher_text[i] > '9') continue;
      n.append(cipher_text.substr(i,1));
    }

    cipher_text = n;

    assert(cipher_text.size() % 2 == 0);

    for (int i = 0; i < cipher_text.size(); i+=2) {
      string cur = cipher_text.substr(i, 2);
      plain_text.append(decrypt_map.at(cur));
    }

    return plain_text;
}

string PolybiusCipher::encdec(string plain_text) {
  return decrypt(encrypt(plain_text));
}

