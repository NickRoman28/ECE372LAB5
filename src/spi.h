#ifndef SPI_H
#define SPI_H

void initSPI(void);
void SPI_Write(unsigned char data);
void SPI_Send16(unsigned char address, unsigned char data);
void clearDisplay(void);
void displaySmiley(void);
void displayFrowny(void);

#endif