#!/bin/bash

image=$1
dockerfiles_dir=$2
container_name=${image}_container
echo $container_name
docker rmi -f $image &>/dev/null

cd ../..
docker build -t $image -f materials/build/custom_dockerfiles/Dockerfile.$image .
docker run -it --rm --name "$container_name" -e PS1="\033[1;34m\u@$container_name\033[0m:\W$ " $image
