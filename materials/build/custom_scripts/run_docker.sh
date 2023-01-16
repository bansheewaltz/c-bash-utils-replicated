#!/bin/bash

# setting colors
CLR="\033[38;5;117m"
RST="\033[0m"

# starting docker if is not running yet
if ! pgrep -x "Docker" >/dev/null; then
  echo "Starting Docker app..."
  open -a Docker
  sleep 10
fi

# setting default image if another is not specified
if test -z "$image"; then
  os="ubuntu"
  image="dondarri/ubuntu_development"
fi

# setting command prompt message
if test -z "$goal"; then
  goal="dev"
fi
container_name="$os.$goal"
prompt="$CLR$container_name@container$RST:\W$ "

# different actions based on whether the container already exists and a goal
if [[ "$goal" == "test" ]]; then
  image="dondarri/ubuntu:test"
  command="echo \"export PS1='${prompt}'\" >> ~/.bashrc && make test && bash"
  RM=--rm
else
  if [ "$(docker ps -a -q -f name=$container_name)" ]; then
    docker start -i $container_name
    exit 0
  else
    command="echo \"export PS1='$prompt'\" >> ~/.bashrc && bash"
  fi
fi

#running image if the container does not exist
docker run -it \
  $RM \
  --cap-add=SYS_PTRACE \
  --security-opt seccomp=unconfined \
  --name "$container_name" \
  -e PS1="$prompt" \
  -v $PWD/../../:/usr/project \
  -w /usr/project/src/$utility \
  $image \
  bash -c "$command"
