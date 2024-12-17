/**
 * @file flashControls.h
 * @author 
 * @brief 
 * @version 0.1
 * @date 
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef FLASHCONTROLS_H_
#define FLASHCONTROLS_H_

/* Standard C includes -------------------------------------------------------*/
#include <stdint.h>

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "bmsConfiguration.h"
#include "memoryMap.h"

/* Private define ------------------------------------------------------------*/

#define FLASH_LOCK()        HAL_FLASH_Lock()
#define FLASH_UNLOCK()      HAL_FLASH_Unlock()

//common data image for bootloader
#define FLASH_COMMON_DATA                       0x8072a60       //stores the image structure for bootloader

#define IMAGE_PAGE_NUMBER                       357
#define IMAGE_DATA_PAGES                        3               //first page for storing image data
#define IMAGE_CONFIG_BANK                       FLASH_BANK_2

//Primary and Secondary App flash parameters
#define PRIMARY_APP                             0x800C800       //stores the starting address of APP 1
#define SECONDARY_APP                           0x8040000       //stores the starting address of APP 2 
#define PRIMARY_APP_BANK                        FLASH_BANK_1    //primary app bank
#define SECONDARY_APP_BANK                      FLASH_BANK_2    //secondary app bank
#define SECONDARY_APP_PAGE_NUMBER               256
#define PRIMARY_APP_PAGES                       100             //number of pages for primary app
#define SECONDARY_APP_PAGES                     100             //number of pages for secondary app

//flash parameters for storing configuration parameters
#define FLASH_CONFIG_ADDRESS                    0x0807F000      
#define FLASH_CONFIG_PAGE_NUMBER                382             
#define FLASH_CONFIG_NUMBER_OF_PAGES            2               
#define FLASH_CONFIG_BANK                       FLASH_BANK_2

//flash parameters for storing exception data
#define FLASH_EXCEPTION_ADDRESS                 0x0807E800
#define FLASH_EXCEPTION_PAGE_NUMBER             381           
#define FLASH_EXCEPTION_NUMBER_OF_PAGES         1
#define FLASH_EXCEPTION_BANK                    FLASH_BANK_2

#define SOC_DATA_ADDRESS                        0x0807E000
#define SOC_DATA_PAGE_NUMBER                    380          
#define SOC_DATA_PAGE_NUMBER_OF_PAGES           1
#define SOC_DATA_PAGE_BANK                      FLASH_BANK_2

#define MARVEL_SELF_DATA_ADDRESS                0x0807D800
#define MARVEL_SELF_DATA_PAGE_NUMBER            379          //fourth last page in bank 2
#define MARVEL_SELF_DATA_PAGE_NUMBER_OF_PAGES   1
#define MARVEL_SELF_DATA_PAGE_BANK              FLASH_BANK_2

#define SOP_DATA_ADDRESS                        0x807C000
#define SOP_DATA_PAGE_NUMBER                    376          
#define SOP_DATA_PAGE_NUMBER_OF_PAGES           3
#define SOP_DATA_PAGE_BANK                      FLASH_BANK_2

#define IS_FLASH_PAGE_NUMBER(PAGE_NUMBER)       (((PAGE_NUMBER) == FLASH_CONFIG_PAGE_NUMBER) || ((PAGE_NUMBER) == FLASH_EXCEPTION_PAGE_NUMBER))
#define KPAGE_PER_BANK (uint32_t)256
#define NULL_VALUE                              0


/* Function prototypes -------------------------------------------------------*/

bmsStatus_t flashWrite(uint32_t startAddress, __IO uint64_t *data, uint16_t dataSize);
bmsStatus_t flashRead(uint64_t startAddress, __IO uint64_t *data, uint16_t dataSize);
bmsStatus_t flashErasePage(uint32_t pageNumber, uint32_t numberOfPages, uint8_t bank);
bmsStatus_t flashErase(uint32_t startAddress);
bmsStatus_t flashWriteOTA(uint32_t startAddress, __IO uint64_t *data, uint16_t dataSize);
uint32_t GetPage(uint32_t address);

#endif

/* End of File ---------------------------------------------------------------*/
