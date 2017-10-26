#!/bin/bash

BASE_DIR=$1
SOURCE_DIR=$2

TEST_DIR=$(dirname $(readlink -f $0))

#$TEST_DIR/t1.1/run.sh $BASE_DIR $SOURCE_DIR
#if [ $? -ne 0 ]; then
#  exit 1
#fi

$TEST_DIR/t1.2/run.sh $BASE_DIR $SOURCE_DIR
if [ $? -ne 0 ]; then
  exit 1
fi

rm exact fdtd3d

exit 0
