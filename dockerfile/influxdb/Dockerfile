FROM influxdb:2-alpine AS base

ENV TZ=Europe/Riga

RUN apk update &&\
	apk upgrade &&\
	apk add --no-cache tzdata

RUN cp /usr/share/zoneinfo/${TZ} /etc/localtime &&\
	echo "${TZ}" > /etc/timezone
