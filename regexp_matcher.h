// PL homework: hw2
// regexp_matcher.h

#ifndef _PL_HOMEWORK_REGEXP_MATCHER_H_
#define _PL_HOMEWORK_REGEXP_MATCHER_H_

#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

struct FSATableElement {
	int state;
	int next_state;
	char str;
};

struct RegExpMatcher {
  // Design your RegExpMatcher structure
  map<pair<set<int>, char>, set<int> > FSA;
  set<int> accept;
  set<int> init_state;
};

// Homework 1.3
bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher);

// Homework 1.3
bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str);

vector<int> FindOR(const char* str, int start, int end);
int BuildNFA(const char*regexp, int state, vector<FSATableElement>* FSA_table, int start_str, int end_str);
void pushNFAelement(vector<FSATableElement>* FSA_table, char input_symbol, int state, int next_state);



#endif  //_PL_HOMEWORK_REGEXP_MATCHER_H_

