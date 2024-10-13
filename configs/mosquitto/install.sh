#!/bin/ash

#dockerize -template /tmp/mosquitto.conf.tmpl:/etc/mosquitto/mosquitto.conf

echo "MQTT server started: current time $(date)"

exec mosquitto -c /etc/mosquitto/mosquitto.conf
