#include "main.h"
#include "w25qxx.h"
#define Sectors (MaxNumberOfVoice * SampleRate) / VoiceArraySize
#define detail_sector ((Sectors + 1) * MaxNumberOfVoice) // some data include the number of voice and .... save in this sector
/**   every page
     in w25q have 256Byte size and every sector in w25q
     have 4KByte size ,
     and we have 128*16*16 page's and addressing start from 0 to 128*16*16
     and we have 128*16  sector's and addressing start from 0 to 128*16
     and we have 128  block's     and addressing start from 0 to 128
     when we wanna getting address we have to careful about
     addressing
     **********************************************************
     we have 10 voice and every voice have 240k 16bit data
     that means we have  480k 8bit data and
     we need 120 sector for recording complete voice


 * if have correct input return 1
 * if dose not return 0
 */

uint8_t W25q_SaveBufferArray(uint8_t count_of_voice, uint8_t count_of_arras, uint16_t *array)
{

    if (count_of_voice > MaxNumberOfVoice || count_of_arras > Sectors)
    {
        return 0;
    }
    uint32_t sector_adrss = 0;
    sector_adrss = ((uint32_t)count_of_voice) * ((uint32_t)Sectors) + ((uint32_t)count_of_arras);
    uint8_t _8bit_array[4000] = {0};
    for (int i = 0; i < 2000; i++)
    {
        _8bit_array[2 * i] = (uint8_t)((array[i] & 0b1111111100000000) >> 8);
        _8bit_array[((2 * i) + 1)] = (uint8_t)(array[i] & 0b0000000011111111);
    }
    W25qxx_WriteSector(_8bit_array, sector_adrss, 0, (uint32_t)sizeof(_8bit_array));
    return 1;
}
/**
 * this function restore 2k 16bite array
 * if have correct input return 1
 * if dose not return 0
 */
uint8_t W25q_RestoreArrayFromFlash(uint8_t count_of_voice, uint8_t count_of_arras, uint16_t *array)
{
    if (count_of_voice > MaxNumberOfVoice || count_of_arras > Sectors)
    {
        return 0;
    }
    uint32_t sector_adrss = 0;
    sector_adrss = ((uint32_t)count_of_voice) * ((uint32_t)Sectors) + ((uint32_t)count_of_arras);
    uint8_t _8bit_array[4000] = {0};
    W25qxx_ReadSector(_8bit_array, sector_adrss, 0, 4000);
    for (int i = 0; i < 2000; i++)
    {
        array[i] = (uint16_t)(_8bit_array[2 * i] << 8) + _8bit_array[(2 * i) + 1];
    }

    return 1;
}

/**
 *
 * this function , remove sector dedicated to any voice
 * that means erase 120 sectors that dedicated to any voice
 *
 * if have correct input return 1
 * if dose not return 0
 */
uint8_t W25q_RemoveVoice(uint8_t *which_voic_is_rec, uint8_t voice_numb)
{
    if (voice_numb > MaxNumberOfVoice)
    {
        return 0;
    }
    uint32_t sector_adrss = 0;
    sector_adrss = ((uint32_t)voice_numb) * ((uint32_t)Sectors);
    for (int i = 0; i < Sectors; i++)
    {
        W25qxx_EraseSector(sector_adrss + i);
    }
    which_voic_is_rec[voice_numb] = 0;
    //    uint32_t pag_addrs_numb_voic = 0;
    //    pag_addrs_numb_voic = (uint32_t)((detail_sector) * 16);
    uint8_t temp[4000] = {0};
    for (uint8_t i = 0; i < MaxNumberOfVoice; i++)
        temp[i] = which_voic_is_rec[i];
    temp[MaxNumberOfVoice + voice_numb] = 0;
    // W25qxx_WritePage(temp, pag_addrs_numb_voic, 0, 256);
    W25qxx_EraseSector(detail_sector);
    W25qxx_WriteSector(temp, detail_sector, 0, (uint32_t)sizeof(temp));
    return 1;
}

/**
 *  we save some data in page's in detail sector
 *  some data such as count of voice
 *  array number and ....
 *  */

void W25q_SaveDetail(uint8_t *which_voic_is_rec, uint8_t voice_numb, uint8_t count_of_array)
{
    //    uint32_t pag_addrs_numb_voic = 0;
    //    pag_addrs_numb_voic = (uint32_t)((detail_sector) );
    uint8_t temp[4000] = {0};
    W25qxx_ReadSector(temp, detail_sector, 0, 4000);
    for (uint8_t i = 0; i < MaxNumberOfVoice; i++)
        temp[i] = which_voic_is_rec[i];
    temp[MaxNumberOfVoice + voice_numb] = count_of_array;
    W25qxx_EraseSector(detail_sector);
    W25qxx_WriteSector(temp, detail_sector, 0, (uint32_t)sizeof(temp));
    // W25qxx_WritePage(temp, pag_addrs_numb_voic, 0, 256);
}
/**
 *  we restore some data in page's in detail sector
 *  some data such as count of voice
 *  array number and ....
 *  */

void W25q_RestoreDetail(uint8_t *which_voic_is_rec, uint8_t voice_numb, uint8_t *count_of_array)
{
    //    uint32_t pag_addrs_numb_voic = 0;
    //    pag_addrs_numb_voic =(uint32_t)((detail_sector) * 16);
    uint8_t temp[4000] = {0};
    // W25qxx_ReadPage(temp, pag_addrs_numb_voic, 0, 256);
    W25qxx_ReadSector(temp, detail_sector, 0, 4000);
    for (uint8_t i = 0; i < MaxNumberOfVoice; i++)
        which_voic_is_rec[i] = temp[i];
    *count_of_array = temp[MaxNumberOfVoice + voice_numb];
}
/**
 * find free space for saving or restoring
 * this finding implement pre number of voice
 *
 */
uint8_t W25q_FindFreeSpace(uint8_t *who_is_present)
{
    uint8_t k = 0;
    for (int i = 0; i < MaxNumberOfVoice; i++)
    {
        if (who_is_present[i] == 1)
        {
            k++;
        }
    }
    return k;
}
uint8_t W25q_NextFreeSpace(uint8_t *who_is_present)
{
    for (int i = 0; i < MaxNumberOfVoice; i++)
    {
        if (who_is_present[i] == 0)
        {
            return i;
        }
    }
    return 0;
}
uint8_t W25q_RecordedVoice(uint8_t WitchVoiceWannaToPlay_, uint8_t *present_voc_arry)
{
    uint8_t k = 0;
    for (uint8_t i = 0; i < MaxNumberOfVoice; i++)
    {
        if (present_voc_arry[i] == 1)
        {
            k++;
        }
        if (k == WitchVoiceWannaToPlay_)
        {
            return i;
        }
    }
    return (uint8_t)0;
}
void W25q_TestingFlash()
{
    if (!W25qxx_Init())
    {
        while (1)
        {
            // flash have problem !
        }
    }
}
