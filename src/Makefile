OBJS = main.o regex.o tokenizer.o lexer.o code_token.o parser.o column.o view.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

AQL : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o AQL
	-rm *.o

main.o : tokenizer.h code_token.h lexer.h parser.h
	$(CC) $(CFLAGS) main.cpp

regex.o : regex.cpp
	$(CC) $(CFLAGS) regex.cpp

tokenizer.o : tokenizer.h tokenizer.cpp
	$(CC) $(CFLAGS) tokenizer.cpp

lexer.o : code_token.h lexer.h
	$(CC) $(CFLAGS) lexer.cpp

code_token.o : code_token.h
	$(CC) $(CFLAGS) code_token.cpp

parser.o : parser.h code_token.h lexer.h column.h view.h span.h
	$(CC) $(CFLAGS) parser.cpp

column.o : tokenizer.h column.h
	$(CC) $(CFLAGS) column.cpp

view.o : column.h view.h
	$(CC) $(CFLAGS) view.cpp

test3:
	./AQL PerLoc.aql PerLoc.input

test2:
	./AQL Revenue.aql Revenue.input

test1:
	./AQL 2016.aql folder

.PHONY : clean
clean:
	-rm *.o 
