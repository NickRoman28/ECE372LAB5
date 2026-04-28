#ifndef PWM_H
#define PWM_H

void initPWM(void);
void buzzerOn(unsigned int f);
void buzzerOff(void);
void incFrequency(void);
void decFrequency(void);

#endif
