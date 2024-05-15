![cmake workflow](https://github.com/carsten0x51h/libstarmathpp/actions/workflows/cmake.yml/badge.svg)

libstarmathpp - A C++ library to process astronomical images based on CImg and range-v3.
===========

## Summary
TODO: This project is about ...


## Concept
TODO: The concept... combine image processing with ranges concept (functional programming / Java: streams)... -> Build intuitive, easy to understand processing pipelines...


## Usage examples
TODO: Add examples


## build

### Install required dependencies
The following libraries are required to build FoFi on Ubuntu 20.04 LTS.
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

The project follows the C++ coding style guide from Google: https://google.github.io/styleguide/cppguide.html
Formatting rules e.g. for Eclipse CDT can be found here: https://github.com/google/styleguide/blob/gh-pages/eclipse-cpp-google-style.xml


## Further information
Please visit my blog https://www.lost-infinity.com for further details.

