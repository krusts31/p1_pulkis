docker run -d \
  --hostname grafana \
  --network p1_port_test_net \
  --name p1_port_grafana \
  --env-file .env \
  p1_port_grafana

docker run -p 8883:8883 -v ./configs/mosquitto/mosquitto.conf:/etc/mosquitto/mosquitto.conf -d --hostname mqtt --network p1_port_test_net p1_port_mqtt

docker run -d \
  --hostname influxdb \
  --network p1_port_test_net \
  --name p1_port_influxdb \
  --env-file .env-influx \
  p1_port_influxdb

docker run -v /home/p1/p1_pulkis/configs/telegraf/telegraf.conf:/etc/telegraf/telegraf.conf -d   --hostname telegraf   --network p1_port_test_net   --name p1_port_telegraf   --env-file .env-telegraf   p1_port_telegraf

docker run -d -p 80:80 -p 443:443 -v  ./certbot/www:/var/www/certbot -v ./certbot/conf/:/etc/letsencrypt/ -v ./configs/nginx/nginx.conf:/etc/nginx/nginx.conf -v ./configs/nginx/grafana.conf:/etc/nginx/http.d/grafana.conf -v ./certbot/conf/options-ssl-nginx.conf:/etc/letsencrypt/options-ssl-nginx.conf -v ./certbot/conf/ssl-dhparams.pem:/etc/letsencrypt/ssl-dhparams.pem --network p1_port_test_net p1_port_nginx
