#include <iostream>
#include <string>
#include <vector>
#include "H5Cpp.h"
#include "TApplication.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TStyle.h"

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
    float one_frame[512][128];
    uint8_t mask[512][128];

    H5File* h5file = new H5File(filename, H5F_ACC_RDONLY);
    DataSet image_data_dset = h5file->openDataSet("INSTRUMENT/SPB_DET_AGIPD1M-1/DET/7CH0:xtdf/image/data");
    DataSet image_mask_dset = h5file->openDataSet("INSTRUMENT/SPB_DET_AGIPD1M-1/DET/7CH0:xtdf/image/mask");

    float* image_data = new float[BATCH_SIZE * 512 * 128];
    uint8_t* image_mask = new uint8_t[BATCH_SIZE * 512 * 128];


    hsize_t mem_dim[] = {BATCH_SIZE, 512, 128};
    DataSpace mem_space(3, mem_dim);
    DataSpace file_space = image_data_dset.getSpace();
    hsize_t offset[] = {0, 0, 0};
    offset[0] = index;
    file_space.selectHyperslab(H5S_SELECT_SET, mem_dim, offset);

    image_data_dset.read(image_data, PredType::NATIVE_FLOAT, mem_space, file_space);
    image_mask_dset.read(image_mask, PredType::NATIVE_UINT8, mem_space, file_space);

    for (size_t i = 0; i < 512; i++) {
        for (size_t j = 0; j < 128; j++) {
            size_t idx = 0 * 512 * 128 + i * 128 + j;
            mask[i][j] = image_mask[idx];

            // one_frame[i][j] = image_data[idx];

            if (image_mask[idx] > 0) {
                one_frame[i][j] = 0;
            } else {
                one_frame[i][j] = image_data[idx];
            }
        }
    }

    delete [] image_data;

    h5file->close();
    delete h5file;

    // print data
    // for (size_t i = 0; i < 512; i++) {
    //     for (size_t j = 0; j < 128; j++) {
    //         cout << (int) mask[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    // draw image
    TApplication* rootapp = new TApplication("image frame", NULL, NULL);
    gStyle->SetOptStat(0);
    TH2F* h2 = new TH2F("image_frame", "image frame", 512, 0, 512, 128, 0, 128);
    for (size_t i = 0; i < 512; i++) {
        for (size_t j = 0; j < 128; j++) {
            h2->SetBinContent(i + 1, j + 1, one_frame[i][j]);
        }
    }
    TCanvas* c1 = new TCanvas();
    c1->cd();
    h2->Draw("colz");

    rootapp->Run();


    return 0;

}
