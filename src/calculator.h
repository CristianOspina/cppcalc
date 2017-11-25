#ifndef calculator_h
#define calculator_h

#include <string>
#include <map>

using namespace std;

class Calculator
{
public:
  Calculator();

  int eval(string expr);
  string evalEWE(string expr, int exprNum);
  string getMemEWE();
  void store(int val);
  int recall();
  void minus(int val);
  void plus(int val);
  int clear();
  void getEnvVars(char *env[]);
  void getArgVars(string arg, size_t n);
  void setVar(string var, int num);
  int getVar(string var);
  void setGlobalVar(string var, int num);
  void resetVars();
  void setActualVar(string var);
  string getActualVar();

private:
  int memory;
  map<string, int> globalVars;
  map<string, int> vars;
  string actualVar;
};

extern Calculator *calc;

#endif