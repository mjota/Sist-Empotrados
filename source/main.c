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

#define msgPumpOn	OSECBP(1)	//Encesa de la bomba
#define msgPumpOff	OSECBP(2)	//Aturada de la bomba
#define msgCH4Low	OSECBP(3)	//Nivell de metà baix
#define msgAlarmOn	OSECBP(4)	//Activa l'alarma
#define msgInfoOp	OSECBP(5)	//Informació al operador	

unsigned int counter;

/******************************************************************************/
/* Interrupts                                                                 */
/******************************************************************************/

/******************************************************************************/
/* Procedures                                                                 */
/******************************************************************************/

void taskSensorLow(void){
	/* Controla si el nivell d'aigua ha activat el sensor baix */
	while(1){
		OS_Yield();
	}
}

void taskSensorHigh(void){
	/* Controla si el nivell d'aigua ha activat el sensor alt */
}

void taskSensorCH4(void){
	/* Controla l'acumulació de gas metà */
}

void taskSensorCO(void){
	/* Controla l'acumulació de monòxid de carboni */
}

void taskSensorAirFlow(void){
	/* Controla fluxe d'aire */
}

void taskH2OFlow(void){
	/* Tasca que controla el correcte funcionament de la bomba */
}

void taskButtons(void){
	/* Tasca que controla els botons de l'operador */
}

void taskAlam(void){
	/* Encén el buzzer d'alarma */
}

void taskInfoOp(void){
	/* Mostra per pantalla l'error rebut */
}

void taskPumpOn(void){
	/* Activa el motor */
}

void taskPumpOff(void){
	/* Apaga el motor */
}

int main( void ){
	IOConfig();
	OSInit();
	OSCreateTask(taskPumpOn,TASK_A, PRIO_A);
	OSCreateTask(taskPumpOff, TASK_B, PRIO_B);
	OSCreateTask(taskSensorLow, TASK_C, PRIO_C);
	while (1) {
		OSSched();
	}
}


