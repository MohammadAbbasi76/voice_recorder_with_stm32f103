#ifndef _PALYS_FUNCTIONS_H
#define _PALYS_FUNCTIONS_H

#include "TypeDefine.h"



/**
 * @brief Starts the playback process.
 * This function turns on the playback LED, prepares the system for playback, starts timers and PWM, 
 * and manages buffer switching during playback.
 * @return void
 */
void StartPlaying();

/**
 * @brief Stops the playback.
 * This function stops the timers and PWM, and sets the read position to the end of the saved data.
 * @return void
 */
void StopPlaying();

/**
 * @brief Converts ADC values to PWM duty cycle values.
 * This function converts an array of ADC values to corresponding PWM duty cycle values.
 * @param val Pointer to the array of ADC values.
 * @return void
 */
void ConversionADCValueToPWMDuty(uint16_t *val);

/**
 * @brief Generates the PWM wave for playback.
 * This function sets the PWM compare value based on the buffer and updates the buffer status.
 * @return void
 */
void MakePwmWave();

#endif