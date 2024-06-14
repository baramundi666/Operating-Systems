all: server client

server: server.c
	mkdir -pv build
	gcc $< -o build/server

client: client.c
	mkdir -pv build
	gcc $< -o build/client

clean:
	rm -rf build
