#!/bin/bash

# Runs demo for this filter program on an image of Donald Trump
# Requires ImageMagick

# Test effects of alpha
li_alpha="0.0 0.3 0.6 0.9"
alpha_out_files=""
for alpha in ${li_alpha}; do
    alpha_out_file=data/trump_unsharp_alpha_${alpha}.jpg
    build/app_unsharp \
        --convolution=spectral \
        --alpha=$alpha \
        --sigma_s=0.4 \
        -G 21 \
        -I data/trump.jpg \
        -O $alpha_out_file
    alpha_out_files="${alpha_out_files} ${alpha_out_file}"
done
echo $alpha_out_files
montage ${alpha_out_files} -geometry +0+0 -tile $(echo ${li_alpha} | wc -w)x1 data/trump_unsharp_alpha_tile.jpg

# Test effects of size
li_size="3 11 21 31"
size_out_files=""
for size in ${li_size}; do
    size_out_file=data/trump_unsharp_size_${size}.jpg
    build/app_unsharp \
        --convolution=spectral \
        --alpha=0.6 \
        --sigma_s=0.4 \
        -G $size \
        -I data/trump.jpg \
        -O $size_out_file
    size_out_files="${size_out_files} ${size_out_file}"
done
echo $size_out_files
montage ${size_out_files} -geometry +0+0 -tile $(echo ${li_size} | wc -w)x1 data/trump_unsharp_size_tile.jpg

# Test effects of different sigma
li_sigma_s="0.1 0.4 0.7 1.0"
li_sigma_r="0.1 0.4 0.7 1.0"
sigma_output_files=""
for sigma_s in ${li_sigma_s}; do
    for sigma_r in ${li_sigma_r}; do
        sigma_out_file=data/trump_bilateral_sigma_${sigma_s}_${sigma_r}.jpg
        build/app_bilateral \
            --sigma_s=$sigma_s \
            --sigma_r=$sigma_r \
            -G 11 \
            -I data/trump.jpg \
            -O $sigma_out_file
        sigma_out_files="${sigma_out_files} ${sigma_out_file}"
    done
done
echo $sigma_out_files
montage ${sigma_out_files} -geometry +0+0 -tile $(echo ${li_sigma_r} | wc -w)x$(echo ${li_sigma_s} | wc -w) data/trump_bilateral_sigma_tile.jpg

# Test running time of convolution

function mtime(){
    local ts=$(date +%s%N) ; $@ ; local tt=$((($(date +%s%N) - $ts)/1000000)) ; echo "$tt"
}

li_convolution="spectral spatial"

for size in ${li_size}; do
    for convolution in ${li_convolution}; do
        t=$(mtime build/app_unsharp \
            --convolution=$convolution \
            --alpha=$alpha \
            --sigma_s=0.4 \
            -G $size \
            -I data/trump.jpg \
            -O /tmp/wow.jpg)
        echo "$size / $convolution : $t"
    done
done

