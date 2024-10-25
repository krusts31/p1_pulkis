FROM alpine:3.20 AS base

ENV TZ=Europe/Riga

CMD ["grafana-server", "--homepath", "/usr/share/grafana"]

RUN apk update && apk upgrade && apk add\
	grafana\
	tzdata

RUN cp /usr/share/zoneinfo/${TZ} /etc/localtime &&\
	echo "${TZ}" > /etc/timezone
