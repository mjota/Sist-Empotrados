//! \file   HIBlib.h
//! \brief  Basic Library for Human Interface Board with iCM4011
//! \author Roger Juanpere
//! \author ingenia-cat S.L. (c)
//! \version 1.0 Initial version
//!

/******************************************************************************/
/* Hardware                                                                   */
/******************************************************************************/
#define FXT                 7372800
#define PLL                 16
#define FCY                 (FXT * PLL) / 4

// We should use PORT for reading operations 
// And LAT for writing

// Onboard LEDs
#define ALL_LED	            LATE
#define LED0                LATEbits.LATE0
#define LED1                LATEbits.LATE1
#define LED2                LATEbits.LATE2
#define LED3                LATEbits.LATE3
#define LED4                LATEbits.LATE4
#define LED5                LATEbits.LATE5

// PushButton Array
#define SW_ROW0	            PORTDbits.RD0
#define SW_ROW1             PORTDbits.RD1
#define SW_ROW2	            PORTDbits.RD2
#define SW_ROW3             PORTDbits.RD3
#define SW_COLUMN0          LATBbits.LATB0
#define SW_COLUMN1          LATBbits.LATB1
#define SW_COLUMN2          LATBbits.LATB2
#define SW_TRIS_ROW0        TRISDbits.TRISD0
#define SW_TRIS_ROW1        TRISDbits.TRISD1
#define SW_TRIS_ROW2        TRISDbits.TRISD2
#define SW_TRIS_ROW3        TRISDbits.TRISD3
#define SW_TRIS_COLUMN0     TRISBbits.TRISB0
#define SW_TRIS_COLUMN1     TRISBbits.TRISB1
#define SW_TRIS_COLUMN2     TRISBbits.TRISB2

// Alphanumeric LCD
#define LCD_DB7_R           PORTDbits.RD3
#define LCD_DB7_W           LATDbits.LATD3
#define LCD_DB6             LATDbits.LATD2
#define LCD_DB5             LATDbits.LATD1
#define LCD_DB4             LATDbits.LATD0
#define LCD_DB3             LATFbits.LATF5
#define LCD_DB2             LATFbits.LATF4
#define LCD_DB1             LATFbits.LATF1
#define LCD_DB0             LATFbits.LATF0
#define LCD_ENABLE          LATBbits.LATB6
#define LCD_R_W             LATEbits.LATE8
#define LCD_STROBE          LATCbits.LATC15

#define LCD_DB7_TRIS        TRISDbits.TRISD3
#define LCD_DB6_TRIS        TRISDbits.TRISD2
#define LCD_DB5_TRIS        TRISDbits.TRISD1
#define LCD_DB4_TRIS        TRISDbits.TRISD0
#define LCD_DB3_TRIS        TRISFbits.TRISF5
#define LCD_DB2_TRIS        TRISFbits.TRISF4
#define LCD_DB1_TRIS        TRISFbits.TRISF1
#define LCD_DB0_TRIS        TRISFbits.TRISF0
#define LCD_ENABLE_TRIS     TRISBbits.TRISB6
#define LCD_R_W_TRIS        TRISEbits.TRISE8
#define LCD_STROBE_TRIS     TRISCbits.TRISC15

//Graphic LCD
#define GLCD_DB7_R          PORTDbits.RD3
#define GLCD_DB7_W          LATDbits.LATD3
#define GLCD_DB6_R          PORTDbits.RD2
#define GLCD_DB6_W          LATDbits.LATD2
#define GLCD_DB5_R          PORTDbits.RD1
#define GLCD_DB5_W          LATDbits.LATD1
#define GLCD_DB4_R          PORTDbits.RD0
#define GLCD_DB4_W          LATDbits.LATD0
#define GLCD_DB3_R          PORTFbits.RF5
#define GLCD_DB3_W          LATFbits.LATF5
#define GLCD_DB2_R          PORTFbits.RF4
#define GLCD_DB2_W          LATFbits.LATF4
#define GLCD_DB1_R          PORTFbits.RF1
#define GLCD_DB1_W          LATFbits.LATF1
#define GLCD_DB0_R          PORTFbits.RF0
#define GLCD_DB0_W          LATFbits.LATF0
#define GLCD_ENABLE2        LATBbits.LATB5
#define GLCD_ENABLE1        LATBbits.LATB6
#define GLCD_R_W            LATEbits.LATE8
#define GLCD_STROBE         LATCbits.LATC15

#define GLCD_DB7_TRIS       TRISDbits.TRISD3
#define GLCD_DB6_TRIS       TRISDbits.TRISD2
#define GLCD_DB5_TRIS       TRISDbits.TRISD1
#define GLCD_DB4_TRIS       TRISDbits.TRISD0
#define GLCD_DB3_TRIS       TRISFbits.TRISF5
#define GLCD_DB2_TRIS       TRISFbits.TRISF4
#define GLCD_DB1_TRIS       TRISFbits.TRISF1
#define GLCD_DB0_TRIS       TRISFbits.TRISF0
#define GLCD_ENABLE2_TRIS   TRISBbits.TRISB5
#define GLCD_ENABLE1_TRIS   TRISBbits.TRISB6
#define GLCD_R_W_TRIS       TRISEbits.TRISE8
#define GLCD_STROBE_TRIS    TRISCbits.TRISC15

//Buzzer
#define BUZZER              LATBbits.LATB7
#define BUZZER_TRIS         TRISBbits.TRISB7

/******************************************************************************/
/* Global Variable declaration                                                */
/******************************************************************************/
typedef union
{
    unsigned int all;
    struct
    {
        unsigned SW0:1;
        unsigned SW1:1;
        unsigned SW2:1;
        unsigned SW3:1;
        unsigned SW4:1;
        unsigned SW5:1;
        unsigned SW6:1;
        unsigned SW7:1;
        unsigned SW8:1;
        unsigned SW9:1;
        unsigned SW10:1;
        unsigned SW11:1;
        unsigned unused:4;
    };
}TButtons;

extern TButtons Buttons;

/******************************************************************************/
/* Procedures                                                                 */
/******************************************************************************/
// Config i/o
void IOConfig();

// Pushbutton array reading
void PushButtonRead();

// ADC Measurement
void AnalogConfig();
unsigned int AnalogAcquireR1();
unsigned int AnalogAcquireR2();

// Low level LCD functions
void LCDWriteString(char *pcBuffer);
void LCDWriteData(char cData);
void LCDWriteNibbleCmd(char cCmd);
void LCDWriteCmd(char cCmd);
char LCDIsBusy();

// High level Alphanumeric LCD functions
void LCDConfig(char is8bitsMode);
void LCDClearScreen();
void LCDMoveHome();
void LCDGotoFirstLine();
void LCDGotoSecondLine();
void LCDTurnONUnderline();
void LCDTurnONBlinking();
void LCDHideCursor();
void LCDSetDisplay();
void LCDScrollLeft();
void LCDScrollRight();
void LCDMoveCursorRight();
void LCDMoveCursorLeft();

void BuzzerPlay(unsigned int periods, unsigned int halfTimeON10us);

// Low level GLCD functions
void GLCDWriteData(char cData);
char GLCDReadData();
void GLCDWriteCmd(char cCmd);

// High level GLCD functions
void GLCDConfig();
void GLCDFillScreen(char cColor);
void GLCDDrawPixel(unsigned int uiX, unsigned int uiY, char cColor);
void GLCDDrawLine(unsigned int uiX1, unsigned int uiY1, 
                  unsigned int uiX2, unsigned int uiY2, char cColor);
void GLCDDrawRect(unsigned int uiX1, unsigned int uiY1, 
                  unsigned int uiX2, unsigned int uiY2, char cFill, char cColor);
void GLCDDrawCircle(unsigned int uiX1, unsigned int uiY1, 
                    unsigned int uiRadius, char cFill, char cColor);

void GLCDDrawText(unsigned int uiX, unsigned int uiY, char* textptr, char cSize, char cColor);

void 
glcd_drawlogo(unsigned int x, 
              unsigned int y, 
              char color
              );

// Auxiliar functions
int bit_test(char data, char bitundertest);
