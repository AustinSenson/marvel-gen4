/**
 * @file marvelStateMachine.h
 * @brief 
 * @version 0.1
 * @date 20/02/2024
 * @copyright Copyright (c) 2024
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef MARVEL_STATE_MACHINE_H_
#define MARVEL_STATE_MACHINE_H_

/* Standard C includes -------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/
#include "threadxMain.h"
#include "dataProcessing.h"
#include "bmsConfiguration.h"
#include "functionalTests.h"
#include "onBoardMemory.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

typedef enum
{
    INITIALIZING = 0,
    CONTACTOR_OVERRIDE_CHECK,  
    CONTACTOR_OVERRIDE,    
    WAIT_FOR_TOGGLE,    
    CONTACTOR_ON,       
    CONTACTOR_OFF,  
}contactorState;

typedef enum
{
    ENABLE_MERLYN =0,
    DISABLE_MERLYN,
    EEPROM_TEST ,
    CONTACTOR_TEST_ON,
    CONTACTOR_TEST_OFF,
    UNIQUE_IDS,
}merlynTestRequest;

/* External variables --------------------------------------------------------*/

extern merlynTestRequest testRequest;


/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
// void marvelStateMachine(bmsMainData_t *mdata);
bmsStatus_t handleMerlynRequest(merlynTestRequest testRequest, bmsMainData_t *mdata);

#endif
/* End of File ---------------------------------------------------------------*/

