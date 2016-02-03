LDLIBS += $(shell pkg-config --libs libftdi)
CFLAGS += -Wall $(shell pkg-config --libs libftdi)
ALL = ftdieeprom

all:${ALL}

clean:
	${RM} *o core ${ALL}
