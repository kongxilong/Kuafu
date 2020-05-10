#!/usr/bin/env bash

###############################################################################
# Copyright 2017 The Apollo Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###############################################################################

LOCAL_IMAGE="no"
FAST_BUILD_MODE="no"
FAST_TEST_MODE="no"
VERSION=""
ARCH=$(uname -m)
VERSION_X86_64="dev-18.04-x86_64-20200510_0922"
VERSION_OPT=""
NO_PULL_IMAGE=""


function show_usage()
{
cat <<EOF
Usage: $(basename $0) [options] ...
OPTIONS:
    -b, --fast-build       Light mode for building without pulling all the map volumes
    -f, --fast-test        Light mode for testing without pulling limited set of map volumes
    -h, --help             Display this help and exit.
    -t, --tag <version>    Specify which version of a docker image to pull.
    -l, --local            Use local docker image.
    -n,                    Do not pull docker image.
    stop                   Stop all running Apollo containers.
EOF
exit 0
}

function stop_containers()
{
running_containers=$(docker ps --format "{{.Names}}")

for i in ${running_containers[*]}
do
  if [[ "$i" =~ kuafu_* ]];then
    printf %-*s 70 "stopping container: $i ..."
    docker stop $i > /dev/null
    if [ $? -eq 0 ];then
      printf "\033[32m[DONE]\033[0m\n"
    else
      printf "\033[31m[FAILED]\033[0m\n"
    fi
  fi
done
}

KUAFU_ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../.." && pwd -P )"

if [ "$(readlink -f /Kuafu)" != "${KUAFU_ROOT_DIR}" ]; then
    sudo ln -snf ${KUAFU_ROOT_DIR} /Kuafu
fi

if [ -e /proc/sys/kernel ]; then
    echo "/Kuafu/data/core/core_%e.%p" | sudo tee /proc/sys/kernel/core_pattern > /dev/null
fi

source ${KUAFU_ROOT_DIR}/scripts/kuafu_base.sh

VOLUME_VERSION="latest"

while [ $# -gt 0 ]
do

    case "$1" in
    -image)
        echo -e "\033[093mWarning\033[0m: This option has been replaced by \"-t\" and \"--tag\", please use the new one.\n"
        show_usage
        ;;
    -t|--tag)
        VAR=$1
        [ -z $VERSION_OPT ] || echo -e "\033[093mWarning\033[0m: mixed option $VAR with $VERSION_OPT, only the last one will take effect.\n"
        shift
        VERSION_OPT=$1
        [ -z ${VERSION_OPT// /} ] && echo -e "Missing parameter for $VAR" && exit 2
        [[ $VERSION_OPT =~ ^-.* ]] && echo -e "Missing parameter for $VAR" && exit 2
        ;;
    dev-*) # keep backward compatibility, should be removed from further version.
        [ -z $VERSION_OPT ] || echo -e "\033[093mWarning\033[0m: mixed option $1 with -t/--tag, only the last one will take effect.\n"
        VERSION_OPT=$1
        echo -e "\033[93mWarning\033[0m: You are using an old style command line option which may be removed from"
        echo -e "further versoin, please use -t <version> instead.\n"
        ;;
    -b|--fast-build)
        FAST_BUILD_MODE="yes"
        ;;
    -f|--fast-test)
        FAST_TEST_MODE="yes"
        ;;
    -h|--help)
        show_usage
        ;;
    -l|--local)
        LOCAL_IMAGE="yes"
        ;;
    -n)
        NO_PULL_IMAGE="yes"
        info "running without pulling docker image"
        ;;
    stop)
	stop_containers
	exit 0
	;;
    *)
        echo -e "\033[93mWarning\033[0m: Unknown option: $1"
        exit 2
        ;;
    esac
    shift
done

VERSION=${VERSION_X86_64}

if [ -z "${DOCKER_REPO}" ]; then
    DOCKER_REPO=kuafu/kuafu
fi

KUAFU_DEV_IMAGE=${DOCKER_REPO}:$VERSION
LOCALIZATION_VOLUME_IMAGE=${DOCKER_REPO}:localization_volume-${ARCH}-latest
PADDLE_VOLUME_IMAGE=${DOCKER_REPO}:paddlepaddle_volume-${ARCH}-2.0.0
LOCAL_THIRD_PARTY_VOLUME_IMAGE=${DOCKER_REPO}:local_third_party_volume-${ARCH}-latest


function local_volumes() {
    set +x
    # Apollo root and bazel cache dirs are required.
    volumes="-v $KUAFU_ROOT_DIR:/Kuafu \
             -v $HOME/.vim:${DOCKER_HOME}/.vim \
             -v $HOME/.vimrc:${DOCKER_HOME}/.vimrc \
             -v $HOME/.zshrc:${DOCKER_HOME}/.zshrc \
             -v $HOME/.oh-my-zsh:${DOCKER_HOME}/.oh-my-zsh \
             -v $HOME/.ssh:${DOCKER_HOME}/.ssh"
    case "$(uname -s)" in
        Linux)

            case "$(lsb_release -r | cut -f2)" in
                14.04)
                    volumes="${volumes} "
                    ;;
                *)
                    volumes="${volumes} -v /dev:/dev "
                    ;;
            esac
            volumes="${volumes} -v /media:/media \
                                -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
                                -v /etc/localtime:/etc/localtime:ro \
                                -v /usr/src:/usr/src \
                                -v /lib/modules:/lib/modules"
            ;;
        Darwin)
            # MacOS has strict limitations on mapping volumes.
            chmod -R a+wr ~/.cache/bazel
            ;;
    esac
    echo "${volumes}"
}

## customized docker cmd
function do_docker_image_inspect()
{
    docker image inspect -f {{.Config.Image}} $1 &> /dev/null
    if [ $? -ne 0 ];then
        error "Failed to find local docker image : $1"
        exit 1
    fi
}

function do_docker_pull()
{
    IMG=$1
    if [ "$NO_PULL_IMAGE" = "yes" ];then
        echo "Skipping pull docker image for $IMG"
        # check for local existence if we skip
        do_docker_image_inspect $IMG
    else
        info "Start pulling docker image $IMG ..."
        docker pull $IMG
        if [ $? -ne 0 ];then
            error "Failed to pull docker image : $IMG"
            exit 1
        fi
    fi
}
function main(){
    if [ "$LOCAL_IMAGE" = "yes" ];then
        info "Start docker container based on local image : $KUAFU_DEV_IMAGE"
    else
        do_docker_pull $KUAFU_DEV_IMAGE
        if [ $? -ne 0 ];then
            error "Failed to pull docker image."
            exit 1
        fi
    fi

    KUAFU_DEV="kuafu_dev_${USER}"
    docker ps -a --format "{{.Names}}" | grep "$KUAFU_DEV" 1>/dev/null
    if [ $? == 0 ]; then
        if [[ "$(docker inspect --format='{{.Config.Image}}' $KUAFU_DEV 2> /dev/null)" != "$KUAFU_DEV_IMAGE" ]]; then
            rm -rf $KUAFU_ROOT_DIR/bazel-*
            rm -rf $HOME/.cache/bazel/*
        fi
        docker stop $KUAFU_DEV 1>/dev/null
        docker rm -v -f $KUAFU_DEV 1>/dev/null
    fi


    local display=""
    if [[ -z ${DISPLAY} ]];then
        display=":0"
    else
        display="${DISPLAY}"
    fi


    USER_ID=$(id -u)
    GRP=$(id -g -n)
    GRP_ID=$(id -g)
    LOCAL_HOST=`hostname`
    DOCKER_HOME="/home/$USER"
    if [ "$USER" == "root" ];then
        DOCKER_HOME="/root"
    fi
    if [ ! -d "$HOME/.cache" ];then
        mkdir "$HOME/.cache"
    fi

    info "Starting docker container \"${KUAFU_DEV}\" ..."

    DOCKER_RUN="docker run"

    set -x

    ${DOCKER_RUN} -it \
        -d \
        --privileged \
        --name $KUAFU_DEV \
        -e DISPLAY=$display \
        -e DOCKER_USER=$USER \
        -e USER=$USER \
        -e DOCKER_USER_ID=$USER_ID \
        -e DOCKER_GRP="$GRP" \
        -e DOCKER_GRP_ID=$GRP_ID \
        -e DOCKER_IMG=$KUAFU_DEV_IMAGE \
        $(local_volumes) \
        --net host \
        -w /Kuafu \
        --add-host in_dev_docker:127.0.0.1 \
        --add-host ${LOCAL_HOST}:127.0.0.1 \
        --hostname in_dev_docker \
        --shm-size 2G \
        --pid=host \
        -v /dev/null:/dev/raw1394 \
        $KUAFU_DEV_IMAGE \
        /bin/bash
    if [ $? -ne 0 ];then
        error "Failed to start docker container \"${KUAFU_DEV}\" based on image: $KUAFU_DEV_IMAGE"
        exit 1
    fi
    set +x

    docker exec -u root $KUAFU_DEV bash -c '/Kuafu/scripts/docker_adduser.sh'

    ok "Finished setting up Kuafu docker environment. Now you can enter with: \nbash docker/scripts/dev_into.sh"
    ok "Enjoy!"
}

main
