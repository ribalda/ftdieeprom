LDLIBS=-lftdi
ALL=ftdieeprom
CFLAGS=-Wall

all:${ALL}

clean:
	${RM} *o core ${ALL}
