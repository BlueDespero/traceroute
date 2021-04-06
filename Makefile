all:
	g++ -std=gnu++17 -Wall -Wextra -o rec_funcs.o -c rec_funcs.cpp
	g++ -std=gnu++17 -Wall -Wextra -o send_funcs.o -c send_funcs.cpp
	g++ -std=gnu++17 -Wall -Wextra -o common.o -c common.cpp
	g++ -std=gnu++17 -Wall -Wextra -o main.o -c main.cpp
	g++ -std=gnu++17 -Wall -Wextra common.o send_funcs.o rec_funcs.o main.o -o traceroute

clean:
	rm -f rec_funcs.o send_funcs.o main.o common.o

distclean:
	rm -f rec_funcs.o send_funcs.o main.o common.o traceroute