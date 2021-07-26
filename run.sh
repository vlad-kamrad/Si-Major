gcc -c ./server/src/main.c -o ./server/dist/main.o
gcc -o ./server/dist/program ./server/dist/main.o
chmod 777 ./server/dist/program 
./server/dist/program