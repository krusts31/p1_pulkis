FROM alpine:3.20 AS base

ENV TZ=Europe/Riga

RUN apk update &&\
	apk upgrade &&\
	apk add --no-cache \
	telegraf\
	tzdata

RUN cp /usr/share/zoneinfo/${TZ} /etc/localtime &&\
	echo "${TZ}" > /etc/timezone

CMD ["telegraf", "--config", "/etc/telegraf/telegraf.conf"]
