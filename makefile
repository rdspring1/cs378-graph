all:
	make Graph.zip

clean:
	rm -f Graph.log
	rm -f Graph.zip
	rm -f TestGraph
	rm -f TestGraph1
	rm -f TestGraph2
	rm -f TestGraph3

doc: Graph.h
	doxygen Doxyfile

turnin-list:
	turnin --list dlessin cs378pj5

turnin-submit: Graph.zip
	turnin --submit dlessin cs378pj5 Graph.zip

turnin-verify:
	turnin --verify dlessin cs378pj5

Graph.log:
	git log > Graph.log

Graph.zip: Graph.h Graph.log TestGraph.c++ TestGraph.out
	zip -r Graph.zip html/ Graph.h Graph.log TestGraph.c++ TestGraph.out

TestGraph: Graph.h TestGraph.c++
	g++ -g -pedantic -std=c++0x -Wall TestGraph.c++ -o TestGraph -lgtest -lpthread -lgtest_main
    
TestGraph1: Graph.h tsm544-TestGraph.c++
	g++ -pedantic -std=c++0x -Wall tsm544-TestGraph.c++ -o TestGraph1 -lgtest -lpthread -lgtest_main

TestGraph2: Graph.h davismc-TestGraph.c++
	g++ -pedantic -std=c++0x -Wall davismc-TestGraph.c++ -o TestGraph2 -lgtest -lpthread -lgtest_main
    
TestGraph3: Graph.h wrj322-TestGraph.c++
	g++ -pedantic -std=c++0x -Wall wrj322-TestGraph.c++ -o TestGraph3 -lgtest -lpthread -lgtest_main
    
TestGraph.out: TestGraph
	valgrind ./TestGraph > TestGraph.out
