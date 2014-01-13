#include <stdio.h>
#include <ftdi.h>

#define EEPROM_SIZE 256

enum {WRITE,READ};

void use(char *name){
	fprintf(stdout,"%s [R|W] file\n",name);
	return;
}

int main(int argc, char *argv[]){
	struct ftdi_context ftdi;
	unsigned char eeprom_buf[EEPROM_SIZE];
	int ret;
	FILE *file;
	int mode=READ;

	if (argc!=3){
		use(argv[0]);
		return -1;
	}

	if ((argv[1][0]=='w') || (argv[1][0]=='W'))
		mode=WRITE;

	file=fopen(argv[2],(mode==READ)?"w":"r");
	if (!file){
		perror(argv[2]);
		return -1;
	}

	ftdi_init(&ftdi);
	ftdi.eeprom_size=EEPROM_SIZE;
	ret=ftdi_usb_open(&ftdi, 0x0403, 0x6010);
	if (ret){
		printf("Error: %s\n", ftdi.error_str);
		fclose(file);
		return -1;
	}

	if (mode==READ){
		ret=ftdi_read_eeprom(&ftdi, eeprom_buf);
		fwrite(eeprom_buf,sizeof(eeprom_buf),1,file);
	}
	else{
		fread(eeprom_buf,sizeof(eeprom_buf),1,file);
		ftdi_erase_eeprom(&ftdi);
		ftdi_write_eeprom(&ftdi, eeprom_buf);
	}

	fclose(file);
	ftdi_deinit (&ftdi);

	return 0;
}
