FROM nikolaik/python-nodejs:python3.12-nodejs24

RUN apt-get update && \
    apt-get install -y xxd && \
    rm -rf /var/lib/apt/lists/*
    
ENV DOCKER_INPUT_DIR=/input
ENV DOCKER_OUTPUT_DIR=/output
ENV DOCKER_SCRIPTS_DIR=/scripts

RUN mkdir -p $DOCKER_INPUT_DIR $DOCKER_OUTPUT_DIR $DOCKER_SCRIPTS_DIR /app /.npm && \
    chmod -R 777 $DOCKER_INPUT_DIR $DOCKER_OUTPUT_DIR $DOCKER_SCRIPTS_DIR /app /.npm

WORKDIR /app

CMD ["sh", "-c", "cp -r $DOCKER_INPUT_DIR/. . && npm ci && npm run build && python3 $DOCKER_SCRIPTS_DIR/script.py dist/index.html $DOCKER_OUTPUT_DIR/ $OUTPUT_FILE_NAME"]

# Copying is done to protect the original repository from changes, as temporary files and 
# directories may be created during the build process and should not enter the original repository.