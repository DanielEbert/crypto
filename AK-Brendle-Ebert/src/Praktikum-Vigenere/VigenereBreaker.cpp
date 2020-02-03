#include <cassert>
#include <map>
#include <iomanip>
#include <set>
#include <string>
#include <vector>
#include "VigenereBreaker.h"


int VigenereBreaker::gcd(int a, int b) const {
    int r;

    do {
        r = a % b;
        a = b;
        b = r;
    } while (r != 0);

    return a;
}


int VigenereBreaker::gcd(const vector<int> &v) const {
    int r, i;

    assert(v.size() > 0);

    if (v.size() == 1) {
        return v[0];
    }
    else {

        r = gcd(v[0], v[1]);
        i = 2;
        while ((r > 1) && (i < v.size())) {
            r = gcd(r, v[i]);
            i++;
        }
        return r;
    }
}


int VigenereBreaker::kasiskiTest
        (
                const vector<byte> &cipher_text,
                int ngram_len
        ) {
    /*************************************************************************
     * Aufgabe 4.
     *************************************************************************/

    string cipher_string;

    for (int i = 0; i < cipher_text.size(); i++) {
        cipher_string.push_back('A' + cipher_text.at(i));
    }

    map<string, vector<int>> m;

    for(int i = 0; i < cipher_string.length() - ngram_len; i++) {
        string s = cipher_string.substr(i, ngram_len);
        auto iter = m.find(s);
        if (iter == m.end()) {
            vector<int> v{i};
            m.insert(pair<string, vector<int>>(s, v));
        }
        else {
            m[s].push_back(i);
        }
    }

    map<int, int> maxgcd;

    for ( const auto &i : m ) {
        if (i.second.size() >= 3) {
            std::cout << ngram_len << "-gramm: " << i.first
                      << ";   #Vorkommen: " << i.second.size()
                      << ";   gcd: " << gcd(i.second) << "\n";
        }
    }

    // TODO: get max

    return 0;
}

float VigenereBreaker::coincidenceIndex(const vector<byte> &text) {
    /*************************************************************************
     * Aufgabe 6.
     *************************************************************************/

    // zero initialize
    int chars[26] = {};

    for (int i = 0; i < text.size(); i++) {
        int num = (int)text.at(i);
        assert(num >= 0 && num <= 25 && "byte value out of range");
        chars[num] += 1;
    }

    int sum = 0;

    for (int i = 0; i < 26; i++) {
        if (chars[i] <= 1) continue;
        sum += chars[i] * (chars[i] - 1);
    }

    if (text.size() <= 1) {
        // dont divide by 0
        return 0;
    }

    float index = float(sum) / float(text.size() * (text.size() - 1));

    return index;
}


bool VigenereBreaker::coincidenceTest
        (
                const vector<byte> &cipher_text,
                int cols,
                float threshold
        ) {
    /*************************************************************************
     * Aufgabe 7.
     *************************************************************************/

    vector<byte> b[cols];

    for (int i = 0; i < cipher_text.size(); i++) {
        b[i % cols].push_back(cipher_text.at(i));
    }

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < b[i].size(); j++) {
            cout << char(b[i][j] + 'A');
        }
        cout << endl;
    }
     cout << endl << endl;


    for (int i = 0; i < cols; i++) {
        cout << coincidenceIndex(b[i]) << " ";
    }

    cout << endl << endl;

    return false;
}

int VigenereBreaker::mutualCoinIndex
        (
                const vector<byte> &cipher_text,
                int cols,
                int col_i,
                int col_j,
                float threshold
	 ) {
    /*************************************************************************
     * Aufgabe 9.
     *************************************************************************/

    string s[2];

    for (int i = 0; i < cipher_text.size(); i++) {
        if (i % cols == col_i) {
            s[0].push_back(cipher_text.at(i));
        } else if (i % cols == col_j) {
            s[1].push_back(cipher_text.at(i));
        }
    }

    int cur = -1;
    float curMax = -1;

    for (int g = 0; g < 26; g++) {
        // zero initialize
        int c1[26] = {};
        int c2[26] = {};

        for (int i = 0; i < s[0].size(); i++) {
            int num = (int)s[0].at(i);
            assert(num >= 0 && num <= 25 && "byte value out of range");
            c1[num] += 1;
        }

        for (int i = 0; i < s[1].size(); i++) {
            int num = (int)s[1].at(i);
            assert(num >= 0 && num <= 25 && "byte value out of range");
            num -= g;
            if (num < 0) num += 26;
            c2[num] += 1;
        }

        int sum = 0;

        for (int i = 0; i < 26; i++) {
            if (c1[i] == 0 || c2[i] == 0) continue;
            sum += c1[i] * c2[i];
        }

        float index = float(sum) / float(s[0].length() * (s[1].length() - 1));
        if (index > curMax) {
            cur = g;
            curMax =  index;
        }
    }

    if (curMax >= threshold) return cur;

    return -1;
}
