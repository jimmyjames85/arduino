#include "ATmega2560_PinMap.h"

int * digitalPortAddresses[] = {&PE0, &PE1, &PE4, &PE5, &PG5, &PE3, &PH3, &PH4, &PH5, &PH6, &PB4, &PB5, &PB6, &PB7, &PJ1, &PJ0, &PH1, &PH0, &PD3, &PD2, &PD1, &PD0, &PA0, &PA1, &PA2, &PA3, &PA4, &PA5, &PA6, &PA7, &PC7, &PC6, &PC5, &PC4, &PC3, &PC2, &PC1, &PC0, &PD7, &PG2, &PG1, &PG0, &PL7, &PL6, &PL5, &PL4, &PL3, &PL2, &PL1, &PL0, &PB3, &PB2, &PB1, &PB0};

int * getPortAddr(unsigned arduinoPin)
{
/*
	dpin_00 PE0
	dpin_01 PE1
	dpin_02 PE4
	dpin_03 PE5
	dpin_04 PG5
	dpin_05 PE3
	dpin_06 PH3
	dpin_07 PH4
	dpin_08 PH5
	dpin_09 PH6
	dpin_10 PB4
	dpin_11 PB5
	dpin_12 PB6
	dpin_13 PB7
	dpin_14 PJ1
	dpin_15 PJ0
	dpin_16 PH1
	dpin_17 PH0
	dpin_18 PD3
	dpin_19 PD2
	dpin_20 PD1
	dpin_21 PD0
	dpin_22 PA0
	dpin_23 PA1
	dpin_24 PA2
	dpin_25 PA3
	dpin_26 PA4
	dpin_27 PA5
	dpin_28 PA6
	dpin_29 PA7
	dpin_30 PC7
	dpin_31 PC6
	dpin_32 PC5
	dpin_33 PC4
	dpin_34 PC3
	dpin_35 PC2
	dpin_36 PC1
	dpin_37 PC0
	dpin_38 PD7
	dpin_39 PG2
	dpin_40 PG1
	dpin_41 PG0
	dpin_42 PL7
	dpin_43 PL6
	dpin_44 PL5
	dpin_45 PL4
	dpin_46 PL3
	dpin_47 PL2
	dpin_48 PL1
	dpin_49 PL0
	dpin_50 PB3
	dpin_51 PB2
	dpin_52 PB1
	dpin_53 PB0


	apin_00 PF0
	apin_01 PF1
	apin_02 PF2
	apin_03 PF3
	apin_04 PF4
	apin_05 PF5
	apin_06 PF6
	apin_07 PF7
	apin_08 PK0
	apin_09 PK1
	apin_10 PK2
	apin_11 PK3
	apin_12 PK4
	apin_13 PK5
	apin_14 PK6
	apin_15 PK7


*/
	return 0;
}

int * getBitMask(unsigned arduinoPin)
{
	return 0;
}



