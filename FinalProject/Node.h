#pragma once

using namespace std;

class Node {

public:
	char character;
	int frequency;
	Node* l;
	Node* r;
	Node(char, int);

};
