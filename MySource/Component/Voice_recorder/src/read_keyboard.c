#include "read_keyboard.h"



#define DEBOUNCE_DELAY 50 // Debounce delay in milliseconds
#define PRESS_DELAY 5000  // Press delay in milliseconds (5 seconds)

void HandleKey1Press(uint32_t *timer)
{
    if (ReadKeyBoard() == Pause_Key_Pin)
    {
        *timer += DEBOUNCE_DELAY;
        if (*timer >= PRESS_DELAY)
        {
            VoiceRecorderSt.State = FlashEraseState;
            *timer = 0; // Reset the timer to prevent immediate toggle
        }
        else
        {
            PauseKey();
        }
    }
}
void KeyBoard()
{
    uint16_t KeysPressed;
    ReadKeyBoard();
    if (DebounceTime < HAL_GetTick())
    {
        if (KeysPressed == Pause_Key_Pin)
        {
            PauseKey();
        }
        if (KeysPressed == Play_Key_Pin)
        {
            VoiceRecorderSt.State = PlayState;
        }
        if (KeysPressed == Record_Key_Pin)
        {
            VoiceRecorderSt.State == RecordState;
        }
        if (KeysPressed == Next_Key_Pin)
        {
           VoiceRecorderSt.State =NextTrackState;
        }
    }
}

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

void PauseKey()
{
    if (VoiceRecorderSt.State == PlayState)
    {
        VoiceRecorderSt.State = StopPlayingState;
    }
    if (VoiceRecorderSt.State == RecordState)
    {
        VoiceRecorderSt.State = StopRecording;
    }
}
