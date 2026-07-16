#!/bin/bash
NAME_DOCKER_IMAGE="lumvia-adminapp-build"
NAME_FILE_OUTPUT="index.h"
REPO_ROOT=$(git rev-parse --show-toplevel)
APP_DIR="$REPO_ROOT/configApp"
OUTPUT_DIR="$REPO_ROOT/include/webserver"
COMPILER_DIR="$REPO_ROOT/tools/web-compiler"
SCRIPTS_DIR="$REPO_ROOT/tools/web-compiler/scripts"


DOCKER_INPUT_DIR=/input
DOCKER_OUTPUT_DIR=/output
DOCKER_SCRIPTS_DIR=/scripts


docker build -t "$NAME_DOCKER_IMAGE" -f "$COMPILER_DIR/Dockerfile" "$APP_DIR"

docker run --rm \
-u "$(id -u):$(id -g)" \
-v "$APP_DIR:$DOCKER_INPUT_DIR" \
-v "$OUTPUT_DIR:$DOCKER_OUTPUT_DIR" \
-v "$SCRIPTS_DIR/generate_header.py:$DOCKER_SCRIPTS_DIR/script.py" \
-e "OUTPUT_FILE_NAME=$NAME_FILE_OUTPUT" \
"$NAME_DOCKER_IMAGE"



