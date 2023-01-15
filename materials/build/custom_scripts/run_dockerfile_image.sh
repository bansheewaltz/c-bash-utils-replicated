#!/bin/bash

image=$1
goal=$2
dockerfiles_dir=$3
CLR="\033[38;5;117m"
RST="\033[0m"

# docker rmi -f $image &>/dev/null
# starting docker if is not running yet
if ! pgrep -x "Docker" >/dev/null; then
  echo "Starting Docker app..."
  open -a Docker
  sleep 10
fi
# setting default image if another is not specified
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
  --cap-add=SYS_PTRACE \
  --security-opt seccomp=unconfined \
  --name "$container_name" \
  -e PS1="$prompt" \
  -v $PWD:/usr/project \
  -w /usr/project/src/$utility \
  $image \
  bash -c "$command"
