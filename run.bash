#!/bin/bash

sigma=0.4
size=41

build/main.out --convolution=spectral --sigma_s=$sigma -G $size -I build/test.jpg -O build/test_spectral_out.jpg
build/main.out --convolution=spatial --sigma_s=$sigma -G $size -I build/test.jpg -O build/test_spatial_out.jpg --kernel_output=build/kernel.jpg
