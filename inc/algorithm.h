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


#define RED			0
#define GREEN 		1
#define BLUE		2
#define RED_GREEN 	3
#define RED_BLUE 	4
#define GREEN_BLUE  5
#define ALL			6

#ifndef FIRST
#	define FIRST 	0
#endif
#ifndef SECOND
#	define SECOND 	1
#endif

#define CALIBRATE_INDX		0
#define MEASURE_INDX		1
#define DIFF_INDX			2
//#ifndef PERCENT
//#	define PERCENT	1
//#endif

// using for eval coefs
#define ADC_MAX_VALUE 	0x3FF

// Wait for foto-resistor in ms
#define MEASURE_DELAY_BEFORE_START_ADC		2

extern uint8_t measure_delay;

extern volatile uint16_t adc_data;
extern uint16_t result[3][7];
//extern double coefs[3];

extern uint16_t hex2dec_result(uint16_t hex);
extern void MeasureAllAlgorithm(uint8_t color);
extern void CalibrationAlgorithm(uint8_t color);
extern void ADC_LoadingAndEvalIt(ptrEvalMe evalMe);
extern void SaveMeasureResultsToCalibrate(void);


#endif /* ALGORITHM_H_ */
