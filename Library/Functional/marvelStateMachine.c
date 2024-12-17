/**

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "marvelStateMachine.h"
#include "bmsConfiguration.h"
#include "onBoardMemory.h"
#include "threadxMain.h"

/* Self include --------------------------------------------------------------*/

/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/
extern bmsMainData_t mdata;
extern uint8_t fullChargeLatch;
extern uint8_t merlynDataWriteBuff[10];
extern uint8_t merlynDataReadBuff[10];
extern uint8_t contactorCommandFromMerlin;
extern uint8_t enableMerlyn;
extern bool marvelInfoReceive;
/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
bool overrideContactorLogic = false;
bool externalToggle = false;


/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/

// void marvelStateMachine(bmsMainData_t *mdata)
// {
//     switch(mdata->bmsState)
//     {
//         case MARVEL_INIT:
        
//             // fullChargeLatch == 1 ? (overrideContactorLogic = true) : (overrideContactorLogic = false);
//             break;

//         case MARVEL_READY:
//             break;

//         case MARVEL_ACTIVATING:
//             break;

//         case MARVEL_ACTIVE:
//             break;

//         case MARVEL_ERROR:
//             break;
//     }
// }

bmsStatus_t handleMerlynRequest(merlynTestRequest testRequest, bmsMainData_t *mdata)
{
    bmsStatus_t status = BMS_ERROR;

    if(NULL == mdata)
	{
		status = BMS_DATA_PIPELINE_FUNCTION_PARAMETER_ERROR;
		return status;
	}
    
    switch(testRequest)
    {
        case ENABLE_MERLYN:
            enableMerlyn=1;
            status = BMS_OK;
            break;
        
        case DISABLE_MERLYN:
            enableMerlyn=0;
            status = BMS_OK;
            break;
        
        case EEPROM_TEST:
            /* write to EEPROM
               read from EEPROM
               check writeData == readData */
            
            RETRY(RETRY_EEPROM, status, memoryWrite(eeprom, merlynDataWriteBuff, sizeof(merlynDataWriteBuff), MERLYN_HANDLER_SECTION));        
	        CHECK_STATUS(status);

            RETRY(RETRY_EEPROM, status, memoryRead(eeprom, merlynDataReadBuff, sizeof(merlynDataReadBuff), MERLYN_HANDLER_SECTION));
	        CHECK_STATUS(status);

            for (uint8_t i = 0; i < sizeof(merlynDataWriteBuff); i++) 
            {
                if (merlynDataWriteBuff[i] != merlynDataReadBuff[i]) 
                { 
                    return BMS_MEMORY_FUNCTION_PARAMETER_ERROR;  // Arrays are not equal
                }
            }            
            break;

        case CONTACTOR_TEST_ON:
            // functionalTests(overTemperatureDischargeTest, mdata);
            contactorCommandFromMerlin = 1;
            status = BMS_OK;
            break;

        case CONTACTOR_TEST_OFF:
        // functionalTests(overTemperatureDischargeTest, mdata);
            contactorCommandFromMerlin = 0;
            status = BMS_OK;
            break;

        case UNIQUE_IDS:
            status = publishMarvelUniqueID();
            break;
        
        case UPDATE_BIO:
            marvelInfoReceive = true;
            status = BMS_OK;
            break;

        default:
            // do nothing here
            status = BMS_OK;
            break;
    }

    return status;
}

/**
/* Function definition -------------------------------------------------------*/
/**

/* Function definition -------------------------------------------------------*/
/**

/* End of File ---------------------------------------------------------------*/

