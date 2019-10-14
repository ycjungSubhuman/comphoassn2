#!/bin/bash

sigma=0.5
size=3

build/app_unsharp --convolution=spectral --sigma_s=$sigma -G $size -I build/test.jpg -O build/test_spectral_out.jpg
build/app_unsharp --convolution=spatial --sigma_s=$sigma -G $size -I build/test.jpg -O build/test_spatial_out.jpg --kernel_output=build/kernel.jpg
