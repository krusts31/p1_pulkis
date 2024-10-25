#!/bin/bash

# Directory for storing certificates and keys
CERT_DIR="./certs"
mkdir -p $CERT_DIR

# Values for CA and certificates
CA_COUNTRY="LV"
CA_STATE="Some-State"
CA_ORG="Internet Widgits Pty Ltd"
CA_COMMON_NAME="CA"

SERVER_COUNTRY="LV"
SERVER_STATE="Some-State"
SERVER_ORG="Internet Widgits Pty Ltd"
SERVER_COMMON_NAME="mqtt.olgrounds.dev"

CLIENT_COUNTRY="LV"
CLIENT_STATE="Some-State"
CLIENT_ORG="Internet Widgits Pty Ltd"
CLIENT_COMMON_NAME="mqtt-client"

# Step 1: Generate CA (Certificate Authority) Key and Self-Signed Certificate
echo "Generating CA key and self-signed certificate..."
openssl genrsa -out $CERT_DIR/ca.key 
openssl req -x509 -new -nodes -key $CERT_DIR/ca.key -sha256 -days 3650 -out $CERT_DIR/ca.crt \
    -subj "/C=$CA_COUNTRY/ST=$CA_STATE/O=$CA_ORG/CN=$CA_COMMON_NAME"

# Step 2: Generate Server Key and CSR
echo "Generating Server key and CSR..."
openssl genrsa -out $CERT_DIR/server.key 
openssl req -new -key $CERT_DIR/server.key -out $CERT_DIR/server.csr \
    -subj "/C=$SERVER_COUNTRY/ST=$SERVER_STATE/O=$SERVER_ORG/CN=$SERVER_COMMON_NAME"

# Step 3: Sign Server Certificate with CA
echo "Signing Server certificate with CA..."
openssl x509 -req -in $CERT_DIR/server.csr -CA $CERT_DIR/ca.crt -CAkey $CERT_DIR/ca.key -CAcreateserial \
    -out $CERT_DIR/server.crt -days 365 -sha256

# Step 4: Generate Client Key and CSR
echo "Generating Client key and CSR..."
openssl genrsa -out $CERT_DIR/client.key 
openssl req -new -key $CERT_DIR/client.key -out $CERT_DIR/client.csr \
    -subj "/C=$CLIENT_COUNTRY/ST=$CLIENT_STATE/O=$CLIENT_ORG/CN=$CLIENT_COMMON_NAME"

# Step 5: Sign Client Certificate with CA
echo "Signing Client certificate with CA..."
openssl x509 -req -in $CERT_DIR/client.csr -CA $CERT_DIR/ca.crt -CAkey $CERT_DIR/ca.key -CAcreateserial \
    -out $CERT_DIR/client.crt -days 365 -sha256

# Clean up CSR files (optional)
rm $CERT_DIR/*.csr

echo "Certificates and keys have been generated in the '$CERT_DIR' directory:"
echo "CA Certificate: ca.crt"
echo "Server Certificate: server.crt, Server Key: server.key"
echo "Client Certificate: client.crt, Client Key: client.key"

