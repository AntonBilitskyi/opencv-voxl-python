#!/bin/bash

PACKAGE_FILE=modalai-opencl-dev_1.0-1.deb
if [ ! -f $PACKAGE_FILE ]; then
    echo "Error: Please download the package file $PACKAGE_FILE before proceeding."
    exit -1
fi

adb shell mkdir -p /data/docker/opencv-opencl
adb push $PACKAGE_FILE /data/docker/opencv-opencl
adb push Dockerfile.base /data/docker/opencv-opencl
adb push Dockerfile.opencv /data/docker/opencv-opencl
adb push Dockerfile.examples /data/docker/opencv-opencl

adb shell mkdir -p /data/docker/opencv-opencl/examples
adb push examples/ /data/docker/opencv-opencl/examples/

adb shell mkdir -p /data/docker/opencv-opencl/scripts
adb push scripts/ /data/docker/opencv-opencl/scripts
