run: ./server/src/main.c
	gcc -g -Wall -Wextra -o ./server/dist/program ./server/src/main.c ./server/src/networking/httpRequest.c ./server/src/networking/httpResponse.c ./server/src/collections/dictionary.c ./server/src/collections/array.c ./server/src/file/file.c ./server/src/file/fileObject.c ./server/src/utils/utils.c ./server/src/utils/mimeTypes.c ./server/src/utils/configWrapper.c ./server/src/file/endpoint.c -I.
	chmod 777 ./server/dist/program
	./server/dist/program

mobile: ./server/src/main.c
	gcc -g -Wall -Wextra -Wunused-variable -Wunused-parameter -o ./server/dist/program ./server/src/main.c ./server/src/networking/httpRequest.c ./server/src/networking/httpResponse.c ./server/src/collections/dictionary.c ./server/src/collections/array.c ./server/src/file/file.c ./server/src/file/fileObject.c ./server/src/utils/utils.c ./server/src/utils/mimeTypes.c ./server/src/utils/configWrapper.c ./server/src/file/endpoint.c -I.
	chmod 777 ./server/dist/program
	./server/dist/program

start: ./server/src/main.c
	gcc  -o ./server/dist/program ./server/src/main.c ./server/src/networking/httpRequest.c ./server/src/networking/httpResponse.c ./server/src/collections/dictionary.c ./server/src/collections/array.c ./server/src/file/file.c ./server/src/file/fileObject.c ./server/src/utils/utils.c ./server/src/utils/mimeTypes.c ./server/src/utils/configWrapper.c ./server/src/file/endpoint.c -I.
	chmod 777 ./server/dist/program
	clear
	./server/dist/program