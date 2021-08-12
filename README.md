# Si major

A simple web server written in C

### TODO

- Implement route system
- SSL/TLS
- Investigate/Implement .env file
- Add json parser
- Check with load testing
- Implement POST requests (pre web api)
- Investigate cache functionality
- Add the ability to make requests
- gzip ?
- Add dynamic allocation memory for headers
- Cookie reader/writer
- Event loop aka Node.js (https://github.com/libuv/libuv)

### Done

- Base implementation
- Investigate multi routes
- Implement header parser
- Implement multi route for get requests
- Write config parser for configure multiple resources
- Added base multithreading functionality
- Write MIME dict file
- Investigate Content-Length header
- Add dynamic header to response (text/css, text/html)
- Sending images
- Write 404 Page

### Use this command to clear a busy port

```kill -9 $(lsof -i:8003 -t) 2> /dev/null```
