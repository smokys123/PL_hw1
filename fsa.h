#pragma once
// PL homework: hw1
// fsa.h

#ifndef _PL_HOMEWORK_FSA_H_
#define _PL_HOMEWORK_FSA_H_

#include <vector>
#include <string>
#include <set>
#include <map>

// Valid characters are alphanumeric and underscore (A-Z,a-z,0-9,_).
// Epsilon moves in NFA are represented by empty strings.

using namespace std;

struct FSATableElement {
	int state;
	int next_state;
	std::string str;
};

struct DFATableElement {
   set<int> state;
   set<int> next_state;
   string str;
};

struct FiniteStateAutomaton {
	map<pair<set<int>, char>, set<int> > FSA;
    set<int> accept;
	set<int> init_state;
};

// Run FSA and return true if str is matched by fsa, and false otherwise.
bool RunFSA(const FiniteStateAutomaton& fsa, const std::string& str);

bool BuildFSA(const std::vector<FSATableElement>& elements,
	const std::vector<int>& accept_states,
	FiniteStateAutomaton* fsa);

bool FindNextState(vector<FSATableElement> elements, char symbol, set<int> state,set<int>& next_states);
bool FindinputSymbols(const std::vector<FSATableElement>& elements);
#endif //_PL_HOMEWORK_FSA_H_

