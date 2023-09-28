main: src/main.cpp parser.o ast-generator.o node.o runner.o
	g++ -Wall $^ -o lazy

parser.o: src/parser.cpp include/parser.hpp
	g++ -c $<

node.o: src/node.cpp include/node.hpp
	g++ -c $<

ast-generator.o: src/ast-generator.cpp include/ast-generator.hpp
	g++ -c $<

runner.o: src/runner.cpp include/runner.hpp
	g++ -c $<

.PHONY = clean

clean:
	del *.o *.exe lazy

wslclean:
	rm *.o *.exe lazy