// PL homework: hw2
// lr_parser.h

#ifndef _PL_HOMEWORK_LR_PARSER_H_
#define _PL_HOMEWORK_LR_PARSER_H_

#include <string>
#include <vector>
#include <unordered_map>

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
	
	/**
	 *  LRParser Action & Goto Table
	 *    selection: LRTableElement elm = table[state][symbol]
	 *    insertion: table[state][symbol] = elm
	 */
	std::unordered_map<int,
		std::unordered_map<int,
			LRTableElement > > table;

	/**
	 *  LRParser Rules
	 *    selection: LRRule rule = rules[id]
	 *    insertion: rules[id] = rule
	 */
	std::unordered_map<int, LRRule> rules;
};




bool BuildLRParser(const std::vector<LRTableElement>& elements,
                   const std::vector<LRRule>& rules,
                   LRParser* lr_parser);
bool RunLRParser(LRParser& lr_parser, const std::string& str);


#endif //_PL_HOMEWORK_LR_PARSER_H_

