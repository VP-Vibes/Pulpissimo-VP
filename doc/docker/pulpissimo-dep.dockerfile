FROM tools AS pulp-gcc

# cross-compiler for Pulpissimo SoC
# gcc
RUN git clone --recursive https://github.com/pulp-platform/pulp-riscv-gnu-toolchain
# TODO: below takes about 2.5 GB of space??
RUN apt install -y autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev

WORKDIR /pulp-riscv-gnu-toolchain/
ENV PATH="$PATH:/opt/riscv/bin/"
RUN ./configure --prefix=/opt/riscv/ --with-arch=rv32imc --with-cmodel=medlow
RUN make -j4
WORKDIR /


FROM tools AS pulpissimo-dep
COPY --from=pulp-gcc /opt/riscv/ /opt/riscv/
ENV PULP_CC=/opt/riscv/bin/riscv32-unknown-elf-gcc
RUN git clone --recursive https://github.com/VP-Vibes/Pulpissimo-VP/

# runtime for fw dev
WORKDIR /Pulpissimo-VP/fw/custom-pulp-sdk-master
RUN make all
WORKDIR /Pulpissimo-VP/
# for getting required conan packages
RUN mkdir build
WORKDIR /Pulpissimo-VP/build/
RUN cmake -DCMAKE_BUILD_TYPE=Release ..
RUN make -j4 tgc-vp
WORKDIR /


FROM tools
COPY --from=pulpissimo-dep /opt/riscv/ /opt/riscv/
COPY --from=pulpissimo-dep /root/.conan/ /root/.conan/
COPY --from=pulpissimo-dep /Pulpissimo-VP/fw/custom-pulp-sdk-master /custom-pulp-sdk/
