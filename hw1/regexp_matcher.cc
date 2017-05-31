// PL homework: hw2
// regexp_matcher.cc

#include <iostream>
#include <stack>
#include <cstring>
#include <sstream>

#include "regexp_matcher.h"

using namespace std;

void putFSAElement(int state,
				   int next_state,
				   string str,
				   vector<FSATableElement>& fsa_elements) {
	
	FSATableElement elem;
	elem.state = state;
	elem.next_state = next_state;
	elem.str = str;

	fsa_elements.push_back(elem);
}


bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher) {
	FiniteStateAutomaton *fsa = new FiniteStateAutomaton;

	vector<FSATableElement> fsa_elements;
	vector<int> accept_states;

	stack<int> lp_stack; // left-parenthesis
	stack<int> or_stack; // 'or' operators
	vector<int> group_starting;

	bool slp_started = false; // square-left-parenthesis started
	vector<char> slp_chars;

	string regstr(regexp);
	regstr = "(" + regstr + ")";


	for (int i = 0; i < regstr.length(); i++) {
		int index = i + 1; // Using index instead of i because state start from 1
		int lp_pos, pos;

		group_starting.push_back(index); // self

		switch (regstr[i]) {
			case '(' :
				if (slp_started)
					// Error
					return false;

				lp_stack.push(index);
				putFSAElement(index, index+1, "#", fsa_elements);
				break;


			case '[' :
				slp_started = true;
				slp_chars.clear();

				putFSAElement(index, index+1, "#", fsa_elements);
				break;

			case ']' :
				if (!slp_started)
					// Error
					return false;

				for (char c : slp_chars)
					putFSAElement(index, index+1, string(1, c), fsa_elements);
				
				slp_started = false;
				break;


			case ')' :
				if (slp_started)
					// Error
					return false;

				lp_pos = lp_stack.top();
				lp_stack.pop();

				while (!or_stack.empty()) {
					pos = or_stack.top();
					or_stack.pop();

					putFSAElement(pos, index, "#", fsa_elements);
					putFSAElement(lp_pos, pos+1, "#", fsa_elements);
				}

				putFSAElement(index, index+1, "#", fsa_elements);
				group_starting[i] = lp_pos;

				break;

			case '|' :
				if (slp_started)
					// Error
					return false;

				or_stack.push(index);
				break;


			case '.' :
				if (slp_started)
					// Error
					return false;

				putFSAElement(index, index+1, "abcdefghijklmnopqrstuvwxyz", fsa_elements);
				break;


			case '*' :
				if (slp_started || i == 0)
					// Error
					return false;


				putFSAElement(index, index+1, "#", fsa_elements);

				putFSAElement(index+1, group_starting[i-1], "#", fsa_elements);
				putFSAElement(group_starting[i-1], index+1, "#", fsa_elements);

				break;


			default :
				if (slp_started) {
					slp_chars.push_back(regstr[i]);
					putFSAElement(index, index+1, "#", fsa_elements);
				}
				else
					putFSAElement(index, index+1, string(1, regstr[i]), fsa_elements);
				
				break;
		}
	}

	accept_states.push_back(regstr.length()+1);

	BuildFSA(fsa_elements, accept_states, fsa);
	regexp_matcher->fsa = fsa;

	// cout << regexp << "\n";
	return true;
}


bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str) {
	return RunFSA(*regexp_matcher.fsa, str);
}




