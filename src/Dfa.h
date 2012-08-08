#ifndef _FINITE_STATE_MACHINE_
#define _FINITE_STATE_MACHINE_
#include<vector>
#include<string>
#include<algorithm>
#include<map>
#include"Matrix.h"
using namespace std;

typedef map<char,int> Alphabetindex;

class Alphabet:public vector<char>{
  public:
	  Alphabet(): vector<char>(){};
	  Alphabet(const char *x1,const char *x2):vector<char>(x1,x2){};
	  Alphabet(const string &patt);
	  void GetAlphabet(const string &patt);
	  friend ostream &operator<<(ostream &os,const Alphabet &alphabet);
};


class MDfa:public Matrix<int>{


public:
	
	MDfa(Alphabet alphabet):alphabet(alphabet),Matrix<int>(0,alphabet.size(),-1),start(-1){
		int cnt=0;
		for(Alphabet::iterator p=alphabet.begin();p!=alphabet.end();p++,cnt++)
			alpindex.insert(make_pair(*p,cnt));
	};

	void NewState(){AddRow();};

	void SetStart(int i){start=i;};
	void AddEnd(int i){end.push_back(i);};
	bool IsEndState(int statenum);

	int GetAlphaIndex(const char c){return alpindex[c];};

	friend ostream &operator<<(ostream &os,const MDfa &mdfa);

	bool accept(const string &matchstr);
	
	bool search(const string &mathcstr,string &restr);

private:
	int start;
	vector<int> end;
	Alphabet alphabet;
	Alphabetindex alpindex;
};

#endif