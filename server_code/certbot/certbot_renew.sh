#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
domains=($@)

rsa_key_size=4096
data_path="$DIR/certbot"
email="" # Adding a valid address is strongly recommended
staging=1 # Set to 1 if you're testing your setup to avoid hitting request limits

echo "### Renewing certificate for $domains ..."

container_id=$(sudo docker run --rm -d \
  -v $DIR/certs/conf:/etc/letsencrypt \
  -v $DIR/certs/www:/var/www/certbot \
  --entrypoint "certbot" \
  certbot/certbot \
  renew --force-renew)

# Wait for the container to complete
sudo docker wait $container_id

docker exec nginx nginx -s reload
