#include <avr/io.h>
#include "spi.h"

// LECTURE 21 CODE FOR SPI COMMUNICATION WITH ADXL345 ACCELEROMETER
void initSPI(void) {
    // set the SS, MOSI, and SCLK pin as output
    DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
    // set the MISO pin as input
    DDRB &= ~(1 << DDB3);


    // pull up resistors
    PINB |= (1 << PINB0) | (1 << PINB1) | (1 << PINB2);

    
    // set SS high at first
    PORTB |= (1 << PORTB0);
    // enable the interrupt, SPI, master mode
    SPCR |= (1 << SPE)|(1 << MSTR);
    //ADXL345 (Accelormeter) CPOL = 1, CPHA = 1
    SPCR |=(1 << CPOL)|(1 << CPHA)|(1 << SPR1)|(1 << SPR0);
    // set DORD to MSB first
    SPCR &= ~(1 <<DORD);
    // disable the SPI interrupt (not using ISRs)
    SPCR &= ~(1 << SPIE);

    SPI_Send16(0x09, 0x00);
    SPI_Send16(0x0a, 0x03);
    SPI_Send16(0x0b, 0x07);
    SPI_Send16(0x0c, 0x01);
    SPI_Send16(0x0f, 0x00);
}

void SPI_Write(unsigned char data) {
    PORTB &= ~(1 << PORTB0);     // SS LOW (select)

    SPDR = data;                 // send byte
    while (!(SPSR & (1 << SPIF))); // wait done

    PORTB |= (1 << PORTB0);      // SS HIGH (deselect)
}

void SPI_Send16(unsigned char address, unsigned char data) {
    PORTB &= ~(1 << PORTB0);   // SS LOW

    SPDR = address;            // send register
    while (!(SPSR & (1 << SPIF)));

    SPDR = data;               // send data
    while (!(SPSR & (1 << SPIF)));

    PORTB |= (1 << PORTB0);    // SS HIGHz
}

void displaySmile() {
    unsigned char smile[8] = {
        0x3C, 0x42, 0xA5, 0x81,
        0xA5, 0x99, 0x42, 0x3C
    };
    for (int i = 1; i <= 8; i++)
        SPI_Send16(i, smiley[i - 1]);
}

void displayFrown() {
    unsigned char frown[8] = {
        0x3C, 0x42, 0xA5, 0x81,
        0x99, 0xA5, 0x42, 0x3C
    };
    for (int i = 1; i <= 8; i++)
        SPI_Send16(i, frown[i - 1]);
}
