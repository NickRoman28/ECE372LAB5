#include <avr/io.h>

// turn I2C hardware on
void InitI2C() {
  // Set SCL frequency to 100kHz
  TWSR = 0x00; // Prescaler value = 1
  TWBR = 0x48; // SCL frequency = F_CPU / (16 + 2 * TWBR * prescaler) //0x48 = 72 decimal
  // enable TWI
  TWCR = (1 << TWEN);
}

void StartI2C_Trans(unsigned char SLA) {
  /*Send START condition
  TWINT flags that reads or writes have been completed
  TWSTA sends a START condition on the bus
  TWEN enables the TWI */
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  // Wait for TWINT flag set, which indicates that the START condition has been transmitted
  while (!(TWCR & (1 << TWINT)));

    // Load SLA+W into TWDR Register. Clear TWINT bit in TWCR to start transmission of address
    TWDR = (SLA << 1); // Load SLA+W into TWDR Register
    TWCR = (1 << TWINT) | (1 << TWEN); // Clear TWINT bit in TWCR to start transmission of address
    // Wait for TWINT flag set, which indicates that the SLA+W has been transmitted
    while (!(TWCR & (1 << TWINT)));
}

// sends the data to the sensor after telling it which register to read from
void Write(unsigned char data) {
  // Load data into TWDR Register. Clear TWINT bit in TWCR to start transmission of data
  TWDR = data; // Load data into TWDR Register
  TWCR = (1 << TWINT) | (1 << TWEN); // Clear TWINT bit in TWCR to start transmission of data
  // Wait for TWINT flag set, which indicates that the data has been transmitted
  while (!(TWCR & (1 << TWINT)));
}

void StopI2C_Trans() {
  // Transmit STOP condition
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

// tells the sensor which register to read from
void Read_from(unsigned char SLA, unsigned char MEMADDRESS) {
    StartI2C_Trans(SLA);          // SLA + W
    Write(MEMADDRESS);            // register address

    // Repeated START
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    // SLA + R
    TWDR = (SLA << 1) | 0x01;
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
}

// reads the data from the sensor after telling it which register to read from
unsigned char Read_data() {
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

