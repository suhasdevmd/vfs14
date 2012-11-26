../bin/vfsdriver: ../bin/n-ary.o ../bin/bst.o ../bin/HashTable.o ../bin/File_Operations.o ../bin/Dir_Operations.o ../bin/Vfs_Operations.o ../bin/vfsdriver.o
	gcc  ../bin/n-ary.o ../bin/bst.o ../bin/HashTable.o ../bin/File_Operations.o ../bin/Dir_Operations.o ../bin/Vfs_Operations.o ../bin/vfsdriver.o \
	-o ../bin/vfsdriver
../bin/vfsdriver.o: ../test/vfsdriver.c ../include/allheaders.h
	gcc  -c ../test/vfsdriver.c -I ../include/ -o ../bin/vfsdriver.o
../bin/Vfs_Operations.o:../src/Vfs_Operations.c ../include/allheaders.h
	gcc -c -o ../bin/Vfs_Operations.o -I ../include/ ../src/Vfs_Operations.c
../bin/Dir_Operations.o:../src/Dir_Operations.c ../include/allheaders.h
	gcc -c -o ../bin/Dir_Operations.o -I ../include/  ../src/Dir_Operations.c
../bin/n-ary.o:../src/n-ary.c ../include/allheaders.h
	gcc -c -o ../bin/n-ary.o -I ../include/ ../src/n-ary.c
../bin/HashTable.o:../src/HashTable.c ../include/allheaders.h
	gcc -c -o ../bin/HashTable.o -I ../include/ ../src/HashTable.c	
../bin/File_Operations.o:../src/File_Operations.c ../include/allheaders.h
	gcc -c -o ../bin/File_Operations.o -I ../include/ ../src/File_Operations.c
../bin/bst.o:../src/bst.c ../include/allheaders.h
	gcc -c -o ../bin/bst.o -I ../include/ ../src/bst.c
