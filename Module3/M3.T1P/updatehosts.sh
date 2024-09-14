#!/bin/bash
# ssh to servers to accept new key
ssh -o StrictHostKeyChecking=accept-new 192.168.142.129
ssh -o StrictHostKeyChecking=accept-new 192.168.142.128
sleep 1
exit
# after this should be in SIT315-Node

# update host file on SIT315-Node
sudo su
echo "192.168.142.128  SIT315-Head" >> /etc/hosts
echo "192.168.142.129  SIT315-Node" >> /etc/hosts
exit
exit
# after this have exited node and back in SIT315-Head

# update hosts file on SIT315-Head
sudo su
echo "192.168.142.128  SIT315-Head" >> /etc/hosts
echo "192.168.142.129  SIT315-Node" >> /etc/hosts
exit
