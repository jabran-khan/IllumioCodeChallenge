CXX = g++
CXXFLAGS = -std=c++17 -Wall -Werror -MMD
EXEC = illumio
OBJECTS = main.o Firewall.o PortInterval.o IPAddressInterval.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
.PHONY: clean
