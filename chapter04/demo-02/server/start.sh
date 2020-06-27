#!/bin/bash

nohup ../build64_release/server/src/echo_server --flagfile=./conf/service.conf > /dev/null 2>&1 &
