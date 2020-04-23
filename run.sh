#! /usr/bin/env bash

DIR=$(cd $( dirname "${BASH_SOURCE[0]}" ) && pwd )
echo "DIR = ${DIR}"

function build(){
  bazel query //... | xargs bazel build --compilation_mode=dbg --cxxopt=-g  --cxxopt=-O1 
}

function bazel_test(){
  bazel test //...
}


function main(){
  local cmd=$1
  case $cmd in
    test)
      bazel_test
      ;;
    build)
      build
      ;;
    *)
      build
      ;;
  esac
}

main $@
