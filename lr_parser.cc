// PL homework: hw2
// lr_parser.cc

#include <assert.h>

#include <iostream>
#include <vector>
#include <stack>
#include <sstream>

#include "lr_parser.h"

#define DISABLE_LOG true
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;


bool BuildLRParser(const std::vector<LRTableElement>& elements,
                   const std::vector<LRRule>& rules,
                   LRParser* lr_parser) {

	for (auto elm : elements)
		lr_parser->table[elm.state][elm.symbol] = elm;
	
	for (auto rule : rules)
		lr_parser->rules[rule.id] = rule;


	return true;
}

enum StackItemType { STATE = 0, SYMBOL = 1 };

void printStack(stack< pair<StackItemType, int> > stk) {
	/// Temp: print stack
	LOG << "    Stack:\t";
	stack< pair<StackItemType, int> > temp;

	while (!stk.empty()) {
		if (stk.top().first == 0)
			LOG << stk.top().second << " ";
		else
			LOG << "(" << stk.top().second << ") ";
		
		temp.push( stk.top() );
		stk.pop();
	}
	LOG << "\n";

	while (!temp.empty()) {
		stk.push( temp.top() );
		temp.pop();
	}
}

bool RunLRParser(LRParser& lr_parser, const std::string& str) {
	int str_index = 0;

	stack< pair<StackItemType, int> > stk;
	stk.push( make_pair(STATE, 0) );

	while (!stk.empty()) {
		StackItemType type = stk.top().first;
		int symbol, state;

		if (!DISABLE_LOG)
			printStack(stk);

		if (type == SYMBOL) {
			// error
			return false;

		}else if (type == STATE) {
			state = stk.top().second;
			symbol = str[str_index];

			if (lr_parser.table[state].find(symbol) == lr_parser.table[state].end())
				// not found
				return false;


			LRTableElement elm = lr_parser.table[state][symbol];
			LRTableElement gotoElm;
			int i;
			LRRule rule;

			LOG << "    Input:\t" << str.substr(str_index) << "\n";

			switch (elm.action) {
				case INVALID :
					// error
					return false;

				case ACCEPT :
					// accept
					return true;

				case SHIFT :
					LOG << "    Action:\tS" << elm.next_state << "\n";

					stk.push( make_pair(SYMBOL, symbol) );
					stk.push( make_pair(STATE, elm.next_state) );

					str_index++;
					break;

				case REDUCE :
					LOG << "    Action:\tR" << elm.next_state;

					rule = lr_parser.rules[ elm.next_state ];

					for (i = 0; i < rule.num_rhs * 2; i++)
						stk.pop();

					i = stk.top().second;
					gotoElm = lr_parser.table[i][rule.lhs_symbol];

					LOG << " (G[" << i << "," << rule.lhs_symbol << "])";

					stk.push( make_pair(SYMBOL, rule.lhs_symbol) );
					stk.push( make_pair(STATE, gotoElm.next_state) );

					LOG << "\n";
					break;

				case GOTO :

					break;
				
			}

		}

		LOG << "\n";

	}

	return false;
}



