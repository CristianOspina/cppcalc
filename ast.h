#ifndef ast_h
#define ast_h
#include <string>

using namespace std;

class AST
{
public:
  AST();
  virtual ~AST() = 0;
  virtual int evaluate() = 0;
  virtual void evaluateEWE(string &code) = 0;
};

class BinaryNode : public AST
{
public:
  BinaryNode(AST *left, AST *right);
  ~BinaryNode();

  AST *getLeftSubTree() const;
  AST *getRightSubTree() const;

private:
  AST *leftTree;
  AST *rightTree;
};

class UnaryNode : public AST
{
public:
  UnaryNode(AST *sub);
  ~UnaryNode();

  AST *getSubTree() const;

private:
  AST *subTree;
};

class AddNode : public BinaryNode
{
public:
  AddNode(AST *left, AST *right);

  int evaluate();
  void evaluateEWE(string &code);
};

class SubNode : public BinaryNode
{
public:
  SubNode(AST *left, AST *right);

  int evaluate();
  void evaluateEWE(string &code);
};

class TimesNode : public BinaryNode
{
public:
  TimesNode(AST *left, AST *right);
  int evaluate();
  void evaluateEWE(string &code);
};

class DivideNode : public BinaryNode
{
public:
  DivideNode(AST *left, AST *right);
  int evaluate();
  void evaluateEWE(string &code);
};

class ModNode : public BinaryNode
{
public:
  ModNode(AST *left, AST *right);
  int evaluate();
  void evaluateEWE(string &code);
};

class NumNode : public AST
{
public:
  NumNode(int n);

  int evaluate();
  void evaluateEWE(string &code);

private:
  int val;
};

class StoreNode : public UnaryNode
{
public:
  StoreNode(AST *sub);
  int evaluate();
  void evaluateEWE(string &code);
};

class MinusNode : public UnaryNode
{
public:
  MinusNode(AST *sub);
  int evaluate();
  void evaluateEWE(string &code);
};

class PlusNode : public UnaryNode
{
public:
  PlusNode(AST *sub);
  int evaluate();
  void evaluateEWE(string &code);
};

class RecallNode : public AST
{
public:
  RecallNode();
  int evaluate();
  void evaluateEWE(string &code);
};

class ClearNode : public AST
{
public:
  ClearNode();
  int evaluate();
  void evaluateEWE(string &code);
};

class IdentNode : public AST
{
public:
  IdentNode(string n);
  int evaluate();
  void evaluateEWE(string &code);

private:
  string name;
};

class AssignNode : public BinaryNode
{
public:
  AssignNode(AST *left, AST *right);
  int evaluate();
  void evaluateEWE(string &code);
};

#endif