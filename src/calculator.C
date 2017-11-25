#include "calculator.h"
#include "parser.h"
#include "ast.h"
#include "calcex.h"
#include <string>
#include <iostream>
#include <sstream>

Calculator::Calculator() : memory(0),
                           actualVar("")
{
}

int Calculator::eval(string expr)
{

  Parser *parser = new Parser(new istringstream(expr));
  AST *tree = parser->parse();
  int result = tree->evaluate();

  setVar(actualVar, result);

  delete tree;
  delete parser;

  return result;
}

string Calculator::evalEWE(string expr, int exprNum)
{

  string code;

  stringstream s;
  s << exprNum;
  string str = s.str();

  code += "# Expresion: " + expr + '\n';
  code += "expr" + str + ":\n";
  code += "# Instrucciones antes del recorrido del arbol sintactico\n";
  code += "   sp := 1000\n   one := 1\n   zero := 0\n";

  if (exprNum == 0)
  {
    for (map<string, int>::iterator it = globalVars.begin(); it != globalVars.end(); ++it)
    {
      stringstream s;
      s << it->second;
      string str = s.str();
      code += "   " + it->first + " := " + str + "\n";
    }
    code += "   memory := zero\n";
  }

  code += "# Comienza el recorrido del arbol\n";
  Parser *parser = new Parser(new istringstream(expr));
  AST *tree = parser->parse();
  tree->evaluateEWE(code);

  setVar(actualVar, 1);

  code += "# Write Result\n   operator1 := M[sp+0]\n   sp := sp - one\n   writeInt(operator1)\n";

  delete tree;
  delete parser;

  return code;
}

string Calculator::getMemEWE()
{
  string code;
  code += "end: halt\nequ memory M[0]\nequ one M[1]\nequ zero M[2]\nequ operator1 M[3]\nequ operator2 M[4]\nequ sp M[5]\n";
  int memPos = 6;
  for (map<string, int>::iterator it = vars.begin(); it != vars.end(); ++it, memPos++)
  {
    stringstream s;
    s << memPos;
    string str = s.str();
    code += "equ " + it->first + " M[" + str + "]\n";
  }
  code += "equ stack M[1000]";

  return code;
}

void Calculator::store(int val)
{
  memory = val;
}

int Calculator::recall()
{
  return memory;
}

void Calculator::minus(int val)
{
  memory -= val;
}

void Calculator::plus(int val)
{
  memory += val;
}

int Calculator::clear()
{
  return memory = 0;
}

void Calculator::setActualVar(string var)
{
  actualVar = var;
}

string Calculator::getActualVar()
{
  return actualVar;
}

int Calculator::getVar(string var)
{
  return vars[var];
}

void Calculator::setVar(string var, int num)
{
  vars[var] = num;
}

void Calculator::resetVars()
{
  vars = globalVars;
}

void Calculator::setGlobalVar(string var, int num)
{
  globalVars[var] = num;
}

void Calculator::getEnvVars(char *env[])
{
  for (int i = 0; env[i] != NULL; i++)
  {
    string search = env[i];
    size_t n = search.find("CALCVAR_");
    if (n == 0 && n != string::npos)
    {
      n = search.find("=");
      string var = search.substr(8, n - 8);
      string val = search.substr(n + 1);

      int num;
      stringstream ss(val);
      if ((ss >> num).fail())
      {
        cerr << "* Warning: wrong value in env var \"" << env[i] << "\", expected a int value. This var has been ignored." << endl;
        return;
      }
      setVar(var, num);
      setGlobalVar(var, num);
    }
  }
}

void Calculator::getArgVars(string arg, size_t n)
{
  string var = arg.substr(0, n);
  string val = arg.substr(n + 1);

  int num;
  stringstream ss(val);
  if ((ss >> num).fail())
  {
    cerr << "* Warning: wrong value in command var \"" << var << "\", expected a int value. This var has been ignored." << endl;
    return;
  }
  setVar(var, num);
  setGlobalVar(var, num);
}