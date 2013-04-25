//! \file   HIBlib.c
//! \brief  Basic Library for Human Interface Board with iCM4011
//! \author Roger Juanpere
//! \author ingenia-cat S.L. (c)
//! \version 1.0 Initial version

#include <p30f4011.h>
#include <adc10.h>
#include "HIBlib.h"

unsigned int m_is8bitsMode;         //!< Define the mode of communication with the Alphanumeric LCD
unsigned int m_uiSide;              //!< Define the active side of the GLCD
TButtons Buttons;                   //!< Contains the state of the push-buttons

#define GLCD_WIDTH	    122         //!< Number of Horizontal Pixels in the GLCD
#define GLCD_HEIGHT	    32          //!< Number of Vertical Pixels in the GLCD
#define RIGHT           0
#define LEFT            1

const char FONT[95][5] ={0x00, 0x00, 0x00, 0x00, 0x00, // SPACE
                         0x00, 0x00, 0x5F, 0x00, 0x00, // !
                         0x00, 0x03, 0x00, 0x03, 0x00, // "
                         0x14, 0x3E, 0x14, 0x3E, 0x14, // #
                         0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
                         0x43, 0x33, 0x08, 0x66, 0x61, // %
                         0x36, 0x49, 0x55, 0x22, 0x50, // &
                         0x00, 0x05, 0x03, 0x00, 0x00, // '
                         0x00, 0x1C, 0x22, 0x41, 0x00, // (
                         0x00, 0x41, 0x22, 0x1C, 0x00, // )
                         0x14, 0x08, 0x3E, 0x08, 0x14, // *
                         0x08, 0x08, 0x3E, 0x08, 0x08, // +
                         0x00, 0x50, 0x30, 0x00, 0x00, // ,
                         0x08, 0x08, 0x08, 0x08, 0x08, // -
                         0x00, 0x60, 0x60, 0x00, 0x00, // .
                         0x20, 0x10, 0x08, 0x04, 0x02, // /
                         0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
                         0x00, 0x04, 0x02, 0x7F, 0x00, // 1
                         0x42, 0x61, 0x51, 0x49, 0x46, // 2
                         0x22, 0x41, 0x49, 0x49, 0x36, // 3
                         0x18, 0x14, 0x12, 0x7F, 0x10, // 4
                         0x27, 0x45, 0x45, 0x45, 0x39, // 5
                         0x3E, 0x49, 0x49, 0x49, 0x32, // 6
                         0x01, 0x01, 0x71, 0x09, 0x07, // 7
                         0x36, 0x49, 0x49, 0x49, 0x36, // 8
                         0x26, 0x49, 0x49, 0x49, 0x3E, // 9
                         0x00, 0x36, 0x36, 0x00, 0x00, // :
                         0x00, 0x56, 0x36, 0x00, 0x00, // ;
                         0x08, 0x14, 0x22, 0x41, 0x00, // <
                         0x14, 0x14, 0x14, 0x14, 0x14, // =
                         0x00, 0x41, 0x22, 0x14, 0x08, // >
                         0x02, 0x01, 0x51, 0x09, 0x06, // ?
                         0x3E, 0x41, 0x59, 0x55, 0x5E, // @
                         0x7E, 0x09, 0x09, 0x09, 0x7E, // A
                         0x7F, 0x49, 0x49, 0x49, 0x36, // B
                         0x3E, 0x41, 0x41, 0x41, 0x22, // C
                         0x7F, 0x41, 0x41, 0x41, 0x3E, // D
                         0x7F, 0x49, 0x49, 0x49, 0x41, // E
                         0x7F, 0x09, 0x09, 0x09, 0x01, // F
                         0x3E, 0x41, 0x41, 0x49, 0x3A, // G
                         0x7F, 0x08, 0x08, 0x08, 0x7F, // H
                         0x00, 0x41, 0x7F, 0x41, 0x00, // I
                         0x30, 0x40, 0x40, 0x40, 0x3F, // J
                         0x7F, 0x08, 0x14, 0x22, 0x41, // K
                         0x7F, 0x40, 0x40, 0x40, 0x40, // L
                         0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
                         0x7F, 0x02, 0x04, 0x08, 0x7F, // N
                         0x3E, 0x41, 0x41, 0x41, 0x3E, // O
                         0x7F, 0x09, 0x09, 0x09, 0x06, // P
                         0x1E, 0x21, 0x21, 0x21, 0x5E, // Q
                         0x7F, 0x09, 0x09, 0x09, 0x76, // R
                         0x26, 0x49, 0x49, 0x49, 0x32, // S
                         0x01, 0x01, 0x7F, 0x01, 0x01, // T
                         0x3F, 0x40, 0x40, 0x40, 0x3F, // U
                         0x1F, 0x20, 0x40, 0x20, 0x1F, // V
                         0x7F, 0x20, 0x10, 0x20, 0x7F, // W
                         0x41, 0x22, 0x1C, 0x22, 0x41, // X
                         0x07, 0x08, 0x70, 0x08, 0x07, // Y
                         0x61, 0x51, 0x49, 0x45, 0x43, // Z
                         0x00, 0x7F, 0x41, 0x00, 0x00, // [
                         0x02, 0x04, 0x08, 0x10, 0x20, // \ //Don't remove this comment.
                         0x00, 0x00, 0x41, 0x7F, 0x00, // ]
                         0x04, 0x02, 0x01, 0x02, 0x04, // ^
                         0x40, 0x40, 0x40, 0x40, 0x40, // _
                         0x00, 0x01, 0x02, 0x04, 0x00, // `
                         0x20, 0x54, 0x54, 0x54, 0x78, // a
                         0x7F, 0x44, 0x44, 0x44, 0x38, // b
                         0x38, 0x44, 0x44, 0x44, 0x44, // c
                         0x38, 0x44, 0x44, 0x44, 0x7F, // d
                         0x38, 0x54, 0x54, 0x54, 0x18, // e
                         0x04, 0x04, 0x7E, 0x05, 0x05, // f
                         0x08, 0x54, 0x54, 0x54, 0x3C, // g
                         0x7F, 0x08, 0x04, 0x04, 0x78, // h
                         0x00, 0x44, 0x7D, 0x40, 0x00, // i
                         0x20, 0x40, 0x44, 0x3D, 0x00, // j
                         0x7F, 0x10, 0x28, 0x44, 0x00, // k
                         0x00, 0x41, 0x7F, 0x40, 0x00, // l
                         0x7C, 0x04, 0x78, 0x04, 0x78, // m
                         0x7C, 0x08, 0x04, 0x04, 0x78, // n
                         0x38, 0x44, 0x44, 0x44, 0x38, // o
                         0x7C, 0x14, 0x14, 0x14, 0x08, // p
                         0x08, 0x14, 0x14, 0x14, 0x7C, // q
                         0x00, 0x7C, 0x08, 0x04, 0x04, // r
                         0x48, 0x54, 0x54, 0x54, 0x20, // s
                         0x04, 0x04, 0x3F, 0x44, 0x44, // t
                         0x3C, 0x40, 0x40, 0x20, 0x7C, // u
                         0x1C, 0x20, 0x40, 0x20, 0x1C, // v
                         0x3C, 0x40, 0x30, 0x40, 0x3C, // w
                         0x44, 0x28, 0x10, 0x28, 0x44, // x
                         0x0C, 0x50, 0x50, 0x50, 0x3C, // y
                         0x44, 0x64, 0x54, 0x4C, 0x44, // z
                         0x00, 0x08, 0x36, 0x41, 0x41, // {
                         0x00, 0x00, 0x7F, 0x00, 0x00, // |
                         0x41, 0x41, 0x36, 0x08, 0x00, // }
                         0x02, 0x01, 0x02, 0x04, 0x02};// ~

//! Check if the alphanumeric LCD is busy.
//! This function check if the alphanumeric LCD is busy processing another command.
//!
//! The commands are send in 4/8bits mode depending on the value set in the config.
//!
//! \return 1 if busy, 0 otherwise.
char 
LCDIsBusy()
{
    char cValue;

    LCD_DB7_TRIS = 1;
    LCD_DB6_TRIS = 1;
    LCD_DB5_TRIS = 1;
    LCD_DB4_TRIS = 1;

    if (m_is8bitsMode == 1)
    {
        LCD_DB3_TRIS = 1;
        LCD_DB2_TRIS = 1;
        LCD_DB1_TRIS = 1;
        LCD_DB0_TRIS = 1;
    }
    Delay20us();
    LCD_R_W = 1;
    LCD_STROBE = 0;
    Delay20us();        // Setup time
    Delay20us();

    LCD_ENABLE = 1;
    Delay20us();        // Data delay time
    Delay20us();

    cValue = LCD_DB7_R;

    Delay20us();
    Delay20us();
    LCD_ENABLE = 0;
    Delay20us();
    Delay20us();        // Hold on time
    LCD_R_W = 0;

    return cValue;
}

//! Writes a command to the alphanumeric LCD.
//! This functions sends a command to the alphanumeric LCD.
//!
//! The commands are send in 4/8bits mode depending on the value set in the config.
//!
//! \return Nothing.
void 
LCDWriteCmd(char cCmd       //!<[in]Command to write.
            )
{
    LCD_R_W = 0;
    LCD_STROBE = 0;

    LCD_DB7_TRIS = 0;
    LCD_DB6_TRIS = 0;
    LCD_DB5_TRIS = 0;
    LCD_DB4_TRIS = 0;
    if (m_is8bitsMode == 1)
    {
        LCD_DB3_TRIS = 0;
        LCD_DB2_TRIS = 0;
        LCD_DB1_TRIS = 0;
        LCD_DB0_TRIS = 0;
    }

    LCD_DB7_W = (unsigned int)((cCmd & 0x80)>>7);
    LCD_DB6 = (unsigned int)((cCmd & 0x40)>>6);
    LCD_DB5 = (unsigned int)((cCmd & 0x20)>>5);
    LCD_DB4 = (unsigned int)((cCmd & 0x10)>>4);
    if (m_is8bitsMode == 1)
    {
        LCD_DB3 = (unsigned int)((cCmd & 0x08)>>3);
        LCD_DB2 = (unsigned int)((cCmd & 0x04)>>2);
        LCD_DB1 = (unsigned int)((cCmd & 0x02)>>1);
        LCD_DB0 = (unsigned int)(cCmd & 0x01);
    }

    Delay20us();       // min 20us
    LCD_ENABLE = 1;
    Delay20us();       // min 230n
    LCD_ENABLE = 0;          
    Delay20us();       

    if (m_is8bitsMode == 0)
    {
        LCD_R_W = 0;
        LCD_STROBE = 0;

        LCD_DB7_W = (unsigned int)((cCmd & 0x08)>>3);
        LCD_DB6 = (unsigned int)((cCmd & 0x04)>>2);
        LCD_DB5 = (unsigned int)((cCmd & 0x02)>>1);
        LCD_DB4 = (unsigned int)(cCmd & 0x01);

        Delay20us();       // min 20us
        LCD_ENABLE = 1;
        Delay20us();
        LCD_ENABLE = 0;
        Delay20us();
    }

    LCD_DB7_TRIS = 1;
    LCD_DB6_TRIS = 1;
    LCD_DB5_TRIS = 1; 
    LCD_DB4_TRIS = 1;

    if (m_is8bitsMode == 1)
    {
        LCD_DB3_TRIS = 1;
        LCD_DB2_TRIS = 1;
        LCD_DB1_TRIS = 1; 
        LCD_DB0_TRIS = 1;
    }
}  

//! Writes the higher nibble of the command to the alphanumeric LCD.
//! This functions writes the higher nibble of a command to the alphanumeric LCD.  
//!
//! The access is done in 8bits mode.
//!
//! \return Nothing.
void 
LCDWriteNibbleCmd(char cCmd     //!<[in]Command to send.
                  )
{
    LCD_R_W = 0;   
    LCD_STROBE = 0;

    LCD_DB7_TRIS = 0;
    LCD_DB6_TRIS = 0;
    LCD_DB5_TRIS = 0;
    LCD_DB4_TRIS = 0;

    LCD_DB7_W = (unsigned int)((cCmd & 0x80)>>7);
    LCD_DB6 = (unsigned int)((cCmd & 0x40)>>6);
    LCD_DB5 = (unsigned int)((cCmd & 0x20)>>5);
    LCD_DB4 = (unsigned int)((cCmd & 0x10)>>4);

    Delay20us();       // min 20us
    LCD_ENABLE = 1;
    Delay20us();   
    LCD_ENABLE = 0;
    Delay20us();
    
    LCD_DB7_TRIS = 1;
    LCD_DB6_TRIS = 1;
    LCD_DB5_TRIS = 1; 
    LCD_DB4_TRIS = 1;
}

//! Writes a data to the alphanumeric LCD.
//! This functions sends a data to the alphanumeric LCD.
//!
//! The data are send in 4/8bits mode depending on the value set in the config.
//!
//! \return Nothing.
void 
LCDWriteData(char cData     //!<[in]Data to write.
             )
{
    LCD_R_W = 0;        // enable write
    LCD_STROBE = 1;     // Select Data Reg
                     
    // configure the common data pins as output
    LCD_DB7_TRIS = 0;
    LCD_DB6_TRIS = 0;
    LCD_DB5_TRIS = 0;
    LCD_DB4_TRIS = 0;
    if (m_is8bitsMode == 1)
    {
        LCD_DB3_TRIS = 0;
        LCD_DB2_TRIS = 0;
        LCD_DB1_TRIS = 0;
        LCD_DB0_TRIS = 0;
    }

    LCD_DB7_W = (unsigned int)((cData & 0x80)>>7);
    LCD_DB6 = (unsigned int)((cData & 0x40)>>6);
    LCD_DB5 = (unsigned int)((cData & 0x20)>>5);
    LCD_DB4 = (unsigned int)((cData & 0x10)>>4);
    if (m_is8bitsMode == 1)
    {
        LCD_DB3 = (unsigned int)((cData & 0x08)>>3);
        LCD_DB2 = (unsigned int)((cData & 0x04)>>2);
        LCD_DB1 = (unsigned int)((cData & 0x02)>>1);
        LCD_DB0 = (unsigned int)((cData & 0x01)>>0);
    }

    Delay20us();
    LCD_ENABLE = 1;
    Delay20us();
    LCD_ENABLE = 0;
    Delay20us();

    if (m_is8bitsMode == 0)
    {
        LCD_R_W = 0;
        LCD_STROBE = 1;

        LCD_DB7_W = (unsigned int)((cData & 0x08)>>3);
        LCD_DB6 = (unsigned int)((cData & 0x04)>>2);
        LCD_DB5 = (unsigned int)((cData & 0x02)>>1);
        LCD_DB4 = (unsigned int)(cData & 0x01);

        Delay20us();       // min 20us
        LCD_ENABLE = 1;
        Delay20us();
        LCD_ENABLE = 0;
        Delay20us();
    }

    LCD_DB7_TRIS = 1;
    LCD_DB6_TRIS = 1;
    LCD_DB5_TRIS = 1;
    LCD_DB4_TRIS = 1;
    if (m_is8bitsMode == 1)
    {
        LCD_DB3_TRIS = 1;
        LCD_DB2_TRIS = 1;
        LCD_DB1_TRIS = 1;
        LCD_DB0_TRIS = 1;
    }
    LCD_R_W = 0;        
    LCD_STROBE = 0;      
}


//! Writes a string to the alphanumeric LCD.
//! The data are send in 4/8bits mode depending on the value set in the config.
//!
//! \return Nothing.
void 
LCDWriteString(char *pcBuffer       //!<[in]Pointer to string buffer.
               )
{
    while(*pcBuffer != '\0')
    {
        while(LCDIsBusy());
        LCDWriteData(*pcBuffer); // calling another function
                                 // to write each char to the lcd module
        pcBuffer++;
    }
}

//! Move the cursor of the alphanumeric LCD to home.
//! This functions set the alphanumeric LCD cursor to the top/left position.
//!
//! \return Nothing.
void 
LCDMoveHome()
{
    LCDWriteCmd(0x02);
    while(LCDIsBusy());
}

//! Clear alphanumeric LCD Screen.
//!
//! \return Nothing.
void 
LCDClearScreen()
{
    LCDWriteCmd(0x01);
    while(LCDIsBusy());
}

//! Scroll Right alphanumeric LCD Screen.
//! This function scroll the display area of the LCD one position to the right.
//!
//! \return Nothing.
void 
LCDScrollRight()
{
    LCDWriteCmd(0x1E);
    while(LCDIsBusy());
}

//! Scroll Left alphanumeric LCD Screen.
//! This function scroll the display area of the LCD one position to the left.
//!
//! \return Nothing.
void 
LCDScrollLeft()
{
    LCDWriteCmd(0x18);
    while(LCDIsBusy());
}

//! Move alphanumeric LCD Cursor Right.
//! This function move the LCD cursor one position to the right.
//!
//! \return Nothing.
void 
LCDMoveCursorRight()
{
    LCDWriteCmd(0x14);
    while(LCDIsBusy());
}

//! Move alphanumeric LCD Cursor Left.
//! This function move the LCD cursor one position to the left.
//!
//! \return Nothing.
void 
LCDMoveCursorLeft()
{
    LCDWriteCmd(0x10);
    while(LCDIsBusy());
}

//! Turn on Underline alphanumeric LCD cursor.
//! This function makes visible the underline at the cursor position.
//!
//! \return Nothing.
void 
LCDTurnONUnderline()
{
    LCDWriteCmd(0x0E);
    while(LCDIsBusy());
}

//! Turn on Blinking-block alphanumeric LCD cursor.
//! This function activates the blincking of the cursor.
//!
//! \return Nothing.
void 
LCDTurnONBlinking()
{
    LCDWriteCmd(0x0F);
    while(LCDIsBusy());
}

//! Hides the alphanumeric LCD cursor.
//!
//! \return Nothing.
void 
LCDHideCursor()
{
    LCDWriteCmd(0x0C);
    while(LCDIsBusy());
}

//! Move alphanumeric LCD cursor to first line.
//! This function move the cursor to the first character of the first line.
//!
//! \return Nothing.
void 
LCDGotoFirstLine()
{
    LCDWriteCmd(0x80);             // Goto to first line of the LCD
    while(LCDIsBusy());
}

//! Move alphanumeric LCD cursor to second line.
//! This function move the cursor to the first character of the second line.
//!
//! \return Nothing.
void 
LCDGotoSecondLine()
{
    LCDWriteCmd(0xC0);             // Goto to second line of the LCD
    while(LCDIsBusy());
}

//! Configures the Alphanumeric LCD.
//! This function forces a reset and initialize the Alphanumeric LCD (16x2).
//!
//! There are two possible interface mode of communication(4bits or 8bits), those 
//! are selectable through the parameter.
//!
//! \return Nothing.
void 
LCDConfig(char is8bitsMode          //!<[in] Select interface mode
          )
{
    char cLcdtype;

    LCD_R_W_TRIS = 0;
    LCD_STROBE_TRIS = 0;
    LCD_ENABLE_TRIS = 0;

    if (is8bitsMode == 1)
    {
       cLcdtype = 0x38;
    }
    else
    {
       cLcdtype = 0x28;
    }
    m_is8bitsMode = is8bitsMode;

    // Reset
    Delay15ms();
    // Set up the interface to the lcd. (give information about the 8-bit/4 bit interface)
    LCDWriteNibbleCmd(0x30);
    Delay5ms();
    // Set up the interface to the lcd. (give information about the 8-bit/4 bit interface)
    LCDWriteNibbleCmd(0x30);
    Delay150us();
    // Set up the interface to the lcd. (give information about the 8-bit/4 bit interface)
    LCDWriteNibbleCmd(0x30);
    while(LCDIsBusy());

    // End reset
    LCDWriteNibbleCmd(cLcdtype);  // Set 4-bits or 8-bits interface (using 8bits interface)
    while(LCDIsBusy());

    LCDWriteCmd(cLcdtype);      
    while(LCDIsBusy());

    LCDWriteCmd(0x08);      // Display OFF
    while(LCDIsBusy());

    LCDWriteCmd(0x01);      // Clear display
    while(LCDIsBusy());

    LCDWriteCmd(0x06);      // Incr. Address and shift cursor 
    while(LCDIsBusy());

    // End Initialization
    while(LCDIsBusy());

    LCDWriteCmd(0x0C);      // Display ON
    while(LCDIsBusy());
}

//! Configures the inputs & outputs of the HIB.
//! This function configures all the pins of the dsPIC as input but the portE (LEDS).
//!
//! Also sets all portB as digital inputs but AN7 & AN8 (Pots)
//!
//! Finally activates the weak pull-ups for the pushbutton array input pins.
//!
//! \return Nothing.
void 
IOConfig()
{
    // Config input & output ports
    LATB   = 0xFFFF;
    TRISB  = 0xFFFF;                // PortB as input 
    ADPCFG = 0xFFFF;                // PortB as digital

    LATC  = 0x0000;               
    TRISC = 0xFFFF;                 // PortC as input
    LATD  = 0x0000;
    TRISD = 0xFFFF;                 // PortD as input
    LATE  = 0x0000;
    TRISE = 0x0000;                 // PortE as outputs
    LATF  = 0x0000;
    TRISF = 0xFFFF;                 // PortF as input	
}

//! Read the state of the push button array.
//! This function check the state of all push button and save the result into the
//! global TButtons structure.
//!
//! The reading is done by columns.
//!
//! \return Nothing.
void 
PushButtonRead()
{
    SW_TRIS_ROW0 = 1;
    SW_TRIS_ROW1 = 1;
    SW_TRIS_ROW2 = 1;
    SW_TRIS_ROW3 = 1;

	// Column0
    SW_TRIS_COLUMN0 = 0;
    SW_TRIS_COLUMN1 = 1;
    SW_TRIS_COLUMN2 = 1;
    SW_COLUMN0 = 0; 
	SW_COLUMN1 = 1; 
	SW_COLUMN2 = 1;	
    Delay10us();

    Buttons.SW0 = 1 - SW_ROW0;
	Buttons.SW3 = 1 - SW_ROW1;
	Buttons.SW6 = 1 - SW_ROW2;
	Buttons.SW9 = 1 - SW_ROW3;

    SW_TRIS_COLUMN0 = 1;
	SW_TRIS_COLUMN1 = 0;
    SW_TRIS_COLUMN2 = 1;
    SW_COLUMN0 = 1;
	SW_COLUMN1 = 0;
	SW_COLUMN2 = 1;
    Delay10us();
	Buttons.SW1 = 1 - SW_ROW0;
	Buttons.SW4 = 1 - SW_ROW1;
	Buttons.SW7 = 1 - SW_ROW2;
	Buttons.SW10 = 1 - SW_ROW3;
    
    SW_TRIS_COLUMN0 = 1;
	SW_TRIS_COLUMN1 = 1;
	SW_TRIS_COLUMN2 = 0;  
    SW_COLUMN0 = 1; 
	SW_COLUMN1 = 1; 
	SW_COLUMN2 = 0;	
    Delay10us();
	Buttons.SW2 = 1 - SW_ROW0;
	Buttons.SW5 = 1 - SW_ROW1;
	Buttons.SW8 = 1 - SW_ROW2;
	Buttons.SW11 = 1 - SW_ROW3;

    SW_TRIS_COLUMN0 = 1;
	SW_TRIS_COLUMN1 = 1;
	SW_TRIS_COLUMN2 = 1;
}

//! Configures the Analog to Digital conversor.
//! This function sets the ACD to convert only one channel (0) and to generate
//! an interrupt after each convertion.
//!
//! The convertion is set to start automatically after sampling end.
//!
//! \return Nothing.
void 
AnalogConfig()
{
    ADCON1bits.ADON = 0;            // Turn off ADC

    OpenADC10(ADC_MODULE_ON &        // Turn on ADC
              ADC_IDLE_CONTINUE &    // Continue in idle mode
              ADC_FORMAT_INTG &      // Integer output format
              ADC_CLK_AUTO &         // Conversion trigger auto
              ADC_AUTO_SAMPLING_ON & // Internal counter ends sampling and starts conversion
              ADC_SAMP_ON,           // A/D Sample Auto-Start
              ADC_VREF_AVDD_AVSS &   // Ext Vref+ / Vref-
              ADC_CONVERT_CH0 &      // Convert only channel 0
              ADC_SAMPLES_PER_INT_1 &// Convert 1 samples per interrupt
              ADC_SCAN_OFF &         // Do not scan inputs
              ADC_ALT_BUF_OFF &      // Buffer mode 16 bits
              ADC_ALT_INPUT_OFF,     // Do not use alternate input mode
              ADC_SAMPLE_TIME_31 &   // Auto-sample time bits -> Tsample = 31 Tad
              ADC_CONV_CLK_SYSTEM &  // Clock derived from system clock
              ADC_CONV_CLK_32Tcy,    // A/D Conversion Clock -> Tad = 32 Tcy
              ENABLE_AN7_ANA &       // AN7 as analog
              ENABLE_AN8_ANA,        // AN8 as analog
              SCAN_NONE);            // Do not perform input scan
}

//! Read the value of the Pot R1.
//! This function configures channel 0 to convert input AN7.
//!
//! Starts the sampling and wait until the convertion is finished.
//!
//! \return ADC measure.
unsigned int 
AnalogAcquireR1()
{
    unsigned int uiTemp;    
    ADPCFGbits.PCFG7 = 0;

    SetChanADC10(ADC_CH0_POS_SAMPLEA_AN7 &    // Set channel 0 positive input to AN7
                 ADC_CH0_NEG_SAMPLEA_NVREF);  // Set channel 0 negative input to Vref-

    IFS0bits.ADIF = 0;              // Reset ADC interrupt flag -> Start Sampling + Conversion
    while(!IFS0bits.ADIF);          // Wait until the conversion is finished
    uiTemp = ReadADC10(0);          // Read the resulting value

    return uiTemp;
}

//! Read the value of the Pot R2.
//! This function configures channel 0 to convert input AN8.
//!
//! Starts the sampling and wait until the convertion is finished.
//!
//! \return ADC measure.
unsigned int 
AnalogAcquireR2()
{
    unsigned int uiTemp;
    ADPCFGbits.PCFG8 = 0;

    SetChanADC10(ADC_CH0_POS_SAMPLEA_AN8 &    // Set channel 0 positive input to AN7
                 ADC_CH0_NEG_SAMPLEA_NVREF);  // Set channel 0 negative input to Vref-

    IFS0bits.ADIF = 0;              // Reset ADC interrupt flag -> Start Sampling + Conversion
    while(!IFS0bits.ADIF);          // Wait until the conversion is finished
    uiTemp = ReadADC10(0);          // Read the resulting value

    return uiTemp;
}

//! Plays the buzzer during a defined time.
//! This function plays the buzzer at a desired frequency during an amount time. 
//!
//! The user can change both the duration (in terms of periods) and the frequency of the sound.
//!
//! \return Nothing.
void
BuzzerPlay(unsigned int uiPeriods,             //!<[in] Number of periods
           unsigned int uiHalfTimeON10us       //!<[in] Time of half-period expressed in 10us 
           )
{
    unsigned int i, j;
    BUZZER_TRIS = 0;

    for (i = 0; i < uiPeriods; i++)
    {
        BUZZER = 1;
        for (j = 0; j < uiHalfTimeON10us; j++)
        {
            Delay10us();
        }
        BUZZER = 0;
        for (j = 0; j < uiHalfTimeON10us; j++)
        {
            Delay10us();
        }
    }

    BUZZER_TRIS = 1;
}

//! Writes a data to the graphical LCD.
//! This functions sends a data to the graphical LCD.
//!
//! \return Nothing.
void 
GLCDWriteData(char cData          //!<[in] Data to write in the GLCD
          )               // 
{
    GLCD_STROBE = 1;     // Select Data Reg
    GLCD_R_W = 0;        // enable write
                      
    // Configure the common data pins as output
    GLCD_DB7_TRIS = 0;
    GLCD_DB6_TRIS = 0;
    GLCD_DB5_TRIS = 0;
    GLCD_DB4_TRIS = 0;
    GLCD_DB3_TRIS = 0;
    GLCD_DB2_TRIS = 0;
    GLCD_DB1_TRIS = 0;
    GLCD_DB0_TRIS = 0;
    
    GLCD_DB7_W = (unsigned int)((cData & 0x80)>>7);
    GLCD_DB6_W = (unsigned int)((cData & 0x40)>>6);
    GLCD_DB5_W = (unsigned int)((cData & 0x20)>>5);
    GLCD_DB4_W = (unsigned int)((cData & 0x10)>>4);
    GLCD_DB3_W = (unsigned int)((cData & 0x08)>>3);
    GLCD_DB2_W = (unsigned int)((cData & 0x04)>>2);
    GLCD_DB1_W = (unsigned int)((cData & 0x02)>>1);
    GLCD_DB0_W = (unsigned int)((cData & 0x01)>>0);
    
    Delay3Tcy();
    if (m_uiSide == LEFT)
    {
        GLCD_ENABLE1 = 1;
    }
    else
    {
        GLCD_ENABLE2 = 1;
    }
    Delay3Tcy();
    if (m_uiSide == LEFT)
    {
        GLCD_ENABLE1 = 0;
    }
    else
    {
        GLCD_ENABLE2 = 0;
    } 

    GLCD_DB7_TRIS = 1;
    GLCD_DB6_TRIS = 1;
    GLCD_DB5_TRIS = 1;
    GLCD_DB4_TRIS = 1;
    GLCD_DB3_TRIS = 1;
    GLCD_DB2_TRIS = 1;
    GLCD_DB1_TRIS = 1;
    GLCD_DB0_TRIS = 1;
    
    GLCD_R_W = 0;        
    GLCD_STROBE = 0;  
}

//! Reads a data to the graphical LCD.
//! This functions reads a data from the graphical LCD.
//!
//! \return Nothing.
char 
GLCDReadData()                 
{
    char cData;

    if (m_uiSide == LEFT)
    {
        GLCD_ENABLE1 = 1;
    }
    else
    {
        GLCD_ENABLE2 = 1;
    }
    GLCD_STROBE = 1;     // Select Data Reg
    GLCD_R_W = 1;        // enable write
    
    if (m_uiSide == LEFT)
    {
        GLCD_ENABLE1 = 0;
    }
    else
    {
        GLCD_ENABLE2 = 0;
    }  

    Delay3Tcy();

    // Configure the common data pins as input
    GLCD_DB7_TRIS = 1;
    GLCD_DB6_TRIS = 1;
    GLCD_DB5_TRIS = 1;
    GLCD_DB4_TRIS = 1;
    GLCD_DB3_TRIS = 1;
    GLCD_DB2_TRIS = 1;
    GLCD_DB1_TRIS = 1;
    GLCD_DB0_TRIS = 1;
    
    if (m_uiSide == LEFT)
    {
        GLCD_ENABLE1 = 1;
    }
    else
    {
        GLCD_ENABLE2 = 1;
    }

    Delay3Tcy();
    cData =  (GLCD_DB7_R << 7);
    cData += (GLCD_DB6_R << 6);
    cData += (GLCD_DB5_R << 5);
    cData += (GLCD_DB4_R << 4);
    cData += (GLCD_DB3_R << 3);
    cData += (GLCD_DB2_R << 2);
    cData += (GLCD_DB1_R << 1);
    cData +=  GLCD_DB0_R;

    if (m_uiSide == LEFT)
    {
        GLCD_ENABLE1 = 0;
    }
    else
    {
        GLCD_ENABLE2 = 0;
    }  

    GLCD_R_W = 0;        
    GLCD_STROBE = 0; 

    return cData;
 }

//! Writes a command to the graphical LCD.
//! This functions sends a command to the graphical LCD.
//!
//! \return Nothing.
void 
GLCDWriteCmd(char cCmd        //!<[in] Instruction to write into LCD
             )
{
    GLCD_R_W = 0;
    GLCD_STROBE = 0;

    if (m_uiSide == LEFT)
    {
        GLCD_ENABLE1 = 0;
    }
    else
    {
        GLCD_ENABLE2 = 0;
    }
    GLCD_DB7_TRIS = 0;
    GLCD_DB6_TRIS = 0;
    GLCD_DB5_TRIS = 0;
    GLCD_DB4_TRIS = 0;
    GLCD_DB3_TRIS = 0;
    GLCD_DB2_TRIS = 0;
    GLCD_DB1_TRIS = 0;
    GLCD_DB0_TRIS = 0;
    
    GLCD_DB7_W = (unsigned int)((cCmd & 0x80)>>7);
    GLCD_DB6_W = (unsigned int)((cCmd & 0x40)>>6);
    GLCD_DB5_W = (unsigned int)((cCmd & 0x20)>>5);
    GLCD_DB4_W = (unsigned int)((cCmd & 0x10)>>4);
    GLCD_DB3_W = (unsigned int)((cCmd & 0x08)>>3);
    GLCD_DB2_W = (unsigned int)((cCmd & 0x04)>>2);
    GLCD_DB1_W = (unsigned int)((cCmd & 0x02)>>1);
    GLCD_DB0_W = (unsigned int)(cCmd & 0x01);
    
    Delay3Tcy();
    if (m_uiSide == LEFT)
    {
        GLCD_ENABLE1 = 1;
    }
    else
    {
        GLCD_ENABLE2 = 1;
    }
    Delay3Tcy();
    if (m_uiSide == LEFT)
    {
        GLCD_ENABLE1 = 0;
    }
    else
    {
        GLCD_ENABLE2 = 0;
    }

    GLCD_DB7_TRIS = 1;
    GLCD_DB6_TRIS = 1;
    GLCD_DB5_TRIS = 1; 
    GLCD_DB4_TRIS = 1;
    GLCD_DB3_TRIS = 1;
    GLCD_DB2_TRIS = 1;
    GLCD_DB1_TRIS = 1; 
    GLCD_DB0_TRIS = 1;
}

//! Fills the graphical LCD screen with a color.
//! The color of the pixel can be ON(1) or OFF(0).
//!
//! This function does not use Drawpixel to increase the speed of the execution.
//!
//! \return Nothing.
void 
GLCDFillScreen(char cColor      //!<[in]Color of the pixels.
               )
{
    int i, j, k;
    char isON;

    if (cColor == 1)
    {
        isON = 0xFF;
    }
    else
    {   
        isON = 0x00;
    } 

    // Loop through the vertical pages
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 2; j++)
        {
            m_uiSide = j;
            GLCDWriteCmd(0xB8 + i);                // Set page address
            
            // Loop through the columns
            GLCDWriteCmd(0x00);
            for (k = 1; k <= (GLCD_WIDTH/2); k++)
            {
                GLCDWriteData(isON);  // Turn pixels on or off
            }
        }
    }
}

//! Configures the Graphical LCD.
//! Set the duty cycle to 1/32, turns on the display and sets the start position to 0,0.
//!
//! \return Nothing.
void GLCDConfig()
{
    int i;

    GLCD_R_W_TRIS = 0;
    GLCD_STROBE_TRIS = 0;
    GLCD_ENABLE1_TRIS = 0;
    GLCD_ENABLE2_TRIS = 0;

    for (i = 0; i < 2; i++)
    {
        m_uiSide = i;
	    GLCDWriteCmd(0xAF);         // Turn on the display
	    GLCDWriteCmd(0xA4);         // Static drive is off      
	    GLCDWriteCmd(0xA9);         // Duty cycle is set to 1/32
	    GLCDWriteCmd(0xE2);         // Reset the chip
	    GLCDWriteCmd(0xA0);         // Set ADC
	    GLCDWriteCmd(0xEE);         // Read modify write disabled
	    GLCDWriteCmd(0xC0);         // Set the start position to 
	    GLCDWriteCmd(0xB8);         // the top left of the display
	    GLCDWriteCmd(0x00);         // in column 0
   }
    
    GLCD_ENABLE1 = 0;               // De-select each side to start
    GLCD_ENABLE2 = 0; 
}

//! Draws a pixel into the graphical LCD screen with a color.
//! The color of the pixel can be ON(1) or OFF(0).
//!
//! Horizontal positions must be between 1 and GLCD_WIDTH.
//! Vertical positions must be between 1 and GLCD_HEIGHT.
//!
//! \return Nothing.
void
GLCDDrawPixel(unsigned int uiX,     //!<[in]Horizontal position of the pixel.
              unsigned int uiY,     //!<[in]Vertical position of the pixel.
              char cColor           //!<[in]Color of the pixel.
              )
{
    int ucMask;
    int i;
    unsigned char ucData;

    // Which side of the display (which SED1520 controller)
    if (uiX <= (GLCD_WIDTH/2))
    {   
        m_uiSide = LEFT;
    }
    else 
    {
        m_uiSide = RIGHT;
    }

    // Which vertical page needs selecting
    ucData = 0xB8 + ((uiY - 1) >> 3);        // Control data to write is
    GLCDWriteCmd(ucData);                    // 0xB8 + the page number (0-3)
    if (uiY > 8)
    {
        uiY -= ((ucData - 0xB8) << 3);       // y should always be 1-8
    }

    // Horizontal position (column)
    if (uiX > (GLCD_WIDTH/2))
    { 	                                    // x should always be 0 and GLCD_WIDTH / 2
	    uiX -= (GLCD_WIDTH/2);	            // for each half of the LCD
    }
    uiX -= 1;                              
    
    GLCDWriteCmd(uiX);                      // Go to column 'x' 
    GLCDWriteCmd(0xe0);	                    // Start R-M-Write operation
    GLCDReadData();		                    // Dummy Read
    ucData = GLCDReadData();		        // Read LCD display data
    ucMask = 1;                             // Create a bit mask of the
    for (i = 1; i < uiY; i++)		        // pixel to change
    {
 	   ucMask = ucMask << 1;               
    }

    if (cColor) 
    {
        ucData |= ucMask;                   // Set or clear the pixel as                        
    }
    else 
    {   
        ucData &= (~ucMask);                // requested
    }
    GLCDWriteData(ucData);
    GLCDWriteCmd(0xee);		                // End R-M-Write operation
}


//! Draws a line into the graphical LCD screen with a color.
//! The color of the pixel can be ON(1) or OFF(0).
//!
//! This function is based on the Optimized Bresenham line algorithm.
//!
//! Horizontal positions must be between 1 and GLCD_WIDTH.
//! Vertical positions must be between 1 and GLCD_HEIGHT.
//!
//! \return Nothing.
void 
GLCDDrawLine(unsigned int uiX1,    //!<[in]Horizontal position of the start.
             unsigned int uiY1,    //!<[in]Vertical position of the start.
             unsigned int uiX2,    //!<[in]Horizontal position of the end.
             unsigned int uiY2,    //!<[in]Vertical position of the end.
             char cColor           //!<[in]Color of the pixel.
             )
{
    int deltay, deltax;
    int d, diff, i;
    char stepx, stepy;
    
    deltax = abs((signed int)(uiX2 - uiX1));
    deltay = abs((signed int)(uiY2 - uiY1));

    if (uiX1 > uiX2)                                // X values are decreasing
    {
        stepx = -1;
    }
    else
    {
        stepx = 1;
    }
    if (uiY1 > uiY2)                                // Y values are decreasing
    {
        stepy = -1;
    }
    else
    {
        stepy = 1;
    }

    if (deltax >= deltay)                           // There is at least one x-value for every y-value
    {
        deltay *= 2;                                // dy is now 2*dy
        d = deltay - deltax;                        // P = 2*Ay - Ax
        diff = d - deltax;                          // Diff = 2*(Ay - Ax)
        
        for(i = 0; i <= deltax; i++)
        {
            GLCDDrawPixel(uiX1, uiY1, cColor);      

            uiX1 += stepx;
            if (d < 0)
            {
                d  += deltay;                       // d = d + 2*(Y1-Y0)
            }
            else
            {
                d  += diff;                         // d = d + 2*( (Y1-Y0) - (X1-X0) ) 
                uiY1 += stepy;
            }
        }
    }
    else                                            // There is at least one y-value for every x-value
    {
        deltax *= 2;
        d = deltax - deltay;
        diff = d - deltay;

        for(i = 0; i <= deltay; i++)
        {
            GLCDDrawPixel(uiX1, uiY1, cColor);

            uiY1 += stepy;
            if (d < 0)
            {
                d  += deltax;
            }
            else
            {
                d  += diff;
                uiX1 += stepx;
            }
        }
    }
}

//! Draws a rectangle into the graphical LCD screen with a color.
//! The color of the pixel can be ON(1) or OFF(0).
//!
//! The rectangle could be drawn filled by setting the corresponding flag to 1.
//!
//! Horizontal positions must be between 1 and GLCD_WIDTH.
//! Vertical positions must be between 1 and GLCD_HEIGHT.
//!
//! \return Nothing.
void 
GLCDDrawRect(unsigned int uiX1,     //!<[in]Horizontal position of the start.
             unsigned int uiY1,     //!<[in]Vertical position of the start.
             unsigned int uiX2,     //!<[in]Horizontal position of the end.
             unsigned int uiY2,     //!<[in]Vertical position of the end.
             char cFill,            //!<[in]Indicates if the rectangle must be filled.
             char cColor            //!<[in]Color of the pixel.
             )
{
    if (cFill)
    {
        int i, xmin, xmax, ymin, ymax;
    
        if (uiX1 < uiX2)                            //  Find x min and max
        {
            xmin = uiX1;
            xmax = uiX2;
        }
        else
        {
            xmin = uiX2;
            xmax = uiX1;
        }
        
        if (uiY1 < uiY2)                            // Find the y min and max
        {
            ymin = uiY1;
            ymax = uiY2;
        }
        else
        {
            ymin = uiY2;
            ymax = uiY1;
        }
    
        for(; xmin < xmax; xmin++)
        {
            for(i = ymin; i < ymax; i++)
            {
                GLCDDrawPixel(xmin, i, cColor);
            }
        }
    }
    else
    {
        GLCDDrawLine(uiX1, uiY1, uiX2, uiY1, cColor);      // Draw the 4 sides
        GLCDDrawLine(uiX1, uiY2, uiX2, uiY2, cColor);
        GLCDDrawLine(uiX1, uiY1, uiX1, uiY2, cColor);
        GLCDDrawLine(uiX2, uiY1, uiX2, uiY2, cColor);
    }
}

//! Draws a circle into the graphical LCD screen with a color.
//! The color of the pixel can be ON(1) or OFF(0).
//!
//! The circle could be drawn filled by setting the corresponding flag to 1.
//!
//! This function is based on the Optimized Bresenham circle algorithm.
//!
//! Horizontal positions must be between 1 and GLCD_WIDTH.
//! Vertical positions must be between 1 and GLCD_HEIGHT.
//!
//! \return Nothing.
void 
GLCDDrawCircle(unsigned int uiX,      //!<[in]Horizontal position of the center.
               unsigned int uiY,      //!<[in]Vertical position of the center.
               unsigned int uiRadius, //!<[in]Radius of the circle.
               char cFill,            //!<[in]Indicates if the circle must be filled.
               char cColor            //!<[in]Color of the pixel.
               )
{
    signed int a, b, d;
   
    a = 0;
    b = uiRadius;
    d = 3 - (2*uiRadius);
        
    do
    {
        if (cFill)
        {
            GLCDDrawLine(uiX - a, uiY + b, uiX + a, uiY + b, cColor);
            GLCDDrawLine(uiX - a, uiY - b, uiX + a, uiY - b, cColor);
            GLCDDrawLine(uiX - b, uiY + a, uiX + b, uiY + a, cColor);
            GLCDDrawLine(uiX - b, uiY - a, uiX + b, uiY - a, cColor);
        }
        else
        {
            GLCDDrawPixel(uiX + a, uiY + b, cColor);
            GLCDDrawPixel(uiX + a, uiY - b, cColor);
            GLCDDrawPixel(uiX - a, uiY + b, cColor);
            GLCDDrawPixel(uiX - a, uiY - b, cColor);
            GLCDDrawPixel(uiX + b, uiY + a, cColor);
            GLCDDrawPixel(uiX + b, uiY - a, cColor);
            GLCDDrawPixel(uiX - b, uiY - a, cColor);
            GLCDDrawPixel(uiX - b, uiY + a, cColor);
        }

        if (d < 0)
        {
            d += (6 + 4 * a++);
        }
        else
        {
            d += (10 + 4 * (a++ - b--));
        }
    } while(a <= b);
}

//! Draws a text into the graphical LCD screen with a color.
//! The color of the pixel can be ON(1) or OFF(0).
//!
//! The size of the characters could be scaled in multiples of 7x5.
//!
//! Horizontal positions must be between 1 and GLCD_WIDTH.
//! Vertical positions must be between 1 and GLCD_HEIGHT.
//!
//! \return Nothing.
void 
GLCDDrawText(unsigned int uiX,      //!<[in]Horizontal position of the center.
             unsigned int uiY,      //!<[in]Vertical position of the center.
             char* pcText,          //!<[in]Pointer to the string to draw.
             char cSize,            //!<[in]Size of the text. (1 means 7x5 size, 2 14x10,...)
             char cColor            //!<[in]Color of the pixel.
             )
{
    char j, k, l, m;                       // Loop counters
    char pixelData[5];                     // Stores character data

    for(; *pcText != '\0'; ++pcText, ++uiX)// Loop through the passed string
    {
        if (*pcText <= '~') // Check if the letter is in the second font array
        {
            memcpy(pixelData, FONT[*pcText - ' '], 5);
        }
        else
        {
            memcpy(pixelData, FONT[0], 5);   // Default to space
        }

        // Handles newline and carriage returns
        switch(*pcText)
        {
            case '\n':
                uiY += 7*cSize + 1;
                continue;
            case '\r':
                uiX = 0;
                continue;
        }

        if (uiX + 5*cSize >= GLCD_WIDTH)            // Performs character wrapping
        {
            uiX = 0;                                // Set x at far left position
            uiY += 7*cSize + 1;                     // Set y at next position down
        }
        for (j = 0; j < 5; ++j, uiX += cSize)       // Loop through character byte data
        {
            for (k = 0; k < 7; ++k)                 // Loop through the vertical pixels
            {
                if ((pixelData[j] >> k) & 0x01)
                {
                    for (l = 0; l < cSize; ++l)     // These two loops change the
                    {                               // character's size
                        for (m = 0; m < cSize; ++m)
                        {
                            GLCDDrawPixel(uiX + m, uiY + k*cSize + l, cColor); // Draws the pixel
                        }
                    }
                }
            }
        }
    }
}

//! Draws an image into the graphical LCD screen with a color.
//! The color of the pixel can be ON(1) or OFF(0).
//!
//! The image must be an array of uiSizeX, uiSizeY elements. Each element correspont to 8 horizontal pixels.
//!
//! Horizontal positions must be between 1 and GLCD_WIDTH.
//! Vertical positions must be between 1 and GLCD_HEIGHT.
//! 
//! I.e.:
//! \code
//! uiSizeX = 2 (2 * 8 = 16 pixels)
//! uiSizeY = 17
//!
//!  Byte1  |  Byte2
//!      XXX|XX       ->  0x07, 0xC0
//!     X   |  X      ->  0x08, 0x20
//!     X   |  X      ->  0x08, 0x20
//!     X   |  X      ->  0x08, 0x20
//!      X  | X       ->  0x04, 0x40
//! XXXXXX  | XXXXXXX ->  0xFC, 0x7F
//! X       |       X ->  0x80, 0x01
//! X       |       X ->  0x80, 0x01
//! X       |   XXX X ->  0x80, 0x1D
//! X       |  X   X  ->  0x80, 0x22
//! X       |  X      ->  0x80, 0x20
//! X     XX|  X   XX ->  0x83, 0x23
//! X    X  |X  XXX X ->  0x84, 0x9D
//! X   X   | X     X ->  0x88, 0x41
//! X   X   | X     X ->  0x88, 0x41
//! X    X  |X     X  ->  0x84, 0x82
//! XXXXXX  |XXXXXX   ->  0xFC, 0xFC
//! \endcode
//! \return Nothing.
void 
GLCDDrawImage(unsigned int uiX,         //!<[in]Horizontal position of the topleft point.
              unsigned int uiY,         //!<[in]Vertical position of the topleft point.
              unsigned int uiSizeX,     //!<[in]Width of the image.
              unsigned int uiSizeY,     //!<[in]Height of the image.
              char* pImage,             //!<[in]Pointer to the image.
              char cColor               //!<[in]Color of the pixel.
              )
{
    unsigned int mx, my;
    char i ,k;
    int iStartX, iStartY;
    int iColum, iItem;

    iStartY = uiY;
    iColum = 0;  

    for (my = 0; my < uiSizeY; my++)
    {
        iStartX = uiX;
        iItem = iColum;
        for (mx = 0; mx < uiSizeX; mx++)
        {
            i = 7;
            for (k = 0; k < 8; k++, i--)
            {
                if ((pImage[iItem] >> i) & 0x01)
                {
                    GLCDDrawPixel(iStartX + k, iStartY, cColor);
                }
            }
            iItem ++;
            iStartX += 8;
        }
        iStartY ++;
        iColum += uiSizeX;
    }
}
