#!/usr/bin/env python3

import h5py
import numpy as np
import os
import sys

from common_func import get_image_dset

det_path = '/INSTRUMENT/SPB_DET_AGIPD1M-1/DET'
file_paths = [
    ['/DATA_HDD/mark/CXIDB/id83/r0243/proc/CORR-R0243-AGIPD%02d-S%05d.h5' % (x, y)
    for y in range(3)] for x in range(16) ]

out_file_name = sys.argv[1] if len(sys.argv) > 1 else 'alignment_index.h5'

h5file_out = h5py.File(out_file_name, 'w')
align_idx_dset = h5file_out.create_dataset("alignment_index", (0, 16, 2),
    maxshape = (None, 16, 2), chunks = (32, 16, 2), dtype = np.int)

current_files = [ h5py.File(file_paths[x][0], 'r') for x in range(16) ]
current_trainId_dsets = [ get_image_dset(x, det_path, 'trainId') for x in current_files ]
current_pulseId_dsets = [ get_image_dset(x, det_path, 'pulseId') for x in current_files ]
current_pos = [ [0, 0] for x in range(16) ]
current_len = [ x.shape[0] for x in current_trainId_dsets ]
reach_end_flag = [ False for x in range(16) ]

event_num = -1

while True:
    event_num += 1
    if event_num % 1000 == 0:
        print('doing alignment for event: ', event_num)
    if np.sum(reach_end_flag) >= 16:
        print('all module reach end.')
        break
    trainId_min = sys.maxsize
    pulseId_min = sys.maxsize
    alignment_index = [ [-1, -1] for x in range(16) ]
    # get minimum trainId and pulseId
    for x in range(16):
        if reach_end_flag[x]: continue
        trainId = current_trainId_dsets[x][current_pos[x][1]][0]
        pulseId = current_pulseId_dsets[x][current_pos[x][1]][0]
        if (trainId < trainId_min) or (trainId == trainId_min and pulseId < pulseId_min):
            trainId_min = trainId
            pulseId_min = pulseId
    # do alignment
    for x in range(16):
        if reach_end_flag[x]: continue
        trainId = current_trainId_dsets[x][current_pos[x][1]][0]
        pulseId = current_pulseId_dsets[x][current_pos[x][1]][0]
        if trainId == trainId_min and pulseId == pulseId_min:
            alignment_index[x] = current_pos[x]
    # print alignment index
    if event_num % 1000 == 0:
        print(alignment_index)
        print(current_pos)
    # save alignment index
    dim1_len = align_idx_dset.shape[0]
    align_idx_dset.resize( (dim1_len + 1, 16, 2) )
    align_idx_dset[-1, :, :] = alignment_index
    # advance aligned module
    for x in range(16):
        if alignment_index[x][0] < 0: continue
        current_pos[x][1] += 1
        if current_pos[x][1] >= current_len[x]:
            current_files[x].close()
            current_pos[x][0] += 1
            current_pos[x][1] = 0
            if current_pos[x][0] >= 3:
                current_files[x] = None
                reach_end_flag[x] = True
            else:
                print('opening file: ' + file_paths[x][current_pos[x][0]])
                current_files[x] = h5py.File(file_paths[x][current_pos[x][0]], 'r')
                current_trainId_dsets[x] = get_image_dset(current_files[x], det_path, 'trainId')
                current_pulseId_dsets[x] = get_image_dset(current_files[x], det_path, 'pulseId')
                current_len[x] = current_pulseId_dsets[x].shape[0]

h5file_out.flush()
h5file_out.close()