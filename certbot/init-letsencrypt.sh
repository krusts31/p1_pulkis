#!/bin/bash

#set -x is for debuging
#set -x

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

domains=("$@")

rsa_key_size=4096
data_path="$DIR"
email="" # Adding a valid address is strongly recommended

if [ ! -e "$data_path/conf/options-ssl-nginx.conf" ] || [ ! -e "$data_path/conf/ssl-dhparams.pem" ]; then
  echo "### Downloading recommended TLS parameters ..."
  mkdir -p "$data_path/conf"
  curl -s https://raw.githubusercontent.com/certbot/certbot/master/certbot-nginx/certbot_nginx/_internal/tls_configs/options-ssl-nginx.conf > "$data_path/conf/options-ssl-nginx.conf"
  curl -s https://raw.githubusercontent.com/certbot/certbot/master/certbot/certbot/ssl-dhparams.pem > "$data_path/conf/ssl-dhparams.pem"
fi

for domain in "${domains[@]}"; do

  path="/etc/letsencrypt/live/$domain"
  mkdir -p "$data_path/conf/live/$domain"

  docker run --rm\
    -v $DIR/conf:/etc/letsencrypt\
    -v $DIR/www:/var/www/certbot\
    --entrypoint "openssl"\
    certbot/certbot\
    req -x509 -nodes -newkey rsa:$rsa_key_size -days 1\
    -keyout "$path/privkey.pem"\
    -out "$path/fullchain.pem"\
    -subj "/CN=$domain"
done
