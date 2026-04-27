#ifndef SPI_H
#define SPI_H

void initSPI(void);
void SPI_Write(unsigned char address, unsigned char data);

void screen_init(void);
void displaySmiley(void);
void displayFrowny(void);

#endif