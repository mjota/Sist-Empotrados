    /******************************************************************************/
/*                                                                            */
/*  Project:     iCM4011 - Human Interface Board Template                     */
/*  Module:                                                                   */
/*  Description:                                                              */
/*                                                                            */
/*  Author:                                                                   */
/*                                                                            */
/*  Revision: 1.0 (22-11-06): Initial version                                 */
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
#define TASK_A	OSTCBP(1) 	//Tasca A
#define TASK_B	OSTCBP(2) 	//Tasca B
#define TASK_C	OSTCBP(3) 	//Tasca C

#define PRIO_A	10		 	//Prioritat tasca A
#define PRIO_B	10		 	//Prioritat tasca B
#define PRIO_C	10		 	//Prioritat tasca C

#define SENSOR_LOW	Buttons.SW1	//Sensor baix
#define SENSOR_HIGH	Buttons.SW2	//Sensor alt
#define H2O_FLOW	Buttons.SW3	//Fluxe d'aigua

#define msgPumpOn	OSECBP(1)	//Encesa de la bomba
#define msgPumpOff	OSECBP(2)	//Aturada de la bomba
#define msgCH4Low	OSECBP(3)	//Nivell de metà baix
#define msgAlarmOn	OSECBP(4)	//Activa l'alarma
#define msgInfoOp	OSECBP(5)	//Informació al operador	

char S_LVL = 'S';		//Activació per sensor de nivell
char E_BOMB = 'B';		//Error de fluxe d'aigua

char motorOn = 'F';		//Bomba d'aigua. Desactivada

/******************************************************************************/
/* Interrupts                                                                 */
/******************************************************************************/

/******************************************************************************/
/* Procedures                                                                 */
/******************************************************************************/

void taskSensorLow(void){
	/* Controla si el nivell d'aigua ha activat el sensor baix */
	while(1){
		if(SENSOR_LOW == 1){
			OSSignalMsg(msgPumpOff, (OStypeMsgP) &S_LVL);
		} 
		OS_Yield();
	}
}

void taskSensorHigh(void){
	/* Controla si el nivell d'aigua ha activat el sensor alt */
	while(1){
		if(SENSOR_HIGH == 1){
			OSSignalMsg(msgPumpOn, (OStypeMsgP) &S_LVL);
		}			
		OS_Yield();
	}
}

void taskSensorCH4(void){
	/* Controla l'acumulació de gas metà */
	OS_Yield();
}

void taskSensorCO(void){
	/* Controla l'acumulació de monòxid de carboni */
	OS_Yield();
}

void taskSensorAirFlow(void){
	/* Controla fluxe d'aire */
	OS_Yield();
}

void taskH2OFlow(void){
	/* Tasca que controla el correcte funcionament de la bomba */
	if((motorON == 'T') && (H2O_FLOW == 0)){
		OSSignalMsg(msgInfoOp, (OStypeMsgP) &E_BOMB);
	}
	if((motorON == 'F') && (H2O_FLOW == 1)){
		OSSignalMsg(msgInfoOp, (OStypeMsgP) &E_BOMB);
	OS_Yield();
}

void taskButtons(void){
	/* Tasca que controla els botons de l'operador */
	OS_Yield();
}

void taskAlam(void){
	/* Encén el buzzer d'alarma */
	OS_Yield();
}

void taskInfoOp(void){
	/* Mostra per pantalla o amb LEDS l'error rebut */
	OS_Yield();
}

void taskPumpOn(void){
	/* Activa el motor */
	OS_Yield();
}

void taskPumpOff(void){
	/* Apaga el motor */
	OS_Yield();
}

int main( void ){
	IOConfig();
	OSInit();
	
	//Crea les 3 tasques permeses
	OSCreateTask(taskPumpOn,TASK_A, PRIO_A);
	OSCreateTask(taskPumpOff, TASK_B, PRIO_B);
	OSCreateTask(taskSensorLow, TASK_C, PRIO_C);
	
	//Crea els missatges
	OSCreateMsg(msgPumpOff, (OStypeMsgP) 0);
	OSCreateMsg(msgPumpOn, (OStypeMsgP) 0);
	OSCreateMsg(msgInfoOp, (OStypeMsgP) 0);	
	
	while (1) {
		OSSched();
	}
}


