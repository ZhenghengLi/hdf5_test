#!/usr/bin/env python3

import os

def get_image_dset(h5file, det_path, name):
    ch_name = list(h5file[det_path].keys())[0]
    return h5file[os.path.join(det_path, ch_name, 'image', name)]
