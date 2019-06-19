#!/bin/bash

if [ -z "$1" ]; then
  echo "usage: $0 <install_prefix>"
  exit 1
fi

install_prefix="$1"

set -xe

root_source_dir="`pwd`"
build_dir="`mktemp -d`"

cd "$build_dir"
cmake "-DCMAKE_INSTALL_PREFIX=$install_prefix" "$root_source_dir"
make
make install
