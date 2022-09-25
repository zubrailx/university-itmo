#!/bin/bash

sudo ip netns add testos
sudo ip link add veth-s type veth peer name veth-t
sudo ip link set veth-s netns testos
sudo ip netns exec testos ifconfig veth-s up 192.168.163.1 netmask 255.255.255.0
sudo ifconfig veth-t up 192.168.163.254 netmask 255.255.255.0
sudo ip netns exec testos route add default gw 192.168.163.254 dev veth-s
sudo su -c "echo 1 > /proc/sys/net/ipv4/ip_forward"
sudo iptables -t nat -A POSTROUTING -s 192.168.163.0/24 -o enp0s3
