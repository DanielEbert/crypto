#include <iostream>
#include <cstdlib>
#include "DHProtocol.h"

using namespace std;

void help(string name) {
	cout << "Usage: " << name << " <mode>" << endl << endl;
	cout << "Modes:" << endl;
	cout << "  Alice <hostname> <port>" << endl;
	cout << "  Bob <port>" << endl;
	cout << "  Oscar <inport> <hostname> <port>" << endl;
}

int main(int argc, char **argv) {
	DHProtocol dh("../../data/dhparams.txt");

	if (argc < 2) {
		help(argv[0]);
		return 1;
	}

	string mode = argv[1];

	if (mode == "Alice") {
		if (argc<3) {
			help(argv[0]);
			return 1;
		} else {
			string host(argv[2]);
			unsigned int port = atoi(argv[3]);
			dh.alice(host, port);
		}
	}

	if (mode == "Bob") {
		if (argc<2) {
			help(argv[0]);
			return 1;
		} else {
			unsigned int port = atoi(argv[2]);
			dh.bob(port);
		}
	}

	if (mode == "Oscar") {
		if (argc<3) {
			help(argv[0]);
			return 1;
		} else {
			unsigned int inport = atoi(argv[2]);
			string host(argv[3]);
			unsigned int port = atoi(argv[4]);
			dh.oscar(inport, host, port);
		}
	}
	return 0;
}
