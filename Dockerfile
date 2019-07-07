ARG BASE_IMAGE
FROM ${BASE_IMAGE}
MAINTAINER Cangjians (https://cangjians.github.io)

# basic environment for building
WORKDIR /usr/local/src/libcangjie

# copy source files to build
COPY "." "./"

# build the library
RUN ./autogen.sh --prefix=/usr && \
  make && make install
