#!/bin/sh

CONTAINER_NAME=$(docker ps -aqf "name=full-stack-2-postgres")

docker exec $CONTAINER_NAME pg_dumpall -h localhost -p 5432 -U dev-user --no-owner > postgres/sql_dump/dump.sql
