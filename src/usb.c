/*
 * usb.c
 *
 *  Created on: 28.04.2010
 *      Author: brunql
 */

/*
This example should run on most AVRs with only little changes. No special
hardware resources except INT0 are used. You may have to change usbconfig.h for
different I/O pins for USB. Please note that USB D+ must be the INT0 pin, or
at least be connected to INT0 as well.
*/

#include <avr/io.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"
//#include "oddebug.h"        /* This is also an example for using debug macros */
#include "main.h"
#include "algorithm.h"

#include "lcd/lcd_nokia_menu.h"
#include "lcd/lcd_nokia_3310_frm_brunql.h"

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

PROGMEM char usbHidReportDescriptor[22] = {    /* USB report descriptor */
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x80,                    //   REPORT_COUNT (128)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0                           // END_COLLECTION
};
/* Since we define only one feature report, we don't use report-IDs (which
 * would be the first byte of the report). The entire report consists of 128
 * opaque data bytes.
 */

/* The following variables store the status of the current data transfer */
static uint8_t    current_data = 'C';

/* ------------------------------------------------------------------------- */

/* usbFunctionRead() is called when the host requests a chunk of data from
 * the device. For more information see the documentation in usbdrv/usbdrv.h.
 */
uint8_t   usbFunctionRead(uint8_t *data, uint8_t len)
{
    data[0] = alg_state;
    if(current_data == 'C'){
    	data[1] = 'C';
    	data[2] = (uint8_t) ((result[CALIBRATE_INDX][RED] & 0xff00) >> 8);
    	data[3] = (uint8_t) result[CALIBRATE_INDX][RED] & 0x00ff;
    	data[4] = (uint8_t) ((result[CALIBRATE_INDX][GREEN] & 0xff00) >> 8);
    	data[5] = (uint8_t) result[CALIBRATE_INDX][GREEN] & 0x00ff;
    	data[6] = (uint8_t) ((result[CALIBRATE_INDX][BLUE] & 0xff00) >> 8);
    	data[7] = (uint8_t) result[CALIBRATE_INDX][BLUE] & 0x00ff;
    }else if(current_data == 'M'){
    	data[1] = 'M';
    	data[2] = (uint8_t) ((result[MEASURE_INDX][RED] & 0xff00) >> 8);
    	data[3] = (uint8_t) result[MEASURE_INDX][RED] & 0x00ff;
    	data[4] = (uint8_t) ((result[MEASURE_INDX][GREEN] & 0xff00) >> 8);
    	data[5] = (uint8_t) result[MEASURE_INDX][GREEN] & 0x00ff;
    	data[6] = (uint8_t) ((result[MEASURE_INDX][BLUE] & 0xff00) >> 8);
    	data[7] = (uint8_t) result[MEASURE_INDX][BLUE] & 0x00ff;
    }else if(current_data == 'D'){
    	data[1] = 'D';
    	data[2] = (uint8_t) ((result[DIFF_INDX][RED] & 0xff00) >> 8);
    	data[3] = (uint8_t) result[DIFF_INDX][RED] & 0x00ff;
    	data[4] = (uint8_t) ((result[DIFF_INDX][GREEN] & 0xff00) >> 8);
    	data[5] = (uint8_t) result[DIFF_INDX][GREEN] & 0x00ff;
    	data[6] = (uint8_t) ((result[DIFF_INDX][BLUE] & 0xff00) >> 8);
    	data[7] = (uint8_t) result[DIFF_INDX][BLUE] & 0x00ff;

//    	Lcd3310_GotoXY(0,1);
//    	Lcd3310_UInt16AsText(result[DIFF_INDX][BLUE], BLACK_TEXT_ON_WHITE);
//    	Lcd3310_UInt8AsText(data[6], WHITE_TEXT_ON_BLACK);
//    	Lcd3310_UInt8AsText(data[7], WHITE_TEXT_ON_BLACK);

    	alg_state = NOTHING_CHANGED;
    }
    return len;
}

/* usbFunctionWrite() is called when the host sends a chunk of data to the
 * device. For more information see the documentation in usbdrv/usbdrv.h.
 */
uint8_t   usbFunctionWrite(uint8_t *data, uint8_t len)
{
//    if(bytesRemaining == 0)
//        return 1;               /* end of transfer */
//    if(len > bytesRemaining)
//        len = bytesRemaining;
//    //eeprom_write_block(data, (uint8_t *)0 + currentAddress, len);
//
////    uint16_t leds = (uint16_t)(data[3] << 8) | data[4];
////    LedDriver_SwitchLeds(leds);
//
//    currentAddress += len;
//    bytesRemaining -= len;

//	return bytesRemaining == 0; /* return 1 if this was the last chunk */


	//Lcd3310_ClearCenter();
	//Lcd3310_GotoXY(0, 3);

	current_data = data[0];
	if(data[0] == 'G'){ // Start algorithm
		menu_now = &go;
		FLAGS_SWITCH_ON( JOYSTICK_CENTER_CLICK_FLAG );
	}else if(data[0] == 'S'){ // Save values
		FLAGS_SWITCH_ON( SAVE_MEASURED_AS_CALIBRATE_FLAG );
	}
//	Lcd3310_GotoXY(0,0);
//	Lcd3310_UInt8AsText(data[0], BLACK_TEXT_ON_WHITE);

	return 1;
}

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uint8_t data[8])
{
	usbRequest_t    *rq = (void *)data;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* HID class request */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
//            /* since we have only one report type, we can ignore the report-ID */
//            bytesRemaining = 128;
//            currentAddress = 0;
            return USB_NO_MSG;  /* use usbFunctionRead() to obtain data */
        }else if(rq->bRequest == USBRQ_HID_SET_REPORT){
//            /* since we have only one report type, we can ignore the report-ID */
//            bytesRemaining = 128;
//            currentAddress = 0;
            return USB_NO_MSG;  /* use usbFunctionWrite() to receive data from host */
        }
    }else{
        /* ignore vendor type requests, we don't use any */
    }
    return 0;
}

/* ------------------------------------------------------------------------- */

void usbInit_FakeUsbDisconnect(void)
{
	/* RESET status: all port bits are inputs without pull-up.
     * That's the way we need D+ and D-. Therefore we don't need any
     * additional hardware initialization.
     */

	usbInit();
    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
    uint8_t i = 0;
    while(i --> 0){             /* fake USB disconnect for > 250 ms */
        _delay_ms(1);
    }
    usbDeviceConnect();
    usbPoll();
}
