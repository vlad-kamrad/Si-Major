gcc -c ./server/src/main.c -o ./server/dist/main.o
gcc -c ./server/src/networking/httpRequest.c -o ./server/dist/httpRequest.o
gcc -o ./server/dist/program ./server/dist/main.o ./server/dist/httpRequest.o
chmod 777 ./server/dist/program 
./server/dist/program