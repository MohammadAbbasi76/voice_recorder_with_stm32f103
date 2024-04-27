#include "read_keyboard.h"

void NextTrack()
{
    VoiceRecorderSt.Track++;
    if (VoiceRecorderSt.Track > (MaxNumberOfVoice - 1))
    {
        VoiceRecorderSt.Track = 1;
    }
    SevenSegmentDisplay(VoiceRecorderSt.Track);
}
uint8_t first_time_holding_flag = 1;
uint16_t Start_time = 0;
uint16_t Hold_time = 0;

uint32_t DebounceTime;

uint16_t LastKeyPress;
uint16_t KeyPress;
void KeyBoard()
{
    KeyPress = ReadKeyBoard();
    if (KeyPress != 0 && LastKeyPress != 0)
    {
        if (first_time_holding_flag)
        {
            Start_time = HAL_GetTick();
            first_time_holding_flag = 0;
            Hold_time = 0;
        }
        else /*detect  double  key pressed and  change mode*/
        {
            Hold_time = HAL_GetTick() - Start_time;
            if (KeyPress == (Pause_Key_Pin | Record_Key_Pin) && Hold_time >= GO_TO_FACTORYRESET_MODE_TIME)
            {
                if (VoiceRecorderSt.DeviceMode == NormalMode) /*go to learning mode*/
                {
                    VoiceRecorderSt.DeviceMode = ResetFactory;
                    RestFactoryFunction();
                }
                else
                {
                    VoiceRecorderSt.DeviceMode = NormalMode;
                }
                first_time_holding_flag = 1;
            }
        }
    }
    if (KeyPress == 0 && LastKeyPress != 0 && DebounceTime < HAL_GetTick())
    {
        first_time_holding_flag = 1;
        if (VoiceRecorderSt.DeviceMode == NormalMode)
        {
            if (LastKeyPress == Play_Key_Pin)
            {
                VoiceRecorderSt.State = PlayState;
            }
            else if (LastKeyPress == Record_Key_Pin)
            {
                VoiceRecorderSt.State = RecordState;
            }
            else if (LastKeyPress == Next_Key_Pin)
            {
                NextTrack();
            }
            SevenSegmentDisplay(VoiceRecorderSt.Track);
            DebounceTime = HAL_GetTick() + DefaultDebounceTime;
        }
        LastKeyPress = 0;
    }
    else
    {
        LastKeyPress = KeyPress;
    }
}

uint16_t ReadKeyBoard(void)
{
    uint16_t KeyPressed = 0;
    if (!(HAL_GPIO_ReadPin(Pause_Key_GPIO_Port, Pause_Key_Pin)))
    {
        while (!(HAL_GPIO_ReadPin(Pause_Key_GPIO_Port, Pause_Key_Pin)))
            ; // wait till the button is pressed
        KeyPressed |= Pause_Key_Pin;
    }
    if (!(HAL_GPIO_ReadPin(Play_Key_GPIO_Port, Play_Key_Pin)))
    {
        while (!(HAL_GPIO_ReadPin(Play_Key_GPIO_Port, Play_Key_Pin)))
            ; // wait till the button is pressed
        KeyPressed |= Play_Key_Pin;
    }
    if (!(HAL_GPIO_ReadPin(Record_Key_GPIO_Port, Record_Key_Pin)))
    {
        while (!(HAL_GPIO_ReadPin(Record_Key_GPIO_Port, Record_Key_Pin)))
            ; // wait till the button is pressed
        KeyPressed |= Record_Key_Pin;
    }
    if (!(HAL_GPIO_ReadPin(Next_Key_GPIO_Port, Next_Key_Pin)))
    {
        while (!(HAL_GPIO_ReadPin(Next_Key_GPIO_Port, Next_Key_Pin)))
            ; // wait till the button is pressed
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
void PauseKey(uint16_t InputKey)
{
    if (DebounceTime < HAL_GetTick())
    {
        if (InputKey == Pause_Key_Pin)
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
        DebounceTime = HAL_GetTick() + DefaultDebounceTime;
    }
}
