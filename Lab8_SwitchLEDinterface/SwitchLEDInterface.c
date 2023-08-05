// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// January 15, 2016
//      Jon Valvano and Ramesh Yerraballi

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
unsigned long SW1;

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void PortE_Init(void);
void Delay100ms(unsigned long int);

// ***** 3. Subroutines Section *****

// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).
int main(void){ 
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);  // activate grader and set system clock to 80 MHz
	PortE_Init();
	
  EnableInterrupts();           // enable interrupts for the grader
  while(1){
    SW1=GPIO_PORTE_DATA_R &0x01;
		if(SW1){
			Delay100ms(1);
			GPIO_PORTE_DATA_R^=0x02; //red led off and on (0010) (0000)
		}else{
			GPIO_PORTE_DATA_R |=0x02; //or=setting, 0x02 and 0x01 = 0x11 (ON)
		}
  }
  
}

void PortE_Init(void){
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000010; // activate clock for Port E
	delay = SYSCTL_RCGC2_R; // allow time for clock to start
	GPIO_PORTE_AMSEL_R = 0x00; // disable analog on PE
	GPIO_PORTE_PCTL_R = 0x00000000; // PCTL GPIO on PE1-0
	GPIO_PORTE_DIR_R = 0x00000002; //red port (OUTPUT)
	GPIO_PORTE_AFSEL_R = 0x00; // not alternative
	GPIO_PORTE_DEN_R |= 0x03; // enable digital I/O on PE0-PE1
	GPIO_PORTE_DATA_R |= 0x03; // enable digital I/O on PF4-0
}

void Delay100ms(unsigned long int ms){
	unsigned long time;
	while(ms > 0){
	time = 1333333; // this number means 100ms
		while(time > 0){
			time--; //time = time - 1;
		}
	ms--; // decrements every 1 ms
	}
}

// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E