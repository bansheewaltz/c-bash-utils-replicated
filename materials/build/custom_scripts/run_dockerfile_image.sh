#!/bin/bash

image=$1
goal=$2
dockerfiles_dir=$3
CLR="\033[38;5;117m"
RST="\033[0m"

# docker rmi -f $image &>/dev/null
if ! pgrep -x "Docker" >/dev/null; then
  open -a Docker
  sleep 10
fi
if test -z "$image"; then
  image="ubuntu"
fi

cd ../..
docker build -t $image -f materials/build/custom_dockerfiles/Dockerfile.$image .

if [[ "$goal" == "test" ]]; then
  container_name=${image}_test
  prompt="$CLR\u@$container_name$RST:\W$ "
  command="echo \"export PS1='${prompt}'\" >> ~/.bashrc && make echo && bash"
  RM=--rm
else
  container_name=${image}_develop
  if [ "$(docker ps -a -q -f name=${container_name})" ]; then
    docker start -i ${container_name}
    exit 0
  else
    prompt="$CLR\u@$container_name$RST:\W$ "
    command="echo \"export PS1='${prompt}'\" >> ~/.bashrc && bash"
  fi
fi

docker run -it \
  $RM \
  --name "$container_name" \
  -e PS1="$prompt" \
  -v $PWD:/usr/project \
  -w /usr/project/src/$utility \
  $image \
  bash -c "$command"
