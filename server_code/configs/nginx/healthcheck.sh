#!/bin/sh

wget --no-check-certificate -q -O /dev/null --spider https://$WORDPRESS_URL:443/healthz

if [ "$?" -eq "0" ]; then
    echo "Nginx is healthy."
    exit 0
else
    echo "Nginx is unhealthy."
    exit 1
fi

