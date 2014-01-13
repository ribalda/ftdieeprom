/*

    Read and Write the eeprom of an ftdi device via libftdi

    Copyright Ricardo Ribalda - 2014 - ricardo.ribalda@gmail.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <ftdi.h>

#define EEPROM_SIZE 256

enum {WRITE,READ};

void use(char *name){
	fprintf(stdout,"%s [R|W] file\n",name);
	return;
}

static int fix_csum(unsigned char *eeprom,int len){
	int i;
	uint16_t csum=0xaaaa;
	uint16_t old_csum=eeprom[len-1]<<8 | eeprom[len-2];

	for (i=0;i<(len/2)-1;i++){
		uint16_t value;
		value=eeprom[i*2]+((eeprom[i*2+1])<<8);

		csum=value^csum;
		csum= (csum<<1)|(csum>>15);
	}

	if (csum!=old_csum){
		fprintf(stderr,"Old csum 0x%.4x, new csum 0x%4.x\n",old_csum,csum);
		eeprom[len-2]=csum;
		eeprom[len-1]=csum>>8;
	}

	return 0;
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
		fix_csum(eeprom_buf,sizeof(eeprom_buf));
		ftdi_erase_eeprom(&ftdi);
		ftdi_write_eeprom(&ftdi, eeprom_buf);
	}

	fclose(file);
	ftdi_deinit (&ftdi);

	return 0;
}
