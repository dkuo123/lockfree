#include <iostream>
using namespace std;
namespace A 
{
 struct BankAcct {};
 void createAcct(const BankAcct&) { cout << "in Namespace" << endl; }
}

 struct BankAcct {};
 void createAcct(const BankAcct&) { cout << "No Namespace" << endl; }

A::BankAcct acct;

main() 
{
 // ::createAcct(acct); // this cause compiler error
 createAcct(acct);  // this surprisely passes and print "in Namespace"
// he compiler fail to find in a global namesapce a function taking a GlobalBanks::BankAccount, so, it search in the namespace of the argument, and find GlobalBanks::createAccount().
}
