CC=g++
AR=ar
CFLAGS=--std=c++11 -Wall
OUTPUT=main
OUTPUT_LIB=libproject.a
OUTPUT_LIB_SH=libproject.so


all: main libs

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

project_lib.o: project_lib.cpp
	$(CC) $(CFLAGS) -c project_lib.cpp

main: main.o project_lib.o
	$(CC) main.o project_lib.o -o main

$(OUTPUT_LIB): project_lib.o
	$(AR) crf libproject.a project_lib.o

project_lib_sh.o: project_lib.cpp
	$(CC) $(CFLAGS) -c project_lib.cpp -o project_lib_sh.o -fPIC

$(OUTPUT_LIB_SH): project_lib_sh.o
	$(CC) project_lib_sh.o -shared -fPIC -o libproject.so

libs: $(OUTPUT_LIB) $(OUTPUT_LIB_SH)

clean:
	rm *.o main
