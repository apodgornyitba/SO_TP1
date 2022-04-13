GCC=gcc
GCC_FLAGS= -g -std=c99 -Wall
O_FLAGS= -lrt -lpthread

all: App Slave View

App: app.c library.c
	$(GCC) $(GCC_FLAGS) -I./include app.c library.c -o app $(O_FLAGS)

Slave: slave.c library.c
	$(GCC) $(GCC_FLAGS) -I./include slave.c library.c -o slave $(O_FLAGS)

View: view.c library.c
	$(GCC) $(GCC_FLAGS) -I./include view.c library.c -o view $(O_FLAGS)

clean:
	rm -rf app
	rm -rf slave
	rm -rf view
	rm -rf result.txt

.PHONY: clean all