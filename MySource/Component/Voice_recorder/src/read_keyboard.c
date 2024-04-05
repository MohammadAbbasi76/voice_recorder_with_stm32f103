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
    VoiceRecorderSt.Track++;
    if (VoiceRecorderSt.Track > (MaxNumberOfVoice - 1))
    {
        VoiceRecorderSt.Track = 1;
    }
    SevenSegmentDisplay(VoiceRecorderSt.Track);
}
void ReadKeyBoard(uint16_t GPIO_Pin)
{
    static uint32_t Time;
    if (HAL_GetTick() - Time > DebounceTime)
    {
        if (GPIO_Pin == Play_Key_Pin) // If The INT Source Is EXTI Line8 (B12 Pin)
        {
            VoiceRecorderSt.State = PlayState;
            DummyFunc();
        }
        else if (GPIO_Pin == Record_Key_Pin) // If The INT Source Is EXTI Line8 (B13 Pin)
        {
            VoiceRecorderSt.State = RecordState;
            DummyFunc();
        }
        else if (GPIO_Pin == Pause_Key_Pin) // If The INT Source Is EXTI Line8 (B14 Pin)
        {
            if (VoiceRecorderSt.State == PlayState)
            {
                StopPlaying();
            }
            if (VoiceRecorderSt.State == RecordState)
            {
                StopRecording();
            }
            DummyFunc();
        }
        else if (GPIO_Pin == Next_Key_Pin) // If The INT Source Is EXTI Line8 (B14 Pin)
        {
            NextTrack();
        }
        Time = HAL_GetTick();
    }
}
