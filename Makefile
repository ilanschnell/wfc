wfc: main.c
	bash -x build.sh

clean:
	rm -f main.o wfc

test: wfc
	./wfc -s 1342.txt
