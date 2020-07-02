#!/usr/bin/env python3

import h5py
import numpy as np
import matplotlib.pyplot as plt
import sys
import os

from common_func import get_image_dset, compose_image

if len(sys.argv) < 2:
    print("usage: ", sys.argv[0], '<align_idx_file.h5>')
    exit(2)

align_idx_filename = sys.argv[1]

align_idx_h5file = h5py.File(align_idx_filename, 'r')
align_idx_dset = align_idx_h5file['alignment_index']

align_idx_len = align_idx_dset.shape[0]

det_path = '/INSTRUMENT/SPB_DET_AGIPD1M-1/DET'
file_paths = [
    ['/DATA_HDD/mark/CXIDB/id83/r0243/proc/CORR-R0243-AGIPD%02d-S%05d.h5' % (x, y)
    for y in range(3)] for x in range(16) ]

current_file_idx = [-1] * 16
image_h5files = [None] * 16
image_dsets = [None] * 16
trainId_dsets = [None] * 16
pulseId_dsets = [None] * 16
cellId_dsets = [None] * 16
mask_dsets = [None] * 16

# image_h5files = [h5py.File(file_paths[x][index_mat[x][0]], 'r') for x in range(16)]

for x in range(align_idx_len):
    if (x % 1000 == 0):
        print("checking ", x)
    index_mat = align_idx_dset[x]
    image_data = np.zeros( (16, 512, 128) )
    trainId_arr = [-1] * 16
    pulseId_arr = [-1] * 16
    cellId_arr = [-1] * 16
    invalid_flag = False
    for d in range(16):
        if index_mat[d][0] < 0:
            invalid_flag = True
            break
        if current_file_idx[d] != index_mat[d][0]:
            if image_h5files[d]: image_h5files[d].close()
            image_h5files[d] = h5py.File(file_paths[d][index_mat[d][0]], 'r')
            image_dsets[d] = get_image_dset(image_h5files[d], det_path, 'data')
            trainId_dsets[d] = get_image_dset(image_h5files[d], det_path, 'trainId')
            pulseId_dsets[d] = get_image_dset(image_h5files[d], det_path, 'pulseId')
            cellId_dsets[d] = get_image_dset(image_h5files[d], det_path, 'cellId')
            mask_dsets[d] = get_image_dset(image_h5files[d], det_path, 'mask')
            current_file_idx[d] = index_mat[d][0]
        cellId = cellId_dsets[d][index_mat[d][1]]
        if cellId % 2 == 1 or cellId == 0 or cellId == 62:
            invalid_flag = True
            break
        image_data[d] = np.nan_to_num(image_dsets[d][index_mat[d][1]])
        mask_data = mask_dsets[d][index_mat[d][1]]
        image_data[d][mask_data > 0] = 0
        cellId_arr[d] = cellId
        trainId_arr[d] = trainId_dsets[d][index_mat[d][1]][0]
        pulseId_arr[d] = pulseId_dsets[d][index_mat[d][1]][0]

    if invalid_flag: continue
    signal_mean = np.sum(image_data) / (1024. * 1024.)
    if signal_mean > 600:
        print("found: %d, %d" % (x % 64, x / 64))
        print("trainId: ", trainId_arr)
        print("pulseId:", pulseId_arr)
        print("cellId:", cellId_arr)
        print()

