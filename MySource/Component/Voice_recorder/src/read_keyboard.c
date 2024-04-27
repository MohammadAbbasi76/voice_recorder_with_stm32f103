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
void ReadKeyBoard_(uint16_t GPIO_Pin)
{
    static uint32_t Time;
    Time = HAL_GetTick();
    if (VoiceRecorderSt.DeviceMode == NormalMode)
    {
        if (HAL_GetTick() - Time > DebounceTime)
        {
            if (GPIO_Pin == Play_Key_Pin) // If The INT Source Is EXTI Line8 (B12 Pin)
            {
                VoiceRecorderSt.State = PlayState;
            }
            else if (GPIO_Pin == Record_Key_Pin) // If The INT Source Is EXTI Line8 (B13 Pin)
            {
                VoiceRecorderSt.State = RecordState;
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
            }
            else if (GPIO_Pin == Next_Key_Pin) // If The INT Source Is EXTI Line8 (B14 Pin)
            {
                NextTrack();
            }
            SevenSegmentDisplay(VoiceRecorderSt.Track);
            Time = HAL_GetTick();
        }
    }
    else if (VoiceRecorderSt.DeviceMode == RestFactory)
    {
        // TO DO
    }
}
uint16_t ReadKeyBoard(void)
{
    if (!(HAL_GPIO_ReadPin(Pause_Key_GPIO_Port, Pause_Key_Pin)))
    {
        while (!(HAL_GPIO_ReadPin(Pause_Key_GPIO_Port, Pause_Key_Pin)))
            ; // wait till the button is pressed
        return Pause_Key_Pin;
    }
    if (!(HAL_GPIO_ReadPin(Play_Key_GPIO_Port, Play_Key_Pin)))
    {
        while (!(HAL_GPIO_ReadPin(Play_Key_GPIO_Port, Play_Key_Pin)))
            ; // wait till the button is pressed
        return Play_Key_Pin;
    }
    if (!(HAL_GPIO_ReadPin(Record_Key_GPIO_Port, Record_Key_Pin)))
    {
        while (!(HAL_GPIO_ReadPin(Record_Key_GPIO_Port, Record_Key_Pin)))
            ; // wait till the button is pressed
        return Record_Key_Pin;
    }
    if (!(HAL_GPIO_ReadPin(Next_Key_GPIO_Port, Next_Key_Pin)))
    {
        while (!(HAL_GPIO_ReadPin(Next_Key_GPIO_Port, Next_Key_Pin)))
            ; // wait till the button is pressed
        return Next_Key_Pin;
    }
}
