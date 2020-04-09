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

    const size_t len = 100;
    ImageData_T* image_data = new ImageData_T[len];

    for (size_t i = 0; i < len; i++) {
        image_data[i].event_time = i;
        for (size_t j = 0; j < 4; j++) {
            image_data[i].status_arr[j] = (j % 2 == 0 ? true : false);
        }
        for (size_t x = 0; x < 4; x++) {
            for (size_t y = 0; y < 6; y++) {
                for (size_t z = 0; z < 8; z++) {
                    image_data[i].imgfrm_arr[x][y][z] = x + y + z + i;
                }
            }
        }
        image_data[i].wait_threshold = (i + 1) * (i + 1);
        image_data[i].late_arrived = (i % 2 == 0 ? false : true);

    }


    H5File* h5file = new H5File("mydata.h5", H5F_ACC_RDWR | H5F_ACC_SWMR_WRITE);



    hsize_t dim[] = {1};
    hsize_t dim_max[] = {H5S_UNLIMITED};
    DataSpace space(1, dim, dim_max);

    DSetCreatPropList create_params;

    hsize_t chunk_dim[] = {CHUNK_SIZE};
    create_params.setChunk(1, chunk_dim);
    create_params.setDeflate(4);

    DataSet dataset = h5file->openDataSet("image_data");
    hid_t dataset_id = dataset.getId();

    size_t batch_size = BATCH_SIZE;
    hsize_t size[1];
    hsize_t mem_dim[1] = {1};
    hsize_t offset[1];
    for (size_t i = 0; i < len / batch_size; i++) {
        size[0] = (i + 1) * batch_size;
        dataset.extend(size);
        DataSpace file_space = dataset.getSpace();
        offset[0] = i * batch_size;
        mem_dim[0] = batch_size;
        file_space.selectHyperslab(H5S_SELECT_SET, mem_dim, offset);
        DataSpace mem_space(1, mem_dim);
        dataset.write(image_data + i * batch_size, image_type, mem_space, file_space);

        H5Dflush(dataset_id);

        cout << "wait ... " << offset[0] + batch_size << endl;
        sleep(2);

    }

    delete [] image_data;

    h5file->close();
    delete h5file;
    h5file = nullptr;

    return 0;
}
