#include <iostream>
#include <string>
#include <vector>
#include "H5Cpp.h"
#include <unistd.h>

using namespace H5;
using namespace std;

struct ImageData_T {
    uint64_t event_time;                // UINT64
    bool status_arr[4];                 // HBOOL
    float imgfrm_arr[4][6][8];          // FLOAT
    uint64_t wait_threshold;            // UIN64
    bool late_arrived;                  // HBOOL
};

#define CHUNK_SIZE 10
#define BATCH_SIZE 2

int main (int argc, char** argv) {

    hsize_t det_dim[] = {4};
    ArrayType status_arr_t(PredType::NATIVE_HBOOL, 1, det_dim);

    hsize_t imgfrm_arr_dim[] = {4, 6, 8};
    ArrayType imgfrm_arr_t(PredType::NATIVE_FLOAT, 3, imgfrm_arr_dim);

    CompType image_type(sizeof(ImageData_T));
    image_type.insertMember("event_time", HOFFSET(ImageData_T, event_time), PredType::NATIVE_UINT64);
    image_type.insertMember("status_arr", HOFFSET(ImageData_T, status_arr), status_arr_t);
    image_type.insertMember("imgfrm_arr", HOFFSET(ImageData_T, imgfrm_arr), imgfrm_arr_t);
    image_type.insertMember("wait_threshold", HOFFSET(ImageData_T, wait_threshold), PredType::NATIVE_UINT64);
    image_type.insertMember("late_arrived", HOFFSET(ImageData_T, late_arrived), PredType::NATIVE_HBOOL);

    H5File* h5file = new H5File("mydata.h5", H5F_ACC_TRUNC | H5F_ACC_SWMR_WRITE);

    hsize_t dim[] = {0};
    hsize_t dim_max[] = {H5S_UNLIMITED};
    DataSpace space(1, dim, dim_max);

    DSetCreatPropList create_params;

    hsize_t chunk_dim[] = {CHUNK_SIZE};
    create_params.setChunk(1, chunk_dim);
    create_params.setDeflate(4);

    DataSet dataset = h5file->createDataSet("image_data", image_type, space, create_params);

    hsize_t attr_dim[] = {1};
    DataSpace attr_space(1, attr_dim);
    Attribute attr_int = dataset.createAttribute("int_attr", PredType::NATIVE_INT32, attr_space);
    int attr_data[] = {123456};
    attr_int.write(PredType::NATIVE_INT32, attr_data);

    StrType str_type(0, H5T_VARIABLE);
    Attribute attr_str = dataset.createAttribute("str_attr", str_type, attr_space);
    attr_str.write(str_type, H5std_string("this is the content of string attribute"));

    h5file->close();
    delete h5file;
    h5file = nullptr;

    return 0;
}
