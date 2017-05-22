// PL homework: hw1
// fsa.cc

#include <iostream>
#include <set>
#include <queue>
#include <cstring>
#include <string>
#include "fsa.h"


#define DISABLE_LOG true
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

const string Epsilon = "#";
set<char> inputSymbols;  //all input string 

using namespace std;

//find all input symbols method
bool FindinputSymbols(const std::vector<FSATableElement>& elements) {
	for (int i = 0; i<elements.size(); i++) {
         for(int j=0; j<elements[i].str.length();j++)
			inputSymbols.insert(elements[i].str[j]);
	}
	return true;
}

//make GTG to NFA method
vector<FSATableElement> BuildNFA(vector<FSATableElement>& elements) {
      
	FSATableElement temp1, temp2;
	//input symbol convert "ab" to "a" or "b"
	// ex) 2 2 bc -> 2 2 b  and 2 2 c
	for (int i = 0;i< elements.size(); i++) {
		if (elements[i].str.length() > 1) {
			temp1 = elements[i];
			elements.erase(elements.begin() + i);
			for (int j = 0; j < temp1.str.length(); j++) {
				temp2.state = temp1.state;
				temp2.next_state = temp1.next_state;
				temp2.str = temp1.str[j];
				elements.push_back(temp2);
			}
		}
	}
	return elements;
}

//find all input Symbol is epsilon method 
vector<pair<int,int> > FindAllEpsilon(const std::vector<FSATableElement>& elements) {
	vector<pair<int,int> > epsilonState;
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i].str.length() == 0) {
			epsilonState.push_back(make_pair(elements[i].state, elements[i].next_state));
		}
	}
	return epsilonState;
}

//find all state can derivation by epsilon method
set<int> StatebyEplison(vector<pair<int,int> >& EpsilonState, set<int>& next_state,int state) {
    for (int i = 0; i < EpsilonState.size(); i++) {
		if (EpsilonState[i].first == state) {
			next_state.insert(EpsilonState[i].second);
			StatebyEplison(EpsilonState, next_state, EpsilonState[i].second);
		}
	}
	return next_state;
}

//find all possible derivation method with NFA condition method
vector<FSATableElement> FindState(const std::vector<FSATableElement>& elements) {
	
	vector<pair<int, int> > epsilon_state;
	set<int>::iterator it;
	vector<FSATableElement> new_elements;
	FSATableElement temp;
	epsilon_state = FindAllEpsilon(elements);
	for (int i = 0; i < elements.size(); i++) { 
        if (elements[i].str.length() == 0) {   //when symbol is epsilon 
            set<int> a;
            //find all possible state can derivation from elements[i].state 
			set<int> possible_state = StatebyEplison(epsilon_state, a, elements[i].state);
            for (it = possible_state.begin(); it != possible_state.end(); it++) {
				for (int k = 0; k < elements.size(); k++) {
					if (*it == elements[k].state && elements[k].str.length() != 0) {
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
			set<int> possible_state = StatebyEplison(epsilon_state,a, elements[i].next_state);
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

//print FSA elements method
bool PrintFSAelements(set<int> state, set<int>next_states, char symbol){
    set<int>::iterator it1,it2;
    cout <<"state : < ";
      for(it1 = state.begin() ; it1 != state.end() ;it1++)
          cout << *it1 << " ";
      cout << "> symbol : " << symbol << " next_state : < " ;
      for(it2 = next_states.begin(); it2 != next_states.end() ; it2++)
          cout << *it2 << " ";
      cout << ">"<<endl;      
}

//convert NFA to DFA method
bool NFAtoDFA(const std::vector<FSATableElement>& elements, const std::vector<int>& accept_states, FiniteStateAutomaton* fsa) {
    
	queue<set<int> > queue;
    set<char>::iterator it;
    set<int>::iterator it1;
    set<set<int> > check_state;
    vector<FSATableElement> new_elements = FindState(elements);
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
        for(it=inputSymbols.begin() ; it != inputSymbols.end() ; it++){
            for(int i=0;i<new_elements.size();i++){  //find next_states can go from state 
                for(it1=state.begin() ; it1 != state.end(); it1++){
                    if(new_elements[i].state == *it1 && new_elements[i].str[0] == *it){
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
            PrintFSAelements(state, next_states,*it);        
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

//for RunFSA, move to next state 
bool MoveToNext(const FiniteStateAutomaton& fsa, set<int>& curstate, const char symbol){
   for(int i=0;i < fsa.FSA.size();i++){
     if(fsa.FSA.find(make_pair(curstate,symbol)) != fsa.FSA.end())
         curstate = (*fsa.FSA.find( make_pair(curstate,symbol) )).second;
         return true;
   } 
   return false;
}
 
bool CheckSymbol(const char str){
    int check = 0;
    for(set<char>::iterator t=inputSymbols.begin(); t != inputSymbols.end(); t++){
        if(str == *t)
            check++;
    }
    if(check != 0)
        return true;
    else 
        return false;
}

//check string that user inputs are aceepted by DFA
bool RunFSA(const FiniteStateAutomaton& fsa, const string& str) {
    set<int> cur_state;
    cur_state = fsa.init_state;
    int index = 0;
    while(index<str.length() && MoveToNext(fsa,cur_state,str[index])){
        if(!CheckSymbol(str[index])){
            cout << "input string is not in Grammer symbols" << endl;
            return false;
        }
        index++;
    }
    for(set<int>::iterator it=fsa.accept.begin() ; it !=fsa.accept.end(); it++)
       if(cur_state.find(*it) != cur_state.end() && str.length() == index)
          return true;
	// Implement this function.
	return false;
}


//using txt file ,make NFA(GTG) -> DFA method
bool BuildFSA(const std::vector<FSATableElement>& elements, const std::vector<int>& accept_states,
	FiniteStateAutomaton* fsa) {

	std::vector<FSATableElement> new_elements=elements;
	//find all input symbols
	FindinputSymbols(elements);
    //GTG->NFA elements
    new_elements = BuildNFA(new_elements);
    //NFA->DFA elements
    NFAtoDFA(new_elements,accept_states,fsa); 
    
    LOG << "num_elements: " << elements.size()
		<< ", accept_states: " << accept_states.size() << endl;

	return true;
}







