LDFLAGS=-lftdi
ALL=ftdieeprom

all:${ALL}

clean:
	${RM} *o core ${ALL}
