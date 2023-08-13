#ifndef ADC_H
#define	ADC_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif

void adc_init(void);
void initBuffer(void);
void putVal(int ADCvalue);
unsigned long int getAvg(void);

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */