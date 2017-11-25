#CXXFLAGS=-g
calc: calc.o scanner.o token.o ast.o parser.o calculator.o
	g++ ${CXXFLAGS} -o calc calc.o scanner.o token.o ast.o parser.o calculator.o
calc.o: calc.C scanner.h token.h
	g++ ${CXXFLAGS} -c calc.C
calculator.o: calculator.C calculator.h parser.h ast.h
	g++ ${CXXFLAGS} -c calculator.C
scanner.o: scanner.C scanner.h token.h
	g++ ${CXXFLAGS} -c scanner.C
token.o: token.C token.h
	g++ ${CXXFLAGS} -c token.C
ast.o: ast.C ast.h
	g++ ${CXXFLAGS} -c ast.C
parser.o: parser.C parser.h
	g++ ${CXXFLAGS} -c parser.C
clean:
	rm -f *.o
	rm -f calc
