up:
	bash ./certbot/init-letsencrypt.sh 
	docker compose up

build:
	bash ./certbot/init-letsencrypt.sh 
	docker compose --build up

down:
	docker compose -v down

stop:
	docker compose down
