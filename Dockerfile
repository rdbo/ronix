FROM alpine:edge

RUN apk add gcc make cmake sdl2-dev
RUN apk add g++ libstdc++-dev

RUN adduser -D ronix
RUN addgroup ronix wheel
USER ronix

WORKDIR /app
