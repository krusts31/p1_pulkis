FROM alpine:3.20 AS base

ENV TZ=Europe/Riga

RUN apk update && apk upgrade && apk add\
	mosquitto\
	tzdata

RUN cp /usr/share/zoneinfo/${TZ} /etc/localtime &&\
	echo "${TZ}" > /etc/timezone

FROM base AS prod

RUN mkdir -p /mosquitto/data

RUN touch /mosquitto/data/mosquitto.db

RUN chown -R mosquitto:mosquitto /mosquitto/data

RUN chmod 0700 /mosquitto/data/mosquitto.db

RUN rm /etc/mosquitto/mosquitto.conf

COPY --chown=mosquitto:mosquitto ./root-ca/certs/ca.crt /mosquitto/config/ca.crt
COPY --chown=mosquitto:mosquitto ./root-ca/certs/server.crt /mosquitto/config/server.crt
COPY --chown=mosquitto:mosquitto ./root-ca/certs/server.key /mosquitto/config/server.key

USER mosquitto

CMD ["mosquitto", "-c", "/etc/mosquitto/mosquitto.conf"]
