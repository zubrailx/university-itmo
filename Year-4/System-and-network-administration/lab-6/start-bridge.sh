#!/bin/bash

ip link add bridge-vm type bridge
ip link set dev bridge-vm up
ip a add 192.168.133.1/24 dev bridge-vm
# ip route add 172.16.0.0/16 via 192.168.133.31 dev bridge-vm

# -- show active links
# ip link show

# -- show links to bridges
# bridge link
