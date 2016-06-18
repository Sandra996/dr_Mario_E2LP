#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "xparameters.h"
#include "xio.h"
#include "vga_periph_mem.h"
#include "xil_exception.h"
#include "xintc.h"
#include <math.h>

#define VIRUS_PROSTOR   104
#define VIRUS_PROSTOR_Y   4
#define MAX_TABLE_X 	  8
#define MAX_TABLE_Y		 16
#define MAX_TABLE 		128


#define VERTIKALNA_IVICA 	0x2D
#define HORIZONTALNA_IVICA	0x3C
#define UGAO_GORNJI_DESNI	0x3D
#define UGAO_GORNJI_LEVI	0x3B
#define UGAO_DONJI_LEVI		0x2F
#define UGAO_DONJI_DESNI	0x3A
#define BACKGROUND_ZNAK		0x40
#define PILL1				0x23
#define PILL2				0x24
#define PILL3				0x25
#define PILL4				0x26
#define PILL5				0x21
#define PILL6				0x22
#define VIRUS1				0x27
#define VIRUS2				0x29
#define VIRUS3				0x2B
#define ANIM_VIRUS1			0x28
#define ANIM_VIRUS2			0x2A
#define ANIM_VIRUS3			0x2C

#define POCETAK_TABLE_X  16
#define POCETAK_TABLE_Y   8

#define LOW 	40
#define MIDDLE  27
#define HIGH    14



XIntc Intc;
enum Stanja {
		ON,
		OFF,
		PADANJE,
		SREDJIVANJE,
		SREDJIVANJE_PADANJE,
		SREDJIVANJE_SREDJIVANJE,
		KRAJ
	} stanje;

unsigned char sNEXT_PILL[] = "NEXT PILL";
unsigned char sDR_MARIO[] = "DR MARIO";
unsigned char sLEVEL[] = "LEVEL";
unsigned char sSPEED[] = "SPEED";
unsigned char sVIRUS[] = "VIRUS";
unsigned char sSCORE[] = "SCORE";

const unsigned char backround[30][40]={
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,4,2,2,  2,2,2,2,2,  2,6,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,3,0,0,  0,0,0,0,0,  0,3,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,7,2,2,  2,2,2,2,2,  2,5,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},

		{1,1,4,2,2,  2,2,2,2,2,  6,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,4,2,2,2,  2,2,2,2,2,  2,2,2,6,1},
		{1,1,3,0,0,  0,0,0,0,0,  3,1,1,1,1,  1,1,1,1,4,  6,1,1,1,1,  1,3,0,0,0,  0,0,0,0,0,  0,0,0,3,1},
		{1,1,3,0,0,  0,0,0,0,0,  3,1,1,1,1,  4,2,2,2,5,  7,2,2,2,6,  1,3,0,0,0,  0,0,0,0,0,  0,0,0,3,1},
		{1,1,3,0,0,  0,0,0,0,0,  3,1,1,1,1,  3,0,0,0,0,  0,0,0,0,3,  1,3,0,0,0,  0,0,0,0,0,  0,0,0,3,1},
		{1,1,3,0,0,  0,0,0,0,0,  3,1,1,1,1,  3,0,0,0,0,  0,0,0,0,3,  1,3,0,0,0,  0,0,0,0,0,  0,0,0,3,1},

		{1,1,3,0,0,  0,0,0,0,0,  3,1,1,1,1,  3,0,0,0,0,  0,0,0,0,3,  1,3,0,0,0,  0,0,0,0,0,  0,0,0,3,1},
		{1,1,7,2,2,  2,2,2,2,2,  5,1,1,1,1,  3,0,0,0,0,  0,0,0,0,3,  1,7,2,2,2,  2,2,2,2,2,  2,2,2,5,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  3,0,0,0,0,  0,0,0,0,3,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  3,0,0,0,0,  0,0,0,0,3,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  3,0,0,0,0,  0,0,0,0,3,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},

		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  3,0,0,0,0,  0,0,0,0,3,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  3,0,0,0,0,  0,0,0,0,3,  1,4,2,2,2,  2,2,2,2,2,  2,2,6,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  3,0,0,0,0,  0,0,0,0,3,  1,3,0,0,0,  0,0,0,0,0,  0,0,3,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  3,0,0,0,0,  0,0,0,0,3,  1,3,0,0,0,  0,0,0,0,0,  0,0,3,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  3,0,0,0,0,  0,0,0,0,3,  1,3,0,0,0,  0,0,0,0,0,  0,0,3,1,1},

		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  3,0,0,0,0,  0,0,0,0,3,  1,3,0,0,0,  0,0,0,0,0,  0,0,3,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  3,0,0,0,0,  0,0,0,0,3,  1,3,0,0,0,  0,0,0,0,0,  0,0,3,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  3,0,0,0,0,  0,0,0,0,3,  1,3,0,0,0,  0,0,0,0,0,  0,0,3,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  3,0,0,0,0,  0,0,0,0,3,  1,3,0,0,0,  0,0,0,0,0,  0,0,3,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  7,2,2,2,2,  2,2,2,2,5,  1,7,2,2,2,  2,2,2,2,2,  2,2,5,1,1},

		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
};

	unsigned long interruptBrojac;
	unsigned long interruptAnim;
    unsigned int lvl; 	// 0d 0 do 20
    unsigned char speed; 	// od 1 do 3
    unsigned char nBugs;
    unsigned int  score;
    unsigned int  s_help;
    unsigned char s_nBugs[2];
    unsigned char s_lvl[2];
    unsigned char s_score[5];
    unsigned char s_low[]="LO";
    unsigned char s_middle[]="MI";
    unsigned char s_high[] = "HI";

    unsigned char table[MAX_TABLE_Y][MAX_TABLE_X]; // double array 16 uspravno i 8 vodoravno
    unsigned char currentPill[5]={0,0,0,0,0}; // prve 2 vrednosti pilula, druge dve pozicija prve kocke u piluli, 5ta orjentacija
    unsigned char newPill[2]	={0,0};

	unsigned char levelBiranje[]={" Level "};
	unsigned char speedBiranje[]={" Speed "};

	unsigned int prekidac;

	//za drawingTable funkciju
	int x,y;
	unsigned char boja;
	unsigned char znak;
	unsigned char pill;
	unsigned char animatedV;

	XStatus Status;

	//za stanje SREDJIVANJE
	unsigned char br_gore1;
	unsigned char br_dole1;
	unsigned char br_levo1;
	unsigned char br_desno1;
	unsigned char br_gore2;
	unsigned char br_dole2;
	unsigned char br_levo2;
	unsigned char br_desno2;

	unsigned char int_to_string[5];


//void print(char *str);
void creatingCurrentNextPills();
void clearTable();
void fillTableWBugs();
void newPillF();
void drawTable();
void drawBackground();
void initializingPlatform();
void clearingPill6();
void drawGameState();
void drawStaticGameMessages();
unsigned char sredjivanje();
//unsigned char proveraGore(unsigned char kordY,unsigned char kordX, unsigned char bojaProvere);
unsigned char proveraDole(unsigned char kordY,unsigned char kordX, unsigned char bojaProvere);
//unsigned char proveraLevo(unsigned char kordY,unsigned char kordX, unsigned char bojaProvere);
unsigned char proveraDesno(unsigned char kordY,unsigned char kordX, unsigned char bojaProvere);
void ponistavanje(unsigned char kordYp, unsigned char kordXp, unsigned char kordYk, unsigned char kordXk);


void my_timer_interrupt_handler(void * baseaddr_p) {
	//drawing screen and counting interrupts
	interruptAnim++;
	interruptBrojac++;
	drawTable();
	drawGameState();
}

int main()
{
	//----------------------------registering variables----------------------------

	int i;
	unsigned char ponisteno;
	unsigned char pillPomoc;
	unsigned char palo;
	unsigned char debouncer;
	unsigned char default_speed;

	interruptBrojac = 0;
	interruptAnim   = 0;
	stanje 			= OFF;
	animatedV		= 0;
	debouncer 		= 0;
	lvl 			= 0;
	score 			= 0;

	s_nBugs[0]		='0';
	s_nBugs[1]		='0';
	for(i=0;i<5;i++)
		s_score[i]	='0';
	s_lvl[0]		='0';
	s_lvl[1]		='0';

	//----------------------------Initializing platform-------------------------------------
	initializingPlatform();
	//print("asdf");
	//xil_printf("jkl;");

	srand(interruptBrojac);
	//============================Infinite loop for game==================================
	while (1){
		prekidac = Xil_In8(XPAR_MY_PERIPHERAL_0_BASEADDR);//iscitavanje prekidaca


		//==============================================  LEVO/DESNO/ROTIRANJE  ==============================================
		if((stanje == PADANJE) && (prekidac != (!0))){
			switch(prekidac){
					//-------- GORE ----------
					case 0x0F: if(debouncer == 0){
									//xil_printf("\n prekidac UP\n\r");
								}
								break;
					//-------- LEVO ----------
					case 0x1D: if(debouncer == 0){//up
									debouncer = 1;
									//xil_printf("\n prekidac LEVO\n\r");
									if((!table[currentPill[2]][currentPill[3] - 1]) && (currentPill[3]>0)){//prazno levo i provera granice ulevo
										if(currentPill[4] && !table[currentPill[2] - 1][currentPill[3] - 1]){//pillula je vertikalna i za drugu slobodno levo
											table[currentPill[2]]	[currentPill[3]] 	 = 0x00;//brisanje pozicije trenutne pilule vertikalno
											table[currentPill[2]-1]	[currentPill[3]] 	 = 0x00;
											table[currentPill[2]]   [currentPill[3] - 1] = currentPill[0];//upis trenutne pilule 1 u novu poziciju
											table[currentPill[2]-1] [currentPill[3] - 1] = currentPill[1];//upis trenutne pilule 2 u novu poziciju
											currentPill[3]--; // nova pozicija trenutne pilule
										}else{//pilula je horizontalna
											table[currentPill[2]][currentPill[3]+1]		 = 0x00;//brisanje pozicije trenutne pilule horizontalno
											table[currentPill[2]][currentPill[3]-1]		 = currentPill[0]; // upis trenutne pilule 1 u novu poziciju
											table[currentPill[2]][currentPill[3]]		 = currentPill[1];
											currentPill[3]--; //nova pozicija trenutne pilule
										}
									}
								}
								break;
					//--------- DESNO ----------
					case 0x17: if(debouncer == 0){
									debouncer = 1;
									//xil_printf("\n prekidac DESNO\n\r");
									//---- HORIZONTALNO ------ prazno desno za 2 i provera granice desno
									if((!table[currentPill[2]][currentPill[3] + 2]) && (currentPill[3] < MAX_TABLE_X - 2) && !currentPill[4]){
											table[currentPill[2]][currentPill[3]] = 0x00;//brisanje prethodne pozicije
											table[currentPill[2]][currentPill[3] + 1] = currentPill[0];//upis u novu poziciju
											table[currentPill[2]][currentPill[3] + 2] = currentPill[1];
											currentPill[3]++;//nova pozicija trenutne pilule
										}
									if((!table[currentPill[2]][currentPill[3] + 1]) && (currentPill[3] < MAX_TABLE_X - 1) && !table[currentPill[2]-1][currentPill[3]+1]){
										table[currentPill[2]]	[currentPill[3]] 		 = 0x00;//brisanje trenutne pilule 1 vertikalno
										table[currentPill[2]-1] [currentPill[3]]		 = 0x00;
										table[currentPill[2]]	[currentPill[3] + 1]	 = currentPill[0];//upis trenutne pilule 1 u novu poziciju
										table[currentPill[2]-1] [currentPill[3] + 1]	 = currentPill[1];
										currentPill[3]++;//nova pozicija trenutne pilule
									}
									}
								break;
					// --------- ROTIRANJE ----------
					case 0x1B: if(debouncer == 0){
									//xil_printf("\n prekidac ROTIRANJE\n\r");
									debouncer = 1;
									if(currentPill[4]){//VERTIKALNO
										if(!table[currentPill[2]][currentPill[3] + 1]){//prazna pozicija za rotiranje
											table[currentPill[2]-1][currentPill[3]]     = 0x00;
											table[currentPill[2]]  [currentPill[3] + 1] = (currentPill[1] & 0x18) + 2;
											table[currentPill[2]]  [currentPill[3]] 	= (currentPill[0] & 0x18) + 1;
											currentPill[4] = 0;
											currentPill[0] = table[currentPill[2]] [currentPill[3]];
											currentPill[1] = table[currentPill[2]] [currentPill[3] + 1];
										}
									}else{ // HORIZONTALNO
										if(!table[currentPill[2] - 1] [currentPill[3] - 1]){//pozicija slobodna za rotiranje
											table[currentPill[2]]    [currentPill[3] + 1] = 0x00;
											table[currentPill[2]]	 [currentPill[3]]	  = (currentPill[1] & 0x18) + 4;
											table[currentPill[2] - 1][currentPill[3]] 	  = (currentPill[0] & 0x18) + 3;
											currentPill[4] = 1; // sad je vertikalna
											currentPill[0] = table[currentPill[2]]	  [currentPill[3]];
											currentPill[1] = table[currentPill[2] - 1][currentPill[3]];
										}
									}
								}
								break;
					default: debouncer = 0;
			}
		}

		//==============================================    OFF    ==============================================
		while(stanje==OFF){
			//biranje levela, brzine i startovanje
			//prilikom startovanja menja se stanje
			speed = (lvl < 8)? LOW :
									(lvl < 14) ? MIDDLE : HIGH;
			nBugs 	= lvl*4+4;
			s_nBugs[1] = (nBugs%10) + 0x30;
			s_nBugs[0] = (nBugs/10)%10 + 0x30;
			stanje 	= ON;
			default_speed = speed;


			drawBackground();
			drawStaticGameMessages();
			//xil_printf("\n stanje OFF\n\r");
		}

		//==============================================    ON    ==============================================
		if(stanje==ON){
			//priprema tabele, trenutne i sledece pilule, varijable
			//STARTOVANJE igre
			clearTable(table);
			fillTableWBugs(table,nBugs);
			creatingCurrentNextPills();
			stanje 	= PADANJE;
			//xil_printf("\n stanje ON\n\r");
		}

		//==============================================  PADANJE  ==============================================
		if((stanje == PADANJE) && (speed < interruptBrojac)){
			if(prekidac == 0x1E){// ubrzano padanje
				//xil_printf("\n prekidac DOWN\n\r");
				speed = 8;
			}else{
				speed = default_speed;
			}
			// kad pilule padaju bez promene polozaja ili rotiranja
			// do "sudara" kad se prelazi na stanje SREDJIVANJE
			if((!table[currentPill[2]+1][currentPill[3]]) && (currentPill[2]+1<MAX_TABLE_Y)){
				if(!currentPill[4]){//horizontal
					if(!table[currentPill[2]+1][currentPill[3]+1]){
						table[currentPill[2]]  [currentPill[3]]		= 0;
						table[currentPill[2]]  [currentPill[3]+1]	= 0;
						table[currentPill[2]+1][currentPill[3]] 	= currentPill[0];
						table[currentPill[2]+1][currentPill[3]+1]	= currentPill[1];
						currentPill[2]++;
					}else stanje = SREDJIVANJE;
				}else{//vertical
					table[currentPill[2]]  [currentPill[3]]	= currentPill[1];
					table[currentPill[2]-1][currentPill[3]]	= 0;
					table[currentPill[2]+1][currentPill[3]] = currentPill[0];
					currentPill[2]++;
				}
			}else stanje = SREDJIVANJE;
			interruptBrojac 	= 0;
			//xil_printf("\n stanje PADANJE\n\r");
		}

		//==============================================  SREDJIVANJE  ==============================================
		if(stanje == SREDJIVANJE){
			ponisteno = sredjivanje();
			clearingPill6();
			//proveriti dali je ponisteno nesto -> dalji tok
			if(ponisteno){
				stanje = SREDJIVANJE_PADANJE;
				ponisteno = 0;
				//xil_printf("\n sledece stanje SREDJIVANJE_PADANJE\n\r");
			}else{
				newPillF();
				stanje = PADANJE;
				//xil_printf("\n sledece stanje PADANJE\n\r");
			}
			//xil_printf("\n u stanju SREDJIVANJE\n\r");
		}

		//==============================================  SREDJIVANJE_PADANJE  ==============================================
		if((stanje == SREDJIVANJE_PADANJE) && (interruptAnim >11)){
			//krece odozdo ka gore
			//promenjiva koja proverava dali je bilo padanja - paziti na resetovanje iste
			palo = 0;
			//xil_printf("\n u stanju SREDJIVANJE_PADANJE\n\r");
			for(y = MAX_TABLE_Y - 2; y >= 0; y--){
				for(x = 0; x < MAX_TABLE_X; x++){
					pillPomoc = table[y][x] & 0x7;
					if(pillPomoc){//nije virus i nije praznina pa obradjujemo u suprotnom "skip"
						if(!table[y+1][x]){//nema nista ispod pa proverava u suprotnom "skip"
							switch(pillPomoc){
								case 1: if((x+1)==MAX_TABLE_X);
											//xil_printf("\n stanje SREDJIVANJE_PADANJE - greska- case 1: "
											//		   "drugi element izlazi iz tabele\n\r");
										else if(!table[y+1][x+1]){//ako nema nista ispod - drugog padanje
													table[y+1][x] 	= table[y][x];
													table[y+1][x+1]	= table[y][x+1];
													table[y][x] 	= 0x00;
													table[y][x+1] 	= 0x00;
													palo++;
												}
										break;
								/*case 2: xil_printf("\n stanje SREDJIVANJE_PADANJE - mozda greska- case 2: "
										   	   	   "ovaj se sa case1 trebao pasti ili ispod prvog ima nesto\n\r");
										break;
								case 3: xil_printf("\n stanje SREDJIVANJE_PADANJE - greska- case 3: "
												   "nije ponisten ili padanje nesredjuje dobro\n\r");
										break;
										*/
								case 4: table[y+1][x] 	= table[y][x];
										table[y][x]		= table[y-1][x];
										table[y-1][x] 	= 0x00;
										palo++;
										break;
								case 5: table[y+1][x] 	= table[y][x];
										table[y][x]   	= 0x00;
										palo++;
										break;
							}
						}
					}
				}
			}
			if(!palo)//ostaje stanje SREDJIVANJE_PADANJE sve dok sve nepadne a onda ide SREDJIVANJE palog
				stanje = SREDJIVANJE;
		}
		/*
		//==============================================  SREDJIVANJE_SREDJIVANJE  ==============================================
		if(stanje == SREDJIVANJE_SREDJIVANJE){
			//xil_printf("\n stanje SREDJIVANJE_SREDJIVANJE\n\r");
			ponisteno = sredjivanje();
			if(ponisteno)
				stanje = SREDJIVANJE_PADANJE;
			else{
				newPillF();
				stanje = PADANJE;
			}
		}
		*/

		//----------- zavrsen lvl --- sledeci lvl ------------------------
		if(!nBugs && (stanje!=KRAJ)){
			lvl++;
			s_lvl[1] = (lvl%10) + 0x30;
			s_lvl[0] = (lvl/10)%10 + 0x30;
			stanje = OFF;

		}
		//==============================================    KRAJ    ==============================================
		if(stanje == KRAJ){
			//xil_printf("\n stanje KRAJ\n\r");
			stanje = OFF;
		}

		//==============================================  ANIMACIJA  ==============================================
		if(interruptAnim > 12){
			animatedV=!animatedV;
			interruptAnim = 0;
		}
	}

    cleanup_platform();
    return 0;
}

void drawGameState(){
	// SCORE
	set_cursor(9*40 + 4);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 3, s_score, 5);

    // NEXT PILL
    set_cursor(9*40 + 32);
	print_char(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, (newPill[0] & 0x18) >> 3, PILL1);
	set_cursor(9*40 + 33);
	print_char(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, (newPill[1] & 0x18) >> 3, PILL2);

	//LEVEL
	set_cursor(18*40 + 34);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 3, s_lvl, 2);

    //SPEED
    set_cursor(20*40 + 34);
	switch(speed){
		case LOW:    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 3, s_low, 2);break;
		case MIDDLE: print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 3, s_middle, 2);break;
		case HIGH:   print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 3, s_high, 2);break;
	}

    //VIRUS
	set_cursor(22*40 + 34);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 3, s_nBugs, 2);
}

void drawStaticGameMessages(){
	//SCORE
	set_cursor(7*40 + 4);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 1, sSCORE, 5);

	// DR MARIO
	set_cursor(2*40 + 28);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 1, sDR_MARIO, 8);

	// NEXT PILL
    set_cursor(7*40 + 28);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 1, sNEXT_PILL, 9);

	//LEVEL SPEED VIRUS
	set_cursor(18*40 + 28);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 1, sLEVEL, 5);
    set_cursor(20*40 + 28);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 1, sSPEED, 5);
    set_cursor(22*40 + 28);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 1, sVIRUS, 5);


}

unsigned char sredjivanje(){
	//xil_printf("\n funk sredjivanje\n\r");
	unsigned char iX,iY;
	unsigned char brojIstih;
	unsigned char bojaPomoc;
	unsigned char ponisteno;
	ponisteno = 0;

	for(iX = 0;iX < MAX_TABLE_X;iX++){
		for(iY = 0;iY < MAX_TABLE_Y; iY++){
			if(table[iY][iX]){
				bojaPomoc = (table[iY][iX] & 0x18) >> 3;
				brojIstih = proveraDole(iY, iX, bojaPomoc);
				xil_printf("\n br istih\n\r %d",brojIstih);
				if(brojIstih + 1 > 3){
					ponistavanje(iY, iX, iY + brojIstih, iX);
					ponisteno++;
				}
				iY+=(brojIstih);
			}
		}
	}
	for(iY=0; iY < MAX_TABLE_Y; iY++){
		for(iX=0; iX < MAX_TABLE_X; iX++){
			if(table[iY][iX]){
				bojaPomoc = (table[iY][iX] & 0x18) >> 3;
				brojIstih = proveraDesno(iY, iX, bojaPomoc);
				xil_printf("\n br istih\n\r %d",brojIstih);
				if(brojIstih + 1 > 3){
					ponistavanje(iY, iX, iY, iX + brojIstih);
					ponisteno++;
				}
				iX+=(brojIstih);
			}
		}
	}
	return ponisteno;
}

void ponistavanje(unsigned char kordYp, unsigned char kordXp, unsigned char kordYk, unsigned char kordXk){
	unsigned char i, pillPomoc, bojaPomoc;
	//xil_printf("\n ulaz funkcija ponistavanje\n\r %d, %d do %d, %d",kordYp,kordXp, kordYk,kordXk);

	if(kordXp == kordXk){//vertikalno ponistavanje
		//xil_printf("\n vertikalno\n\r");
		for(i = kordYp; i <= kordYk; i++){
			//if(!table[i][kordXp]){// da omoguci drugi prolaz preko centra
				pillPomoc = table[i][kordXp] & 0x07;
				bojaPomoc = table[i][kordXp] & 0x18;
				table[i][kordXp] = bojaPomoc | 6;
				switch(pillPomoc){
					case 0: if(bojaPomoc){//virus
								xil_printf("\n virus\n\r");
								score++;
								s_help = score;
								for(i=0;i<5;i++){
									s_score[4-i] = (s_help%10) + 0x30;
									s_help 	   = s_help/10;
								}
								nBugs--;
								s_nBugs[1] = (nBugs%10) + 0x30;
								s_nBugs[0] = (nBugs/10)%10 + 0x30;
							}
							break;
					case 1: bojaPomoc = table[i][kordXp+1] & 0x18;
							table[i][kordXp+1] = bojaPomoc | 5;
							break;
					case 2: bojaPomoc = table[i][kordXp-1] & 0x18;
							table[i][kordXp-1] = bojaPomoc | 5;
							break;
					case 3: if(i == kordYk){
								bojaPomoc = table[i+1][kordXp] & 0x18;
								table[i+1][kordXp] = bojaPomoc | 5;
							}
							break;
					case 4: if(i == kordYp){
								bojaPomoc = table[i-1][kordXp] & 0x18;
								table[i-1][kordXp] = bojaPomoc | 5;
							}
				}
			//}
		}
	}
	if(kordYp == kordYk){//horizontalno ponistavanje
		//xil_printf("\n ulaz funkcija ponistavanje horizontalno\n\r");
		for(i = kordXp; i <= kordXk; i++){
			//if(!table[kordYp][i]){//da omoguci drugi prolaz preko centra
				pillPomoc = table[kordYp][i] & 0x07;
				bojaPomoc = table[kordYp][i] & 0x18;
				table[kordYp][i] = bojaPomoc + 6;
				switch(pillPomoc){
					case 0: if(bojaPomoc){//virus
								score++;
								s_help = score;
								for(i=0;i<5;i++){
									s_score[4-i] = (s_help%10) + 0x30;
									s_help 	   = s_help/10;
								}
								nBugs--;
								s_nBugs[1] = (nBugs%10)    + 0x30;
								s_nBugs[0] = (nBugs/10)%10 + 0x30;
							}
							break;
					case 1: if(i == kordXk){
								bojaPomoc = table[kordYp][i+1] & 0x18;
								table[kordYp][i+1] = bojaPomoc | 5;
							}
							break;
					case 2: if(i == kordXp){
								bojaPomoc = table[kordYp][i-1] & 0x18;
								table[kordYp][i-1] = bojaPomoc | 5;
							}
							break;
					case 3: bojaPomoc = table[kordYp+1][i] & 0x18;
							table[kordYp+1][i] = bojaPomoc | 5;
							break;
					case 4: bojaPomoc = table[kordYp-1][i] & 0x18;
							table[kordYp-1][i] = bojaPomoc | 5;
							break;
					}
			//}
		}
	}
	//xil_printf("\n ulaz funkcija ponistavanje gotovo\n\r");
}
void clearingPill6(){
	unsigned char iX, iY;
	//xil_printf("\n clearingPill6\n\r");
	for(iY = 0; iY < MAX_TABLE_Y; iY++){
		for(iX = 0; iX < MAX_TABLE_X; iX++){
			if((table[iY][iX] & 0x07) == 6)
				table[iY][iX] = 0x00;
		}
	}
}
/*
unsigned char proveraGore(unsigned char kordY,unsigned char kordX, unsigned char bojaProvere){
	//xil_printf("\n ulaz funkcija proveraGore\n\r");
	unsigned char i;
	unsigned char br_gore;
	unsigned char bojaGore;
	unsigned char bojaPodudara;

	i=0;
	br_gore = 0;
	bojaPodudara = 1;

	do{
		++i;
		if((kordY-1)>= 0){
			bojaGore = (table[kordY-i][kordX] & 0x18) >> 3;
			if(bojaGore == bojaProvere)
				br_gore++;
			else
				bojaPodudara = 0;
		}else
			bojaPodudara = 0;
	}while(bojaPodudara);

	return br_gore;
}
*/
unsigned char proveraDole(unsigned char kordY,unsigned char kordX, unsigned char bojaProvere){
	//xil_printf("\n ulaz funkcija proveraDole\n\r");
	unsigned char i;
	unsigned char br_dole;
	unsigned char bojaDole;
	unsigned char bojaPodudara;


	i=0;
	br_dole = 0;
	bojaPodudara = 1;

	do{
		++i;
		if((kordY+i) <= MAX_TABLE_Y){
			bojaDole = (table[kordY+i][kordX] & 0x18) >> 3;
			if(bojaDole == bojaProvere)
				br_dole++;
			else
				bojaPodudara = 0;
		}else
			bojaPodudara = 0;
	}while(bojaPodudara);
	return br_dole;
}
/*
unsigned char proveraLevo(unsigned char kordY,unsigned char kordX, unsigned char bojaProvere){
	//xil_printf("\n ulaz funkcija proveraLevo\n\r");
	unsigned char i;
	unsigned char br_levo;
	unsigned char bojaLevo;
	unsigned char bojaPodudara;

	i=0;
	br_levo = 0;
	bojaPodudara = 1;

	do{
		++i;
		if((kordX-i) >= 0){
			bojaLevo = (table[kordY][kordX-i] & 0x18) >> 3;
			if(bojaLevo == bojaProvere)
				br_levo++;
			else
				bojaPodudara = 0;
		}else
			bojaPodudara = 0;
	}while(bojaPodudara);
	return br_levo;
}
*/
unsigned char proveraDesno(unsigned char kordY,unsigned char kordX, unsigned char bojaProvere){
	//xil_printf("\n ulaz funkcija proveraDesno\n\r");
	unsigned char i;
	unsigned char br_desno;
	unsigned char bojaDesno;
	unsigned char bojaPodudara;

	i=0;
	br_desno = 0;
	bojaPodudara = 1;

	do{
		++i;
		if((kordX+i) <= MAX_TABLE_X){
			bojaDesno = (table[kordY][kordX+i] & 0x18) >> 3;
			if(bojaDesno == bojaProvere)
				br_desno++;
			else
				bojaPodudara = 0;
		}else
			bojaPodudara = 0;
	}while(bojaPodudara);
	return br_desno;
}

void creatingCurrentNextPills(){
	//xil_printf("\n ulaz funkcija creatingCurrentNextPills\n\r");
	currentPill[0]	= ((rand()%3 + 1) << 3) + 1;
	currentPill[1]	= ((rand()%3 + 1) << 3) + 2;
	currentPill[2] 	= 0;
	currentPill[3] 	= 3;
	currentPill[4]	= 0;
	newPill[0] 		= ((rand()%3 + 1) << 3) + 1;
	newPill[1] 		= ((rand()%3 + 1) << 3) + 2;
	table[0][3]		= currentPill[0];
	table[0][4]		= currentPill[1];
}

void newPillF(){
	//xil_printf("\n ulaz funkcija newPillF\n\r");
	if( (!table[0][3]) || (!table[0][4])){
		currentPill[0]	= newPill[0];
		currentPill[1]	= newPill[1];
		currentPill[2] 	= 0;
		currentPill[3] 	= 3;
		currentPill[4]	= 0;
		newPill[0] 	= ((rand()%3 + 1) << 3) + 1;
		newPill[1] 	= ((rand()%3 + 1) << 3) + 2;
		table[0][3]		= currentPill[0];
		table[0][4]		= currentPill[1];
	}
	else
		stanje = KRAJ;
}

void clearTable(){
	//xil_printf("\n ulaz funkcija clearTable\n\r");
	int i1,i2;
	for(i1=0;i1<MAX_TABLE_Y;i1++)
		for(i2=0;i2<MAX_TABLE_X;i2++){
			table[i1][i2]=0;
		}
}

void fillTableWBugs(){
	//xil_printf("\n ulaz funkcija fillTableWBugs\n\r");

	int i;
	int iStart;
	int iY,iX;


	int nasao;
	int nBugsC = nBugs;

	iStart = VIRUS_PROSTOR_Y * MAX_TABLE_X;

	while (nBugsC){
		i = iStart + rand()%VIRUS_PROSTOR;
		nasao=0;
		do{
			iY = floor(i/MAX_TABLE_X);
			iX = i - iY * MAX_TABLE_X;
			if(table[iY][iX] == 0){
				table[iY][iX] = (((unsigned char)rand())%3 + 1)<<3;
				nBugsC--;
				nasao++;
			}
			else
				(i==MAX_TABLE) ? i = iStart: i++;
		}while(!nasao);
	}
}

void drawBackground(){
	//xil_printf("\n ulaz funkcija drawBackground\n\r");
	for(y=0; y<30; y++)
		for(x=0; x<40; x++){
			set_cursor(y*40+x);
			boja = 0;
			switch(backround[y][x]){
				case 0: znak = ' '; break;
				case 1: znak = BACKGROUND_ZNAK;    break;
				case 2: znak = HORIZONTALNA_IVICA; break;
				case 3: znak = VERTIKALNA_IVICA;   break;
				case 4: znak = UGAO_GORNJI_LEVI;   break;
				case 5: znak = UGAO_DONJI_DESNI;   break;
				case 6: znak = UGAO_GORNJI_DESNI;  break;
				case 7: znak = UGAO_DONJI_LEVI;    break;
			}
			print_char(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, boja, znak);
		}
}

void drawTable(){
	for(y=POCETAK_TABLE_Y; y<POCETAK_TABLE_Y + MAX_TABLE_Y; y++){
		for(x=POCETAK_TABLE_X;x<POCETAK_TABLE_X + MAX_TABLE_X; x++){
					    set_cursor(y*40+x);
					    //xil_printf("\n DRAWING TABLE\n");
						boja = table[y - POCETAK_TABLE_Y][x - POCETAK_TABLE_X] & 0x18;
						pill = table[y - POCETAK_TABLE_Y][x - POCETAK_TABLE_X] & 0x7;
						boja = boja >> 3;
						if(boja){
							if(pill){
								switch (pill){
									case 1: znak = PILL1; break;
									case 2: znak = PILL2; break;
									case 3: znak = PILL3; break;
									case 4: znak = PILL4; break;
									case 5: znak = PILL5; break;
									case 6: znak = PILL6; break;
								}
								//xil_printf("\n CHOOSING PILL\n");
							}
							else{
								if(animatedV){
									switch (boja){
										case 1: znak = ANIM_VIRUS1; break;
										case 2: znak = ANIM_VIRUS2; break;
										case 3: znak = ANIM_VIRUS3; break;
									}
									//xil_printf("\n CHOOSING VIRUS ANIM\n");
								}
								else{
									switch (boja){
										case 1: znak = VIRUS1; break;
										case 2: znak = VIRUS2; break;
										case 3: znak = VIRUS3; break;
									}
								//xil_printf("\n CHOOSING VIRUS\n");
								}
							}
						}
						else
							znak=' ';
						print_char(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, boja, znak);
		}
	}

}

void initializingPlatform(){
	//xil_printf("\n ulaz funkcija initializingPlatform\n \r");
    init_platform();

	Status = XIntc_Initialize (&Intc, XPAR_INTC_0_DEVICE_ID);
/*	if (Status != XST_SUCCESS)
		xil_printf ("\r\nInterrupt controller initialization failure");
	else
		xil_printf("\r\nInterrupt controller initialized");
*/
	// Connect my_timer_interrupt_handler
	Status = XIntc_Connect (&Intc, XPAR_INTC_0_DEVICE_ID,
							(XInterruptHandler) my_timer_interrupt_handler,(void *)0);
	/*if (Status != XST_SUCCESS)
		xil_printf ("\r\nRegistering MY_TIMER Interrupt Failed");
	else
		xil_printf("\r\nMY_TIMER Interrupt registered");
*/
	//start the interrupt controller in real mode
	Status = XIntc_Start(&Intc, XIN_REAL_MODE);
	//enable interrupt controller
	XIntc_Enable (&Intc, XPAR_INTC_0_DEVICE_ID);
	microblaze_enable_interrupts();

    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x00, 0x0);// direct mode   0
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x04, 0x3);// display_mode  1
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x08, 0x1);// show frame      2
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x0C, 0x1);// font size       3
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x10, 0x2D1E4B);// foreground 4 tamno ljubicasta/plava 0xFF0000-crvena
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x14, 0x00FF00);// foreground 4 green
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x18, 0x0000FF);// foreground 4 plava
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x1c, 0xffff00);// foreground 4 zuta
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x20, 0x000000);// background color crna
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x24, 0xFF0000);// frame color      6

    //xil_printf("\n Dr. Mario starting\n\r");
    //TODO Debug.
    clear_text_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);
    clear_graphics_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);

}
