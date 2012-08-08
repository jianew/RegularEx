#include"Dfa.h"
Alphabet::Alphabet(const string &patt)
{
	for(unsigned int i=0;i<patt.size();i++)
	{
		char curc=patt.at(i);
		if(curc!='*'&&curc!='|'&&curc!='('&&curc!=')'&&curc!=',')
		{
			Alphabet::iterator p=this->begin();
			for(;p!=this->end()&&*p!=curc;p++);
			if(p==this->end()) this->push_back(curc);
		}
	}
};
void Alphabet::GetAlphabet(const string &patt)
{
	for(unsigned int i=0;i<patt.size();i++)
	{
		char curc=patt.at(i);
		if(curc!='*'&&curc!='|'&&curc!='('&&curc!=')')
		{
			Alphabet::iterator p=this->begin();
			for(;p!=this->end()&&*p!=curc;p++);
			if(p==this->end()) this->push_back(curc);
		}
	}
}





ostream &operator<<(ostream &os,const Alphabet &alphabet)
{
	for(Alphabet::const_iterator p=alphabet.begin();p!=alphabet.end();p++)
		os<<*p<<" ";
	return os;
};

ostream &operator<<(ostream &os,const MDfa &mdfa)
{
	os<<"My ALphabet:"<<endl;
	for(unsigned int i=0;i<mdfa.alphabet.size();i++)
		os<<mdfa.alphabet.at(i)<<" ";
	os<<endl;
	os<<(Matrix<int>)(mdfa)<<endl;
	os<<"start:"<<endl;
	os<<mdfa.start<<endl;
	os<<"end :"<<endl;
	for(unsigned int i=0;i<mdfa.end.size();i++)
		os<<mdfa.end.at(i)<<" ";
	os<<endl;
	return os;
	
};
bool MDfa::accept(const string &matchstr)
{
	int curstate=start;
	for(string::const_iterator p=matchstr.begin();p!=matchstr.end();p++)
	{
		if(alpindex.find(*p)==alpindex.end()) return false;
		int index=alpindex[*p];
		curstate=(*this)[curstate][index];
		if(curstate==-1) return false;
	}
	for(unsigned int i=0;i<end.size();i++)
		if(end.at(i)==curstate) return true;
	return false;
};
bool MDfa::IsEndState(int statenum)
{
		for(unsigned int i=0;i<end.size();i++)
		if(end.at(i)==statenum) return true;
}
bool MDfa::search(const string &matchstr,string &restr)
{
	int curstate=start;
	string::const_iterator reStIt,reEnIt;
	bool refind=false;
	for(string::const_iterator pst=matchstr.begin();pst!=matchstr.end();pst++)
	{
		for(string::const_iterator p=pst;p!=matchstr.end();p++)
		{
			if(alpindex.find(*p)==alpindex.end()) break;
			int index=alpindex[*p];
			curstate=(*this)[curstate][index];
			if(curstate==-1) break;
			if(IsEndState(curstate))  {refind=true;reStIt=pst;reEnIt=p;};
		}
		if(refind) {restr.clear();restr.insert(restr.begin(),reStIt,++reEnIt); return true;} 
	}
	return false;

	
};