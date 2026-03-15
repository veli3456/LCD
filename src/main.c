#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define INSTR_REG_ENABLE (PORTB &= ~(1<<PB0) )
#define DATA_REG_ENABLE (PORTB |= (1<<PB0) )
#define RW_WRITE_MODE (PORTB &= ~(1<<PB1) )
#define RW_READ_MODE (PORTB |= (1<<PB1) )
#define ENABLE (PORTB |= (1<<PB2) )
#define DISABLE (PORTB &= ~(1<<PB2) )


void checkBusyFlag () {
    DDRC &= ~(0x0F); // Set as INPUT
    RW_READ_MODE;
    INSTR_REG_ENABLE;

    unsigned char BUSY_FLAG;

    do {
        ENABLE;
        _delay_us (1);

        BUSY_FLAG = PINC; // CATCH THE BUSY FLAG

        DISABLE;
        _delay_us (1);

        ENABLE;
        _delay_us(1);

        DISABLE;
        _delay_us (1);

    } while ( (BUSY_FLAG & 0x08) );

    DDRC |= (0x0F);
    RW_WRITE_MODE;

} 

void lcd_cmd (unsigned char command) {
    checkBusyFlag ();
    INSTR_REG_ENABLE;
    RW_WRITE_MODE; 

    _delay_us (1);
    ENABLE; 
    PORTC = (command>>4); // High Bits
    _delay_us (1);
    DISABLE;

    _delay_us (1);
    ENABLE; 
    PORTC = command; // Low Bits
    _delay_us (1);                    
    DISABLE;

}

void lcd_initialise () {
    // Special Initialization Sequence
    INSTR_REG_ENABLE;
    RW_WRITE_MODE;

    _delay_ms (30);

    PORTC = 0x03;
    ENABLE;
    _delay_us(2);
    DISABLE;

    _delay_ms (10);

    PORTC = 0x03;
    ENABLE;
    _delay_us(2);
    DISABLE;

    _delay_us (150);

    PORTC = 0x03;
    ENABLE;
    _delay_us(2);
    DISABLE;
    _delay_us(2);

    PORTC = 0x02;
    ENABLE;
    _delay_us(2);
    DISABLE;

    lcd_cmd (0x20); // Function set: data length 4 bit, # of lines = 1, Font: 5x8
    lcd_cmd (0x0C); // Display ON, Cursor OFF
    lcd_cmd (0x01); // Display Clear
    lcd_cmd (0x06); // Increment by 1
}

void lcd_data (unsigned char data) {
    checkBusyFlag();
    DATA_REG_ENABLE;
    RW_WRITE_MODE; 

    _delay_us (1);
    ENABLE; 
    PORTC = (data>>4); // High Bits
    _delay_us (1);
    DISABLE;

    _delay_us (1);
    ENABLE; 
    PORTC = data; // Low Bits
    _delay_us (1);                    
    DISABLE;

}

void lcd_string (const unsigned char *str) {

    for (int i = 0; i < strlen (str); i++) {
        lcd_data (str [i]);
    }
}


int main () {

    /* LCD */
    DDRC |= (1<<DDC0) | (1<<DDC1) | (1<<DDC2) | (1<<DDC3); // D4 to D7
    DDRB |= (1<<DDB0) | (1<<DDB1) | (1<<DDB2); // RS, RW, E

    /* SEGMENT DISPLAY */
    DDRD = 0x7F; // PD0 to PD6

    char LCD_Arr [] [7] = {"ZERO", "ONE", "TWO", "THREE", "FOUR", 
                           "FIVE", "SIX", "SEVEN", "EIGHT", "NINE"};

    char SEG_Arr [] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

    lcd_initialise();


    while (1) {


        lcd_cmd (0x80); // Set DDRAM Adress 0 in address counter

        for (int i = 0; i < 10; i++) {
        lcd_string (LCD_Arr [i]);
        PORTD = SEG_Arr [i];
        _delay_ms (1000);

        lcd_cmd (0x01); // Clear Display and set DDRAM Address 0 in address counter
        _delay_ms (4);
        }
        

    }


    return 0;
}