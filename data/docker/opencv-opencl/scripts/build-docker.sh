#!/bin/bash

print_help () {
    echo "Usage: build-docker.sh [ 3.4.6 | 4.2.0 ]"
}

if [ ! $# -eq 1 ]; then
    print_help
    exit -1
fi

case "$1" in
    "3.4.6") :
        ;;
    "4.2.0") :
        ;;
    *) echo "Bad argument: $1"
        print_help
        exit -2
        ;;
esac

OPENCV_VERSION=$1

echo "Building OpenCV $OPENCV_VERSION"

docker build -t opencv-opencl:base -f Dockerfile.base .

docker build -t opencv-opencl:opencv \
             --build-arg opencv_version=$OPENCV_VERSION \
             -f Dockerfile.opencv .

# docker build -t opencv-opencl:v1.0 \
#              --build-arg opencv_version=$OPENCV_VERSION \
#              -f Dockerfile.examples .
