    /******************************************************************************/
/*                                                                            */
/*  Project:     Control d'una mina						                      */
/*  Module:                                                                   */
/*  Description:                                                              */
/*                                                                            */
/*  Author:	Manuel Pol                                                        */
/*                                                                            */
/*  Revision:                                  								  */
/*                                                                            */
/******************************************************************************/
/*  ingenia-cat S.L. (c)                                                      */
/******************************************************************************/
#include <salvo.h>
#include <p30f4011.h>
#include "HIBlib.h"

/******************************************************************************/
/* Configuration words                                                        */
/******************************************************************************/
_FOSC(CSW_FSCM_OFF & EC_PLL16);
_FWDT(WDT_OFF);
_FBORPOR(MCLR_EN & PBOR_OFF & PWRT_OFF);
_FGS(CODE_PROT_OFF);

/******************************************************************************/
/* Hardware                                                                   */
/******************************************************************************/

/******************************************************************************/
/* Global Variable declaration                                                */
/******************************************************************************/
#define NTASK_A	taskButtons		//Nom Tasca-Funció A
#define NTASK_B	taskSensorCH4	//Nom Tasca-Funció B
#define	NTASK_C	taskPump		//Nom Tasca-Funcio C

#define TASK_A	OSTCBP(1) 	//Tasca A
#define TASK_B	OSTCBP(2) 	//Tasca B
#define TASK_C	OSTCBP(3) 	//Tasca C

#define PRIO_A	10		 	//Prioritat tasca A
#define PRIO_B	10		 	//Prioritat tasca B
#define PRIO_C	10		 	//Prioritat tasca C

#define SENSOR_LOW	Buttons.SW1	//Sensor baix
#define SENSOR_HIGH	Buttons.SW2	//Sensor alt
#define H2O_FLOW	Buttons.SW3	//Fluxe d'aigua
#define BUT_ACT		Buttons.SW4	//Botó activació bomba
#define BUT_DES		Buttons.SW5	//Botó desactivació bomba

#define msgPump		OSECBP(1)	//Encesa de la bomba
#define msgAlarmOn	OSECBP(2)	//Activa l'alarma
#define msgInfoOp	OSECBP(3)	//Informació al operador

char S_ON = 'O';	//Senyal de encesa del motor
char S_OFF = 'F';	//Senyal d'aturada del motor
char S_CH4 = '4';	//Senyal del sensor de CH4
char S_CO = 'C';	//Senyal del sensor de CO
char S_AIR = 'A';	//Senyal de fluxe d'aire
char E_BOMB = 'B';	//Senyal d'error de fluxe d'aigua

char motorOn = 'F';		//Bomba d'aigua. Desactivada inicialment
char lvlCH4 = 'F';	//Estat del metà. Inicialment F

/******************************************************************************/
/* Interrupts                                                                 */
/******************************************************************************/

/******************************************************************************/
/* Procedures                                                                 */
/******************************************************************************/

void taskSensorLvl(void){
	/*Controla si el nivell d'aigua ha activat els sensors*/
	while(1){
		PushButtonRead();
		if(SENSOR_LOW == 1){
			OSSignalMsg(msgPump, (OStypeMsgP) &S_OFF);
		}
		if(SENSOR_HIGH == 1){
			OSSignalMsg(msgPump, (OStypeMsgP) &S_ON);
		}
		OS_Yield();
	}
}

void taskButtons(void){
	/* Tasca que controla els botons de l'operador */
	while(1){
		PushButtonRead();
		if(BUT_ACT == 1){
			OSSignalMsg(msgPump, (OStypeMsgP) &S_ON);
		}	
		if(BUT_DES == 1){
			OSSignalMsg(msgPump, (OStypeMsgP) &S_OFF);
		}	
		OS_Yield();
	}
}

void taskSensorCH4(void){
	/* Controla l'acumulació de gas metà */
	unsigned int uiMeasure;
	
	while(1){		
		uiMeasure = AnalogAcquireR1();
		if(uiMeasure > 500){
			lvlCH4 = 'T';
			//OSSignalMsg(msgAlarmOn, (OStypeMsgP) &S_CH4);
			OSSignalMsg(msgInfoOp, (OStypeMsgP) &S_CH4);
			OSSignalMsg(msgPump, (OStypeMsgP) &S_OFF);
		}
		if(uiMeasure <= 500){
			lvlCH4 = 'F';
		}
		
		OS_Yield();
	}
}

void taskSensorCO(void){
	/* Controla l'acumulació de monòxid de carboni */
	unsigned int uiMeasure;
	
	while(1){		
		uiMeasure = AnalogAcquireR1();
		if(uiMeasure > 700){
			OSSignalMsg(msgAlarmOn, (OStypeMsgP) &S_CO);
			OSSignalMsg(msgInfoOp, (OStypeMsgP) &S_CO);
		}		
		OS_Yield();
	}
}

void taskSensorAirFlow(void){
	/* Controla fluxe d'aire */
	unsigned int uiMeasure;
	
	while(1){		
		uiMeasure = AnalogAcquireR1();
		if(uiMeasure < 200){
			OSSignalMsg(msgAlarmOn, (OStypeMsgP) &S_AIR);
			OSSignalMsg(msgInfoOp, (OStypeMsgP) &S_AIR);
		}		
		OS_Yield();
	}
}

void taskH2OFlow(void){
	/* Tasca que controla el correcte funcionament de la bomba */
	while(1){
		PushButtonRead();
		if((motorOn == 'T') && (H2O_FLOW == 0)){
			OSSignalMsg(msgInfoOp, (OStypeMsgP) &E_BOMB);
		}
		if((motorOn == 'F') && (H2O_FLOW == 1)){
			OSSignalMsg(msgInfoOp, (OStypeMsgP) &E_BOMB);
		}	
		OS_Yield();
	}
}

void taskAlam(void){
	/* Encén el buzzer d'alarma */
	OStypeMsgP msgP;
	
	while(1){
		OS_WaitMsg(msgAlarmOn, &msgP, OSNO_TIMEOUT);
		if(*(char *)msgP == S_CH4){
			BuzzerPlay(20, 10);
		}
		if(*(char *)msgP == S_CO){
			BuzzerPlay(20, 60);
		}
		if(*(char *)msgP == S_AIR){
			BuzzerPlay(20, 120);
		}
	}
}

void taskInfoOp(void){
	/* Mostra per pantalla l'error rebut */
	OStypeMsgP msgP;
	
	while(1){
		OS_WaitMsg(msgInfoOp, &msgP, OSNO_TIMEOUT);
		LCDGotoSecondLine();
		if(*(char *)msgP == S_CH4){
			LCDWriteString("CH4 elevat");
		}
		if(*(char *)msgP == S_CO){
			LCDWriteString("CO elevat");
		}
		if(*(char *)msgP == S_AIR){
			LCDWriteString("Fluxe d'aire");
		}
		if(*(char *)msgP == E_BOMB){
			LCDWriteString("Error de bomba");
		}
	}
}

void taskPump(void){
	OStypeMsgP msgP;
	
	while(1){
		OS_WaitMsg(msgPump, &msgP, OSNO_TIMEOUT);
		if(*(char *)msgP == S_ON){
			if(lvlCH4 == 'F'){
				motorOn = 'T';
				LED1 = 1;
				LCDGotoSecondLine();
				LCDWriteString("Encen");
			}
		}
		if(*(char *) msgP == S_OFF){
			LCDGotoSecondLine();
			LCDWriteString("Apaga");
			motorOn = 'F';
			LED1 = 0;
		}
	}
}

int main( void ){
	U1MODE = 0; //Clear UART config - to avoid problems with bootloader
	IOConfig();
	AnalogConfig();
	LCDConfig(0);
	OSInit();
	
	LCDWriteString((char*)"Control de mina");
	
	//Crea les 3 tasques permeses
	OSCreateTask(NTASK_A,TASK_A, PRIO_A);
	OSCreateTask(NTASK_B, TASK_B, PRIO_B);
	OSCreateTask(NTASK_C, TASK_C, PRIO_C);
	
	//Crea els 3 missatges
	OSCreateMsg(msgPump, (OStypeMsgP) 0);
	OSCreateMsg(msgInfoOp, (OStypeMsgP) 0);
	//OSCreateMsg(msgAlarmOn, (OStypeMsgP) 0);
	
	while (1) {
		OSSched();
	}
}


