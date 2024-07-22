#ifndef _RECORD_FUNCTION_H
#define _RECORD_FUNCTION_H

#include "TypeDefine.h"

/**
 * @brief Stops the recording process.
 * This function stops the recording timer and updates the stop time counter.
 * @return void
 */
void StopRecording();

/**
 * @brief Starts the recording process.
 * This function prepares the system to record and enters a loop to handle ADC sampling and data storage.
 * @return void
 */
void StartRecording();

/**
 * @brief Handles ADC sampling.
 * This function samples ADC values, updates buffers, and sets flags when buffers are full.
 * @return void
 */
void ADC_Sampling();
#endif