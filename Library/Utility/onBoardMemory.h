/**
 * @file onBoardMemory.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef ONBOARDMEMORY_H_
#define ONBOARDMEMORY_H_

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
#include "threadxMain.h"
#include "structures.h"
#include "flashControls.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

#define EEPROM_CRC_SIZE                     4
#define EEPROM_SECTION_WIDTH                50              // configurable
#define EEPROM_SECTION_PADDING              1

// address mapping of EEPROM sections
#define EEPROM_SOC_SECTION                  EEPROM_START_ADDRESS                             // 0x00
#define EEPROM_CRITICAL_FLAGS               EEPROM_SOC_SECTION    + EEPROM_SECTION_WIDTH     // 0x50  + 1
#define EEPROM_COMMON_DATA                  EEPROM_CRITICAL_FLAGS + EEPROM_SECTION_WIDTH     // 0x101 + 1
#define EEPROM_MERLYN_HANDLER               EEPROM_COMMON_DATA    + EEPROM_SECTION_WIDTH     // 0x152 + 1

/* add new sections if necessary here */

#define CALCULATE_CRC_ADDRESS(memorySection) \
    ({ \
        uintptr_t addr = 0; \
        switch (memorySection) { \
            case SOC_SECTION: \
                addr = (EEPROM_SOC_SECTION + EEPROM_SECTION_WIDTH) - EEPROM_CRC_SIZE; \
                break; \
            case CRITICAL_FLAGS: \
                addr = (EEPROM_CRITICAL_FLAGS + EEPROM_SECTION_WIDTH) - EEPROM_CRC_SIZE; \
                break; \
            case COMMON_DATA: \
                addr = (EEPROM_COMMON_DATA + EEPROM_SECTION_WIDTH) - EEPROM_CRC_SIZE; \
                break; \
            case MERLYN_HANDLER_SECTION: \
                addr = (EEPROM_MERLYN_HANDLER + EEPROM_SECTION_WIDTH) - EEPROM_CRC_SIZE; \
                break; \
            /* Add more cases for other sections if needed */ \
            default: \
                /* Handle invalid section */ \
                break; \
        } \
        addr; \
    })

#define CALCULATE_MEMORY_ADDRESS(memorySection) \
    ({ \
        uintptr_t addr = 0; \
        switch (memorySection) { \
            case SOC_SECTION: \
                addr = EEPROM_SOC_SECTION; \
                break; \
            case CRITICAL_FLAGS: \
                addr = EEPROM_CRITICAL_FLAGS; \
                break; \
            case COMMON_DATA: \
                addr = EEPROM_COMMON_DATA; \
                break; \
            case MERLYN_HANDLER_SECTION: \
                addr = EEPROM_MERLYN_HANDLER; \
                break; \
            /* Add more cases for other sections if needed */ \
            default: \
                /* Handle invalid section */ \
                break; \
        } \
        addr; \
    })


/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern uint8_t fullChargeLatch;
extern uint8_t latchCheckFlag;
extern uint8_t pseudoLatchFlag;
extern uint8_t eepromMagic;
extern bmsStatus_t statusSoC;

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

#define REMAINING_CAPACITY_ADDRESS  0

bmsStatus_t memoryRead(uint8_t memoryType, uint8_t *dataBuffer, uint16_t dataLength, eepromSection_t memorySection);
bmsStatus_t memoryWrite(uint8_t memoryType, uint8_t *dataBuffer, uint16_t dataLength, eepromSection_t memorySection); 
bmsStatus_t writeEEPROMDataToBuffer(coulombCountingOutput_t *ccOutput, uint8_t *writeBufferEEPROM, uint64_t *writeBufferFLASH, eepromSection_t memorySection);
bmsStatus_t readEEPROMDataFromBuffer(coulombCountingOutput_t *ccOutput, uint8_t *readBufferEEPROM, uint64_t *readBufferFLASH, eepromSection_t memorySection);
bmsStatus_t handleEEPROMDataCollection(void);


#endif
/* End of File ---------------------------------------------------------------*/

