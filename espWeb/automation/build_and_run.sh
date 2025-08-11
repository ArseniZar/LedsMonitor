#!/bin/bash

NAME="espweb"
REPO_ROOT=$(git rev-parse --show-toplevel)
FRONTEND_DIR="$REPO_ROOT/espWeb/frontend"
BACKEND_DIR="$REPO_ROOT/espWeb/backend"
AUTOMATION_DIR="$REPO_ROOT/espWeb/automation"

DOCKERFILE_DIR1="/include"
DOCKERFILE_DIR2="/automation"


docker build -t "$NAME" -f "$AUTOMATION_DIR/Dockerfile" "$FRONTEND_DIR"

docker run --rm \
  -v "$BACKEND_DIR/include/:$DOCKERFILE_DIR1" \
  -v "$AUTOMATION_DIR/:$DOCKERFILE_DIR2" \
  "$NAME"


