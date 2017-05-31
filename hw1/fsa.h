// PL homework: hw1
// fsa.h

#ifndef _PL_HOMEWORK_FSA_H_
#define _PL_HOMEWORK_FSA_H_

#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <set>

// Valid characters are alphanumeric and underscore (A-Z,a-z,0-9,_).
// Epsilon moves in NFA are represented by empty strings.

struct FSATableElement {
	int state;
	int next_state;
	std::string str;
};


struct FiniteStateAutomaton {

	/**
	 *  All possible charactors
	 */
	std::set<char> allchars;


	/**
	 *  NFA(Nondeterministic Finite-state Automaton) Table
	 *    selection: int s = nfa_table[state][character]
	 *    insertion: nfa_table[state][character].insert(next_state);
	 */
	std::unordered_map<int,
		std::unordered_map<char,
			std::set<int> > > nfa_table;

	/**
	 * DFA(Deterministic Finite-state Automaton) States & Accepts
	 */
	std::unordered_map<std::string,
		std::unordered_map<char,
			std::string> > dfa_nexts;

	std::unordered_map<std::string, bool> dfa_accepts;
	std::string starting_dfa_state;


	/**
	 * Get hash key of dfs set
	 */
	std::string dfa_hash(std::set<int> s) {
		std::stringstream ss;
		ss << '{';
		for (auto it = s.begin(); it != s.end(); ++it) {
			ss << *it;
			ss << ',';
		}
		ss << '}';

		std::string str = ss.str();
		if (str == "{}")  return "$";
		else 			return str;
	}
};



// Run FSA and return true if str is matched by fsa, and false otherwise.
bool RunFSA(FiniteStateAutomaton fsa, const std::string& str);

bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa);

void BuildDFA(const std::vector<int>& accept_states,
			  FiniteStateAutomaton* fsa);

std::set<int> StickedStates(int state,
					       FiniteStateAutomaton* fsa);

std::set<int> NextStatesOfDFA(std::set<int> dfa_state,
						char character,
						FiniteStateAutomaton* fsa);



#endif //_PL_HOMEWORK_FSA_H_

