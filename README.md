# voxl-docker-opencv-opencl

Example of how to use OpenCV with OpenCL support in a 64-bit Docker container running on target.

This project creates a Docker image that can be used for OpenCV development on target. The OpenCV
libraries contain OpenCL support for offloading certain operations to the GPU.

## Prerequisites

Before starting make sure that Docker is installed and running properly on target. See
the [Docker development user guide](https://docs.modalai.com/docker-on-voxl/) for more information.

ADB is required for the build scripts to function properly. [adb setup instructions](https://docs.modalai.com/setup-adb/).

This also requires some EULA protected proprietary binary libraries from Qualcomm. These
can be accessed via the ModalAI [Developer Dashboard](https://developer.modalai.com/)
Protected Downloads page. Look for the 64-bit OpenCL Package named: modalai-opencl-dev_1.0-1.deb.
Download the package into the project directory before starting.

## Prepare environment and target

Most of the setup work happens on target. To set up the target with the correct files
run the script `./deploy.sh`

## Create Docker image on target

Note: All commands must be run in the bash shell

To build OpenCV version 3.4.6:
```
# cd /data/docker/opencv-opencl
# scripts/build-docker.sh 3.4.6
```

To build OpenCV version 4.2.0:
```
# cd /data/docker/opencv-opencl
# scripts/build-docker.sh 4.2.0
```
## Test on target

Run the docker:
```
# docker run -it --rm --privileged \
> -v /data/docker/opencv-opencl:/opt/workspace \
> opencv-opencl:opencv
```

In the docker:
```
# cd /opt/workspace/examples/fast_corners/
# cmake .
# make
# ./fast-corners
```

Expected fast-corners output:
> root@0a4f66eee0a6:/opt/workspace/examples/fast_corners# ./fast-corners
>
> FAST ( 300 x  168), keypoints: 557, time:0.487192 cpu
>
> [ WARN:0] Using world accessible cache directory. This may be not secure: /var/tmp/
>
> FAST ( 300 x  168), keypoints: 557, time:6.319238 gpu
