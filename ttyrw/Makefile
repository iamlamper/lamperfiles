all: read.c write.c 
	make read 
	make write
read: read.c
	gcc read.c -o read
write: write.c
	gcc write.c -o write

clean:
	rm *~ read write