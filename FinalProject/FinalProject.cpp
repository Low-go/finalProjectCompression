

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Node.h"
#include <queue>
#include <unordered_map>
#include <bitset>
#include <filesystem>
#include <chrono>

using namespace std::chrono;


unordered_map<char, string> huffman_list;
int possible_characters[128] = { 0 };		// all of these are set to 0. All ascii encodings
Node* root1;
int total_bits = 0;
string full_text;



// used as comparison for priority queue
struct CompareFreq {
	bool operator()(Node* const& n1, Node* const& n2) {
		return n1->frequency > n2->frequency;
	}
};


// called to sort node objects by frequency
void showpq(priority_queue<Node*, vector<Node*>, CompareFreq > gq)
{
	priority_queue<Node*, vector<Node*>, CompareFreq > g = gq;
	while (!g.empty()) {
		g.pop();
	}
	cout << '\n';

}

vector <Node*> nodes;
priority_queue<Node*, vector<Node*>, CompareFreq> sortedNodes; 

//goes down method recursivly adding 1's and 0's
void grabEncoding(Node* root, string s,ofstream& out) { 
	

	if (root->character != NULL) {
		
		huffman_list.insert({ root->character,s }); // inserts it into the tree
		if (root->character == '\n') {
			out << s << " " << "newline" << endl;
			
		}
		else if (root->character == ' ') {
			out << s << " " << "space" << endl;
			
		}
		else if (root->character == '\r') {
			out << s << " " << "return" << endl;
			
		}
		else if (root->character == '\t') {
			out << s << " " << "tab" << endl;
			
		}
		else {
			out << s << " " << root->character << endl;
			
		}
		
		int temp = root->frequency * s.size();
		total_bits += temp;
		
	}
	else {
		grabEncoding(root->l, s + "0",out);		//recursive method. Goes down tree assigning 0's and 1's
		grabEncoding(root->r, s + "1",out);
	}
}

//responsible for building a Huffman tree
void encoding() {

	
	
	// to test if the letters came out correctly
	for (int j = 0; j < 128; j++) {
		if (possible_characters[j] != 0) {									// only if character was found

			Node* temp = new Node(char(j), possible_characters[j]);			// makes a new node object. Pushes into a vector
			nodes.push_back(temp);
			sortedNodes.push(temp);
		}
	}
	showpq(sortedNodes);

	//pops, adds, resorts until only root remains
	while (sortedNodes.size() != 1) {



		Node* temp = new Node(NULL, NULL); // new Node
		temp->l = sortedNodes.top();		// neighbor nodes are memorized, popped then frequencies used for 
		sortedNodes.pop();
		temp->r = sortedNodes.top();
		sortedNodes.pop();

		temp->frequency = temp->l->frequency + temp->r->frequency;
		sortedNodes.push(temp);
	}
	root1 = sortedNodes.top();
	showpq(sortedNodes);
}


void rawBits(ofstream& out) {
	

	
	string binaryCode;
	
	// reads through text file, assigns each character their respective binary. Binary stored in binaryCode variable
	for (char c : full_text) {			
		
		string code = huffman_list.find(c)->second;

		binaryCode += code;


	}

	// makes sure it is divisible by 8, if not 0s are added onto the end of string
	while (binaryCode.size() % 8 != 0) {	
		binaryCode += "0";
	}

	if (out.is_open()) {
		for (int i = 0; i < binaryCode.length(); i += 8) {

			bitset<8> byte(binaryCode.substr(i, 8));
			out << static_cast<char>(byte.to_ulong());
			
		}
		
		out.close();
	}
	else {
		cout << "File Was NOT able to be opened and written";
	}

	
}


int main(int argc,char** argv)
{
	auto once = high_resolution_clock::now();

	if (argc == 1) {
		cout << " too little or too many command arguments. Just one mate";
		exit(0);
	}

	ifstream ifs;
	ifs.open(argv[1], ios::in);
	if (!ifs) {
		cout << "No text file was found";
		exit(0);
	}

	//file ouput is similar to input
	filesystem::path inputFilePath = argv[1];
	filesystem::path outputFilePath =  (inputFilePath.stem().string() + ".zip301");	// makes sure the file produced has the same name as the original
	ofstream out(outputFilePath, ios::binary);																			// but with a 2 and .zip301 extension
	
	

	// loops through text and gets frequency using ascii
    char c;
	while (ifs.get(c)) {


		full_text += c; 
		int get = c; 
		possible_characters[get] ++; 
	}

	//builds tree
	encoding();	

	// recursively goes down tree, assigns 1's and 0's
	grabEncoding(root1, "",out); 

	out << "*****" << endl;
	out << total_bits << endl;
	
	//translates text into rawbits
	rawBits(out);


	//timer
	auto no_more = high_resolution_clock::now();
	auto last_of_all = (duration<double, milli>(no_more - once)) / 1000.0;

	cout << "Total time : " << last_of_all.count() << endl;
	cout << " " << endl;
	cout << "File Written Succesfully to " << outputFilePath << endl;
	

	return 0;

}

