
#ifndef _WRITE_TO_FLASH_H
#define _WRITE_TO_FLASH_H

uint8_t save_2k_array(uint8_t count_of_voice, uint8_t count_of_arras, uint16_t *array);
uint8_t RestoreArrayFromFlash(uint8_t count_of_voice, uint8_t count_of_arras, uint16_t *array);
uint8_t RemoveVoice(uint8_t *which_voic_is_rec, uint8_t voice_numb);
void RestoreDetail(uint8_t *which_voic_is_rec, uint8_t which_voic_is_rec_count, uint8_t *count_of_array);
void SaveDetail(uint8_t *which_voic_is_rec, uint8_t which_voic_is_rec_count, uint8_t count_of_array);
uint8_t FindFreeSpceInflash(uint8_t *who_is_present);
uint8_t HowManyVoiceIsRecord(uint8_t
                                 WitchVoiceWannaToPlay_,
                             uint8_t *present_voc_arry);
uint8_t NextFreeSpaceInFlash(uint8_t *who_is_present);
void TestingFlash();
#endif
