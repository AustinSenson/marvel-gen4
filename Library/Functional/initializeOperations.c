/**
 * @file initializeOperations.c
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-05-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/
#include <string.h>
/* Private includes ----------------------------------------------------------*/
#include "bmsCANMatrix.h"
#include "bmsErrorCodes.h"
#include "bmsConfiguration.h"
#include "onBoardCCM2949.h"
#include "onBoardCANv2b.h"
#include "onBoardMemory.h"
#include "gpioControls.h"
#include "conversion.h"
#include "monitorOperations.h"
#include "adBms_Application.h"

/* Self include --------------------------------------------------------------*/
#include "initializeOperations.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

extern DynamicLimitsTable_t    dynamicLimitsTable;
extern contactorInput_t        cInputData;
extern thresholdData_t         thresholdData;
extern coulombCountingInput_t  ccInputData;
extern dataPipelineInput_t     dpInputData;
extern protectionInput_t       pInputData;
extern cellBalancingInput_t    cbInputData;


/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief initialize cell monitoring unit
 * @param 
 * @retval BMS status
 */
bmsStatus_t initializeCMU(bmsMainData_t *mdata)
{
    bmsStatus_t status = BMS_ERROR;
	if((NULL == mdata))
	{
		status = BMS_CMU_FUNCTION_PARAMETER_ERROR;
		return status;
	}	

    status = adBms6830_init_config(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
	return status;
}


/* Function definition -------------------------------------------------------*/
/**
 * @brief initialize CAN
 * @param canID can ID
 * @retval BMS status
 */
bmsStatus_t initializeCAN(uint8_t canPort)
{
    bmsStatus_t status = BMS_ERROR;

	if(!(IS_CAN_PORT(canPort)))
	{
		status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    status = initializeCANMatrix(canPort);
#ifdef DEBUG_CAN
    initializeDebugCANMatrix();
#endif
    CHECK_STATUS(status);
    status = initializeCANCommunication(canPort);
    return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief initialize EEPROM
 * @param mdata pointer to bmsData structure
 * @retval BMS status
 */
bmsStatus_t initializeEEPROM(bmsMainData_t *mdata)
{
	bmsStatus_t status = BMS_ERROR;
	
	if((NULL == mdata))
	{
		status = BMS_EEPROM_FUNCTION_PARAMETER_ERROR;
		return status;
	}	

	// status = eepromEnableWriteProtection(eepromID);
	eepromDisableWriteProtection(eepromID);

	return status;
}

bmsStatus_t initializeMarvelFunctionalities(void)
{
	bmsStatus_t status = BMS_ERROR;

	status = pipelineInit();
    status = readFromPipeline(&mdata);

	// MATLAB initializations
    CellBalancing_initialize();
    DataPipeline_initialize();

    thresholdInit(&thresholdData, &dpInputData, &cbInputData, &pInputData, &cInputData, &ccInputData);
    
    dataPipelineInit(&dynamicLimitsTable, &dpInputData, &DataPipeline_U);
    cellBalancingInit(&thresholdData, &cbInputData, &CellBalancing_U, &dpInputData);

    // publish unique ID of Marvel controller on startup
    status = publishMarvelUniqueID();

    // publish GIT_SHA on startup
    status = publishMarvelGitSha();

	// initializing CAN communication
	RETRY(RETRY_CAN_INIT, status, initializeCAN(communicationCAN));

	// initializing EEPROM
	RETRY(RETRY_EEPROM, status, initializeEEPROM(&mdata));
	
	// initializing config section
	RETRY(RETRY_FLASH_READ, status, readDataFromFlash(FLASH_CONFIG_SECTION, FLASH_CONFIG_ADDRESS, defaultFlashConfigData, FLASH_CONFIG_TOTAL_LENGTH));

	// initializing ADBMS6832
	RETRY(RETRY_CMU_INIT, status, initializeCMU(&mdata));

    // initializing ADBMS2950
	RETRY(RETRY_CCM_INIT, status, initializeCCM());

	return status;
}


/* End of File ---------------------------------------------------------------*/
