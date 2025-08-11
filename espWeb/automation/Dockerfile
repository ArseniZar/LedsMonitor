FROM node:24-bookworm

RUN apt-get update && apt-get install -y python3 python3-pip


ENV DOCKER_DIR1=/include
ENV DOCKER_DIR2=/automation

RUN mkdir -p /include && chown -R 1000:1000 $DOCKER_DIR1
RUN mkdir -p /automation && chown -R 1000:1000 $DOCKER_DIR2

WORKDIR /app

COPY ./ /app

RUN npm ci

RUN npm run build

USER 1000:1000

CMD ["sh", "-c", "python3 $DOCKER_DIR2/generate_header.py dist/index.html $DOCKER_DIR1/index.h"]