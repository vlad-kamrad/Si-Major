gcc -c ./server/src/main.c -o ./server/dist/main.o
gcc -c ./server/src/collections/dictionary.c -o ./server/dist/dictionary.o
gcc -c ./server/src/networking/httpRequest.c -o ./server/dist/httpRequest.o
gcc -c ./server/src/file/file.c -o ./server/dist/file.o

gcc -o ./server/dist/program ./server/dist/main.o ./server/dist/httpRequest.o ./server/dist/dictionary.o ./server/dist/file.o
chmod 777 ./server/dist/program 
./server/dist/program