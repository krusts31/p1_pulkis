up:
	bash ./certbot/init-letsencrypt.sh olgrounds.dev www.olgrounds.dev
	docker compose up

prod: up
	bash ./certbot/prod-cert-letsncrypt.sh\
		olgrounds.dev www.olgrounds.dev
	#bash ./srcs/cert/bottest_renew.sh TODO
	bash ./srcs/tools/reload_nginx.sh

build:
	bash ./certbot/init-letsencrypt.sh 
	docker compose --build up

down:
	docker compose -v down

stop:
	docker compose down
