#include "ast.h"
#include <iostream>
#include <string>
#include <sstream>
#include "calculator.h"

//for debug information uncomment
//#define debug

AST::AST() {}

AST::~AST() {}

BinaryNode::BinaryNode(AST *left, AST *right) : AST(),
                                                leftTree(left),
                                                rightTree(right)
{
}

BinaryNode::~BinaryNode()
{
#ifdef debug
  cout << "In BinaryNode destructor" << endl;
#endif

  try
  {
    delete leftTree;
  }
  catch (...)
  {
  }

  try
  {
    delete rightTree;
  }
  catch (...)
  {
  }
}

AST *BinaryNode::getLeftSubTree() const
{
  return leftTree;
}

AST *BinaryNode::getRightSubTree() const
{
  return rightTree;
}

UnaryNode::UnaryNode(AST *sub) : AST(),
                                 subTree(sub)
{
}

UnaryNode::~UnaryNode()
{
#ifdef debug
  cout << "In UnaryNode destructor" << endl;
#endif

  try
  {
    delete subTree;
  }
  catch (...)
  {
  }
}

AST *UnaryNode::getSubTree() const
{
  return subTree;
}

AddNode::AddNode(AST *left, AST *right) : BinaryNode(left, right)
{
}

int AddNode::evaluate()
{
  return getLeftSubTree()->evaluate() + getRightSubTree()->evaluate();
}

void AddNode::evaluateEWE(string &code)
{
  getLeftSubTree()->evaluateEWE(code);
  getRightSubTree()->evaluateEWE(code);

  code += "# Add\n";
  code += "   operator2 := M[sp+0]\n   operator1 := M[sp+1]\n   operator1 := operator1 + operator2\n   sp := sp + one\n   M[sp+0] := operator1\n";
}

SubNode::SubNode(AST *left, AST *right) : BinaryNode(left, right)
{
}

int SubNode::evaluate()
{
  return getLeftSubTree()->evaluate() - getRightSubTree()->evaluate();
}

void SubNode::evaluateEWE(string &code)
{
  getLeftSubTree()->evaluateEWE(code);
  getRightSubTree()->evaluateEWE(code);

  code += "# Sub\n";
  code += "   operator2 := M[sp+0]\n   operator1 := M[sp+1]\n   operator1 := operator1 - operator2\n   sp := sp + one\n   M[sp+0] := operator1\n";
}

TimesNode::TimesNode(AST *left, AST *right) : BinaryNode(left, right)
{
}

int TimesNode::evaluate()
{
  return getLeftSubTree()->evaluate() * getRightSubTree()->evaluate();
}

void TimesNode::evaluateEWE(string &code)
{
  getLeftSubTree()->evaluateEWE(code);
  getRightSubTree()->evaluateEWE(code);

  code += "# Times\n";
  code += "   operator2 := M[sp+0]\n   operator1 := M[sp+1]\n   operator1 := operator1 * operator2\n   sp := sp + one\n   M[sp+0] := operator1\n";
}

DivideNode::DivideNode(AST *left, AST *right) : BinaryNode(left, right)
{
}

int DivideNode::evaluate()
{
  return getLeftSubTree()->evaluate() / getRightSubTree()->evaluate();
}

void DivideNode::evaluateEWE(string &code)
{
  getLeftSubTree()->evaluateEWE(code);
  getRightSubTree()->evaluateEWE(code);

  code += "# Divide\n";
  code += "   operator2 := M[sp+0]\n   operator1 := M[sp+1]\n   operator1 := operator1 / operator2\n   sp := sp + one\n   M[sp+0] := operator1\n";
}

ModNode::ModNode(AST *left, AST *right) : BinaryNode(left, right)
{
}

int ModNode::evaluate()
{
  return getLeftSubTree()->evaluate() % getRightSubTree()->evaluate();
}

void ModNode::evaluateEWE(string &code)
{
  getLeftSubTree()->evaluateEWE(code);
  getRightSubTree()->evaluateEWE(code);

  code += "# Mod\n";
  code += "   operator2 := M[sp+0]\n   operator1 := M[sp+1]\n   operator1 := operator1 % operator2\n   p := sp + one\n   M[sp+0] := operator1\n";
}

NumNode::NumNode(int n) : AST(),
                          val(n)
{
}

int NumNode::evaluate()
{
  return val;
}

void NumNode::evaluateEWE(string &code)
{
  stringstream s;
  s << val;
  string str = s.str();

  code += "# push(" + str + ")\n";
  code += "   sp := sp - one\n   operator1 := " + str + "\n   M[sp+0] := operator1\n";
}

StoreNode::StoreNode(AST *sub) : UnaryNode(sub) {}

int StoreNode::evaluate()
{
  int ret = getSubTree()->evaluate();
  calc->store(ret);
  return ret;
}

void StoreNode::evaluateEWE(string &code)
{
  getSubTree()->evaluateEWE(code);
  code += "# Store\n";
  code += "   memory := M[sp+0]\n";
}

PlusNode::PlusNode(AST *sub) : UnaryNode(sub) {}

int PlusNode::evaluate()
{
  int ret = getSubTree()->evaluate();
  calc->plus(ret);
  return calc->recall();
}

void PlusNode::evaluateEWE(string &code)
{
  getSubTree()->evaluateEWE(code);
  code += "# Memory Plus\n";
  code += "   operator2 := M[sp+0]\n   memory := memory + operator2\n   M[sp+0] := memory\n";
}

MinusNode::MinusNode(AST *sub) : UnaryNode(sub) {}

int MinusNode::evaluate()
{
  int ret = getSubTree()->evaluate();
  calc->minus(ret);
  return calc->recall();
}

void MinusNode::evaluateEWE(string &code)
{
  getSubTree()->evaluateEWE(code);
  code += "# Memory Minus\n";
  code += "   operator2 := M[sp+0]\n   memory := memory - operator2\n   M[sp+0] := memory\n";
}

ClearNode::ClearNode() {}

int ClearNode::evaluate()
{
  return calc->clear();
}

void ClearNode::evaluateEWE(string &code)
{
  code += "# Clear\n";
  code += "   memory := zero\n   sp := sp - one\n   M[sp+0] := memory\n";
}

RecallNode::RecallNode() {}

int RecallNode::evaluate()
{
  return calc->recall();
}

void RecallNode::evaluateEWE(string &code)
{
  code += "# Recall\n";
  code += "   sp := sp - one\n   M[sp+0] := memory\n";
}

IdentNode::IdentNode(string n) : AST(),
                                 name(n)
{
}

int IdentNode::evaluate()
{
  return calc->getVar(name);
}

void IdentNode::evaluateEWE(string &code)
{
  code += "# push(" + name + ")\n";

  int value = calc->getVar(name);
  if (value == 0)
  {
    code += "   sp := sp - one\n   operator1 := zero\n   M[sp+0] := operator1\n";
  }
  else
  {
    code += "   sp := sp - one\n   operator1 := " + name + "\n   M[sp+0] := operator1\n";
  }
}

AssignNode::AssignNode(AST *left, AST *right) : BinaryNode(left, right)
{
}

int AssignNode::evaluate()
{
  return getRightSubTree()->evaluate();
}

void AssignNode::evaluateEWE(string &code)
{
  getRightSubTree()->evaluateEWE(code);
  code += "# Assign\n";
  code += "   " + calc->getActualVar() + " := M[sp+0]\n";
}