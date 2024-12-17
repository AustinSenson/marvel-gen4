/**
 * @file onBoardFlash.h
 * @author
 * @brief 
 * @version 0.1
 * @date
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef ONBOARDFLASH_H_
#define ONBOARDFLASH_H_

/* Standard C includes -------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

typedef enum
{
    CONFIG_VERSION = 0,         // store the major, minor and patch here in each bytes
    
    //CMU and cell
    NUMBER_OF_CMU_INDEX,
    CELL_IN_SERIES_INDEX,

    //shunt
    SHUNT_RESISTOR_uOhm_INDEX,

	//cell voltage
    CELL_MAX_VOLTAGE_THRESHOLD_mV_INDEX,
    CELL_MIN_VOLTAGE_THRESHOLD_mV_INDEX,
    CELL_BALANCING_START_VOLTAGE_mV_INDEX,
    CELL_IMBALANCE_THRESHOLD_mV_INDEX,

    //capacity
    PACK_MAX_CAPACITY_Ah_INDEX,
    PACK_USABLE_CAPACITY_mAh_INDEX,
    EEPROM_CAP_WRITE_mAh_INDEX,

    //current
    IR_WAIT_SOC_INDEX,
    IR_START_SOC_INDEX,
    IR_CYCLE_COUNT_THRESHOLD_INDEX,
    SLOW_CHARGING_MAX_CURRENT_A_INDEX,
    FAST_CHARGING_MAX_CURRENT_A_INDEX,
    CHARGE_CURRENT_DETECTION_THRESHOLD_mA_INDEX,
    SLOW_CHARGING_TARGET_mV_INDEX,
    FAST_CHARGING_TARGET_mV_INDEX,
    CV_TRANSITION_mV_INDEX,
    FAST_CHARGING_SCALING_FACTOR_INDEX,
    SLOW_CHARGING_CV_SCALING_FACTOR_INDEX,
    SLOW_CHARGING_CC_SCALING_FACTOR_INDEX,
    
    //protection thresholds
    OCC_ERROR_CURRENT_A_INDEX,
    OCC_WARNING_CURRENT_A_INDEX,
    OCD_ERROR_CURRENT_A_INDEX,
    OCD_WARNING_CURRENT_A_INDEX,

    //timeout
    ERROR_TIMEOUT_ms_INDEX,
    WARNING_TIMEOUT_ms_INDEX,
    RECOVERY_TIMEOUT_ms_INDEX,

    //balancing derating
    BALANCING_DERATING_START_TEMP_C_INDEX,
    BALANCING_DERATING_END_TEMP_C_INDEX,
    BALANCING_MAX_ON_TIME_ms_INDEX,
    BALANCING_MIN_ON_TIME_ms_INDEX,
    BALANCING_MAX_OFF_TIME_ms_INDEX,

    //protection threshold
    //temp group1
    OTC_ERROR_TEMPERATURE_GROUP_1_INDEX,
    OTC_WARNING_TEMPERATURE_GROUP_1_INDEX,
    OTC_RECOVERY_TEMPERATURE_GROUP_1_INDEX,

    OTD_ERROR_TEMPERATURE_GROUP_1_INDEX,
    OTD_WARNING_TEMPERATURE_GROUP_1_INDEX,
    OTD_RECOVERY_TEMPERATURE_GROUP_1_INDEX,

    UTC_ERROR_TEMPERATURE_GROUP_1_INDEX,
    UTC_WARNING_TEMPERATURE_GROUP_1_INDEX,
    UTC_RECOVERY_TEMPERATURE_GROUP_1_INDEX,

    UTD_ERROR_TEMPERATURE_GROUP_1_INDEX,
    UTD_WARNING_TEMPERATURE_GROUP_1_INDEX,
    UTD_RECOVERY_TEMPERATURE_GROUP_1_INDEX,

    //temp group2
    OTC_ERROR_TEMPERATURE_GROUP_2_INDEX,
    OTC_WARNING_TEMPERATURE_GROUP_2_INDEX,
    OTC_RECOVERY_TEMPERATURE_GROUP_2_INDEX,

    OTD_ERROR_TEMPERATURE_GROUP_2_INDEX,
    OTD_WARNING_TEMPERATURE_GROUP_2_INDEX,
    OTD_RECOVERY_TEMPERATURE_GROUP_2_INDEX,

    UTC_ERROR_TEMPERATURE_GROUP_2_INDEX,
    UTC_WARNING_TEMPERATURE_GROUP_2_INDEX,
    UTC_RECOVERY_TEMPERATURE_GROUP_2_INDEX,

    UTD_ERROR_TEMPERATURE_GROUP_2_INDEX,
    UTD_WARNING_TEMPERATURE_GROUP_2_INDEX,
    UTD_RECOVERY_TEMPERATURE_GROUP_2_INDEX,

    HIGH_IMBALANCE_ERROR_mV_INDEX,

    //contactor
    CONTACTOR_CUT_OFF_TIME_ms_INDEX,    
    PRECHARGE_RETRY_TIMEOUT_INDEX,
    PRECHARGE_RETRY_LIMIT_INDEX,
    PRECHARGE_TIMEOUT_INDEX,   

    //Config threshold
    MINIMUM_REQUIRED_CONFIG_INDEX,

    //crc
    FLASH_CONFIG_CRC_INDEX,

}flashConfigDataIndex_t;



typedef enum
{
    CONFIG_MAJOR = 0,
    CONFIG_MINOR,
    CONFIG_PATCH

}marvelConfigVersion_t;
typedef enum
{   
    HW_MAJOR = 0,
    HW_MINOR,
    HW_PATCH,
    FLASH_VERSION_CRC_INDEX,
}marvelVersion_t;

#define DATA_EMPTY  		                    0xFFFFFFFFFFFFFFFFUL
#define FLASH_CONFIG_LAST_DATA_INDEX            FLASH_CONFIG_CRC_INDEX

#define CRC_LENGTH                              1
#define FLASH_CONFIG_CRC_LENGTH                 CRC_LENGTH
#define FLASH_CONFIG_NUMBER_OF_PARAMETERS       FLASH_CONFIG_LAST_DATA_INDEX
#define FLASH_CONFIG_TOTAL_LENGTH               ((FLASH_CONFIG_NUMBER_OF_PARAMETERS) + (FLASH_CONFIG_CRC_LENGTH))
#define CRC_LENGTH_OTA                          4
#define FLASH_MARVEL_SELF_DATA_LENGTH           (FLASH_VERSION_CRC_INDEX + CRC_LENGTH)

#define FLASH_CONFIG_SECTION		            1
#define FLASH_COTA_UPDATE                       2
#define FLASH_EXCEPTION_SECTION 	            3
#define FLASH_SOC_SECTION                       4
#define FLASH_MARVEL_SELF_DATA_SECTION          5
#define FLASH_SOP_DATA_SECTION                  6

#define IS_FLASH_SECTION_TYPE(VALUE)            (((VALUE) == FLASH_CONFIG_SECTION) || ((VALUE) == FLASH_EXCEPTION_SECTION) || \
                                                 ((VALUE) == FLASH_COTA_UPDATE)    || ((VALUE) == FLASH_SOC_SECTION) ||\
                                                 ((VALUE) == FLASH_MARVEL_SELF_DATA_SECTION) ||  ((VALUE) == FLASH_SOP_DATA_SECTION))

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
extern __IO uint64_t defaultFlashConfigData[FLASH_CONFIG_TOTAL_LENGTH];
extern __IO uint64_t defaultVersionData[FLASH_MARVEL_SELF_DATA_LENGTH];

bmsStatus_t initializeFlashConfigData   (__IO uint64_t *data, uint8_t sectionType);
bmsStatus_t readDataFromFlash           (uint8_t sectionType, uint32_t flashAddress, __IO uint64_t *flashData, uint16_t dataSize);
bmsStatus_t writeDataToFlash            (uint8_t sectionType, uint32_t flashAddress, __IO uint64_t *flashData, uint16_t dataSize, uint8_t *receivedData);

bmsStatus_t checkFlashData              (uint8_t sectionType, uint32_t flashAddress, __IO uint64_t *flashData, uint16_t dataSize);
uint32_t    calculateFlashCRC           ( uint8_t sectionType, __IO uint64_t *data, uint16_t size);
// bmsStatus_t updateReceivedData          (uint8_t *receivedData, uint16_t *index, uint64_t *updatedData);
bmsStatus_t checkReceivedDataIsSame     (__IO uint64_t *flashData, uint16_t *index, uint64_t *updatedData);
bmsStatus_t updateStoredData            (__IO uint64_t *flashData, uint16_t *index, uint64_t *updatedData);
bmsStatus_t checkFirmwareConfigCompatibility(__IO uint64_t *flashData, uint8_t sectionType);

#endif
/* End of File ---------------------------------------------------------------*/