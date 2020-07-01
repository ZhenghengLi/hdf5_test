#!/usr/bin/env python3

import h5py
import numpy as np
import matplotlib.pyplot as plt
import sys
import os

from common_func import get_image_dset, compose_image

if len(sys.argv) < 3:
    print("usage: ", sys.argv[0], '<align_idx_file.h5>', '<event_num>')
    exit(2)

align_idx_filename = sys.argv[1]
event_num = int(sys.argv[2])

if event_num < 0:
    print("event_num < 0")
    exit(1)

align_idx_h5file = h5py.File(align_idx_filename, 'r')
align_idx_dset = align_idx_h5file['alignment_index']

if event_num >= align_idx_dset.shape[0]:
    print("event_num is too large.")
    exit(1)

index_mat = align_idx_dset[event_num]

det_path = '/INSTRUMENT/SPB_DET_AGIPD1M-1/DET'
file_paths = [
    ['/DATA_HDD/mark/CXIDB/id83/r0243/raw/RAW-R0243-AGIPD%02d-S%05d.h5' % (x, y)
    for y in range(3)] for x in range(16) ]

image_h5files = [h5py.File(file_paths[x][index_mat[x][0]], 'r') for x in range(16)]
image_dsets = [get_image_dset(x, det_path, 'data') for x in image_h5files]

image_data_1 = np.empty( (16, 512, 128) )
image_data_2 = np.empty( (16, 512, 128) )

for x in range(16):
    with image_dsets[x].astype('float64'):
        image_data_1[x] = image_dsets[x][index_mat[x][1]][0]
        image_data_2[x] = image_dsets[x][index_mat[x][1]][1]

image_size = (1300, 1300)

offset_1 = 25
offset_2 = 5

quad_offset = [
    (-offset_1, offset_2),
    (offset_2, offset_1),
    (offset_1, -offset_2),
    (-offset_2, -offset_1)
]

mod_gap = 30

full_image_1 = compose_image(image_data_1, image_size, quad_offset, mod_gap)
full_image_2 = compose_image(image_data_2, image_size, quad_offset, mod_gap)

pos = plt.imshow(full_image_1, cmap = "rainbow", vmin = 3000, vmax = 10000)
plt.colorbar(pos)

plt.show()
