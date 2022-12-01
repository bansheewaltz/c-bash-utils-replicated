#!/bin/bash

docker rmi -f alpine
cd ../../ && docker build --network host -t alpine -f ./materials/build/custom_docker/Dockerfile.alpn .
docker run --rm -it alpine
