#!/bin/bash


service ssh start
sleep 2


if [ "$(hostname -I | grep -o '172.28.1.1')" ]; then
    echo "Setting up SSH keys from node1 to node2..."
    sshpass -p 'mpipassword' ssh-copy-id -o StrictHostKeyChecking=no root@172.28.1.2
    sshpass -p 'mpipassword' ssh-copy-id -o StrictHostKeyChecking=no root@172.28.1.1
elif [ "$(hostname -I | grep -o '172.28.1.2')" ]; then
    echo "Setting up SSH keys from node2 to node1..."
    sleep 3 
    sshpass -p 'mpipassword' ssh-copy-id -o StrictHostKeyChecking=no root@172.28.1.1
    sshpass -p 'mpipassword' ssh-copy-id -o StrictHostKeyChecking=no root@172.28.1.2
fi


echo "Testing SSH connectivity..."
ssh -o ConnectTimeout=5 root@172.28.1.1 "echo 'SSH to node1 successful'" 2>/dev/null || echo "SSH to node1 failed"
ssh -o ConnectTimeout=5 root@172.28.1.2 "echo 'SSH to node2 successful'" 2>/dev/null || echo "SSH to node2 failed"

sleep 5

if [[ "$HOSTNAME" == "node1" ]]; then
    sshpass -p root ssh -o StrictHostKeyChecking=no root@172.28.1.2 "mkdir -p /root/.ssh && cat >> /root/.ssh/authorized_keys" < /root/.ssh/id_rsa.pub
elif [[ "$HOSTNAME" == "node2" ]]; then
    sshpass -p root ssh -o StrictHostKeyChecking=no root@172.28.1.1 "mkdir -p /root/.ssh && cat >> /root/.ssh/authorized_keys" < /root/.ssh/id_rsa.pub
fi

echo "[INFO] SSH is ready on $HOSTNAME"

sleep 5



if [ -f main.c ] && [ -f matrix_parser.c ]; then
    mpicc -o mpi_program main.c matrix_parser.c
else
    echo "Source files not found. Skipping compilation."
fi

if [[ "$HOSTNAME" == "node1" ]]; then
    if [ -f ./mpi_program ]; then
        echo "Running MPI job from master node..."
        sleep 10 
        # mpirun --allow-run-as-root -host node1,node2 -np 4 ./mpi_program
        mpirun -hostfile hostfile -np 4 ./mpi_program --allow-run-as-root
    fi
fi


# if [ -f ./mpi_program ]; then
#     echo "Running MPI job..."
#     mpirun -host node1,node2 -np 2 ./mpi_program --allow-run-as-root
# else
#     echo "Compiled program not found. Skipping run."
# fi


tail -f /dev/null

