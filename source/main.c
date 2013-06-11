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
#include "timer.h"
#include <stdlib.h>

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
//Tasques activades
#define NTASK_A	taskH2OFlow	
#define NTASK_B	taskPump		
#define	NTASK_C	taskButtons

//Configuració de tasques
#define TASK_A	OSTCBP(1) 	//Tasca A
#define TASK_B	OSTCBP(2) 	//Tasca B
#define TASK_C	OSTCBP(3) 	//Tasca C

#define PRIO_A	10	//Prioritat tasca A
#define PRIO_B	10	//Prioritat tasca B
#define PRIO_C	10	//Prioritat tasca C

//Missatges
#define msgPump		OSECBP(1)	//Encesa de la bomba
#define msgAlarmOn	OSECBP(2)	//Activa l'alarma
#define msgInfoOp	OSECBP(3)	//Informació al operador

//Botons
#define SENSOR_LOW	Buttons.SW1	//Sensor baix
#define SENSOR_HIGH	Buttons.SW2	//Sensor alt
#define H2O_FLOW	Buttons.SW3	//Fluxe d'aigua
#define BUT_ACT		Buttons.SW4	//Botó activació bomba
#define BUT_DES		Buttons.SW5	//Botó desactivació bomba

//Configuració del Timer
#define TIMER_PERIOD 0x250	//(100ms)
#define TMR0_RELOAD  156

//Límits d'activació dels sensors
#define	L_CH4	500	//Sensor de CH4
#define L_CO	700	//Sensor de CO
#define L_FW	200	//Sensor de fluxe d'aire

//Senyals dels missatges
char S_ON = 'O';	//Senyal de encesa del motor
char S_OFF = 'F';	//Senyal d'aturada del motor
char S_CH4 = '4';	//Senyal del sensor de CH4
char S_CO = 'C';	//Senyal del sensor de CO
char S_AIR = 'A';	//Senyal de fluxe d'aire
char S_BOMB = 'B';	//Senyal d'error de fluxe d'aigua

//Estats del sistema
char motorOn = 'F';	//Bomba d'aigua. Desactivada inicialment
char lvlCH4 = 'F';	//Estat del metà.

/******************************************************************************/
/* Interrupts                                                                 */
/******************************************************************************/
void __attribute__((interrupt, auto_psv)) _T1Interrupt(){
	if (IEC0bits.T1IE && IFS0bits.T1IF) {
		WriteTimer1(0x0000);            // Reset time counter
		OSTimer();
		IFS0bits.T1IF = 0;
		TMR1 -= TMR0_RELOAD;		
	}
}

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
		OS_Delay(2);
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
		OS_Delay(2);
	}
}

void taskSensorCH4(void){
	/* Controla l'acumulació de gas metà */
	unsigned int uiMeasure;
	
	while(1){		
		uiMeasure = AnalogAcquireR1();
		if(uiMeasure > L_CH4){
			lvlCH4 = 'T';
			OSSignalMsg(msgAlarmOn, (OStypeMsgP) &S_CH4);
			OSSignalMsg(msgInfoOp, (OStypeMsgP) &S_CH4);
			OSSignalMsg(msgPump, (OStypeMsgP) &S_OFF);
		}
		if(uiMeasure <= L_CH4){
			lvlCH4 = 'F';
		}
		
		OS_Delay(1);
	}
}

void taskSensorCO(void){
	/* Controla l'acumulació de monòxid de carboni */
	unsigned int uiMeasure;
	
	while(1){		
		uiMeasure = AnalogAcquireR1();
		if(uiMeasure > L_CO){
			OSSignalMsg(msgAlarmOn, (OStypeMsgP) &S_CO);
			OSSignalMsg(msgInfoOp, (OStypeMsgP) &S_CO);
		}		
		OS_Delay(1);
	}
}

void taskSensorAirFlow(void){
	/* Controla fluxe d'aire */
	unsigned int uiMeasure;
	
	while(1){		
		uiMeasure = AnalogAcquireR1();
		if(uiMeasure < L_FW){
			OSSignalMsg(msgAlarmOn, (OStypeMsgP) &S_AIR);
			OSSignalMsg(msgInfoOp, (OStypeMsgP) &S_AIR);
		}		
		OS_Delay(1);
	}
}

void taskH2OFlow(void){
	/* Tasca que controla el correcte funcionament de la bomba */
	while(1){
		PushButtonRead();
		if(((motorOn == 'T') && (H2O_FLOW == 0)) ||
			((motorOn == 'F') && (H2O_FLOW == 1))){
			OSSignalMsg(msgInfoOp, (OStypeMsgP) &S_BOMB);
			LED5 = 1;
		}else{
			LED5 = 0;
		}
		OS_Delay(9);
	}
}

void taskAlarm(void){
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
		LCDGotoSecondLine();
		LCDWriteString("                ");
		OS_WaitMsg(msgInfoOp, &msgP, OSNO_TIMEOUT);
		LCDGotoSecondLine();
		if(*(char *)msgP == S_CH4){
			LCDWriteString("CH4 elevat     ");
		}
		if(*(char *)msgP == S_CO){
			LCDWriteString("CO elevat       ");
		}
		if(*(char *)msgP == S_AIR){
			LCDWriteString("Fluxe d'aire    ");
		}
		if(*(char *)msgP == S_BOMB){
			LCDWriteString("Error de bomba  ");
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
			}
		}
		if(*(char *) msgP == S_OFF){
			motorOn = 'F';
			LED1 = 0;
		}
	}
}

void Micro_Init(){
	OpenTimer1(T1_ON &              // Start 16-bit Timer
               T1_IDLE_CON &        // Continue operation on idle
               T1_GATE_OFF &        // Timer gate accumulation disabled
               T1_PS_1_256 &        // Prescale 1:256
               T1_SYNC_EXT_OFF &    // Do not synchronize external clock input (N/A)
               T1_SOURCE_INT,       // Internal osc (FCY)
               TIMER_PERIOD);       // Desired Period

    ConfigIntTimer1(T1_INT_PRIOR_7 & // Interrupt Priority 4 (7 Highest)
                    T1_INT_ON);      // Interrupt enabled
}

int main( void ){
	U1MODE = 0; //Clear UART config - to avoid problems with bootloader
	Micro_Init();
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
	OSCreateMsg(msgAlarmOn, (OStypeMsgP) 0);
	OSCreateMsg(msgInfoOp, (OStypeMsgP) 0);
	OSCreateMsg(msgPump, (OStypeMsgP) 0);
	
	while (1) {
		OSSched();
	}
}


