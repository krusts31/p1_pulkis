#!/bin/bash

#for debugging
#set -x

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

domains=("$@")

rsa_key_size=2048

data_path="$DIR/requirements/certbot"
email="" # Adding a valid address is strongly recommended

staging=0 # Set to 1 if you're testing your setup to avoid hitting request limits


for DOMAIN in "${domains[@]}"; do
  echo "### Deleting dummy certificate for $DOMAIN"
  docker run --rm\
    -v $DIR/conf:/etc/letsencrypt\
    -v $DIR/www:/var/www/certbot\
    --entrypoint "rm"\
    certbot/certbot\
    -Rf /etc/letsencrypt/live/$DOMAIN\
    /etc/letsencrypt/archive/$DOMAIN\
    /etc/letsencrypt/renewal/$DOMAIN.conf certbot
done

# Enable staging mode if needed
if [ $staging != "0" ]; then staging_arg="--staging"; fi

for DOMAIN in "${domains[@]}"; do
  echo "getting certificate for $DOMAIN"
  docker run\
    -v $DIR/conf:/etc/letsencrypt\
    -v $DIR/www:/var/www/certbot\
    --entrypoint "certbot"\
    certbot/certbot\
    certonly --webroot -w /var/www/certbot\
      $staging_arg\
      --email $email\
      -d $DOMAIN\
      --rsa-key-size $rsa_key_size\
      --agree-tos\
      --force-renewal
done
