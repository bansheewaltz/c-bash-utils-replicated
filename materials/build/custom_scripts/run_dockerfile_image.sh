#!/bin/bash

image=$1
goal=$2
dockerfiles_dir=$3
docker rmi -f $image &>/dev/null

cd ../..
docker build -t $image -f materials/build/custom_dockerfiles/Dockerfile.$image .
if [[ "$goal" == "test" ]]; then
  container_name=${image}_testing
  prompt="\033[1;34m\u@$container_name\033[0m:\W$ "
  docker run -it --name "$container_name" -e PS1="$prompt" --rm $image
else
  container_name=${image}_develop
  prompt="\033[1;34m\u@$container_name\033[0m:\W$ "
  docker run -it --name "$container_name" -e PS1="$prompt" -v $PWD:/usr/project -w /usr/project/src/cat $image
fi
