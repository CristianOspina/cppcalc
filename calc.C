#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include "calcex.h"
#include "calculator.h"
#include <cstdlib>

using namespace std;

Calculator *calc;

char test(short unsigned char ac)
{
  return ac;
}

int main(int argc, char *argv[], char *env[])
{

  string line;
  string argvs;
  ifstream fichero;
  int ficheros[argc];
  for (int i = 0; i < argc; ++i)
    ficheros[i] = 0;
  int ifFichero = 0;
  int leyendoFichero = 0;
  int nF = 0;
  int ifCompile = 0;
  ofstream eweFile;
  string resultEWE = "";
  string fileName = "";
  string outFileName = "a";
  int exprNum = 0;

  calc = new Calculator();
  calc->getEnvVars(env);

  if (argc > 0)
  {
    for (int i = 1; i < argc; i++)
    {
      argvs = argv[i];
      if (argvs == "-c")
      {
        ifCompile = 1;
        continue;
      }
      else if (argvs == "-v")
      {
        if (++i < argc)
        {
          argvs = argv[i];
          size_t n = argvs.find("=");
          if (n > 0 && n != string::npos)
          {
            calc->getArgVars(argvs, n);
          }
          else
          {
            cerr << "* Uso: " << argv[0] << " -v \"identifier = number\"" << endl;
            return 1;
          }
        }
        else
        {
          cerr << "* Uso: " << argv[0] << " -v \"identifier = number\"" << endl;
          return 1;
        }
      }
      else
      {
        ficheros[i] = 1;
        ifFichero++;
      }
    }
  }

  while (true)
  {
    if (!leyendoFichero && ifFichero)
    {
      while (nF < argc)
      {
        if (ficheros[nF] == 1)
        {
          fichero.open(argv[nF]);
          nF++;
          if (!fichero.good())
          {
            fichero.close();
            cerr << "* File: " << argv[nF - 1] << " not found. This command has been ignored." << endl;
          }
          else
          {
            outFileName = argv[nF - 1];
            size_t n = outFileName.find(".");
            if (n != 0 && n != string::npos)
            {
              outFileName = outFileName.substr(0, n);
            }
            else
            {
              cerr << "* Wrong file name." << endl;
              continue;
            }
            leyendoFichero = 1;
            ifFichero--;
            break;
          }
        }
        else
        {
          ++nF;
          continue;
        }
      }
    }

    if (leyendoFichero && getline(fichero, line))
    {
    }
    else
    {
      if (leyendoFichero)
      {
        if (ifCompile)
        {
          eweFile << calc->getMemEWE();
          exprNum = 0;
          calc->resetVars();
          if (!ifFichero)
            ifCompile = 0;
        }
        fichero.close();
        leyendoFichero = 0;
        if (!ifFichero)
          break;
        continue;
      }
      clog << "> ";
      getline(cin, line);
      if (cin.eof())
        break;
    }
    if (line.empty() || line == "\r")
      continue;
    try
    {
      if (ifCompile)
      {
        resultEWE = calc->evalEWE(line, exprNum++);

        if (fileName != outFileName)
        {
          eweFile.close();
          fileName = outFileName;
          string finalName = outFileName + ".ewe";
          eweFile.open(finalName.c_str());
        }
        eweFile << resultEWE;
      }
      else
      {
        int result = calc->eval(line);
        string var = calc->getActualVar();
        cout << "= " << var << " <- " << result << endl;
      }
    }
    catch (Exception ex)
    {
    }
  }
  if (ifCompile)
  {
    eweFile << calc->getMemEWE();
  }
  delete calc;
}