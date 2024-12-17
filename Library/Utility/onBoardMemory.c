/**
 * @file onBoardMemory.c
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "onBoardMemory.h"
#include "onBoardIOs.h"
#include "onBoardEEPROM.h"
#include "conversion.h"
#include "crc.h"
/* Self include --------------------------------------------------------------*/
#include "stm32l4xx_it.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

uint64_t ccWriteBufferFLASH[7]  = {0};
uint64_t ccReadBufferFLASH[6]   = {0};

extern bool SoCFromLookupTableFlag;

// should use array pointers here
uint8_t socDataWriteBuff[EEPROM_SECTION_WIDTH]       = {0};
uint8_t socDataReadBuff[EEPROM_SECTION_WIDTH]        = {0};

uint8_t criticalFlagsWriteBuff[EEPROM_SECTION_WIDTH] = {0};
uint8_t criticalFlagsReadBuff[EEPROM_SECTION_WIDTH]  = {0};

uint8_t commonDataWriteBuff[EEPROM_SECTION_WIDTH]    = {0};
uint8_t commonDataReadBuff[EEPROM_SECTION_WIDTH]     = {0};

// should need to find out why the stack overflow issue is happening

// uint8_t *commonDataWriteBuff;
// uint8_t *commonDataReadBuff;

uint8_t merlynDataWriteBuff[EEPROM_SECTION_WIDTH]    = {1, 2, 3, 4, 5};
uint8_t merlynDataReadBuff[EEPROM_SECTION_WIDTH]     = {0};

extern uint8_t resetDetectedEEPROM;

extern coulombCountingOutput_t ccOutputDataEEPROM;
extern coulombCountingOutput_t ccOutputDataFlash;
extern coulombCountingOutput_t ccOutputData;
extern ULONG actualFlag;
/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief read data from memory
 * @param memoryType memory Type
 * @param dataBuffer base address of data array that needs to be read from memory
 * @param dataLength Total number of bytes need to read from memory in bytes
 * @param eeprom memory section
 * @retval BMS status
 */
bmsStatus_t memoryRead(uint8_t memoryType, uint8_t *dataBuffer, uint16_t dataLength, eepromSection_t memorySection)
{
    bmsStatus_t status      = BMS_ERROR;
    uint32_t calculatedCRC  = 0;
    uint32_t storedCRC      = 0;
    uint32_t crcAddress     = 0;
    uint32_t memoryAddress  = 0;

	if( !(IS_MEMORY_TYPE(memoryType))       ||\
		(memoryAddress < 0)                 ||\
		(NULL == dataBuffer)                ||\
		(dataLength <= 0))
	{
		status = BMS_MEMORY_FUNCTION_PARAMETER_ERROR;
		return status;
	}
    
    switch(memorySection)
    {
        case SOC_SECTION:
            
            memoryAddress           = CALCULATE_MEMORY_ADDRESS(memorySection);
            crcAddress              = CALCULATE_CRC_ADDRESS(memorySection);
            break;

        case CRITICAL_FLAGS:
            
            memoryAddress           = CALCULATE_MEMORY_ADDRESS(memorySection);
            crcAddress              = CALCULATE_CRC_ADDRESS(memorySection);
            break;

        case COMMON_DATA:

            memoryAddress           = CALCULATE_MEMORY_ADDRESS(memorySection);
            crcAddress              = CALCULATE_CRC_ADDRESS(memorySection);
            break;

        case MERLYN_HANDLER_SECTION:

            memoryAddress           = CALCULATE_MEMORY_ADDRESS(memorySection);
            crcAddress              = CALCULATE_CRC_ADDRESS(memorySection);
            break;
    }

    switch(memoryType)
    {
        case eeprom:
            //read data from eeprom
            status = eepromRead(EEPROM_DEVICE_ADDRESS, memoryAddress, dataBuffer, dataLength - EEPROM_CRC_SIZE);
            CHECK_STATUS(status);
            //read the stored crc from eeprom
            status = eepromRead(EEPROM_DEVICE_ADDRESS, crcAddress, (uint8_t*)&storedCRC, EEPROM_CRC_SIZE);
            CHECK_STATUS(status);
            //calculate CRC
            calculatedCRC = calculateCRC(dataBuffer, dataLength - EEPROM_CRC_SIZE);
            //compare the crc
            status = (storedCRC == calculatedCRC ? BMS_OK : BMS_EEPROM_READ_FAILED);
            break;
        default:
            status = BMS_ERROR;
            break;
    }
    
    return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief write data to memory
 * @param memoryType memory Type
 * @param dataBuffer base address of data array that needs to be write from memory
 * @param dataLength Total number of bytes need to write from memory in bytes
 * @param eeprom memory section
 * @retval BMS status
 */
bmsStatus_t memoryWrite(uint8_t memoryType, uint8_t *dataBuffer, uint16_t dataLength, eepromSection_t memorySection) 
{
    bmsStatus_t statusMemoryWrite      = BMS_ERROR;

    uint32_t calculatedCRC  = 0;
    uint32_t storedDataCRC  = 0;
    uint32_t crcAddress     = 0;
    uint8_t readData[100]   = {0};
    uint32_t memoryAddress  = 0; 

	if( !(IS_MEMORY_TYPE(memoryType))       ||\
		(memoryAddress < 0)                 ||\
		(NULL == dataBuffer)                ||\
		(dataLength <= 0))
	{
		statusMemoryWrite = BMS_MEMORY_FUNCTION_PARAMETER_ERROR;
		return statusMemoryWrite;
	}

    switch(memorySection)
    {
        case SOC_SECTION:
            
            memoryAddress           = CALCULATE_MEMORY_ADDRESS(memorySection);
            calculatedCRC           = calculateCRC(dataBuffer, dataLength - EEPROM_CRC_SIZE);
            crcAddress              = CALCULATE_CRC_ADDRESS(memorySection);
            break;

        case CRITICAL_FLAGS:
            
            memoryAddress           = CALCULATE_MEMORY_ADDRESS(memorySection);
            calculatedCRC           = calculateCRC(dataBuffer, dataLength - EEPROM_CRC_SIZE);
            crcAddress              = CALCULATE_CRC_ADDRESS(memorySection);
            break;

        case COMMON_DATA:

            memoryAddress           = CALCULATE_MEMORY_ADDRESS(memorySection);
            calculatedCRC           = calculateCRC(dataBuffer, dataLength - EEPROM_CRC_SIZE);
            crcAddress              = CALCULATE_CRC_ADDRESS(memorySection);
            break;

        case MERLYN_HANDLER_SECTION:

            memoryAddress           = CALCULATE_MEMORY_ADDRESS(memorySection);
            calculatedCRC           = calculateCRC(dataBuffer, dataLength - EEPROM_CRC_SIZE);
            crcAddress              = CALCULATE_CRC_ADDRESS(memorySection);
            break;
    }

    switch(memoryType)
    {
        case eeprom:
            //write data to eeprom
            statusMemoryWrite = eepromWrite(EEPROM_DEVICE_ADDRESS, memoryAddress, dataBuffer, dataLength - EEPROM_CRC_SIZE);
            CHECK_STATUS(statusMemoryWrite);
            //write crc to eeprom
            statusMemoryWrite = eepromWrite(EEPROM_DEVICE_ADDRESS, crcAddress, (uint8_t*)&calculatedCRC, EEPROM_CRC_SIZE);
            CHECK_STATUS(statusMemoryWrite);
            //read back the data
            statusMemoryWrite = eepromRead(EEPROM_DEVICE_ADDRESS, memoryAddress, readData, dataLength - EEPROM_CRC_SIZE);
            CHECK_STATUS(statusMemoryWrite);
            //calculate CRC
            storedDataCRC = calculateCRC(readData, dataLength - EEPROM_CRC_SIZE);
            //compare the crc
            statusMemoryWrite = (calculatedCRC == storedDataCRC ? BMS_OK : BMS_EEPROM_WRITE_FAILED);
            break;
        default:
            statusMemoryWrite = BMS_ERROR;
            break;
    }
    
    return statusMemoryWrite;
}

bmsStatus_t writeEEPROMDataToBuffer(coulombCountingOutput_t *ccOutput, uint8_t *writeBufferEEPROM, uint64_t *writeBufferFLASH, eepromSection_t memorySection)
{
    bmsStatus_t status = BMS_ERROR;
    
	if( (NULL == ccOutput)  
      )
	{
		status = BMS_COULOMB_COUNTING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    switch(memorySection)
    {
        case SOC_SECTION:

            for(int i = 0; i < sizeof(int32_T); i++) //i < 4, number of bytes - all data types are of 4 bytes
            {
                //if more elements are added then increase the size of ccWriteBuffer and socDataWriteBuff array.
                writeBufferEEPROM[i]            = (int) ccOutput->initialCapacity           >> (i*8);
                writeBufferEEPROM[i+4]          = (int) ccOutput->totalCapacityRemaining    >> (i*8);
                writeBufferEEPROM[i+8]          = (int) ccOutput->totalCapacityDischarge    >> (i*8);
                writeBufferEEPROM[i+12]         = (int) ccOutput->TotalCapacityExchange_mAh >> (i*8);
                writeBufferEEPROM[i+16]         = (int) ccOutput->SOC_pct                   >> (i*8);
            }
            



                #ifdef SOC_IN_FLASH

                writeBufferFLASH[0] = ccOutput->initialCapacity;
                writeBufferFLASH[1] = ccOutput->totalCapacityRemaining;
                writeBufferFLASH[2] = ccOutput->totalCapacityDischarge; 
                writeBufferFLASH[3] = ccOutput->TotalCapacityExchange_mAh;
                writeBufferFLASH[4] = ccOutput->SOC_pct;
                
                #endif

            break;

        case CRITICAL_FLAGS:

                // add critical flags here
                writeBufferEEPROM[0]         = (uint8_t) fullChargeLatch;
                writeBufferEEPROM[1]         = (uint8_t) pseudoLatchFlag;
                // writeBufferEEPROM[2]         = (uint8_t) externalShortCircuitDetectionFlag;
                
            break;

        case COMMON_DATA:

                // add the miscellaneous data here
                writeBufferEEPROM[9]        = (uint8_t) resetDetectedEEPROM;
                writeBufferEEPROM[10]       = (uint8_t) EEPROM_MAGIC;

            break;

        case MERLYN_HANDLER_SECTION:

                // add merlyn test data here
            break; 
    }

	return status;
}

bmsStatus_t readEEPROMDataFromBuffer(coulombCountingOutput_t *ccOutput, uint8_t *readBufferEEPROM, uint64_t *readBufferFLASH, eepromSection_t memorySection)
{
    bmsStatus_t status = BMS_ERROR;
    
	if( (NULL == ccOutput)  
      )
	{
		status = BMS_COULOMB_COUNTING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    switch(memorySection)
    {
        case SOC_SECTION:

            for(int i = 0; i < 4; i++)
            {
                ccOutput->initialCapacity           |= readBufferEEPROM[i]    << (i*8);
                ccOutput->totalCapacityRemaining    |= readBufferEEPROM[i+4]  << (i*8);
                ccOutput->totalCapacityDischarge    |= readBufferEEPROM[i+8]  << (i*8);
                ccOutput->TotalCapacityExchange_mAh |= readBufferEEPROM[i+12] << (i*8);
                ccOutput->SOC_pct                   |= readBufferEEPROM[i+16] << (i*8);
            }

                #ifdef SOC_IN_FLASH
                
                    status = flashRead(SOC_DATA_ADDRESS, readBufferFLASH, 5);

                    if(0xFFFFFFFFFFFFFFFF != readBufferFLASH[0]) //When the device turns on for the very first time, FLASH will be empty
                    // if(ccReadBuffer[3] > 0) //When the device turns on for the very first time, FLASH will be empty
                    {
                        ccOutputDataFlash.initialCapacity           = readBufferFLASH[0];
                        ccOutputDataFlash.totalCapacityRemaining    = readBufferFLASH[1];
                        ccOutputDataFlash.totalCapacityDischarge    = readBufferFLASH[2];
                        ccOutputDataFlash.TotalCapacityExchange_mAh = readBufferFLASH[3];
                        ccOutputDataFlash.SOC_pct                   = readBufferFLASH[4];
                    }

                #endif

                #ifdef DEBUG_CHANGE_RELEASE_0A

                eepromMagic = readBufferEEPROM[22];            /* 0x00 to 0x100 addresses are reserved for SoC section in the new code and will never be written by the previous version.
                                                                  and if the new location space is being used, it will get overitten
                                                                  this line can be removed when 100% of live population is running on releases 0.A and higher! */

                #endif
                                                               
            break;

        case CRITICAL_FLAGS:

                // add critical flags here
                fullChargeLatch                     = readBufferEEPROM[0];
                pseudoLatchFlag                     = readBufferEEPROM[1];      // these variables are already getting read and being used as it is. So the implementation of this dosent matter
                // externalShortCircuitDetectionFlag   = readBufferEEPROM[2];
            break;

        case COMMON_DATA:

                // add the miscellaneous data here
                resetDetectedEEPROM        = readBufferEEPROM[9];
                eepromMagic                = readBufferEEPROM[10];

            break;

        case MERLYN_HANDLER_SECTION:

                // add merlyn test data here

            break; 
    }

    status = BMS_OK;
	return status;
}

bmsStatus_t handleEEPROMDataCollection(void)
{
    bmsStatus_t status = BMS_ERROR;
    uint8_t eepromCorrupted = 0;

    // Fetching data from EEPROM
    RETRY(RETRY_EEPROM, status, memoryRead(eeprom, commonDataReadBuff, sizeof(commonDataReadBuff), COMMON_DATA));
    status != 0 ? tx_event_flags_set(&systemEventFlag, EEPROM_READ_FAIL, TX_OR) : 0;
    readEEPROMDataFromBuffer(&ccOutputDataEEPROM, commonDataReadBuff, ccReadBufferFLASH, COMMON_DATA);

    if(!resetDetectedEEPROM)
    {
        RETRY(RETRY_EEPROM, status, memoryRead(eeprom, socDataReadBuff, sizeof(socDataReadBuff), SOC_SECTION));
        status != 0 ? tx_event_flags_set(&systemEventFlag, EEPROM_READ_FAIL, TX_OR) : 0;
        
        #ifdef DEBUG_CHANGE_RELEASE_0A
            readEEPROMDataFromBuffer(&ccOutputDataEEPROM, socDataReadBuff, ccReadBufferFLASH, SOC_SECTION);
            readEEPROMDataFromBuffer(&ccOutputDataEEPROM, commonDataReadBuff, ccReadBufferFLASH, COMMON_DATA);
        #endif

        RETRY(RETRY_EEPROM, status, memoryRead(eeprom, criticalFlagsReadBuff, sizeof(criticalFlagsReadBuff), CRITICAL_FLAGS));
        status != 0 ? tx_event_flags_set(&systemEventFlag, EEPROM_READ_FAIL, TX_OR) : 0;
    }
    else
    {
        // resetDetected flag to 0
        resetDetectedEEPROM = 0;
        writeEEPROMDataToBuffer(&ccOutputData, commonDataWriteBuff, ccWriteBufferFLASH, COMMON_DATA);
        RETRY(RETRY_EEPROM, status, memoryWrite(eeprom, commonDataWriteBuff, (sizeof(commonDataWriteBuff)), COMMON_DATA)); 
    }

    // add eeprom read failure error handling here
    status = flashRead(SOC_DATA_ADDRESS, ccReadBufferFLASH, 6);

    if(0xFFFFFFFFFFFFFFFF != ccReadBufferFLASH[0])  // When the device turns on for the very first time, FLASH will be empty
    {
        ccOutputDataEEPROM.initialCapacity           = ccReadBufferFLASH[0];
        ccOutputDataEEPROM.totalCapacityRemaining    = ccReadBufferFLASH[1];
        ccOutputDataEEPROM.totalCapacityDischarge    = ccReadBufferFLASH[2];
        ccOutputDataEEPROM.TotalCapacityExchange_mAh = ccReadBufferFLASH[3];
        ccOutputDataEEPROM.SOC_pct                   = ccReadBufferFLASH[4];
        eepromCorrupted                              = ccReadBufferFLASH[5];    
    }

    // if((eepromMagic != EEPROM_MAGIC) && (tx_event_flags_get(&systemEventFlag, EEPROM_READ_FAIL, TX_OR, &actualFlag, 0x10) != TX_SUCCESS))  // checking whether eeprom is empty
    if(eepromMagic != EEPROM_MAGIC)  // checking whether eeprom is empty
    {
        writeEEPROMDataToBuffer(&ccOutputData, socDataWriteBuff, ccWriteBufferFLASH, SOC_SECTION);
        RETRY(RETRY_EEPROM, status, memoryWrite(eeprom, socDataWriteBuff, (sizeof(socDataWriteBuff)), SOC_SECTION)); 

        // writing MAGIC for the first time
        writeEEPROMDataToBuffer(&ccOutputData, commonDataWriteBuff, ccWriteBufferFLASH, COMMON_DATA);
        RETRY(RETRY_EEPROM, status, memoryWrite(eeprom, commonDataWriteBuff, (sizeof(commonDataWriteBuff)), COMMON_DATA)); 
    }
    else if((!eepromCorrupted) && (tx_event_flags_get(&systemEventFlag, EEPROM_READ_FAIL, TX_OR, &actualFlag, 0x10) != TX_SUCCESS))        // normal EEPROM reading
    {
        readEEPROMDataFromBuffer(&ccOutputDataEEPROM, socDataReadBuff, ccReadBufferFLASH, SOC_SECTION);
        readEEPROMDataFromBuffer(&ccOutputDataEEPROM, criticalFlagsReadBuff, ccReadBufferFLASH, CRITICAL_FLAGS);
    } 
    else
    {
        // code flow will come here when data is not written to flash and EEPROM read was failed after retry
        // SoC should be chosen from SoC-OCV curve and the readBuff should be filled accordingly
        SoCFromLookupTableFlag = true;
    }

    return status;
}



/* End of File ---------------------------------------------------------------*/

