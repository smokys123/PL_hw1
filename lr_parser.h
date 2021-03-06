// PL homework: hw2
// lr_parser.h

#ifndef _PL_HOMEWORK_LR_PARSER_H_
#define _PL_HOMEWORK_LR_PARSER_H_

#include <string>
#include <vector>
#include <stack>
using namespace std;

enum LRAction {
  INVALID = 0,
  SHIFT = 1,
  REDUCE = 2,
  ACCEPT = 3,
  GOTO = 4
};

// The data fields for each type of action:
//   SHIFT : symbol = the input character, next_state = the next state.
//   REDUCE : symbol = lhs of the rule, next_state = the rule id.
//   ACCEPT : none.
//   GOTO : symbol = the state on the stack top, next_state = the next state.

struct LRTableElement {
  int state;
  int symbol;
  LRAction action;
  int next_state;
};

struct LRRule {
  int id;
  int lhs_symbol;
  int num_rhs;  // You only need the number of symbols in RHS.
};

struct LRParser {
  // Design your LRParser structure.
  vector<LRTableElement> lr_table;
  vector<LRRule> lr_rules;
};

// Homework 1.2
bool BuildLRParser(const std::vector<LRTableElement>& elements,
                   const std::vector<LRRule>& rules,
                   LRParser* lr_parser);

// Homework 1.2
bool RunLRParser(const LRParser& lr_parser, const std::string& str);

int Parsing_action(LRTableElement element, stack<int>& LR_stack, const LRParser& lr_parser);
#endif //_PL_HOMEWORK_LR_PARSER_H_

