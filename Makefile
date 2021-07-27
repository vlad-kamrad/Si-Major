run: ./server/src/main.c 
	gcc -o ./server/dist/program ./server/src/main.c ./server/src/networking/httpRequest.c -I.
	chmod 777 ./server/dist/program
	clear
	./server/dist/program

