CC                  = $(CROSS_COMPILE)g++
MAKE                = make


run:
	${CC} -o simplex simplex.cpp
	
clean:
	rm -f simplex
	   

	

