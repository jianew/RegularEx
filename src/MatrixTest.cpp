#include"RegularEx.h"
#include"string"
using namespace std;
void main()
{
  RegEx reg("a(h|b|c)*d");
  reg.PrintNfa();
  reg.toDfa();
  MDfa dfa=reg.GetDfa();
  string restr;
  cout<<dfa;
  cout<<dfa.search("kkkkkabhcdffggg",restr)<<restr;
}                                                                                                                                                                