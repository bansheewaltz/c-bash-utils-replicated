#!/bin/bash

docker rmi -f ubuntu
cd ../../ && docker build -t ubuntu -f ./materials/build/custom_docker/Dockerfile.ubnt .
docker run --rm -it ubuntu
