#include "read_keyboard.h"

void DummyFunc()
{
    int Temp = 0;
    for (int i = 0; i < 10000; i++)
    {
        Temp++;
    }
}
void NextTrack()
{
    WitchVoiceWantToPlay++;
    // if (WitchVoiceWannaToPlay > (FindFreeSpceInflash(voice.RecodedArray) + 1))
    if (WitchVoiceWantToPlay > (MaxNumberOfVoice - 1))
    {
        WitchVoiceWantToPlay = 1;
    }
    SevenSegmentDisplay(WitchVoiceWantToPlay);
}
void ReadKeyBoard(uint16_t GPIO_Pin)
{
    static uint32_t Time;
    if (HAL_GetTick() - Time > DebounceTime)
    {
        //  play_Pin | record_Pin | pause_Pin
        if (GPIO_Pin == Play_Key_Pin) // If The INT Source Is EXTI Line8 (B12 Pin)
        {
            state = PlayState;
            DummyFunc();
        }
        else if (GPIO_Pin == Record_Key_Pin) // If The INT Source Is EXTI Line8 (B13 Pin)
        {
            state = RecordState;
            DummyFunc();
        }
        else if (GPIO_Pin == Pause_Key_Pin) // If The INT Source Is EXTI Line8 (B14 Pin)
        {
            if (state == PlayState)
            {
                StopPlaying();
            }
            if (state == RecordState)
            {
                StopRecording();
            }
        }
        else if (GPIO_Pin == Next_Key_Pin) // If The INT Source Is EXTI Line8 (B14 Pin)
        {
            NextTrack();
        }
        Time = HAL_GetTick();
    }
}
