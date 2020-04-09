.PHONY : all clean

all: create write read

create: create.cpp
	g++ -I/usr/include/hdf5/serial create.cpp -o create -lhdf5_cpp -lhdf5_serial

write: write.cpp
	g++ -I/usr/include/hdf5/serial write.cpp -o write -lhdf5_cpp -lhdf5_serial

read: read.cpp
	g++ -I/usr/include/hdf5/serial read.cpp -o read -lhdf5_cpp -lhdf5_serial

clean:
	rm -rf create write read mydata.h5
