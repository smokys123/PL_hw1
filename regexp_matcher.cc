// PL homework: hw2
// regexp_matcher.cc

#include "regexp_matcher.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <stack>
#include <iostream>
#include <cstring>
#include <string>


using namespace std;

vector<char> all_char;
vector<int> states;
stack<pair<int,int> > star_pairs;
set<char> inputSymbols1;  //all input string 

//find all input symbols method
bool FindinputSymbols1(const std::vector<FSATableElement>& elements) {
	for (int i = 0; i<elements.size(); i++) {
			inputSymbols1.insert(elements[i].str);
	}
	return true;
}


bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher) {

  vector<FSATableElement> FSA_table;
  RegExpMatcher temp;
  set<char> symbols;
  int init_state =1;
  bool vaild_regexp=true;
  vector<int> accept_states;
  set<int>::iterator it1,it2;

  //all alphabet  
  for(char c='a'; c<='z';c=c+1)
    all_char.push_back(c);
  for(char c='A'; c<='Z';c=c+1)
    all_char.push_back(c);
  
  vaild_regexp = CheckRegexp(regexp);    //check correct regexp
  if(vaild_regexp == false){
    cout << "intput regexp is not vaild regexp. " << endl;
    return false;
  }
  //buildNFA on the basis of input regexp  and  find accept state;
  int accept = BuildNFA(regexp,init_state,&FSA_table,0,strlen(regexp));
  cout <<"accept : " <<accept << endl;
  accept_states.push_back(accept);
  SetStarProduction(&FSA_table, star_pairs);    //make star production  
  FindinputSymbols1(FSA_table);
  
  NFAtoDFA1(FSA_table,accept_states ,regexp_matcher);
  


    return true;
}

int BuildNFA(const char*regexp, int state, vector<FSATableElement>* FSA_table, int start_str, int end_str){
    const int START_OR = state;
    int cur_state = state;
    int next_state = state+1;
    vector<int> OR_Vec = FindOR(regexp,start_str,end_str);
    vector<int> save_indexVec;
    int max=0;

    //build NFA start
    for(int i=0;i< OR_Vec.size();i++){   // for loop by num of | in regexp
      pair<int, int> star_pair;
      int compare=0;
      int parenthesis=0;   //find correct () pair
      int index_paren=0;   //when ()is exsited, to save ('s index;
      int bracket=0;       //find correct [] pair
      int index_bracket=0; //for calculate num of char in []

      // OR is exsited in regexp
      if(OR_Vec.size()>1){  
          max = CalculNextState(states);
          if(states.size() == 0){
            max = cur_state;
          }
          pushNFAelement(FSA_table,'#',START_OR,max+1);
          cur_state = max+1;
          next_state++;
        }
      //variable to implement for loop on the basis of OR
      if(OR_Vec.size()==i+1)
        compare = end_str;
      else 
        compare = OR_Vec[i+1]-1;
      max=0;

      for(int j=OR_Vec[i]; j<compare; j++){   // for loop  forward/back on basis of OR
        
        if(regexp[j] == '('){  //when (, make production by # 
          if(parenthesis==0){
            index_paren = j+1;
            int max1=0;
            max1 = CalculNextState(states);
            if(states.size() == 0){
              max1 = cur_state;
            }

            pushNFAelement(FSA_table,'#',cur_state,max1+1);
            cur_state = max1+1;
            next_state++;
          }
          parenthesis++;
          continue;
        }
        else if(regexp[j] == ')'){  //when regexp[j]== ) and parenthesis is set pair
          int star_check=0;
          parenthesis--;
          if(parenthesis==0){
            if(regexp[j+1]=='*'){ //when (..)* to find * production pair
                star_check++;
                star_pair.first = cur_state;
            }

            cur_state = BuildNFA(regexp,cur_state,FSA_table,index_paren,j);   //recursion regexp that is inside parenthesis
            
            if(star_check != 0){   //when (..)* to find * production pair
              star_pair.second = cur_state;
              star_pairs.push(star_pair);              
            }
            max = CalculNextState(states);
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
          for(int k = index_bracket; k<j;k++)   //make production about set of characters;
            pushNFAelement(FSA_table,regexp[k],cur_state,next_state);
          if(regexp[j+1] =='*'){ //when [..]*, find * production pair
            pair<int,int> temp;
            temp.first = cur_state;
            temp.second = next_state;
            star_pairs.push(temp);
          }
          bracket--;
          cur_state++;
          next_state++;
          continue;
        }
        else if(parenthesis != 0 || bracket != 0){ //search parenthesis and brecket range
          continue;
        }
        else if(regexp[j] == '.'){  //when any symbol 
          if(regexp[j+1] == '*'){   //when next symbol is * 
            pushNFAelement(FSA_table,regexp[j],cur_state,cur_state);
            continue;
          }
          if(cur_state == next_state){
            next_state++;
          }
          pushNFAelement(FSA_table,regexp[j],cur_state,next_state);
          cur_state++;
        }
        else if(regexp[j] == '*'){   //when star, find star range
          continue;
        }
        else{   //when char, just make production
          if(regexp[j+1] == '*'){ //when next symbol is * 
            pushNFAelement(FSA_table,regexp[j],cur_state,cur_state);
            continue;
          }
          if(cur_state == next_state){
            next_state++;
          }
          pushNFAelement(FSA_table,regexp[j],cur_state,next_state);
          cur_state = next_state;
          next_state++;
        }
      }
      save_indexVec.push_back(next_state);  //when exist OR, save split state.

      if(i==0){
        pushNFAelement(FSA_table,'#',cur_state,save_indexVec[0]); 
      }else if(i>0){   
        pushNFAelement(FSA_table,'#',cur_state,save_indexVec[i-1]); 
      }
    }
    return save_indexVec[0];
}

bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str) {
    return false;
}

//check input regexp is correct
bool CheckRegexp(const char* regexp){

    int paren=0;
    int brack=0;
    for(int i=0;i<strlen(regexp);i++){
      if(regexp[i] == '('){
        paren++;
        if(brack != 0)
            return false;
      }
      else if(regexp[i] == ')'){
        paren--;
      }
      else if(regexp[i] == '['){
        brack++;
      }
      else if(regexp[i] == ']'){
        brack--;
      }
      else if(regexp[i] == '|'){
        if(brack != 0)
          return false;
      }
      else if(regexp[i] == '*'){
        if(i==0 || regexp[i+1] == '*'){
          return false;
        }
      }
    }
    if(paren !=0 || brack !=0){
      return false;
    }
    return true;
}

//find or index
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

//push NFA element into FSA_table
void pushNFAelement(vector<FSATableElement>* FSA_table, char input_symbol, int state, int next_state){
    FSATableElement element;
    element.state = state;
    element.next_state = next_state;
    element.str = input_symbol;
    states.push_back(next_state);
    cout << "state : " << state << ", next_state : " << next_state << ", symbol : " << input_symbol << endl;
    FSA_table->push_back(element);
}

//make star production
void SetStarProduction(vector<FSATableElement>* FSA_table, stack<pair<int,int> > pairs){

    int state1, state2;
    char epsilon = '#';
    int stack_size = pairs.size();
    pair<int, int> star_set;
    for(int i=0; i<stack_size ;i++){
        star_set = pairs.top();
        state1 = star_set.first;
        state2 = star_set.second;
        pushNFAelement(FSA_table,epsilon,state1,state2);
        pushNFAelement(FSA_table,epsilon,state2,state1);
        pairs.pop();
    }
}

int CalculNextState(vector<int> state_list){
    int max=0;
    for(int i=0;i<state_list.size();i++){
      if(state_list[i] > max){
        max = state_list[i];
      }
    }
    return max;
}

//convert NFA to DFA method
bool NFAtoDFA1(const std::vector<FSATableElement>& elements, const std::vector<int>& accept_states, RegExpMatcher* fsa) {
    
	queue<set<int> > queue;
    set<char>::iterator it;
    set<int>::iterator it1;
    set<set<int> > check_state;
    vector<FSATableElement> new_elements = FindState1(elements);
    cout <<  "DFA grammer  "<<endl;
    for(int i=0;i< new_elements.size();i++){ 
        cout << "<" << new_elements[i].state << "," << new_elements[i].next_state << "," << new_elements[i].str << ">" << endl;
    }	
	set<int> state;
    state.insert(1);
    queue.push(state);
    //fsa confiqure
    fsa->init_state.insert(1);
    for(int i =0; i<accept_states.size();i++)
        fsa->accept.insert(accept_states[i]);
    //using quque make DFA
    while (!queue.empty()) {
	    set<int> next_states;
		state = queue.front();
        for(it=inputSymbols1.begin() ; it != inputSymbols1.end() ; it++){
            for(int i=0;i<new_elements.size();i++){  //find next_states can go from state 
                for(it1=state.begin() ; it1 != state.end(); it1++){
                    if(new_elements[i].state == *it1 && new_elements[i].str == *it){
                        next_states.insert(new_elements[i].next_state);
                    }
                }
            }
            //when trap
            if(next_states.empty()){
                next_states.insert(-1);
            }
            
            int check=0; //except state that is already checked next_states
            for(set<set<int> >::iterator t = check_state.begin(); t!=check_state.end();t++){
              if( equal( (*t).begin(), (*t).end(), next_states.begin() ) ) {
                check++;
              }
            }
            PrintFSAelements1(state, next_states,*it);        
            fsa->FSA.insert(make_pair(make_pair(state,*it),next_states));
            if(check==0){  //about state that is not checked next_states -> insert to queue, check_state struct
                queue.push(next_states);
                check_state.insert(next_states);
            }
            next_states.clear();
        }
        queue.pop();

	}
	
	return true;
}

//find all possible derivation method with NFA condition method
vector<FSATableElement> FindState1(const std::vector<FSATableElement>& elements) {
	
	vector<pair<int, int> > epsilon_state;
	set<int>::iterator it;
	vector<FSATableElement> new_elements;
	FSATableElement temp;
	epsilon_state = FindAllEpsilon1(elements);
	for (int i = 0; i < elements.size(); i++) { 
        if (elements[i].str == '#') {   //when symbol is epsilon 
            set<int> a;
            //find all possible state can derivation from elements[i].state 
			set<int> possible_state = StatebyEplison1(epsilon_state, a, elements[i].state);
            for (it = possible_state.begin(); it != possible_state.end(); it++) {
				for (int k = 0; k < elements.size(); k++) {
					if (*it == elements[k].state && elements[k].str != '#') {
						temp.state = elements[i].state;
						temp.next_state = elements[k].next_state;
						temp.str = elements[k].str;
						new_elements.push_back(temp);
					}
				}
            }
	    }
		else {   //when symbol is not epsilon
			new_elements.push_back(elements[i]);
            set<int> a;
            //find all possible state can derivation from elements[i].next_state
			set<int> possible_state = StatebyEplison1(epsilon_state,a, elements[i].next_state);
            for (it = possible_state.begin(); it != possible_state.end(); it++) {
              temp.state = elements[i].state;
              temp.next_state = *it;
              temp.str = elements[i].str;
			  new_elements.push_back(temp);
			}
		}
	}
	return new_elements;
}


//find all input Symbol is epsilon method 
vector<pair<int,int> > FindAllEpsilon1(const std::vector<FSATableElement>& elements) {
	vector<pair<int,int> > epsilonState;
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i].str == '#') {
			epsilonState.push_back(make_pair(elements[i].state, elements[i].next_state));
		}
	}
	return epsilonState;
}

//find all state can derivation by epsilon method
set<int> StatebyEplison1(vector<pair<int,int> >& EpsilonState, set<int>& next_state,int state) {
    set<int>::iterator it;
    int check=0;
    for (int i = 0; i < EpsilonState.size(); i++) {
		if (EpsilonState[i].first == state) {
            for(it = next_state.begin(); it != next_state.end();it++){
              if(*it == EpsilonState[i].second){
                check++;
              }
            }  
            if(check == 0){
			next_state.insert(EpsilonState[i].second);
			StatebyEplison1(EpsilonState, next_state, EpsilonState[i].second);
            }
		}
	}
	return next_state;
}

//print FSA elements method
bool PrintFSAelements1(set<int> state, set<int>next_states, char symbol){
    set<int>::iterator it1,it2;
    cout <<"state : < ";
      for(it1 = state.begin() ; it1 != state.end() ;it1++)
          cout << *it1 << " ";
      cout << "> symbol : " << symbol << " next_state : < " ;
      for(it2 = next_states.begin(); it2 != next_states.end() ; it2++)
          cout << *it2 << " ";
      cout << ">"<<endl;      
}






