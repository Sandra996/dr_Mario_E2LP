/*****************************************************************************
* Filename:          D:\work\lprs2\2013_2014\Resene_vezbe\lab56\basic_system/drivers/vga_periph_mem_v1_00_a/src/vga_periph_mem.c
* Version:           1.00.a
* Description:       vga_periph_mem Driver Source File
* Date:              Wed Mar 05 10:25:21 2014 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "vga_periph_mem.h"
#include "math.h"
Xuint32 cursor_position;
/************************** Function Definitions ***************************/

void set_cursor(Xuint32 new_value){
	cursor_position = new_value;
}

void clear_text_screen(Xuint32 BaseAddress){
	int i;
	for (i = 0; i < 4800; i++){
		VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + i*4, 0x20);
	}
}

void print_string(Xuint32 BaseAddress, int boja, unsigned char string_s[], int lenght){
	int i;
	int bojaSh = boja<<6;
	int zapis;
	for (i = 0; i < lenght; i++){
		unsigned char znak = string_s[i];
		zapis = bojaSh;
		if(znak >= 0x40){
			znak -= 0x40;
		}
		zapis|=znak;
		VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + (cursor_position + i)*4, zapis);
	}
}

//printing char
void print_char(Xuint32 BaseAddress, int boja, unsigned char znak){
	int zapis;
	zapis = boja<<6;
	if(znak >= 0x40){
		znak -= 0x40;
	}
	zapis|=znak;
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + cursor_position*4, zapis);
}
//printing all chars
void print_all(Xuint32 BaseAddress, int boja){
	int zapis;
	unsigned char i;
	int znak;
	znak = boja<<6;
	cursor_position = 0;
	for(i=0;i<64;i++){
			zapis = znak | (i+0x40);
			VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + cursor_position*4, zapis);
			cursor_position+=2;
	}
}
//clearing screen
void clear_screen(Xuint32 BaseAddress){
	int i;
	for (i = 0; i < 4800; i++){
		VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + i*4, 0x20);
		VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + i*4, 0x0);
	}
	for (i = 4800; i < 9600; i++){
	    VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + i*4, 0x0);
	}
}

void set_foreground_color(Xuint32 BaseAddress, Xuint32 color){
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress+0x10, color);
}
void set_background_color(Xuint32 BaseAddress, Xuint32 color){
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + 0x14, color);
}
void set_font_size(Xuint32 BaseAddress, unsigned char size){
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + 0x0C, size);
}
/*int get_resolution(Xuint32 BaseAddress){
	VGA_PERIPH_MEM_mReadMemory(BaseAddress+4*Index);
}*/
void clear_graphics_screen(Xuint32 BaseAddress){
	int i;
	for (i = 0; i < 9600; i++){
	    VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + i*4, 0x0);
	}
}
/*
void draw_square(Xuint32 BaseAddress){
	int i, j, k;
		for (j = 0; j < 480; j++){
			for (k = 0; k<(640/32); k++){
				i = j*(640/32) + k;
				if ((j > 200) && (j < 280) && (k > 8) && (k < 12)) {
					VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + i*4, 0xFFFFFFFF);
				}
				else{
					VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + i*4, 0x0);
				}
			}
		}
}

void draw_circle(Xuint32 BaseAddress, unsigned int radius, unsigned int centarX, unsigned int centarY){
	
	unsigned int i, k, k1, k2, radiusKV;
	double x, xStart, xEnd;
	int y, xBrojac;
	Xuint32 kData;
	// ubaciti kod da start i end budu u ekranu
	radiusKV=radius*radius;
	set_cursor(250);
    print_string(BaseAddress, 1,"f1", 2);
	for(y=-radius;y<=radius;y++){
		if(centarY+y >=0 && centarY+y<=480){
			set_cursor(280);
			print_string(BaseAddress, "f2", 2);
			x = sqrt (radiusKV - y*y);
			xStart=centarX-x;
			if(xStart>=0 && xStart<=640){
				set_cursor(320);
				print_string(BaseAddress, "f3", 2);
				xEnd=centarX+x;
				if(xEnd >=0 && xEnd<=640){
					set_cursor(365);
					print_string(BaseAddress, "f4", 2);
					k1=(int)xStart/20;
					k2=(int)xEnd/20;
					for(k=k1;k<=k2;k++){
						kData=0x0;
						for(xBrojac=((int)xStart)%20;xBrojac<((k!=k2)?32:(((int)xEnd%20)));xBrojac++){
							kData|=(0x80000000>>xBrojac);
						}
						set_cursor(400);
						print_string(BaseAddress, "f5", 2);
						xStart=20;
						i=(centarY+y)*(640/32)+	k;			
						VGA_PERIPH_MEM_mWriteMemory(BaseAddress+GRAPHICS_MEM_OFF+i*4,kData);
					}
				}
			}
		}
	}
}
void draw_circle1(Xuint32 BaseAddress, unsigned int radius, unsigned int centarX, unsigned int centarY){
	
	unsigned int i, k, k1, k2, radiusKV;
	double x, xStart, xEnd;
	int y, yTestEnd, yTestStart;
	Xuint32 kData;
	
	// ubaciti kod da start i end budu u ekranu
	radiusKV=radius*radius;
	yTestStart=centarY-radius;
	yTestEnd=centarY+radius;
	for(y=(yTestStart<0?0:yTestStart); y<=(yTestEnd<0?0:yTestEnd);y++){
		if(centarY+y >=0 && centarY+y<=480){
			x = sqrt (radiusKV - y*y);
			xStart=centarX-x;
			if(xStart>=0 && xStart<=640){
				xEnd=centarX+x;
				if(xEnd >=0 && xEnd<=640){
					k1=(int)xStart/20.0;
					k2=(int)xEnd/20.0;
					for(k=k1;k<=k2;k++){
						kData=0x0;
						for(xBrojac=xStart%20;xBrojac<((k!=k2)?32:(xEnd%20));xBrojac++){
							kData|=(0x80000000>>xBrojac);
						}
						xStart=20;
						i=(centarY+y)*(640/32)+	k;			
						VGA_PERIPH_MEM_mWriteMemory(BaseAddress+GRAPHICS_MEM_OFF+i*4,kData);
					}
				}
			}
		}
	}
}
void draw_circle(Xuint32 BaseAddress, unsigned char radius){
	int y, x, k;
	Xuint32 k;
	int radiusKV = radius * radius;
	
	for (y=-radius;y<=radius;y++){     // 0 - 480
		for (x=-radius;x<=radius;x++){ // 0 - 640
			if (x*x + y*y = radiusKV){
				i=(640/32)*(centarY+y) + (int)((centarX+x)/20.0);
				k= 0x80000000>> ((centarX+x)%20);
				VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + i*4, k);
				setpixel (centarX+x, centarY+y);
			}
		}
	}
	
	for(int y=-radius; y<=radius; y++)
		for(int x=-radius; x<=radius; x++)
			if(x*x+y*y <= radius*radius)
				setpixel(origin.x+x, origin.y+y);
}
*/