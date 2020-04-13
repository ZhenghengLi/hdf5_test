.PHONY : all clean

all: read

read: read.cpp
	g++ -I/usr/include/hdf5/serial read.cpp -o read -lhdf5_cpp -lhdf5_serial

clean:
	rm -rf read
