#include <iostream>
#include <string>
#include <vector>
#include "H5Cpp.h"

using namespace H5;
using namespace std;

#define BATCH_SIZE 1

int main (int argc, char** argv) {

    if (argc < 3) {
        cout << "usage: " << argv[0] << " <file.h5> <index>" << endl;
    }

    string filename = argv[1];
    int index = atoi(argv[2]);

    // read data of one frame
    int one_frame[512][128];

    H5File* h5file = new H5File(filename, H5F_ACC_RDONLY);
    DataSet image_data_dset = h5file->openDataSet("INSTRUMENT/SPB_DET_AGIPD1M-1/DET/7CH0:xtdf/image/data");

    uint16_t* image_data = new uint16_t[BATCH_SIZE * 2 * 512 * 128];

    hsize_t mem_dim[] = {BATCH_SIZE, 2, 512, 128};
    DataSpace mem_space(4, mem_dim);
    DataSpace file_space = image_data_dset.getSpace();
    hsize_t offset[] = {0, 0, 0, 0};
    offset[0] = index;
    file_space.selectHyperslab(H5S_SELECT_SET, mem_dim, offset);

    image_data_dset.read(image_data, PredType::NATIVE_UINT16, mem_space, file_space);

    for (size_t i = 0; i < 512; i++) {
        for (size_t j = 0; j < 128; j++) {
            size_t idx = 0 * 2 * 512 * 128 + 0 * 512 * 128 + i * 128 + j;
            one_frame[i][j] = image_data[idx];
        }
    }

    delete [] image_data;

    h5file->close();
    delete h5file;

    // print data
    for (size_t i = 500; i < 512; i++) {
        for (size_t j = 120; j < 128; j++) {
            cout << one_frame[i][j] << " ";
        }
        cout << endl;
    }

    return 0;

}
