// PL homework: hw2
// regexp_matcher.cc

#include "regexp_matcher.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <iostream>
#include <cstring>

using namespace std;

vector<char> all_char;
vector<int> states;
int recursion =0;

bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher) {

    vector<FSATableElement> FSA_table;
    RegExpMatcher temp;
    set<char> symbols;
    int init_state =1;
    
    //all alphabet  
    for(char c='a'; c<='z';c=c+1)
     all_char.push_back(c);
    for(char c='A'; c<='Z';c=c+1)
      all_char.push_back(c);
    
    int accept = BuildNFA(regexp,init_state,&FSA_table,0,strlen(regexp));


    //전체를 돌면서 확인 괄호가 나올경우 어떻게 할것인가?
    for(int i=0; i<strlen(regexp);i++){}
    
    
    
    return true;
}

int BuildNFA(const char*regexp, int state, vector<FSATableElement>* FSA_table, int start_str, int end_str){
   cout << "start : " <<start_str << "  end : " << end_str<< endl;
  recursion++;
   const int START_OR = state;
  int cur_state = state;
  int next_state = state+1;
  vector<int> OR_Vec = FindOR(regexp,start_str,end_str);
  vector<int> save_indexVec;
vector<int> save_split;
    int max=0;
  for(int i=0;i< OR_Vec.size();i++){
    // OR is exsited in parenthesis , 괄호안에 OR이 있는 경우
    if(OR_Vec.size()>1){
     for(int a=0; a<states.size();a++){
        if(max < states[a])
            max = states[a];
      }
      if(states.size() == 0){
        max = cur_state;
      }
      cout << START_OR << endl;
      save_split.push_back(START_OR);
      pushNFAelement(FSA_table,'#',START_OR,max+1);
      cur_state = max+1;
      next_state++;
    }

    int compare;
    if(OR_Vec.size()==i+1)
        compare = end_str;
    else 
        compare = OR_Vec[i+1]-1;
 
    int parenthesis=0;
    int index_paren=0;
    int bracket=0;
    int index_bracket=0;
    max=0;
    for(int j=OR_Vec[i]; j<compare; j++){
      
      if(regexp[j] == '('){
        if(parenthesis==0){
          index_paren = j+1;
          int max1=0;
      for(int a=0; a<states.size();a++){
        if(max1 < states[a])
            max1 = states[a];
      }
      if(states.size() == 0){
        max1 = cur_state;
      }
      //save_split.push_back(START_OR);
      pushNFAelement(FSA_table,'#',cur_state,max1+1);
      cur_state = max1+1;
      next_state++;

        }
        parenthesis++;
        continue;
      }
      else if(regexp[j] == ')'){
        parenthesis--;
        if(parenthesis==0){
          cur_state = BuildNFA(regexp,cur_state,FSA_table,index_paren,j);
          for(int a=0; a<states.size();a++){
             if(max < states[a])
                 max = states[a];
        }
          next_state = max+1;
          continue;
        }
      }
      else if(regexp[j] == '['){
        index_bracket = j+1;
        bracket++;
        continue;
      }
      else if(regexp[j] == ']'){
        for(int k = index_bracket; k<j;k++)
            pushNFAelement(FSA_table,regexp[k],cur_state,next_state);
        bracket--;
        cur_state++;
        next_state++;
        continue;
      }
      else if(parenthesis != 0 || bracket != 0){
        continue;
        }
      else if(regexp[j] == '.'){
        if(cur_state == next_state)
            next_state++;
        pushNFAelement(FSA_table,regexp[j],cur_state,next_state);
        /*for(int k=0;k<all_char.size();k++){
          pushNFAelement(FSA_table,all_char[k],cur_state,next_state);
        }*/
        cur_state++;
      }
      else{
        /*if(OR_Vec.size()>0 && j==OR_Vec[i]){
          save_split.push_back(cur_state);
        }*/
        /*if(i>0 && j==OR_Vec[i]){
          cur_state = save_split[i-1];
          //cur_state = (*save_split.end() -1);
          next_state++;  
          pushNFAelement(FSA_table,regexp[j],cur_state,next_state);
          cur_state = next_state;
          next_state++;
        }*/
       // else{
          if(cur_state == next_state)
            next_state++;
          pushNFAelement(FSA_table,regexp[j],cur_state,next_state);
          cur_state = next_state;
          next_state++;
        //}
      }
    }
    save_indexVec.push_back(next_state);
  
  //if(OR_Vec.size()>1){
      for(int l=0; l < save_indexVec.size(); l++)
          cout << "sace_indexVec["<<l <<"] : " << save_indexVec[l] << endl;
    if(i==0){
        cout <<"vvv" <<endl;
     pushNFAelement(FSA_table,'#',cur_state,save_indexVec[0]); 
    }else if(i>0){   
        cout <<"zzz" <<endl;
     pushNFAelement(FSA_table,'#',cur_state,save_indexVec[i-1]); 
    }
  // for(int i=0;i<save_indexVec.size();i++)
  //   pushNFAelement(FSA_table,'#',save_indexVec[i],next_state); 
 // }
  }
  recursion--;
  cout << "Build NFA end" << endl;
  return save_indexVec[0];
}

bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str) {
  return false;
}

vector<int> FindOR(const char* str, int start, int end){
 
  vector<int> OR_index;
  bool check_paren = true;
  char sub_str;
  int paren =0;
  OR_index.push_back(start);
  for(int i=start;i< end-1;i++){
    sub_str = str[i];
    if(sub_str == '('){
        paren++;
        check_paren = false;
    }else if (sub_str == ')'){
        paren--;
      check_paren = true;
    }else if(check_paren == true && sub_str == '|' && paren == 0){
      OR_index.push_back(i+1);
    }
  } 
  return OR_index;
}

void pushNFAelement(vector<FSATableElement>* FSA_table, char input_symbol, int state, int next_state){
  FSATableElement element;
  element.state = state;
  element.next_state = next_state;
  element.str = input_symbol;
  states.push_back(next_state);
  cout << "state : " << state << ", next_state : " << next_state << ", symbol : " << input_symbol << endl;
  FSA_table->push_back(element);
}

