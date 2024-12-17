/**
 * @file onBoardFlash.c
 * @author 
 * @brief 
 * @version 0.1
 * @date 
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
/* Private includes ----------------------------------------------------------*/
#include "flashControls.h"
#include "bmsConfiguration.h"
/* Self include --------------------------------------------------------------*/
#include "onBoardFlash.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/
#define FLASH_CONFIG_DATA_RANGE_MIN_INDEX					0
#define FLASH_CONFIG_DATA_RANGE_MAX_INDEX					1

__IO uint64_t defaultFlashConfigData[FLASH_CONFIG_TOTAL_LENGTH] = {0};
__IO uint64_t defaultVersionData[FLASH_MARVEL_SELF_DATA_LENGTH] = {0};
uint8_t configWriteFlag = 0;
uint8_t marvelConfigVersion[3] = {0};
uint8_t marvelHardwareVersion[3] = {0};

// uint32_t flashConfigDataRange[FLASH_CONFIG_TOTAL_LENGTH][2] = {\
// 				            {NUMBER_OF_CMU_MIN_VALUE, 	   							NUMBER_OF_CMU_MAX_VALUE},\
// 			                {CELL_IN_SERIES_MIN_VALUE, 	   							CELL_IN_SERIES_MAX_VALUE},\
// 			                {MAX_CELL_PER_SLAVE_MIN_VALUE, 	  						MAX_CELL_PER_SLAVE_MAX_VALUE},\
// /*1 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
// /*2 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
// /*3 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
// /*4 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
// /*5 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
// /*6 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
// /*7 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
// /*8 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
// /*9 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
// /*10*/		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
// /*11*/		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
// /*12*/		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
// /*ntc*/		                {EXTERNAL_NTC_PER_SLAVE_MIN_VALUE, 	   					EXTERNAL_NTC_PER_SLAVE_MAX_VALUE},\
// 			                {INTERNAL_NTC_PER_SLAVE_MIN_VALUE, 	   					INTERNAL_NTC_PER_SLAVE_MAX_VALUE},\
// 			                {NTC_BETA_VALUE_AT_25C_MIN_VALUE, 	   					NTC_BETA_VALUE_AT_25C_MIN_VALUE},\
// /*shunt*/	                {SHUNT_RESISTOR_uOhm_MIN_VALUE, 	   					SHUNT_RESISTOR_uOhm_MAX_VALUE},\
// /*cell voltage*/			{CELL_MAX_VOLTAGE_THRESHOLD_mV_MIN_VALUE, 	   			CELL_MAX_VOLTAGE_THRESHOLD_mV_MAX_VALUE},\
// 							{CELL_MIN_VOLTAGE_THRESHOLD_mV_MIN_VALUE, 	   			CELL_MIN_VOLTAGE_THRESHOLD_mV_MAX_VALUE},\
// 							{CELL_BALANCING_START_VOLTAGE_mV_MIN_VALUE, 	  	 	CELL_BALANCING_START_VOLTAGE_mV_MAX_VALUE},\
// 							{CELL_IMBALANCE_THRESHOLD_mV_MIN_VALUE, 	   			CELL_IMBALANCE_THRESHOLD_mV_MAX_VALUE},\
// /*current*/					{MAX_CHARGE_CURRENT_THRESHOLD_A_MIN_VALUE, 	   			MAX_CHARGE_CURRENT_THRESHOLD_A_MAX_VALUE},\
// 							{MAX_DISCHARGE_CURRENT_THRESHOLD_A_MIN_VALUE, 	   		MAX_DISCHARGE_CURRENT_THRESHOLD_A_MAX_VALUE},\
// /*capacity*/				{PACK_MAX_CAPACITY_Ah_MIN_VALUE, 	   					PACK_MAX_CAPACITY_Ah_MAX_VALUE},\
// 							{PACK_MIN_CAPACITY_Ah_MIN_VALUE, 	   					PACK_MIN_CAPACITY_Ah_MAX_VALUE},\
// /*cell temp*/				{CELL_MAX_TEMP_THRESHOLD_C_MIN_VALUE, 	   				CELL_MAX_TEMP_THRESHOLD_C_MAX_VALUE},\
// 							{CELL_MIN_TEMP_THRESHOLD_C_MIN_VALUE, 	   				CELL_MIN_TEMP_THRESHOLD_C_MAX_VALUE},\
// /*board temp*/				{BOARD_MAX_TEMP_THRESHOLD_C_MIN_VALUE, 	   				BOARD_MAX_TEMP_THRESHOLD_C_MAX_VALUE},\
// 							{BOARD_MIN_TEMP_THRESHOLD_C_MIN_VALUE, 	   				BOARD_MIN_TEMP_THRESHOLD_C_MAX_VALUE},\
// /*timeout*/					{ERROR_TIMEOUT_ms_MIN_VALUE, 	   						ERROR_TIMEOUT_ms_MAX_VALUE},\
// 							{WARNING_TIMEOUT_ms_MIN_VALUE, 	   						WARNING_TIMEOUT_ms_MAX_VALUE},\
// 							{RECOVERY_TIMEOUT_ms_MIN_VALUE, 	   					RECOVERY_TIMEOUT_ms_MAX_VALUE},\
// /*protection threshold*/	{MAX_ALLOWED_TEMP_CHANGE_C_MIN_VALUE, 	   				MAX_ALLOWED_TEMP_CHANGE_C_MAX_VALUE},\
// 							{MAX_ALLOWED_TEMP_GRADIENT_C_MIN_VALUE, 	   			MAX_ALLOWED_TEMP_GRADIENT_C_MAX_VALUE},\
// 							{HIGH_IMBALANCE_ERROR_mV_MIN_VALUE, 	  				HIGH_IMBALANCE_ERROR_mV_MAX_VALUE},\
// /*balancing*/				{BALANCING_OFF_TIME_ms_MIN_VALUE, 	   					BALANCING_OFF_TIME_ms_MAX_VALUE},\
// 							{BALANCING_ON_TIME_ms_MIN_VALUE, 	   					BALANCING_ON_TIME_ms_MAX_VALUE},\
// /*contactor*/				{CONTACTOR_CUT_OFF_TIME_ms_MIN_VALUE, 	   				CONTACTOR_CUT_OFF_TIME_ms_MAX_VALUE},\
// 							{PRE_CHARGE_CURRENT_mA_MIN_VALUE, 	   					PRE_CHARGE_CURRENT_mA_MAX_VALUE},\
// /*cc*/						{VEHICLE_REST_TIME_m_MIN_VALUE, 	   					VEHICLE_REST_TIME_m_MAX_VALUE},\
// 							{VEHICLE_REST_CURRENT_A_MIN_VALUE, 	   					VEHICLE_REST_CURRENT_A_MAX_VALUE},\
// /*data pipeline*/			{CHARGE_CURRENT_DETECTION_THRESHOLD_mA_MIN_VALUE, 	    CHARGE_CURRENT_DETECTION_THRESHOLD_mA_MAX_VALUE},\
// };



/* Function definition -------------------------------------------------------*/
/**
 * @brief initialize all the configuration parameters from the #define
 * @param data pointer to the array
 * @retval BMS status
 */
bmsStatus_t initializeFlashConfigData(__IO uint64_t *data, uint8_t sectionType)
{
	bmsStatus_t status = BMS_ERROR;

	if(NULL == data)
	{
		status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
		return status;
	}
	switch(sectionType)
	{
		case FLASH_CONFIG_SECTION:
		case FLASH_COTA_UPDATE:
			// what will be the default config version and how will it come into play?
			// could be like each firmware version is married to a default config version

			data[CONFIG_VERSION]						= ((uint64_t)CONFIG_MAJOR_VERSION << 16) | ((uint64_t)CONFIG_MINOR_VERSION << 8) | CONFIG_PATCH_VERSION;
			
			//CMU and cell
			data[NUMBER_OF_CMU_INDEX]                	= NUMBER_OF_CMU;
			data[CELL_IN_SERIES_INDEX]               	= CELL_IN_SERIES;
			
			//shunt
			data[SHUNT_RESISTOR_uOhm_INDEX]             = SHUNT_RESISTOR_uOhm;

			//cell voltage
			data[CELL_MAX_VOLTAGE_THRESHOLD_mV_INDEX]   = CELL_MAX_VOLTAGE_THRESHOLD_mV;
			data[CELL_MIN_VOLTAGE_THRESHOLD_mV_INDEX]   = CELL_MIN_VOLTAGE_THRESHOLD_mV;
			data[CELL_BALANCING_START_VOLTAGE_mV_INDEX] = CELL_BALANCING_START_VOLTAGE_mV;
			data[CELL_IMBALANCE_THRESHOLD_mV_INDEX]     = CELL_IMBALANCE_THRESHOLD_mV;

			//capacity
			data[PACK_MAX_CAPACITY_Ah_INDEX]      		= PACK_MAX_CAPACITY_Ah;
			data[PACK_USABLE_CAPACITY_mAh_INDEX]   		= INSTALLED_CAPACITY;
			data[EEPROM_CAP_WRITE_mAh_INDEX]   			= EEPROM_CAP_WRITE;

			//current
			data[IR_WAIT_SOC_INDEX]						=	IR_WAIT_SOC;
			data[IR_START_SOC_INDEX]  					=	IR_START_SOC;
			data[IR_CYCLE_COUNT_THRESHOLD_INDEX] 		= 	IR_CYCLE_COUNT_THRESHOLD;
			data[SLOW_CHARGING_MAX_CURRENT_A_INDEX] 	=	SLOW_CHARGING_MAX_CURRENT_A;
			data[FAST_CHARGING_MAX_CURRENT_A_INDEX] 	=	FAST_CHARGING_MAX_CURRENT_A;

			data[CHARGE_CURRENT_DETECTION_THRESHOLD_mA_INDEX] = CHARGE_CURRENT_DETECTION_THRESHOLD_mA;
			data[SLOW_CHARGING_TARGET_mV_INDEX]			= SLOW_CHARGING_TARGET_mV;
			data[FAST_CHARGING_TARGET_mV_INDEX]			= FAST_CHARGING_TARGET_mV;
			data[CV_TRANSITION_mV_INDEX]				= CV_TRANSITION_mV;

			//charging k factors
			data[FAST_CHARGING_SCALING_FACTOR_INDEX]       = FAST_CHARGING_SCALING_FACTOR;
			data[SLOW_CHARGING_CV_SCALING_FACTOR_INDEX]    = SLOW_CHARGING_CV_SCALING_FACTOR;
			data[SLOW_CHARGING_CC_SCALING_FACTOR_INDEX]    = SLOW_CHARGING_CC_SCALING_FACTOR;


			data[OCC_ERROR_CURRENT_A_INDEX]				= OCC_ERROR_CURRENT_A;
			data[OCC_WARNING_CURRENT_A_INDEX]			= OCC_WARNING_CURRENT_A;
			data[OCD_ERROR_CURRENT_A_INDEX]				= OCD_ERROR_CURRENT_A;
			data[OCD_WARNING_CURRENT_A_INDEX]			= OCD_WARNING_CURRENT_A;

			//Timeouts
			data[ERROR_TIMEOUT_ms_INDEX] 				= ERROR_TIMEOUT_ms;
			data[WARNING_TIMEOUT_ms_INDEX]     			= WARNING_TIMEOUT_ms;
			data[RECOVERY_TIMEOUT_ms_INDEX]      		= RECOVERY_TIMEOUT_ms;

			// balancing derating
			data[BALANCING_DERATING_START_TEMP_C_INDEX] = BALANCING_DERATING_START_TEMP_C;
			data[BALANCING_DERATING_END_TEMP_C_INDEX]	= BALANCING_DERATING_END_TEMP_C;
			data[BALANCING_MAX_ON_TIME_ms_INDEX]		= BALANCING_MAX_ON_TIME_ms;
			data[BALANCING_MIN_ON_TIME_ms_INDEX]		= BALANCING_MIN_ON_TIME_ms;
			data[BALANCING_MAX_OFF_TIME_ms_INDEX]		= BALANCING_MAX_OFF_TIME_ms;

			//protection threshold
			//temp group1
			data[OTC_ERROR_TEMPERATURE_GROUP_1_INDEX]   = OTC_ERROR_TEMPERATURE_GROUP_1;
			data[OTC_WARNING_TEMPERATURE_GROUP_1_INDEX] = OTC_WARNING_TEMPERATURE_GROUP_1;
			data[OTC_RECOVERY_TEMPERATURE_GROUP_1_INDEX]= OTC_RECOVERY_TEMPERATURE_GROUP_1;

			data[OTD_ERROR_TEMPERATURE_GROUP_1_INDEX]   = OTD_ERROR_TEMPERATURE_GROUP_1;
			data[OTD_WARNING_TEMPERATURE_GROUP_1_INDEX] = OTD_WARNING_TEMPERATURE_GROUP_1;
			data[OTD_RECOVERY_TEMPERATURE_GROUP_1_INDEX]= OTD_RECOVERY_TEMPERATURE_GROUP_1;

			data[UTC_ERROR_TEMPERATURE_GROUP_1_INDEX]   = UTC_ERROR_TEMPERATURE_GROUP_1;
			data[UTC_WARNING_TEMPERATURE_GROUP_1_INDEX] = UTC_WARNING_TEMPERATURE_GROUP_1;
			data[UTC_RECOVERY_TEMPERATURE_GROUP_1_INDEX]= UTC_RECOVERY_TEMPERATURE_GROUP_1;

			data[UTD_ERROR_TEMPERATURE_GROUP_1_INDEX]   = UTD_ERROR_TEMPERATURE_GROUP_1;
			data[UTD_WARNING_TEMPERATURE_GROUP_1_INDEX] = UTD_WARNING_TEMPERATURE_GROUP_1;
			data[UTD_RECOVERY_TEMPERATURE_GROUP_1_INDEX]= UTD_RECOVERY_TEMPERATURE_GROUP_1;

			//temp group2
			data[OTC_ERROR_TEMPERATURE_GROUP_2_INDEX]   = OTC_ERROR_TEMPERATURE_GROUP_2;
			data[OTC_WARNING_TEMPERATURE_GROUP_2_INDEX] = OTC_WARNING_TEMPERATURE_GROUP_2;
			data[OTC_RECOVERY_TEMPERATURE_GROUP_2_INDEX]= OTC_RECOVERY_TEMPERATURE_GROUP_2;

			data[OTD_ERROR_TEMPERATURE_GROUP_2_INDEX]   = OTD_ERROR_TEMPERATURE_GROUP_2;
			data[OTD_WARNING_TEMPERATURE_GROUP_2_INDEX] = OTD_WARNING_TEMPERATURE_GROUP_2;
			data[OTD_RECOVERY_TEMPERATURE_GROUP_2_INDEX]= OTD_RECOVERY_TEMPERATURE_GROUP_2;

			data[UTC_ERROR_TEMPERATURE_GROUP_2_INDEX]   = UTC_ERROR_TEMPERATURE_GROUP_2;
			data[UTC_WARNING_TEMPERATURE_GROUP_2_INDEX] = UTC_WARNING_TEMPERATURE_GROUP_2;
			data[UTC_RECOVERY_TEMPERATURE_GROUP_2_INDEX]= UTC_RECOVERY_TEMPERATURE_GROUP_2;

			data[UTD_ERROR_TEMPERATURE_GROUP_2_INDEX]   = UTD_ERROR_TEMPERATURE_GROUP_2;
			data[UTD_WARNING_TEMPERATURE_GROUP_2_INDEX] = UTD_WARNING_TEMPERATURE_GROUP_2;
			data[UTD_RECOVERY_TEMPERATURE_GROUP_2_INDEX]= UTD_RECOVERY_TEMPERATURE_GROUP_2;

			data[HIGH_IMBALANCE_ERROR_mV_INDEX] 		= HIGH_IMBALANCE_ERROR_mV;

			//Precharge
			data[CONTACTOR_CUT_OFF_TIME_ms_INDEX] 		= CONTACTOR_CUT_OFF_TIME_ms;
			data[PRECHARGE_RETRY_TIMEOUT_INDEX]			= PRECHARGE_TIMEOUT_RETRY_mSec;
			data[PRECHARGE_RETRY_LIMIT_INDEX]			= PRECHARGE_RETRY_LIMIT_COUNT;
			data[PRECHARGE_TIMEOUT_INDEX]				= PRECHARGE_TIMEOUT_VALUE;

			//Minimum required config will be based on the vehicle type
			data[MINIMUM_REQUIRED_CONFIG_INDEX]  		= MINIMUM_REQUIRED_CONFIG;

			//crc
			data[FLASH_CONFIG_CRC_INDEX]     			= calculateFlashCRC(sectionType, data, FLASH_CONFIG_NUMBER_OF_PARAMETERS);
			status = BMS_OK;
			break;
		
		case FLASH_MARVEL_SELF_DATA_SECTION:
				               
			data[HW_MAJOR]				      =					 HW_MAJOR_VERSION;               
			data[HW_MINOR]					  =					 HW_MINOR_VERSION;              
			data[HW_PATCH]					  =					 HW_PATCH_VERSION;  
			data[FLASH_VERSION_CRC_INDEX]     =				     calculateFlashCRC(sectionType, data, FLASH_VERSION_CRC_INDEX);        
			status = BMS_OK;
			break;
		
		default:
			status = BMS_ERROR;
			break;

	}

	
	return status;
}


/* Function definition -------------------------------------------------------*/
/**
 * @brief Read data From Flash memory
 * @param sectionType section of the flash
 * @param flashAddress starting address of the Flash memory
 * @param flashData pointer to data to be read
 * @param dataSize size of data in Bytes
 * @retval BMS status
 */
bmsStatus_t readDataFromFlash(uint8_t sectionType, uint32_t flashAddress, __IO uint64_t *flashData, uint16_t dataSize)
{
	bmsStatus_t status 	= BMS_ERROR;

	if( !(IS_FLASH_SECTION_TYPE(sectionType)) 		||\
		!(IS_FLASH_PROGRAM_ADDRESS(flashAddress)) 	||\
		(NULL == flashData) 						||\
		(dataSize <= 0))
	{
		status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	if(*(__IO uint64_t *)(flashAddress) == DATA_EMPTY)
	{
		switch(sectionType)
		{
			case FLASH_CONFIG_SECTION:
			case FLASH_MARVEL_SELF_DATA_SECTION:
				//Data is not present
				//1. Initialize the flash config data with default values
				initializeFlashConfigData(flashData, sectionType);

				//2. Write data to the flash
				status = flashWrite(flashAddress, flashData, dataSize);
				CHECK_STATUS(status);
				break;
		
			case FLASH_EXCEPTION_SECTION:
				// read from flash exception section
				status = flashRead(flashAddress, flashData, dataSize);
				CHECK_STATUS(status);
				
				if(0xFFFFFFFFFFFFFFFF == flashData[0])
				{
					status = BMS_FLASH_EXCEPTION_DATA_NOT_FOUND;
				}
				
				CHECK_STATUS(status);
				break;

			case FLASH_SOC_SECTION:
				status = flashRead(flashAddress, flashData, dataSize);
				CHECK_STATUS(status);
				break;

			default:
				status 	= BMS_ERROR;
				break;
		}
	}

	status = checkFlashData(sectionType, flashAddress, flashData, dataSize);
	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief write data to flash
 * @param sectionType section of the flash
 * @param flashAddress starting address of the Flash memory
 * @param flashData pointer to data
 * @param dataSize size of data in Bytes
 * @param receivedData data to be updated
 * @retval BMS status
 */
bmsStatus_t writeDataToFlash(uint8_t sectionType, uint32_t flashAddress, __IO uint64_t *flashData, uint16_t dataSize, uint8_t *receivedData)
{
	bmsStatus_t status 	   = BMS_ERROR;

	uint32_t crcCalculated = 0;

	if( !(IS_FLASH_SECTION_TYPE(sectionType)) 		||\
		!(IS_FLASH_PROGRAM_ADDRESS(flashAddress)) 	||\
		(NULL == flashData) 						||\
		(dataSize <= 0))
	{
		status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	switch(sectionType)
	{
		case FLASH_COTA_UPDATE:

			crcCalculated = calculateFlashCRC(sectionType, flashData, FLASH_CONFIG_TOTAL_LENGTH - 1);
	 		
			// crcStored = flashData[FLASH_CONFIG_CRC_INDEX];

			if(crcCalculated == flashData[FLASH_CONFIG_CRC_INDEX])	// receiving the crc here from server
			{
				configWriteFlag = 1;		
			}
			
			break;
	
		case FLASH_EXCEPTION_SECTION:
			
			status = flashWrite(flashAddress, flashData, dataSize);
			status = checkFlashData(sectionType, flashAddress, flashData, dataSize);
			CHECK_STATUS(status);
			break;
		
		case FLASH_SOC_SECTION:
		case FLASH_MARVEL_SELF_DATA_SECTION:
			flashData[dataSize - CRC_LENGTH] = calculateFlashCRC(sectionType ,flashData, dataSize - CRC_LENGTH);
			status = flashWrite(flashAddress, flashData, dataSize);
			status = checkFlashData(sectionType, flashAddress, flashData, dataSize);
			CHECK_STATUS(status);
			break;

		default:
			status = BMS_ERROR;
			break;
	}

	if(configWriteFlag)	// only allow this if CRC matches
	{
		status = flashWrite(flashAddress, flashData, dataSize);
		CHECK_STATUS(status);

		status = checkFlashData(sectionType, flashAddress, flashData, dataSize);
		configWriteFlag = 0;
	}
		return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief check that the data was written properly
 * @param sectionType section of the flash
 * @param flashAddress starting address of the Flash memory
 * @param flashData pointer to data
 * @param dataSize size of data in Bytes
 * @retval BMS status
 */
bmsStatus_t checkFlashData(uint8_t sectionType, uint32_t flashAddress, __IO uint64_t *flashData, uint16_t dataSize)
{
	bmsStatus_t status 			= BMS_ERROR;
	uint32_t 	calculatedCRC 	= 0;
	uint32_t 	storedCRC		= 0;

	if(!(IS_FLASH_PROGRAM_ADDRESS(flashAddress)) 	||\
		(NULL == flashData) 						||\
		(dataSize <= 0))
	{
		status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	switch(sectionType)
		{
			case FLASH_CONFIG_SECTION:
			case FLASH_COTA_UPDATE:
			case FLASH_MARVEL_SELF_DATA_SECTION:
				//1. read the data from flash
				status = flashRead(flashAddress, flashData, dataSize);
				CHECK_STATUS(status);

				//2. read the stored CRC value from flash
				storedCRC = flashData[dataSize - CRC_LENGTH]; 

				//3. calculate the CRC for the data read
				calculatedCRC = calculateFlashCRC(sectionType, flashData, dataSize - CRC_LENGTH);

				//4. Check the firmware compatibility
				status = checkFirmwareConfigCompatibility(flashData, sectionType);

				//5. Check CRC and status
				if((calculatedCRC != storedCRC) || (status != BMS_OK))
				{
					//1. Erase the page
					status = flashErase(flashAddress);
					CHECK_STATUS(status);

					//2. initialize with the default data
					initializeFlashConfigData(flashData, sectionType);
					status = BMS_FLASH_CRC_NOT_EQUAL;
				}
				/* 
					updating the config version here
					each bytes will correspond to the major, minor and patch versions respectively
				*/
				if(sectionType == FLASH_MARVEL_SELF_DATA_SECTION)
				{
					marvelHardwareVersion[HW_MAJOR]	 =  (uint8_t) flashData[HW_MAJOR];
					marvelHardwareVersion[HW_MINOR]	 =  (uint8_t) flashData[HW_MINOR];
					marvelHardwareVersion[HW_PATCH]	 =  (uint8_t) flashData[HW_PATCH];
				}
				else
				{
					marvelConfigVersion[CONFIG_MAJOR] = ((flashData[CONFIG_VERSION] >> 16) & 0xFF);
					marvelConfigVersion[CONFIG_MINOR] = ((flashData[CONFIG_VERSION] >> 8) & 0xFF);
					marvelConfigVersion[CONFIG_PATCH] = (flashData[CONFIG_VERSION] & 0xFF);
				}
				break;
		
			case FLASH_EXCEPTION_SECTION:
			case FLASH_SOC_SECTION:
				status = flashRead(flashAddress, flashData, dataSize);
				CHECK_STATUS(status);
				storedCRC = flashData[dataSize - CRC_LENGTH]; 
				calculatedCRC = calculateFlashCRC(sectionType, flashData, dataSize - CRC_LENGTH);

				if((calculatedCRC != storedCRC) || (status != BMS_OK))
				{
					status = flashErase(flashAddress);
					CHECK_STATUS(status);
					status = BMS_FLASH_CRC_NOT_EQUAL;
				}
				break;

			default:
				status = BMS_ERROR;
				break;
	}

	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief update the rececive data 
 * @param receivedData received data 
 * @param index pointer to index
 * @param updatedData pointer to the updated data
 * @retval BMS status
 */
// bmsStatus_t updateReceivedData(uint8_t *receivedData, uint16_t *index, uint64_t *updatedData)
// {
// 	bmsStatus_t status 	= BMS_ERROR;

// 	if(	(NULL == receivedData) 	||\
// 		(NULL == index) 		||\
// 		(NULL == updatedData))
// 	{
// 		status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
// 		return status;
// 	}
	
// 	*index 	= (receivedData[1] << 8) | (receivedData[0] & 0xFF); //little endian
// 	if(!((*index >= 0) && ((*index <= (FLASH_CONFIG_TOTAL_LENGTH - FLASH_CONFIG_CRC_LENGTH)) || (*index == 0xFFFF))))
// 	{
// 		status = BMS_FLASH_CONFIG_DATA_OUT_OF_INDEX;
// 		return status;
// 	}

// 	if(*index == 0xFFFF)
// 	{
// 		status = BMS_OK;
// 		configWriteFlag = 1;
// 		return status;
// 	}

// 	for (int i = 0; i < 6; i++)
// 	{
// 		// 0th and 1st element are the index and data is stored in the rest 6 bytes
// 		*updatedData |= (receivedData[i + 2] << (8 * i));
// 	}

// 	if(!((*updatedData >= flashConfigDataRange[*index][FLASH_CONFIG_DATA_RANGE_MIN_INDEX]) &&\
// 		  (*updatedData <= flashConfigDataRange[*index][FLASH_CONFIG_DATA_RANGE_MAX_INDEX])))
// 	{
// 		status = BMS_FLASH_CONFIG_DATA_OUT_OF_RANGE;
// 		return status;
// 	}

// 	status = BMS_OK;
// 	return status;
// }


/* Function definition -------------------------------------------------------*/
/**
 * @brief check if received data is same as stored data, if yes don't write again on falsh
 * @param flashData pointer to data
 * @param index pointer to index
 * @param updatedData pointer to the updated data
 * @retval BMS status
 */
bmsStatus_t checkReceivedDataIsSame(__IO uint64_t *flashData, uint16_t *index, uint64_t *updatedData)
{
	bmsStatus_t status 	= BMS_ERROR;
	__IO uint64_t storedData = 0;

	if(	(NULL == flashData) ||\
		(NULL == index) 	||\
		(NULL == updatedData))
	{
		status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	storedData = flashData[*index];

	if(storedData == *updatedData)
	{
		status = BMS_FLASH_CONFIG_RECEIVED_DATA_IS_SAME;
		return status;
	}

	status = BMS_OK;
	return status;
}


/* Function definition -------------------------------------------------------*/
/**
 * @brief update the stored data
 * @param flashData pointer to data
 * @param index pointer to index
 * @param updatedData pointer to the updated data
 * @retval BMS status
 */
bmsStatus_t updateStoredData(__IO uint64_t *flashData, uint16_t *index, uint64_t *updatedData)
{
	bmsStatus_t status 	= BMS_ERROR;

	if(	(NULL == flashData) ||\
		(NULL == index) 	||\
		(NULL == updatedData))
	{
		status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	flashData[*index] = *updatedData;
	flashData[FLASH_CONFIG_CRC_INDEX] = calculateFlashCRC(FLASH_CONFIG_SECTION, flashData, FLASH_CONFIG_NUMBER_OF_PARAMETERS);

	status = BMS_OK;
	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief checks for firmware config major version compactibility
 * @param flashData pointer to data
 * @retval BMS status
 */
bmsStatus_t checkFirmwareConfigCompatibility(__IO uint64_t *flashData, uint8_t sectionType)
{
	bmsStatus_t status 	= BMS_CONFIG_MINIMUM_VERSION_UNSATISFIED;
	uint8_t configMajorVersion = 0;
	uint8_t hwMajorversion 	   = 0;
	// uint8_t swMajorversion 	   = 0;
	uint8_t hwMinorversion 	   = 0;
	// uint8_t swMinorversion 	   = 0;
	uint8_t hwPatchversion 	   = 0;
	// uint8_t swPatchversion 	   = 0;

	switch(sectionType)
	{
		case FLASH_CONFIG_SECTION:
		case FLASH_COTA_UPDATE: 
				configMajorVersion = ((flashData[CONFIG_VERSION] >> 16) & 0xFF);
				
				// make a compatibility matrix here
				/*
					firmware version v00.0A.0B, compatible with configs 1 and 2

					IMPORTANT
					---------
					v00.0A.0B and older is only compactible with configs 1 and 2

					firmware version v00.0A.0C, min required config 3


					all the newer firmwares will be configured to be reverse compatible with older configs

					if any new configs are added, min required config will be changed

					older firmwares starting from v00.0A.0C will be compatible with all the newer firmwares

				*/

				//Minimum required config will be based on the vehicle type 155Ah = 100,  180Ah = 4 from v00.0A.0E

				if(configMajorVersion >= flashData[MINIMUM_REQUIRED_CONFIG_INDEX])
				{
					status = BMS_OK;
				}

				break;
	
		case FLASH_MARVEL_SELF_DATA_SECTION:

				hwMajorversion = flashData[HW_MAJOR];
				hwMinorversion = flashData[HW_MINOR];
				hwPatchversion = flashData[HW_PATCH];

				/*Add comparisons for FW versions compatible with the HW versions*/

				if(((hwMajorversion >= HW_MAJOR_VERSION) && (hwMinorversion >= HW_MINOR_VERSION) && (hwPatchversion >= HW_PATCH_VERSION)))
				{
					status = BMS_OK;
				}
				
				break;
		
		default:
			break;


	}

	return status;
}


/* Function definition -------------------------------------------------------*/
/**
 * @brief calculates CRC for the flash data
 * @param data pointer to data
 * @param size size of data in Bytes
 * @retval crc
 */
uint32_t calculateFlashCRC( uint8_t sectionType, __IO uint64_t *data, uint16_t size)
{
	if(	(NULL == data) ||!(IS_FLASH_SECTION_TYPE(sectionType)) ||\
		(size <= 0))
	{
		return BMS_FLASH_FUNCTION_PARAMETER_ERROR;
	}

	uint32_t poly = 0xEDB88320;
	uint32_t crc = 0xFFFFFFFF;
	uint8_t *val = (uint8_t *)data;
	
	switch(sectionType)
	{
		case FLASH_SOP_DATA_SECTION:
			for (uint16_t i = 0; i < size; i++) 
			{
				crc ^= val[i];

				for (int j = 0; j < 32; j++) 
				{ 
					uint32_t mask = -(crc & 1);
					crc = (crc >> 1) ^ (poly & mask);
				}
    		}
			break;
		
		default:
			for (uint16_t i = 0; i < size; i++)
			{
				crc ^= (uint32_t)data[i];

				for (int j = 0; j < 32; j++)
				{
					uint32_t mask = -(crc & 1);
					crc = (crc >> 1) ^ (poly & mask);
				}
			}
			break;
		
	}

	return (~crc);

}

/* End of File ---------------------------------------------------------------*/