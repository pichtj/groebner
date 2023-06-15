FROM ubuntu:jammy

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get -y upgrade && apt-get -y install vim make gcc g++ git gdb m4 unzip libboost-dev ffmpeg zlib1g-dev yasm wget bzip2

VOLUME ["/groebner"]

WORKDIR /groebner
