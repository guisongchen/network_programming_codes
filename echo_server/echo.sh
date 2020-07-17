#!/usr/bin/env bash
echo -e "please enter port number:"

# read serv info from stdin
read port_num

serv_name="echo_server"

# enter into build folder
cd ./build

# execute server application, convert to deamon process, so we can handle client
./$serv_name $port_num &

# search ip config and parse ip from config info
ip_line=$(ifconfig | grep "inet addr" | grep 192)
host_ip_coarse=${ip_line#*:}
host_ip=${host_ip_coarse%?Bcast*}

# prepare client application
# echo -e "please enter client name:"
# read client_name

client_name="echo_client"

# execute client application
./$client_name $host_ip $port_num 
