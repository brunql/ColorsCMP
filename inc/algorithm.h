/*
 * algorithm.h
 *
 *  Created on: 11.02.2010
 *      Author: brunql
 */

#ifndef ALGORITHM_H_
#define ALGORITHM_H_


typedef void (*ptrEvalMe)(uint8_t);

typedef enum {
	FIRST=0, SECOND=1
} FirstOrSecond;


#define EVP_ALGORITM
#define EVP_ALG_PERCENT  (0.50)


#define RED		0
#define GREEN 	1
#define BLUE	2

#ifndef FIRST
#	define FIRST 	0
#endif
#ifndef SECOND
#	define SECOND 	1
#endif
#ifndef PERCENT
#	define PERCENT	2
#endif

// using for eval coefs
#define ADC_MAX_VALUE 	0x3FF

// Wait for foto-resistor in ms
#define DELAY_BEFORE_START_ADC		2000



extern volatile int16_t adc_data;
extern uint16_t result[3][3];
extern double coefs[2][3];
extern uint16_t zero[2];

extern void CalibrationAlgorithm(uint8_t color);
extern void SetZeroAlgorithm(uint8_t color);
extern void SaveResultsAlgorithm(uint8_t color);
extern void ADC_LoadingAndEvalIt(ptrEvalMe evalMe);


#endif /* ALGORITHM_H_ */
