FROM alpine:3.20 AS base

ENV TZ=Europe/Riga

CMD ["telegraf", "--config", "/etc/telegraf/telegraf.conf"]

RUN apk update && apk upgrade && apk add\
	telegraf\
	tzdata

RUN cp /usr/share/zoneinfo/${TZ} /etc/localtime &&\
	echo "${TZ}" > /etc/timezone
