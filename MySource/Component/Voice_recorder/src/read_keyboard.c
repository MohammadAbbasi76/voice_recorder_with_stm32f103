#include "read_keyboard.h"

uint16_t HandleKey1Press(uint32_t DebounceTime)
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
                if (HandleKey1Press(DebounceTime))
                    DebounceTime = 0;
            }
        }
    }
    DebounceTime += DEBOUNCE_DELAY;
}
