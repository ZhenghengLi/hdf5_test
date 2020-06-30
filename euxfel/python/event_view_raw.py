#!/usr/bin/env python3

import h5py
import numpy as np
import matplotlib.pyplot as plt
import os

from common_func import get_image_dset

det_path = '/INSTRUMENT/SPB_DET_AGIPD1M-1/DET'
file_paths = [
    ['/DATA_HDD/mark/CXIDB/id83/r0243/raw/RAW-R0243-AGIPD%02d-S%05d.h5' % (x, y) for y in range(3)] for x in range(16) ]

h5file00 = h5py.File(file_paths[0][0], 'r')
h5file01 = h5py.File(file_paths[0][1], 'r')
h5file02 = h5py.File(file_paths[0][2], 'r')

trainId_dset00 = get_image_dset(h5file00, det_path, "trainId")
trainId_dset01 = get_image_dset(h5file01, det_path, "trainId")
trainId_dset02 = get_image_dset(h5file02, det_path, "trainId")

trainId_arr_all = []
trainId_arr_all += trainId_dset00
trainId_arr_all += trainId_dset01
trainId_arr_all += trainId_dset02

# plt.plot(trainId_dset00[:])
# plt.plot(trainId_dset01[:])
# plt.plot(trainId_dset02[:])
plt.plot(trainId_arr_all)
print(len(trainId_arr_all))

plt.show()

# h5files = [h5py.File(fn, 'r') for fn in file_paths]

# data_dsets = [get_image_dset(h5f, det_path, 'data') for h5f in h5files]
# trainId_dsets = [get_image_dset(h5f, det_path, 'trainId') for h5f in h5files]
# pulseId_dsets = [get_image_dset(h5f, det_path, 'pulseId') for h5f in h5files]
# 
# len_arr = [data_dsets[it].shape[0] for it in range(16)]
# print(len_arr)
# 
# min_len = np.min(len_arr)
# 
# for x in range(500):
#     trainId_target = trainId_dsets[0][x]
#     for y in range(1, 16):
#         if (trainId_dsets[y][x] != trainId_target):
#             print("not match train Id found: (%d, %d)" % (x, y))

# arr = None

# plt.plot(trainId_dset[:])

# plt.plot(list(pulseId_dset))

# with data_dset.astype('float'):
#     arr = data_dset[0][0]
# 
# arr[10][10] = np.nan
# 
# pos = plt.contourf(arr, 100, cmap = "rainbow")
# plt.colorbar(pos)

# plt.show()
