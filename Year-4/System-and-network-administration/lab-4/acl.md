1) permit tcp syn to server
2) deny tcp syn ack to server
3) permit tcp between server and client
4) deny everything else

deny:
* tcp syn ack to anybody

rule 5 permit tcp source 192.168.1.100 0 destination 10.0.23.3 0 destination-port eq 2323 tcp-flag syn
rule 10 deny tcp source 192.168.1.100 0 destination 10.0.23.3 0 tcp-flag syn ack
rule 15 permit tcp source 192.168.1.100 0 destination 10.0.23.3 0 destination-port eq 2323
rule 30 deny ip

