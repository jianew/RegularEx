#ifndef _LIUZHANG_REGULAREX_
#define _LIUZHANG_REGULAREX_
#include"Dfa.h"
#include<map>
#include<vector>
#include<string>
#include<stack>
#include<iostream>
#include<set>
using namespace std;
class NFAState{
public:
	typedef NFAState * NFAStatePtr;
	typedef multimap<char,NFAStatePtr> Transitions;
	NFAState();
	void connect(const char tranchar,const NFAStatePtr rePtr);
	friend ostream &operator<<(ostream &os,const NFAState &state);
	Transitions::iterator OutIterBegin(){return transtions.begin();};
	Transitions::iterator OutIterEnd(){return transtions.end();;}
	set<NFAStatePtr>  getTransfer(const char tranchar);

private:
	int StateID;
    
	Transitions transtions;
	
	static int m_NextStateID;
};
typedef NFAState *  NFAStatePtr;
typedef NFAState DFAState;
typedef DFAState * DFAStatePtr;
typedef vector<NFAStatePtr> NFATABLE;
typedef vector<DFAStatePtr> DFATABLE;
class RegEx{
public:
	RegEx(const string &patt):patt(patt),dfa(patt){
		oppr.insert(make_pair('(',0));
		oppr.insert(make_pair(')',0));
		oppr.insert(make_pair('|',5));
		oppr.insert(make_pair(',',5));
		oppr.insert(make_pair('*',8));
		parse(patt);};
	~RegEx();
	void parse(const string &patt);
	void push(const char input);
	void Eval();
	bool RegEx::Prior(const char c1,const char c2);
	void Union();
	void Concat();
	void Repeat();
	void PrintNfa();
	void toDfa();
	MDfa GetDfa();
	set<NFAStatePtr> e_Closure(NFAStatePtr stPrt);
	set<NFAStatePtr> getTransfer(const char tranchar,const set<NFAStatePtr> &stPrtSet);
private:
	NFATABLE nfa;
	MDfa dfa;
	string patt;
	stack<NFATABLE> opstack;
	stack<char> operatorstack;
	map<char,int> oppr;
};
#endif