#ifndef READ_KEYBOARD_H_
#define READ_KEYBOARD_H_


#include"TypeDefine.h"


#define DEBOUNCE_DELAY 50 // Debounce delay in milliseconds
#define PRESS_DELAY 5000  // Press delay in milliseconds (5 seconds)

/**
 * @brief Main keyboard handler.
 * This function reads key presses, manages debounce, and updates the recorder state based on the pressed key.
 * @return void
 */
void KeyBoard();

#endif