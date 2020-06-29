ROOT_FLAGS := $(shell root-config --cflags)
ROOT_LIBS := $(shell root-config --libs)

.PHONY : all clean

all: read_raw read_corr

read_raw: read_raw.cpp
	g++ -I/usr/include/hdf5/serial read_raw.cpp $(ROOT_FLAGS) -o read_raw -lhdf5_cpp -lhdf5_serial $(ROOT_LIBS)

read_corr: read_corr.cpp
	g++ -I/usr/include/hdf5/serial read_corr.cpp $(ROOT_FLAGS) -o read_corr -lhdf5_cpp -lhdf5_serial $(ROOT_LIBS)

clean:
	rm -rf read_raw read_corr
