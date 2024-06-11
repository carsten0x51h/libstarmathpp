![cmake workflow](https://github.com/carsten0x51h/libstarmathpp/actions/workflows/cmake.yml/badge.svg)

libstarmathpp - A C++ library to process astronomical images based on CImg and range-v3.
===========

## Summary
This project combines the power of the C++ CImg template library (https://cimg.eu/) (which is used
to process images) with the famous range-v3 library from Eric Niebler (https://github.com/ericniebler/range-v3)
to simplify the programatical processing of astronomical images by allowing the construction of intuitive and
easy to understand image processing pipelines.

## Usage examples

### Calculation of star metrics
In this example all files with extension "*.fit.gz" from directory "my/image/directory" are processed by the pipeline. The processing takes place one by one. Each image is opened and pushed through the processing pipeline. For each image the pipeline suntracts the background, scales the image, performs a centroiding algorithm, again scales down the image, crops the image from the center toa dimension of 61x61 pixels and then calculates the SNR (Signal-to-noise ratio), the HFD (Half-flux diameter) and the FWHM (Full-width half maximum) for each star.

TODO/IDEA: Introduce filter: "has_star" / "has_single_star"...

#### Input images
TODO:...

#### Code
```cpp

  ...
  
  auto star_metrics =
      | files("my/image/directory", "(.*\\.fit\\.gz)")
      | read()
      | subtract_background(OtsuThresholder<float>(16 /*image bit depth*/))
      | scale_up(3.0F /*scale up factor*/)
      | center_on_star(IntensityWeightedCentroider<float>())
      | scale_down(3.0F /*scale down factor*/)
      | crop_from_center(Size<int>(61,61))
      | ranges::view::transform(
            [](const auto & img_ptr) {
                return std::make_tuple(
                    starmathpp::algorithm::snr(*img_ptr),
                    starmathpp::algorithm::hfd(*img_ptr),
                    starmathpp::algorithm::fwhm(*img_ptr).value()
                );
            })
      | to<std::vector>();


      for (const auto &m : star_metrics) {
          std::cout << "SNR: " << std::get<0>(m)
                    << ", HFD: " << std::get<1>(m)
                    << ", FWHM: " << std::get<2>(m)
                    << std::endl;
      }
	  
	  ...
```

#### Output
```bash
SNR: 3.42481, HFD: 27.2951, FWHM: 7.02249
SNR: 4.01684, HFD: 25.4318, FWHM: 3.2577
SNR: 4.23981, HFD: 23.907, FWHM: 4.95197
SNR: 4.51064, HFD: 21.7979, FWHM: 3.26535
SNR: 5.93667, HFD: 18.6469, FWHM: 5.11132
SNR: 5.92148, HFD: 15.9004, FWHM: 9.73768
SNR: 7.20587, HFD: 12.0811, FWHM: 3.72651
SNR: 6.72696, HFD: 9.18208, FWHM: 4.97865
SNR: 7.42427, HFD: 6.29746, FWHM: 3.84221
SNR: 5.27879, HFD: 3.84385, FWHM: 2.46015
SNR: 4.14073, HFD: 3.44348, FWHM: 1.85491
```


TODO: Add further examples




## build

### Install required dependencies
The following libraries are required to build libstarmathpp on Ubuntu 20.04 LTS or Ubuntu 22.04.
For other Linux distributions the package names may slightly vary.

	sudo apt-get update
	sudo apt-get install zlib1g-dev cimg-dev libgsl-dev libccfits-dev libboost-dev \
	                     libboost-log-dev libboost-regex-dev libboost-thread-dev \
						 libboost-test-dev clang-tidy

### Checkout repository

	git clone https://github.com/carsten0x51h/libstarmath.git


### Configure the build
To build from command line, simply do the following steps:

	cd libstarmathpp
	mkdir build
	cd build
	cmake ..

This will generate the build environment for your operating system. It will fail
if at least one dependency to an external library could not be resolved.

#### Enable X11 display for debug

	cmake .. -DDEBUG_IMAGE_DISPLAY_SWITCH=1


### Build the code
Run the following command to build the project: 

	cmake --build . -- all

or to build parallel on e.g. 12 cores, run

	cmake --build . -j12 -- all

### Run the unit tests

	cmake --build . -- test


### Package / Install
TODO...



## Project structure
The project structure follows the approach presented by Boris Kolpackov: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1204r0.html
Also see https://stackoverflow.com/questions/2360734/whats-a-good-directory-structure-for-larger-c-projects-using-makefile

The project mostly follows the C++ coding style guide from Google: https://google.github.io/styleguide/cppguide.html
Formatting rules e.g. for Eclipse CDT can be found here: https://github.com/google/styleguide/blob/gh-pages/eclipse-cpp-google-style.xml


## Further information
Please visit my blog https://www.lost-infinity.com for further details.

