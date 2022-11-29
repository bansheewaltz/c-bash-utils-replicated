#!/bin/bash

docker rmi -f valgrind
cd ../../ && docker build -t valgrind -f ./materials/build/custom_docker/Dockerfile.ubnt .
docker run --rm -it valgrind
