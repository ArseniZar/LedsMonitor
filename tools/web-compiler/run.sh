#!/bin/bash
NAME_DOCKER_IMAGE="lumvia-adminapp-build"
NAME_FILE_OUTPUT="index.h"
REPO_ROOT=$(git rev-parse --show-toplevel)
FRONTEND_DIR="$REPO_ROOT/configApp/"
BACKEND_DIR="$REPO_ROOT/include/webserver/"
AUTOMATION_DIR="$REPO_ROOT/tools/web-compiler/"

DOCKER_DIR1=/src_app
DOCKER_DIR2=/include_out
DOCKER_DIR3=/automation
DOCKER_VAR=FILE_NAME

docker build -t "$NAME_DOCKER_IMAGE" -f "$AUTOMATION_DIR/Dockerfile" "$FRONTEND_DIR"

docker run --rm \
  -e "$DOCKER_VAR=$NAME_FILE_OUTPUT" \
  -v "$FRONTEND_DIR:$DOCKER_DIR1" \
  -v "$BACKEND_DIR:$DOCKER_DIR2" \
  -v "$AUTOMATION_DIR:$DOCKER_DIR3" \
  "$NAME_DOCKER_IMAGE"
  


