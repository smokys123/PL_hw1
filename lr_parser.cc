// PL homework: hw2
// lr_parser.cc

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <stack>
#include <string>

#include "lr_parser.h"

#define DISABLE_LOG false
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;

//make LRParser struct, print LRParser data
bool BuildLRParser(const std::vector<LRTableElement>& elements,
                   const std::vector<LRRule>& rules,
                   LRParser* lr_parser) {
  //make lr_parser 
  lr_parser->lr_table = elements;
  lr_parser->lr_rules = rules;
  //print LR action table elements and goto table elements
  cout << "------LR Table elements------" << endl;
  for(int i=0; i<lr_parser->lr_table.size(); i++){
      cout << "state : "<<lr_parser->lr_table[i].state << ", symbol : " << lr_parser->lr_table[i].symbol << ", action : " << lr_parser->lr_table[i].action << ", next_state : " << lr_parser->lr_table[i].next_state <<endl;
  }

  //print LR rules elements;
  cout << "------Rule elements------" << endl;
  for(int i=0; i<lr_parser->lr_rules.size();i++){
      cout <<"rule id : " <<lr_parser->lr_rules[i].id <<", left hand side : " <<lr_parser->lr_rules[i].lhs_symbol << ", right hand side : " << lr_parser->lr_rules[i].num_rhs << endl;
  }
  return true;
}

//check input is accepted or not
bool RunLRParser(const LRParser& lr_parser, const std::string& str) {
    
    stack<int>  LR_stack;
    int cur_state = 0;
    int str_index = 0;
    int check_accept = -1;
    //initial statck to 0
    LR_stack.push(0);
    while(check_accept !=ACCEPT){
        //find action element
        int ch =0;
        for(int i=0; i<lr_parser.lr_table.size();i++){
            // find matched action
            if(lr_parser.lr_table[i].state == cur_state && lr_parser.lr_table[i].symbol == str[str_index]){
                ch++;
                check_accept = Parsing_action(lr_parser.lr_table[i], LR_stack, lr_parser);
                if(check_accept == INVALID){
                  cout << "invalid lr parser action" << endl;
                  return false;
                }
                else if(check_accept == SHIFT){  //when not accpted
                    str_index++;
                    cur_state = LR_stack.top();
                    break;
                }
                else if(check_accept == REDUCE){
                   cur_state = LR_stack.top();
                   break;
                }
                else if(check_accept == ACCEPT){
                  break;
                }
            }
        }
        if(ch == 0){
          cout <<"input string can not parsing" << endl;
            return false;
        }
    
    }
    return true;
}

//find matched action
int Parsing_action(LRTableElement action_element, stack<int>& LR_stack, const LRParser& lr_parser){
    int pop_state;
    int pop_inputChar;
    int push_state;
    int push_inputChar;
    int pop_num;

    switch(action_element.action){
      case INVALID:
        break;
  
      case SHIFT:
        //push char that str[str_index] and next_state;
        LR_stack.push(action_element.symbol);
        LR_stack.push(action_element.next_state);
        break;

      case REDUCE:
        //imply rule = pop() and push new state, new inputChar
        pop_num = lr_parser.lr_rules[action_element.next_state-1].num_rhs;
        for(int i=0;i<pop_num;i++){
          LR_stack.pop();
          LR_stack.pop();
        }
        pop_state = LR_stack.top();
        //find new inputChar
        for(int i =0;i< lr_parser.lr_rules.size();i++){
          if(lr_parser.lr_rules[i].id == action_element.next_state){
            push_inputChar = lr_parser.lr_rules[i].lhs_symbol;
            break;
          }
        }
        //find new state and push 
        pop_state = LR_stack.top();
        for(int i=0;i <lr_parser.lr_table.size();i++){
            if(lr_parser.lr_table[i].state == pop_state && lr_parser.lr_table[i].symbol == push_inputChar){
              push_state = lr_parser.lr_table[i].next_state;
              LR_stack.push(push_inputChar);
              LR_stack.push(push_state);
            }
        }
        break;

      case ACCEPT:
        //stack clear
        while(LR_stack.empty()==false)
            LR_stack.pop();
        break;

      case GOTO:
        break;
    }
    return action_element.action;
}
