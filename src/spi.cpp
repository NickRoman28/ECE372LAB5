#include <avr/io.h>
#include "spi.h"
#include "Arduino.h"
#include <avr/interrupt.h>

void initSPI(void) {
    DDRB = (1 << DDB2) | (1 << DDB1) | (1 << DDB0); // Ensure MOSI, SCK, and SS are outputs
    PORTB |= (1 << PORTB0); // Set SS high
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1 << SPR1); // Enable SPI, Master mode, set clock rate fck/16
    SPCR &= ~((1 << CPOL) | (1 << CPHA));

}

void SPI_Write(unsigned char data, unsigned char isData) {
    PORTB &= ~(1 << PORTB0); // Set SS low to select the device
    SPDR = data; // Load data into SPI Data Register
    while (!(SPSR & (1 << SPIF))); // Wait for transmission to complete
    SPDR = isData;
    while (!(SPSR & (1 << SPIF))); // Wait for transmission to complete
    PORTB |= (1 << PORTB0); // Set SS high to deselect the device
}

void displaySmiley() {
    SPI_Write(0x01, 0b00000000);
    SPI_Write(0x02, 0b00100100);
    SPI_Write(0x03, 0b00100100);
    SPI_Write(0x04, 0b00000000);
    SPI_Write(0x05, 0b00000000);
    SPI_Write(0x06, 0b01000010);
    SPI_Write(0x07, 0b00111100);
    SPI_Write(0x08, 0b00000000);

}

void displayFrowny() {
    SPI_Write(0x01, 0b00000000);
    SPI_Write(0x02, 0b00100100);
    SPI_Write(0x03, 0b00100100);
    SPI_Write(0x04, 0b00000000);
    SPI_Write(0x05, 0b00000000);
    SPI_Write(0x06, 0b00111100);
    SPI_Write(0x07, 0b01000010);
    SPI_Write(0x08, 0b00000000);
}

void screen_init() {
    SPI_Write(0x09, 0x00); // no decode
    SPI_Write(0x0A, 0x03);
    SPI_Write(0x0B, 0x07);
    SPI_Write(0x0C, 0x01);
    SPI_Write(0x0F, 0x00);
}