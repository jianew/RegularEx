#include"RegularEx.h"
#include<ctype.h>
#include<set>
#include<queue>
int NFAState::m_NextStateID=0;

NFAState::NFAState():StateID(m_NextStateID++){
};

void NFAState::connect(const char tranchar,const NFAStatePtr rePtr)
{
	transtions.insert(make_pair(tranchar,rePtr));
};
set<NFAStatePtr> NFAState::getTransfer(const char tranchar)
{
	set<NFAStatePtr> result;
	Transitions::iterator iter=transtions.find(tranchar);
	Transitions::size_type count=transtions.count(tranchar);
	for(Transitions::size_type cnt=0;cnt!=count;++cnt,++iter)
		result.insert(iter->second);
	return result;
};
ostream &operator<<(ostream &os,const NFAState &state)
{
	os<<"StateID:"<<state.StateID<<endl;
	for(NFAState::Transitions::const_iterator p=state.transtions.begin();p!=state.transtions.end();p++)
		os<<p->first<<"->"<<p->second->StateID<<endl;
	return os;
};
const string AddConcatOperator(const string &pat)
{
	string result;
	char forward=0;
	for(unsigned int i=0;i<pat.size();i++)
	{
		if(i==0) {forward=pat.at(i);result.push_back(forward);}
		else
		{
			if(isalpha(pat.at(i))||isdigit(pat.at(i))||pat.at(i)=='(')
			{
				if(isalpha(forward)||isdigit(forward)||forward==')'||forward=='*')
					result.push_back(',');
			}
			forward=pat.at(i);
			result.push_back(forward);
		}
	}
	return result;
}
void RegEx::parse(const string &pat)
{
	const string patt=AddConcatOperator(pat);
	for(string::const_iterator p=patt.begin();p!=patt.end();p++)
	{
		if(isalpha(*p)||isdigit(*p)) push(*p);
		else if(operatorstack.empty()) operatorstack.push(*p);
		else if(*p=='(') operatorstack.push(*p);
		else if(*p==')')
		{
			while(operatorstack.top()!='(')
				Eval();
			operatorstack.pop();
		}
		else
		{
			while(!operatorstack.empty()&&Prior(operatorstack.top(),*p))
				Eval();
			operatorstack.push(*p);
		}

	}
	while(!operatorstack.empty())
		Eval();
	nfa=opstack.top();
	opstack.pop();
};
void RegEx::Eval()
{
	switch(operatorstack.top())
	{
	case '|':
		Union();
		break;
	case ',':
		Concat();
		break;
	case '*':
		Repeat();
		break;
	/*case '(':
		Concat();
		break*/;
		
	}
	operatorstack.pop();
};
bool RegEx::Prior(const char c1,const char c2)
{
	return oppr[c1]>=oppr[c2];
}
void RegEx::push(const char input)
{
	NFAStatePtr s0=new NFAState;
	NFAStatePtr s1=new NFAState;
	s0->connect(input,s1);
	NFATABLE nft;
	nft.push_back(s0);
	nft.push_back(s1);
	opstack.push(nft);
};
void RegEx::Union()
{
	NFAStatePtr s0=new NFAState;
	NFAStatePtr s1=new NFAState;
	NFATABLE a,b;
	b=opstack.top();
	opstack.pop();
	a=opstack.top();
	opstack.pop();
	s0->connect(0,a[0]);
	s0->connect(0,b[0]);
	a.back()->connect(0,s1);
	b.back()->connect(0,s1);
	a.insert(a.end(),b.begin(),b.end());
	a.insert(a.begin(),s0);
	a.insert(a.end(),s1);
	opstack.push(a);
}
void RegEx::Concat()
{
	NFATABLE a,b;
	b=opstack.top();
	opstack.pop();
	a=opstack.top();
	opstack.pop();
	a.back()->connect(0,b[0]);
	a.insert(a.end(),b.begin(),b.end());
	opstack.push(a);
}
void RegEx::Repeat()
{
    NFATABLE a;
	a=opstack.top();
	opstack.pop();
	NFAStatePtr s0=new NFAState;
	NFAStatePtr s1=new NFAState;
	s0->connect(0,a[0]);
	s0->connect(0,s1);
	a.back()->connect(0,s1);
	a.back()->connect(0,s0);
	a.insert(a.begin(),s0);
	a.insert(a.end(),s1);
	opstack.push(a);
};
void RegEx::PrintNfa()
{
	NFAStatePtr queue[10000];
	int head,tail;
	set<NFAStatePtr> had;
	head=tail=0;
#define qpush(x) queue[tail++]=x;
#define qpop(x) x=queue[head];head++;
	NFAStatePtr start=nfa.at(0);
	qpush(start);
	had.insert(start);
	while(head!=tail)
	{
		NFAStatePtr qhead;
		qpop(qhead);
		cout<<*qhead;
		for(NFAState::Transitions::iterator p=qhead->OutIterBegin();p!=qhead->OutIterEnd();p++)
		{
			if(had.find(p->second)==had.end()) {had.insert(p->second);qpush(p->second);}
		}
     }
	

};
set<NFAStatePtr> RegEx::e_Closure(NFAStatePtr stPrt)
{
	set<NFAStatePtr> re;
	re.insert(stPrt);
	queue<NFAStatePtr> tempqueue;
	tempqueue.push(stPrt);
	while(!tempqueue.empty())
	{
		NFAStatePtr headp;
		headp=tempqueue.front();
		tempqueue.pop();
		for(NFAState::Transitions::iterator p=headp->OutIterBegin();p!=headp->OutIterEnd();p++)
		{
			if(p->first==0&&re.find(p->second)==re.end())  {re.insert(p->second);tempqueue.push(p->second);}
		}
	}
	return re;
}
set<NFAStatePtr> RegEx::getTransfer(const char tranchar,const set<NFAStatePtr> &stPrtSet)
{
	set<NFAStatePtr> result;
	for(set<NFAStatePtr>::const_iterator p=stPrtSet.begin();p!=stPrtSet.end();p++)
	{
		set<NFAStatePtr> temp;
		temp=(*p)->getTransfer(tranchar);
		if( !temp.empty())
		{
			for(set<NFAStatePtr>::iterator q=temp.begin();q!=temp.end();q++)
			{
				set<NFAStatePtr> temp1=e_Closure(*q);
				result.insert(temp1.begin(),temp1.end());
			}
		}
	}
	return result;
}
void RegEx::toDfa()
{
	Alphabet alphabet(patt);
	
	map<set<NFAStatePtr>,int > toMatrixEntry;
	int cnt=0;
	queue<set<NFAStatePtr> > dfaStateSetsQueue;

	dfaStateSetsQueue.push(e_Closure(nfa.at(0)));
	toMatrixEntry.insert(make_pair(e_Closure(nfa.at(0)),cnt++));
	dfa.AddRow();
	dfa.SetStart(0);
	while(!dfaStateSetsQueue.empty())
	{
		set<NFAStatePtr> qhead=dfaStateSetsQueue.front();
		dfaStateSetsQueue.pop();
		set<NFAStatePtr> temp;
		for(unsigned int i=0;i<alphabet.size();i++)
		{
			temp=getTransfer(alphabet.at(i),qhead);
			if(!temp.empty())
			{
				
				
				if(toMatrixEntry.find(temp)==toMatrixEntry.end())
				{

					dfa.AddRow();
					dfaStateSetsQueue.push(temp);
				
					toMatrixEntry.insert(make_pair(temp,cnt++));
					if(temp.find(nfa.back())!=temp.end())
						dfa.AddEnd(cnt-1);
				}
				dfa[toMatrixEntry[qhead]][dfa.GetAlphaIndex(alphabet.at(i))]=toMatrixEntry[temp];
			}
		}

	}
}
MDfa RegEx::GetDfa()
{
	return dfa;
}
RegEx::~RegEx()
{
	for(NFATABLE::iterator p=nfa.begin();p!=nfa.end();p++)
		delete(*p);
};