FROM alpine:3.20 AS base

ENV DOCKERIZE_VERSION=v0.8.0

EXPOSE 1883 8883

ENV TZ=Europe/Riga

COPY install.sh /tmp/install.sh

CMD ["sh", "/tmp/install.sh"]

RUN apk update && apk upgrade && apk add\
	mosquitto\
	tzdata

RUN cp /usr/share/zoneinfo/${TZ} /etc/localtime &&\
	echo "${TZ}" > /etc/timezone

FROM base AS prod

RUN wget https://github.com/jwilder/dockerize/releases/download/$DOCKERIZE_VERSION/dockerize-alpine-linux-amd64-$DOCKERIZE_VERSION.tar.gz \
       && tar -C /usr/local/bin -xzvf dockerize-alpine-linux-amd64-$DOCKERIZE_VERSION.tar.gz \
       && rm dockerize-alpine-linux-amd64-$DOCKERIZE_VERSION.tar.gz

RUN mkdir -p /mosquitto/data

RUN touch /mosquitto/data/mosquitto.db

RUN chown -R mosquitto:mosquitto /mosquitto/data

RUN chmod 0700 /mosquitto/data/mosquitto.db

USER mosquitto
