FROM ubuntu:19.04

RUN apt-get update
RUN apt-get install -qq -y --no-install-recommends \
            cmake \
            g++ \
            make \
            python3 \
            libboost-dev \
            libboost-python1.67-dev \
            libboost-numpy1.67-dev \
            qt5-default \
            libqt5opengl5-dev \
            libqt5designer5 \
            qttools5-dev \
            xvfb
            
ENTRYPOINT ["/bin/bash"]
