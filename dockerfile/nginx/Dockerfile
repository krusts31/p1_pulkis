FROM alpine:latest as base

RUN apk update &&\
	apk upgrade &&\
	apk add --no-cache\
	tzdata &&\
	nginx

RUN mkdir -p /run/nginx

RUN rm /etc/nginx/http.d/default.conf

ENTRYPOINT ["nginx", "-g", "daemon off;"]
