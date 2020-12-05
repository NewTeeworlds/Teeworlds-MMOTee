#!/bin/bash

# usage ./build.sh <bam_path>
# bam need be compiled

BASEDIR=$(dirname "$0")
cd $BASEDIR && docker build -t mmotee_build . && docker run --rm --name mmotee_build -v $PWD:/mmotee_cn -v $(realpath $1):/bam --privileged mmotee_build sh -c "cd /mmotee_cn && ../bam/bam config && ../bam/bam server_sql_release && ../bam/bam server_sql_debug"
