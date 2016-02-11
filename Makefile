LDLIBS += $(shell pkg-config --libs libftdi libusb)
CFLAGS += -Wall $(shell pkg-config --cflags libftdi libusb)
ALL = ftdieeprom

all:${ALL}

clean:
	${RM} *o core ${ALL}
