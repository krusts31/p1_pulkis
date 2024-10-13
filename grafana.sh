docker run -d --hostname grafana --network p1_port_test_net p1_port_grafana
docker run -v ./configs/mosquitto/mosquitto.conf:/etc/mosquitto/mosquitto.conf -d --hostname mqtt --network p1_port_test_net p1_port_mqtt

docker run -p 80:80 -p 443:443 -v  ./certbot/www:/var/www/certbot -v ./certbot/conf/:/etc/letsencrypt/ -v ./root-ca/ca.crt:/etc/nginx/certs/ca.crt -v ./configs/nginx/nginx.conf:/etc/nginx/nginx.conf -v ./configs/nginx/mqtt.conf:/etc/nginx/http.d/mqtt.conf -v ./configs/nginx/grafana.conf:/etc/nginx/http.d/grafana.conf -v ./certbot/conf/options-ssl-nginx.conf:/etc/letsencrypt/options-ssl-nginx.conf -v ./certbot/conf/ssl-dhparams.pem:/etc/letsencrypt/ssl-dhparams.pem --network p1_port_test_net p1_port_nginx
