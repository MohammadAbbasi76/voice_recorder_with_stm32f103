#include "ReadKeyboard.h"

/**
 * @brief Handles the press of Key 1 with debounce.
 * This function manages the debounce timing for Key 1 and updates the recorder state if the key press is confirmed.
 * @param DebounceTime Time elapsed since the last key press in milliseconds.
 * @return uint16_t Returns true if the key press is valid, false otherwise.
 */
uint16_t ChangeMode(uint32_t DebounceTime)
{
    static uint32_t timer;
    timer += DebounceTime;
    if (timer >= PRESS_DELAY)
    {
        VoiceRecorderSt.State = FlashEraseState;
        timer = 0;
        return true;
    }
    return false;
}

/**
 * @brief Reads the state of the keyboard.
 * This function reads the state of multiple keys and returns the state of the pressed key.
 * @return uint16_t Returns the pin number of the pressed key or 0 if no key is pressed.
 */
uint16_t ReadKeyBoard(void)
{
    uint16_t KeyPressed = 0;
    if (!(HAL_GPIO_ReadPin(Pause_Key_GPIO_Port, Pause_Key_Pin)))
    {
        HAL_GPIO_ReadPin(Pause_Key_GPIO_Port, Pause_Key_Pin);
        KeyPressed |= Pause_Key_Pin;
    }
    if (!(HAL_GPIO_ReadPin(Play_Key_GPIO_Port, Play_Key_Pin)))
    {
        HAL_GPIO_ReadPin(Play_Key_GPIO_Port, Play_Key_Pin);
        KeyPressed |= Play_Key_Pin;
    }
    if (!(HAL_GPIO_ReadPin(Record_Key_GPIO_Port, Record_Key_Pin)))
    {
        HAL_GPIO_ReadPin(Record_Key_GPIO_Port, Record_Key_Pin);
        KeyPressed |= Record_Key_Pin;
    }
    if (!(HAL_GPIO_ReadPin(Next_Key_GPIO_Port, Next_Key_Pin)))
    {
        HAL_GPIO_ReadPin(Next_Key_GPIO_Port, Next_Key_Pin);
        KeyPressed |= Next_Key_Pin;
    }
    if (KeyPressed != 0)
    {
        return KeyPressed;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief Handles the pause key functionality.
 * This function updates the recorder state when the pause key is pressed, stopping playback or recording as needed.
 * @return void
 */
void PauseKey(void)
{
    if (VoiceRecorderSt.State == PlayState)
    {
        VoiceRecorderSt.State = StopPlayingState;
    }
    if (VoiceRecorderSt.State == RecordState)
    {
        VoiceRecorderSt.State = StopRecordingState;
    }
}

/**
 * @brief Main keyboard handler.
 * This function reads key presses, manages debounce, and updates the recorder state based on the pressed key.
 * @return void
 */
void KeyBoard(void)
{
    uint16_t KeysPressed;
    static uint32_t DebounceTime;
    KeysPressed = ReadKeyBoard();
    if (KeysPressed != 0)
    {
        if (DebounceTime >= (6 * DEBOUNCE_DELAY))
        {
            if (KeysPressed == Pause_Key_Pin)
            {
                PauseKey();
                DebounceTime = 0;
            }
            else if (KeysPressed == Play_Key_Pin)
            {
                VoiceRecorderSt.State = PlayState;
                DebounceTime = 0;
            }
            else if (KeysPressed == Record_Key_Pin)
            {
                VoiceRecorderSt.State == RecordState;
                DebounceTime = 0;
            }
            else if (KeysPressed == Next_Key_Pin)
            {
                VoiceRecorderSt.State = NextTrackState;
            }
            else if (KeysPressed == Pause_Key_Pin | Next_Key_Pin)
            {
                if (ChangeMode(DebounceTime))
                    DebounceTime = 0;
            }
        }
    }
    DebounceTime += DEBOUNCE_DELAY;
}
