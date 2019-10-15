# Simple filtering application

This project builds two targets:

## `app_unsharp`

Applies unsharp masking to an image

### Usage
```
Unsharp masking:
  -H [ --help ]                       print this message
  -A [ --alpha ] arg (=0.5)           high -> very sharp. low -> not so sharp
  -S [ --sigma_s ] arg (=1)           stdev for Gaussian filter
  -C [ --convolution ] arg (=spatial) Convolution implementation to use
  -G [ --g_size ] arg (=21)           width,height for Gaussian filter
  -I [ --input ] arg                  Input image path
  -O [ --output ] arg                 Output image path
  --kernel_output arg                 kernel image output path

```

## `app_bilateral`

Applies bilateral filtering to an image

### Usage
```
Bilateral Gaussian filtering:
  -H [ --help ]             print this message
  -S [ --sigma_s ] arg (=1) stdev for Gaussian filter
  -S [ --sigma_r ] arg (=1) stdev for bilateral filter
  -G [ --g_size ] arg (=21) width,height for Gaussian filter
  -I [ --input ] arg        Input image path
  -O [ --output ] arg       Output image path
```

## Build

### Dependencies

* OpenCV 4.1.1 
* Boost 1.71
* CMake

```bash
mkdir build && cd build
cmake ../ && make
```

