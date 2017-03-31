CC=g++
AR=ar
CFLAGS=--std=c++11 -Wall
OUTPUT=main
OUTPUT_LIB=libproject_lib.a
OUTPUT_LIB_SH=libproject.so


all: main main_dynamic main_static libs

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

#main.o: main.cpp
#	$(CC) $(CFLAGS) -c main.cpp

#project_lib.o: project_lib.cpp
#	$(CC) $(CFLAGS) -c project_lib.cpp

main_static: main.o $(OUTPUT_LIB)
	$(CC) main.o -L. -lproject_lib -o main_static

main_dynamic: main.o $(OUTPUT_LIB_SH)
	$(CC) main.o -L. -lproject -o main_dynamic -Wl,-rpath=.

main: main.o project_lib.o
	$(CC) main.o project_lib.o -o main

$(OUTPUT_LIB): project_lib.o
	$(AR) crf $(OUTPUT_LIB) project_lib.o

project_lib_sh.o: project_lib.cpp
	$(CC) $(CFLAGS) -c project_lib.cpp -o project_lib_sh.o -fPIC

$(OUTPUT_LIB_SH): project_lib_sh.o
	$(CC) project_lib_sh.o -shared -o $(OUTPUT_LIB_SH)

libs: $(OUTPUT_LIB) $(OUTPUT_LIB_SH)

clean:
	rm *.o main_*
