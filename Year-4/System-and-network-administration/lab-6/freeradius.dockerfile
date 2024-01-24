FROM 'ubuntu:latest'

ARG DEBIAN_FRONTEND=noninteractive

RUN apt update -y && apt upgrade -y

RUN apt install -y freeradius freeradius-ldap
