
#ifndef _WRITE_TO_FLASH_H
#define _WRITE_TO_FLASH_H

void W25q_RestoreDetail(uint8_t *which_voic_is_rec, uint8_t which_voic_is_rec_count, uint8_t *count_of_array);
void W25q_SaveDetail(uint8_t *which_voic_is_rec, uint8_t which_voic_is_rec_count, uint8_t count_of_array);
uint8_t W25q_SaveBufferArray(uint8_t count_of_voice, uint8_t count_of_arras, uint16_t *array);
uint8_t W25q_RestoreArrayFromFlash(uint8_t count_of_voice, uint8_t count_of_arras, uint16_t *array);
uint8_t W25q_RemoveVoice(uint8_t *which_voic_is_rec, uint8_t voice_numb);
uint8_t W25q_FindFreeSpace(uint8_t *who_is_present);
uint8_t W25q_RecordedVoice(uint8_t WitchVoiceWannaToPlay_, uint8_t *present_voc_arry);
uint8_t W25q_NextFreeSpace(uint8_t *who_is_present);
void W25q_TestingFlash();
#endif
