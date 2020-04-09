#include <iostream>
#include <string>
#include <vector>
#include "H5Cpp.h"

using namespace H5;
using namespace std;

struct ImageData_T {
    uint64_t event_time;                // UINT64
    bool status_arr[4];                 // HBOOL
    float imgfrm_arr[4][6][8];          // FLOAT
    uint64_t wait_threshold;            // UIN64
    bool late_arrived;                  // HBOOL
    hvl_t raw_data_h[4];
};

#define CHUNK_SIZE 3
#define BATCH_SIZE 2

int main (int argc, char** argv) {

    hsize_t det_dim[] = {4};
    ArrayType status_arr_t(PredType::NATIVE_HBOOL, 1, det_dim);

    hsize_t imgfrm_arr_dim[] = {4, 6, 8};
    ArrayType imgfrm_arr_t(PredType::NATIVE_FLOAT, 3, imgfrm_arr_dim);

    VarLenType varlen_char(&PredType::NATIVE_UCHAR);
    ArrayType raw_data_t(varlen_char, 1, det_dim);

    CompType image_type(sizeof(ImageData_T));
    image_type.insertMember("event_time", HOFFSET(ImageData_T, event_time), PredType::NATIVE_UINT64);
    image_type.insertMember("status_arr", HOFFSET(ImageData_T, status_arr), status_arr_t);
    image_type.insertMember("imgfrm_arr", HOFFSET(ImageData_T, imgfrm_arr), imgfrm_arr_t);
    image_type.insertMember("wait_threshold", HOFFSET(ImageData_T, wait_threshold), PredType::NATIVE_UINT64);
    image_type.insertMember("late_arrived", HOFFSET(ImageData_T, late_arrived), PredType::NATIVE_HBOOL);
    image_type.insertMember("raw_data", HOFFSET(ImageData_T, raw_data_h), raw_data_t);

    const size_t len = 10;
    ImageData_T* image_data = new ImageData_T[len];

    H5File* h5file = new H5File("mydata.h5", H5F_ACC_RDONLY);

    DataSet dataset = h5file->openDataSet("image_data");

    size_t batch_size = BATCH_SIZE;
    hsize_t mem_dim[1] = {1};
    hsize_t offset[1];
    for (size_t i = 0; i < len / batch_size; i++) {

        DataSpace file_space = dataset.getSpace();
        offset[0] = i * batch_size;
        mem_dim[0] = batch_size;
        file_space.selectHyperslab(H5S_SELECT_SET, mem_dim, offset);
        DataSpace mem_space(1, mem_dim);

        dataset.read(image_data, image_type, mem_space, file_space);

        cout << "==============" << endl;


        for (size_t i = 0; i < batch_size; i++) {

            cout << "event_time = " << image_data[i].event_time << endl;
            cout << "first image:" << endl;
            for (size_t j = 0; j < 6; j++) {
                for (size_t k = 0; k < 8; k++) {
                    cout << image_data[i].imgfrm_arr[0][j][k] << " ";
                }
                cout << endl;
            }

            for (size_t j = 0; j < image_data[i].raw_data_h[0].len; j++) {
                cout << ((char*) image_data[i].raw_data_h[0].p)[j];
            }
            cout << endl;
            cout << "------------------" << endl;
        }

        DataSet::vlenReclaim(image_data, image_type, mem_space);

    }

    delete [] image_data;

    h5file->close();
    delete h5file;
    h5file = nullptr;

    return 0;
}
