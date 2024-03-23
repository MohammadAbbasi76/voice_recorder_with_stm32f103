#ifndef __TYPEDEF_H1_
#define __TYPEDEF_H1_

#define AdcArraySize 2000
#define StopTimeInSec 10
#define MaxNumberOfVoice 10
#define SampleRate 25000
#define UartArraySize 3
#define UartTimeDataExpire 4 // in millis
#define M66DelayTime 3000    // in millis
#define PlayLoopTime 3000    // in millis
#define Print
#define DebounceTime 400 // in millis
uint16_t Buffer1[AdcArraySize];
uint16_t Buffer2[AdcArraySize];
uint8_t InputKey;
uint8_t WitchVoiceWantToPlay;
uint8_t VoiceArrayReadFromFlash;
uint8_t state;
uint32_t ccr_pwm_val;
uint8_t FirstTimeFLG;
uint8_t UartReceive[UartArraySize];
uint32_t UartFLush_T1;
uint8_t VoiceForPlayLoop;
uint8_t FLGForVoiceStop;
uint32_t LastTimeKeyPress;
uint8_t NumberOfVoiceForLoopPlay;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
typedef enum
{
    record_key,
    play_key,
    pause_key,
    next_,
    back,
    non_
} keys;

typedef enum
{
    RecordState,
    PlayState,
    ChooseVoiceForPlay,
    PauseState,
    ReadKeyboardState,
    UartDataReceive,
    M66State,
    FlashEraseState,
    PLayLoop
} state_;

typedef enum
{
    M66_mic_in,
    Speaker
} AudioOutput;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

struct adc_
{
    uint32_t counter;
    uint32_t TotallyStopTim;
    uint32_t StopTimeCounter;
} adc_stru;
struct pwm_
{
    uint32_t counter;
    uint32_t CountDataFromTotally[MaxNumberOfVoice];

} pwm_stru;
struct voice_struc
{
    uint8_t ArrayGoToSave;
    uint8_t number;
    uint8_t WitchVoiceIsRecord[MaxNumberOfVoice];
} voice;
struct flags
{
    uint8_t AdcArryFull;
    uint8_t InterruptSwitch;
    uint8_t PwmArrayEmpty;
} flag;

#endif
