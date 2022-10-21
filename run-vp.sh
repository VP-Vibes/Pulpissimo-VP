#!/bin/sh

# if you want to compile VP first time
#mkdir -p build/
#cd build/
#cmake -DCMAKE_BUILD_TYPE=Release ..
#make -j4 tgc-vp

# if you want to compile fw
#cd ../fw/udma_spim_tests
#mkdir -p build/obj/
#make

# if build and run vp simulation
FW_TO_RUN=../fw/udma_spim_tests/build/test

cd build/
make -j4 tgc-vp
src/tgc-vp -f ${FW_TO_RUN}
