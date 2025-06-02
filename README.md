# To run via docker
docker-compose up --build

To alter slots for each node please modify ./src/hostfile file as desired. Additionally, WSL can also be configured to allocate more cores for VM.

More nodes can also be added by modifying docker compose file. Make sure that you copy node with env value of NODE_ROLE=worker. Entrypoint script also needs to be modified if new node is added. Otherwise, ssh won't be configured for the new node 

Currently there are two nodes with 2 slots each.

# To run locally
cd src

make

make run

This way app will be launched with 2 processes only. The value of -np param can be modified in Makefile.

# To use matrix generator
just compile and run gen_save_matrix.c

Answer the prompts, and matrix with specified properties will be generated and save to the current directory. The names have to be "ma.txt", and "mb.txt" because they're hardcoded to the main.c

Currently both .txt files are in appropriate places and filled with matrices with 10_000 x 10_000 dimensions. In main.c N <= 10_000 can be set without generating new matrix files# OPENMPI_TASK
# OPENMPI_TASK
