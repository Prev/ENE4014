// PL homework: hw2
// regexp_matcher.h

#ifndef _PL_HOMEWORK_REGEXP_MATCHER_H_
#define _PL_HOMEWORK_REGEXP_MATCHER_H_

#include <vector>
#include <string>
#include "fsa.h"

struct RegExpMatcher {
	FiniteStateAutomaton *fsa;
};


bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher);
bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str);

#endif  //_PL_HOMEWORK_REGEXP_MATCHER_H_

