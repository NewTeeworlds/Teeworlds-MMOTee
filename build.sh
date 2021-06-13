#!/bin/bash

# usage ./build.sh 

BASEDIR=$(dirname "$0")
cd $BASEDIR && docker build -t mmotee_build . && docker run --rm --name mmotee_build -v $PWD:/mmotee_cn --privileged mmotee_build sh -c "apt install wget unzip -y && wget https://github.com/matricks/bam/archive/refs/tags/v0.4.0.zip && unzip v0.4.0.zip && cd bam-0.4.0 && ./make_unix.sh&&cd /mmotee_cn && /bam-0.4.0/bam config && /bam-0.4.0/bam server_sql_release && /bam-0.4.0/bam server_sql_debug"
