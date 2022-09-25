#!/bin/bash

ps --forest $(ps -e --no-header -o pid,ppid|awk -vp=$1 'function r(s){print s;s=a[s];while(s){sub(",","",s);t=s;sub(",.*","",t);sub("[0-9]+","",s);r(t)}}{a[$2]=a[$2]","$1}END{r(p)}')
