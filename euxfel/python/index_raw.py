#!/usr/bin/env python3

import h5py
import numpy as np
import os

from common_func import get_image_dset

det_path = '/INSTRUMENT/SPB_DET_AGIPD1M-1/DET'
file_paths = [
    ['/DATA_HDD/mark/CXIDB/id83/r0243/raw/RAW-R0243-AGIPD%02d-S%05d.h5' % (x, y) for y in range(3)] for x in range(16) ]


