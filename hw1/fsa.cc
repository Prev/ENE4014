// PL homework: hw1
// fsa.cc

#include <iostream>

#include "fsa.h"

#define DISABLE_LOG true
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;


bool RunFSA(FiniteStateAutomaton fsa, const string& str) {
	string state = fsa.starting_dfa_state;

	for (char character : str) {
		string next = fsa.dfa_nexts[state][character];
		
		if (next == "")
			return false;

		state = next;
	}

	return fsa.dfa_accepts[state];
}


bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa) {
	
	// Make NFA Table
	for (FSATableElement elm : elements) {

		if (elm.str == "")
			// epsilon
			fsa->nfa_table[elm.state]['#'].insert(elm.next_state);

		for (int j = 0; j < elm.str.length(); j++) {
			char character = elm.str[j];
			fsa->allchars.insert(character);
			fsa->nfa_table[elm.state][character].insert(elm.next_state);

			//LOG << elm.state << " " << character << " " << elm.next_state << "\n";
		}
	}
		
	// Build DFA
	BuildDFA(accept_states, fsa);

	return true;
}


/**
 * Build DFA by NFA table
 */
void BuildDFA(const std::vector<int>& accept_states,
			  FiniteStateAutomaton* fsa) {

	set<int> start = StickedStates(1, fsa);
	fsa->starting_dfa_state = fsa->dfa_hash(start);

	queue< set<int> > que;
	que.push(start);

	while (!que.empty()) {
		set<int> states = que.front();
		que.pop();
		string s_key = fsa->dfa_hash(states);

		if (fsa->dfa_nexts.find(s_key) != fsa->dfa_nexts.end())
			// Continue if state is already exists in FSA
			continue;


		LOG << s_key << "\n";

		for (char character : fsa->allchars) {
			set<int> ns = NextStatesOfDFA(states, character, fsa);
			que.push(ns);
			string ns_key = fsa->dfa_hash(ns);

			fsa->dfa_nexts[s_key][character] = ns_key;

			LOG << "   [" << character << "]: " << ns_key << "\n";
		}

		bool accept = false;
		for (int ac : accept_states) {
			if (states.find(ac) != states.end()) {
				accept = true;
				break;
			}
		}
		fsa->dfa_accepts[s_key] = accept;
	}
}

/**
 * Get current state and other states that connected with epsilon character
 */
set<int> StickedStates(int state,
					   FiniteStateAutomaton* fsa) {

	set<int> ret;   ret.insert(state);
	queue<int> que; que.push(state);

	while (!que.empty()) {
		int cur_state = que.front();
		que.pop();

		for (int new_state : fsa->nfa_table[cur_state]['#']) {
			if (ret.find(new_state) == ret.end()) {
				// if not exists in ret
				ret.insert(new_state);
				que.push(new_state);
			}
		}
	}
	return ret;
}

/**
 * Get next states of DFA
 */
set<int> NextStatesOfDFA(set<int> dfa_state,
						char character,
						FiniteStateAutomaton* fsa) {

	set<int> ret;

	for (int old_state : dfa_state) {
		for (int new_state : fsa->nfa_table[old_state][character]) {
			set<int> new_states = StickedStates(new_state, fsa);

			for (int item : new_states)
				ret.insert(item);
		}
	}

	return ret;
}

