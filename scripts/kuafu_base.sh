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

KUAFU_ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )"

BOLD='\033[1m'
RED='\033[0;31m'
GREEN='\033[32m'
WHITE='\033[34m'
YELLOW='\033[33m'
NO_COLOR='\033[0m'

function info() {
  (>&2 echo -e "[${WHITE}${BOLD}INFO${NO_COLOR}] $*")
}

function error() {
  (>&2 echo -e "[${RED}ERROR${NO_COLOR}] $*")
}

function warning() {
  (>&2 echo -e "${YELLOW}[WARNING] $*${NO_COLOR}")
}

function ok() {
  (>&2 echo -e "[${GREEN}${BOLD} OK ${NO_COLOR}] $*")
}

function print_delim() {
  echo '============================'
}

function get_now() {
  echo $(date +%s)
}

function print_time() {
  END_TIME=$(get_now)
  ELAPSED_TIME=$(echo "$END_TIME - $START_TIME" | bc -l)
  MESSAGE="Took ${ELAPSED_TIME} seconds"
  info "${MESSAGE}"
}

function success() {
  print_delim
  ok "$1"
  print_time
  print_delim
}

function fail() {
  print_delim
  error "$1"
  print_time
  print_delim
  exit -1
}

function check_in_docker() {
  if [ -f /.dockerenv ]; then
    KUAFU_IN_DOCKER=true
  else
    KUAFU_IN_DOCKER=false
  fi
  export KUAFU_IN_DOCKER
}

function set_lib_path() {
  export LD_LIBRARY_PATH=/usr/lib:/usr/lib/x86_64-linux-gnu

  local CYBER_SETUP="/apollo/cyber/setup.bash"
  if [ -e "${CYBER_SETUP}" ]; then
    source "${CYBER_SETUP}"
  fi
  PY_LIB_PATH=${KUAFU_ROOT_DIR}/py_proto
  PY_TOOLS_PATH=${KUAFU_ROOT_DIR}/modules/tools
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/Kuafu/lib:/apollo/bazel-genfiles/external/caffe/lib
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/Kuafu/lib:/usr/local/apollo/local_integ/lib
  export LD_LIBRARY_PATH=/usr/local/Qt5.12.2/5.12.2/gcc_64/lib:$LD_LIBRARY_PATH
  export LD_LIBRARY_PATH=/usr/local/kuafu/boost/lib:$LD_LIBRARY_PATH
  export LD_LIBRARY_PATH=/usr/local/kuafu/paddlepaddle_dep/mkldnn/lib/:$LD_LIBRARY_PATH
  export PYTHONPATH=${PY_LIB_PATH}:${PY_TOOLS_PATH}:${PYTHONPATH}
}

function create_data_dir() {
  local DATA_DIR="${HOME}/data"

  mkdir -p "${DATA_DIR}/log"
  mkdir -p "${DATA_DIR}/bag"
  mkdir -p "${DATA_DIR}/core"
}

function determine_bin_prefix() {
  KUAFU_ROOT_DIR_BIN_PREFIX=$KUAFU_ROOT_DIR
  if [ -e "${KUAFU_ROOT_DIR}/bazel-bin" ]; then
    KUAFU_BIN_PREFIX="${KUAFU_ROOT_DIR}/bazel-bin"
  fi
  export KUAFU_BIN_PREFIX
}

function find_device() {
  # ${1} = device pattern
  local device_list=$(find /dev -name "${1}")
  if [ -z "${device_list}" ]; then
    warning "Failed to find device with pattern \"${1}\" ..."
  else
    local devices=""
    for device in $(find /dev -name "${1}"); do
      ok "Found device: ${device}."
      devices="${devices} --device ${device}:${device}"
    done
    echo "${devices}"
  fi
}

function setup_device() {
  if [ $(uname -s) != "Linux" ]; then
    echo "Not on Linux, skip mapping devices."
    return
  fi
}



# Note: This 'help' function here will overwrite the bash builtin command 'help'.
# TODO: add a command to query known modules.
function help() {
cat <<EOF
Invoke ". scripts/apollo_base.sh" within docker to add the following commands to the environment:
Usage: COMMAND [<module_name>]

COMMANDS:
  help:      show this help message
  start:     start the module in background
  start_fe:  start the module without putting in background
  start_gdb: start the module with gdb
  stop:      stop the module
EOF
}



