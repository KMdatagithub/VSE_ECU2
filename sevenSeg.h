/*
 * sevenSeg_BCD.h
 *
 *  Created on: Oct 24, 2019
 *      Author: Sprints
 */

#ifndef SEVENSEG_H_
#define SEVENSEG_H_

#include "gpio.h"
#include "sevenSegConfig.h"


typedef enum En_SevenSegId_t{
	SEG_0,
	SEG_1,
}En_SevenSegId_t;

/**
 * Description: Initializes The GPIO For 7Seg Display (EN Signal, BCD Pins)
 * @param a_segment_id: The ID OF The Specific 7Segment Unit You Want To Initialize
 */
void sevenSegInit(En_SevenSegId_t en_segment_id);

/**
 * Description: Enables The 7Segment Unit By Switching ON The Control Transistor
 * @param en_segment_id: The ID OF The Specific 7Segment Unit You Want To Enable
 */
void sevenSegEnable(En_SevenSegId_t en_segment_id);
/**
 * Description: Disables The 7Segment Unit By Switching OFF The Control Transistor
 * @param en_segment_id: The ID OF The Specific 7Segment Unit You Want To Disable
 */
void sevenSegDisable(En_SevenSegId_t en_segment_id);

/**
 * Description: Writes The Required Number On The BCD Data Pins
 * @param en_segment_id: The ID OF The Specific 7Segment Unit You Want To Write To
 * @param u8_number:     The Actual Data You Want To Display
 */
void sevenSegWrite(En_SevenSegId_t en_segment_id, uint8_t u8_number);


#endif /* SEVENSEG_BCD_H_ */
