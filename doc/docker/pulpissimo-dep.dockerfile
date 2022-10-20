FROM tools

RUN git clone --recursive https://github.com/VP-Vibes/Pulpissimo-VP/

# get required conan packages for Pulpissimo-VP
WORKDIR /Pulpissimo-VP/
RUN mkdir build
WORKDIR /Pulpissimo-VP/build/
RUN cmake -DCMAKE_BUILD_TYPE=Release ..
WORKDIR /

# cross-compiler for Pulpissimo SoC
# gcc
RUN git clone --recursive https://github.com/pulp-platform/pulp-riscv-gnu-toolchain
RUN apt install -y autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev

WORKDIR /pulp-riscv-gnu-toolchain/
RUN export PATH=$PATH:/opt/riscv/
RUN ./configure --prefix=/opt/riscv/ --with-arch=rv32imc --with-cmodel=medlow 
RUN make -j4
