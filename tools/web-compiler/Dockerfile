FROM node:24-bookworm

RUN apt-get update && apt-get install -y python3 python3-pip


ENV DOCKER_DIR1=/src_app
ENV DOCKER_DIR2=/include_out
ENV DOCKER_DIR3=/automation

RUN mkdir -p $DOCKER_DIR1 $DOCKER_DIR2 $DOCKER_DIR3 /app && \
    chown -R 1000:1000 $DOCKER_DIR1 $DOCKER_DIR2 $DOCKER_DIR3 /app

WORKDIR /app
USER 1000:1000

CMD ["sh", "-c", "cp -r $DOCKER_DIR1/. . && npm ci && npm run build && python3 $DOCKER_DIR3/generate_header.py dist/index.html $DOCKER_DIR2/index.h $FILE_NAME"]