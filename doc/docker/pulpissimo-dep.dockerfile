FROM tools

RUN git clone --recursive https://github.com/VP-Vibes/Pulpissimo-VP

WORKDIR /Pulpissimo-VP/
RUN git pull
RUN mkdir build

WORKDIR /Pulpissimo-VP/build/
RUN cmake -DCMAKE_BUILD_TYPE=Release ..
WORKDIR /
