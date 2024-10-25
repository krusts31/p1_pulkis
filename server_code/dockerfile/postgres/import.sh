#!/bin/sh

docker exec postgres psql -f /tmp/dump.sql -U dev-user -h localhost -p 5432 postgres
#docker exec $CONTAINER_NAME psql -U dev-user -h localhost -p 5432 postgres <  postgres/sql_dump/dump.sql
