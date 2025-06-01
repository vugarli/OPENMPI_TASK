FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y iputils-ping openssh-server openssh-client sshpass \
    openmpi-bin libopenmpi-dev build-essential && \
    apt-get clean

RUN mkdir /var/run/sshd && \
    echo 'root:mpipassword' | chpasswd && \
    sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config && \
    sed -i 's/#PasswordAuthentication yes/PasswordAuthentication yes/' /etc/ssh/sshd_config && \
    echo "StrictHostKeyChecking no" >> /etc/ssh/ssh_config && \
    echo "UserKnownHostsFile /dev/null" >> /etc/ssh/ssh_config

RUN ssh-keygen -t rsa -f /root/.ssh/id_rsa -N "" && \
    cat /root/.ssh/id_rsa.pub >> /root/.ssh/authorized_keys && \
    chmod 600 /root/.ssh/authorized_keys && \
    chmod 700 /root/.ssh

COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh && \
    sed -i 's/\r$//' /entrypoint.sh

WORKDIR /root
EXPOSE 22
ENTRYPOINT ["/entrypoint.sh"]
