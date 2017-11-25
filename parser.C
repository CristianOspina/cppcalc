#include "parser.h"
#include "calcex.h"
#include <string>
#include <sstream>
#include "calculator.h"

Parser::Parser(istream *in)
{
  scan = new Scanner(in);
}

Parser::~Parser()
{
  try
  {
    delete scan;
  }
  catch (...)
  {
  }
}

AST *Parser::parse()
{
  return Prog();
}

AST *Parser::Prog()
{

  AST *result = Stmts();
  Token *t = scan->getToken();

  if (t->getType() != eof)
  {
    cout << "* Syntax Error: Expected EOF, found token at " << t->getCol() << endl;
    throw ParseError;
  }

  return result;
}

AST *Parser::Stmts()
{
  return Stmts(Stmt());
}

AST *Parser::Stmts(AST *e)
{
  return e;
}

AST *Parser::Stmt()
{
  Token *t = scan->getToken();
  if (t->getType() != identifier)
  {
    cout << "* Syntax Error: Expected identifier, at column " << t->getCol() << endl;
    throw ParseError;
  }

  string name = t->getLex();
  calc->setActualVar(name);

  t = scan->getToken();
  if (t->getType() != assign)
  {
    cout << "* Syntax Error: Expected assign (=), at column " << t->getCol() << endl;
    throw ParseError;
  }

  AST *evalExpr = Expr();

  t = scan->getToken();
  if (t->getType() != semiColon)
  {
    cout << "* Syntax Error: Expected semicolon (;), at column " << t->getCol() << endl;
    throw ParseError;
  }

  return new AssignNode(new IdentNode(name), evalExpr);
}

AST *Parser::Expr()
{
  return RestExpr(Term());
}

AST *Parser::RestExpr(AST *e)
{
  Token *t = scan->getToken();

  if (t->getType() == add)
    return RestExpr(new AddNode(e, Term()));

  if (t->getType() == sub)
    return RestExpr(new SubNode(e, Term()));

  scan->putBackToken();

  return e;
}

AST *Parser::Term()
{
  return RestTerm(Storable());
}

AST *Parser::RestTerm(AST *e)
{
  Token *t = scan->getToken();

  if (t->getType() == times)
    return RestTerm(new TimesNode(e, Storable()));

  if (t->getType() == divide)
    return RestTerm(new DivideNode(e, Storable()));

  if (t->getType() == mod)
    return RestTerm(new ModNode(e, Storable()));

  scan->putBackToken();

  return e;
}

AST *Parser::Storable()
{
  return MemOperation(Factor());
}

AST *Parser::MemOperation(AST *e)
{
  Token *t = scan->getToken();
  if (t->getType() == keyword)
  {
    if (t->getLex().compare("S") == 0)
    {
      return new StoreNode(e);
    }
    else if (t->getLex().compare("P") == 0)
    {
      return new PlusNode(e);
    }
    else if (t->getLex().compare("M") == 0)
    {
      return new MinusNode(e);
    }
    else
    {
      cout << "* Expected S or P or M found: " << t->getType() << " at col: " << t->getCol() << endl;
      throw ParseError;
    }
  }
  scan->putBackToken();
  return e;
}

AST *Parser::Factor()
{
  Token *t = scan->getToken();

  if (t->getType() == number)
  {
    istringstream in(t->getLex());
    int val;
    in >> val;
    return new NumNode(val);
  }

  if (t->getType() == identifier)
  {
    istringstream in(t->getLex());
    string name;
    in >> name;
    return new IdentNode(name);
  }

  if (t->getType() == keyword)
  {
    if (t->getLex().compare("R") == 0)
    {
      return new RecallNode();
    }
    else if (t->getLex().compare("C") == 0)
    {
      return new ClearNode();
    }
    else
    {
      cout << "* Expected R or C found: " << t->getType() << " at col: " << t->getCol() << endl;
      throw ParseError;
    }
  }

  if (t->getType() == lparen)
  {
    AST *result = Expr();
    t = scan->getToken();

    if (t->getType() == rparen)
    {
      return result;
    }
    else
    {
      cout << "* Expected ) found: " << t->getType() << " at col: " << t->getCol() << endl;
      throw ParseError;
    }
  }

  if (t->getType() == sub)
  {
    t = scan->getToken();
    if (t->getType() == number)
    {
      istringstream in(t->getLex());
      int val;
      in >> val;
      return new NumNode(val * -1);
    }
    cout << "* Expected number found: " << t->getType() << " at col: " << t->getCol() << endl;
    throw ParseError;
  }

  cout << "* Expected number or R or ( found: " << t->getType() << " at col: " << t->getCol() << endl;
  throw ParseError;
}