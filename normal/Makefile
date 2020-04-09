.PHONY : all clean

all: create_and_write read

create_and_write: create_and_write.cpp
	g++ -I/usr/include/hdf5/serial create_and_write.cpp -o create_and_write -lhdf5_cpp -lhdf5_serial

read: read.cpp
	g++ -I/usr/include/hdf5/serial read.cpp -o read -lhdf5_cpp -lhdf5_serial

clean:
	rm -rf create_and_write read mydata.h5
