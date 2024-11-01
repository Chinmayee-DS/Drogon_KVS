FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libjsoncpp-dev \
    uuid-dev \
    zlib1g-dev \
    openssl \
    libssl-dev \
    curl

RUN git clone https://github.com/drogonframework/drogon
WORKDIR /drogon
RUN git submodule update --init
RUN mkdir build && cd build && cmake .. && make && make install

WORKDIR /app
COPY . .

EXPOSE 8080

RUN mkdir build && cd build && cmake .. && make

CMD ["./build/sdhwo"]
