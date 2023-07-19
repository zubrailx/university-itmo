#Base Image
FROM ubuntu:23.04

#Update APT repository & Install OpenSSH
RUN apt-get update \
    && apt-get install -y openssh-server \
    && apt-get install -y mysql-client \
    && apt-get -y install curl wget sudo \
    && apt-get -y install ca-certificates gnupg

#Postgres 15
ARG DEBIAN_FRONTEND=noninteractive

RUN sudo sh -c 'echo "deb http://apt.postgresql.org/pub/repos/apt $(lsb_release -cs)-pgdg main" > /etc/apt/sources.list.d/pgdg.list'
RUN wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | sudo apt-key add -
RUN apt-get -y install postgresql-15
RUN apt-get -y install pgpool2 libpgpool2 postgresql-15-pgpool2

RUN apt-get -y install ssh iputils-ping vim nano

RUN cp -s /usr/lib/postgresql/15/bin/* /usr/bin 2> dev/null; exit 0
#Postgres 15

##Establish the operating directory of OpenSSH
#RUN mkdir /var/run/sshd

#Set Root password
RUN echo 'root:root' | chpasswd

#Allow Root login
RUN sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' \
    /etc/ssh/sshd_config

#SSH login fix
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional \
    pam_loginuid.so@g' -i /etc/pam.d/sshd

#expose port 22
EXPOSE 22

#Commands to be executed by default
CMD ["/usr/sbin/sshd","-D"]