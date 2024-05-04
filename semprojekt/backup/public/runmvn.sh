#!/usr/bin/bash

export DEPLOYED=true
export JDBC_USER=postgres
export JDBC_PASSWORD="bwQc)89P"
export JDBC_CONNECTION_STRING_STARTCODE="jdbc:postgresql://104.248.251.153:5432/%s?currentSchema=public"
export JDBC_DB="carport"

mvn exec:java -Dexec.mainClass="carport.Main"

unset DEPLOYED
unset JDBC_USER
unset JDBC_PASSWORD
unset JDBC_CONNECTION_STRING_STARTCODE
unset JDBC_DB
