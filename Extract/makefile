all : my_extract
.PHONY : all
my_extract : my_extract.cpp
	g++ -o my_extract my_extract.cpp -fopenmp `pkg-config opencv --cflags` `pkg-config opencv --libs`
clean :
	rm my_extract
