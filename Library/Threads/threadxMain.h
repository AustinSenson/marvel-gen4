/**
 * @file threadxMain.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-03-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef THREADXMAIN_H_
#define THREADXMAIN_H_

/* Standard C includes -------------------------------------------------------*/
#include "stdint.h"
/* Private includes ----------------------------------------------------------*/
#include "tx_api.h"
#include "bmsConfiguration.h"
#include "dataProcessing.h"

#include "CellBalancing.h"
#include "Contactors.h"
#include "SOCEstimation.h"
#include "DataPipeline.h"
#include "EKF.h"
#include "Protection.h"
#include "HighLevelStatemachine.h"
#include "ChargingAlgorithm.h"
#include "lookupTable.h"
#include "onBoardMemory.h"
#include "batteryCooling.h"
#include "structures.h"
#include "firmwareUpgradeApp.h"
#include "marvelStateMachine.h"

/* Private define ------------------------------------------------------------*/

//dcli, dclo
#define DOD_FACTOR      0.9F

#define MIN(x,y) (((x) < (y)) ? (x) : (y) )
#define MAX(x,y) (((x) > (y)) ? (x) : (y) )

/* ------------------------------- Tasks size and Priority  -------------------------*/
#define sizeBytePool_mainBytePool           60000
#define sizeStack_collectBatteryData        8192
#define sizeStack_protection                8192
#define sizeStack_contactorControl          4096
#define sizeStack_coulombCounting           4096
#define sizeStack_ccEKF                     4096
#define sizeStack_chargingAlgorithm         4096
#define sizeStack_canHandler                2048
#define sizeStack_firmwareUpgradeHandler    8196
#define sizeStack_iwdgHandler               1024

#define sizeStack_canTransmission           sizeStack_canHandler
#define sizeStack_canTxHandler              sizeStack_canHandler
#define sizeStack_canRxHandler              sizeStack_canHandler

#define priority_P0                     0
#define priority_P1                     2
#define priority_P2                     4
#define priority_P3                     8
#define priority_P4                     16
#define priority_P5                     32

#define TIME_SLICE_DEFAULT          (uint32_t)100 

#define QUEUE_SEND(queue, data, wait)       tx_queue_send(queue, data, wait)
#define QUEUE_RECEIVE(queue, data, wait)    tx_queue_receive(queue, data, wait)

// #define SUCCESS                             TX_SUCCESS
#define WAIT_FOREVER                        TX_WAIT_FOREVER
#define NO_WAIT                             TX_NO_WAIT
#define TRUE            1
#define FALSE           0
/* ------------------------------- Loop delays and variables  -------------------------*/

#define LOOP_DELAY_10MS                      10
#define LOOP_DELAY_50MS                      50
#define LOOP_DELAY_100MS                     100
#define LOOP_DELAY_250MS                     250
#define LOOP_DELAY_1000MS                    1000
#define LOOP_DELAY_CAN_TRANSMISSION          LOOP_DELAY_10MS
#define LOOP_DELAY_CONTACTOR_CONTROL         LOOP_DELAY_100MS
#define LOOP_DELAY_CMU_DATA_COLLECTION       LOOP_DELAY_100MS
#define LOOP_DELAY_PROTECTION                LOOP_DELAY_100MS
#define LOOP_DELAY_STATE_MACHINE             LOOP_DELAY_100MS
#define LOOP_DELAY_FIRMWARE_UPGRADE          LOOP_DELAY_100MS
#define LOOP_COULOMB_COUNTING                LOOP_DELAY_100MS
#define LOOP_DELAY_IWDG_RESET                LOOP_DELAY_1000MS

/* ------------------------------------ Threadx Flags  -------------------------------- */

#define SANITY_CHECK_FLAG                    BIT5
#define ERROR_DISPLAY_FLAG                   BIT6
#define ACTIVATE_PSEUDO_LATCH                BIT7
#define EEPROM_READ_FAIL                     BIT8
#define EEPROM_WRITE_FAIL                    BIT9
#define DATA_COLLECTION_SUCCESSFUL           BIT11
#define CAN_TRANSMISSION                     BIT12
#define POWER_OFF_SIG                        BIT14

// Define the maximum number of CAN messages that the queue can hold
#define CAN_QUEUE_LENGTH               200
#define CAN_TX_QUEUE_LENGTH            CAN_QUEUE_LENGTH
#define CAN_RX_QUEUE_LENGTH            CAN_QUEUE_LENGTH

// Threadx Timers
#define TIMER_TICKS                     30

// Define the size of each CAN message in the queue
#define CAN_QUEUE_MESSAGE_SIZE        sizeof(canQueueData_t)
#define CAN_TX_QUEUE_MESSAGE_SIZE     CAN_QUEUE_MESSAGE_SIZE
#define CAN_RX_QUEUE_MESSAGE_SIZE     CAN_QUEUE_MESSAGE_SIZE

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern TX_QUEUE canTxQueue;
extern TX_QUEUE canRxQueue;
extern TX_EVENT_FLAGS_GROUP systemEventFlag;

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
void entry_canTransmission(ULONG threadInput);
void entry_canTxHandler(ULONG threadInput);
void entry_canRxHandler(ULONG threadInput);
void entry_collectBatteryData(ULONG threadInput);
void entry_protection(ULONG threadInput);
void entry_contactorControl(ULONG threadInput);
void entry_coulombCounting(ULONG threadInput);
void entry_chargingAlgorithm(ULONG threadInput);
void entry_ccEKF(ULONG threadInput);
void entry_firmwareUpgrade(ULONG thread_input);
void iwdgResetEntry(ULONG threadInput);
VOID threadxTimerCallback(ULONG input); 


#ifdef EKF
bmsStatus_t thresholdInit(thresholdData_t *tData, dataPipelineInput_t *dpInput, cellBalancingInput_t *cbInput, \
            protectionInput_t *pInput, contactorInput_t *cInput, ccEKFInput_t *ccEKFInput);

#else
bmsStatus_t thresholdInit(thresholdData_t *tData, dataPipelineInput_t *dpInput, cellBalancingInput_t *cbInput, \
            protectionInput_t *pInput, contactorInput_t *cInput, coulombCountingInput_t *ccInput);

#endif
bmsStatus_t dataPipelineInit(DynamicLimitsTable_t *ldata, dataPipelineInput_t *dpInput, ExtU_DataPipeline_T *dpInputMBD);
bmsStatus_t cellBalancingInit(thresholdData_t *tData, cellBalancingInput_t *cbInput, ExtU_CellBalancing_T *cbInputMBD, dataPipelineInput_t *dpInput);
bmsStatus_t contactorInit(thresholdData_t *tData, contactorInput_t *cInput, ExtU_Contactors_T *cInputMBD);
bmsStatus_t protectionInit(thresholdData_t *tData, dataPipelineInput_t *dpInput, dataPipelineOutput_t  *dpOutput, protectionInput_t *pInput, ExtU_Protection_T *pInputMBD);

#ifdef EKF
bmsStatus_t dataPipelineInput(bmsMainData_t *mdata, dataPipelineInput_t *dpInput, ccEKFOutput_t *ccEKFOutput, ExtU_DataPipeline_T *dpInputMBD);
#else
bmsStatus_t dataPipelineInput(bmsMainData_t *mdata, dataPipelineInput_t *dpInput, coulombCountingOutput_t *ccOutput, ExtU_DataPipeline_T *dpInputMBD);
#endif
bmsStatus_t dataPipelineOutput (bmsMainData_t *mdata, dataPipelineOutput_t  *dpOutput, ExtY_DataPipeline_T *dpOutputMBD);

bmsStatus_t cellBalancingInput (bmsMainData_t *mdata, dataPipelineOutput_t  *dpOutput, cellBalancingInput_t *cbInput, ExtU_CellBalancing_T *cbInputMBD);
bmsStatus_t cellBalancingOutput(bmsMainData_t *mdata, cellBalancingOutput_t *cbOutput, ExtY_CellBalancing_T *cbOutputMBD);

bmsStatus_t protectionInput (bmsMainData_t *mdata, dataPipelineOutput_t  *dpOutput, protectionInput_t *pInput, ExtU_Protection_T *pInputMBD);
bmsStatus_t protectionOutput (bmsMainData_t *mdata, protectionOutput_t *pOutput, ExtY_Protection_T *pOutputMBD);

bmsStatus_t contactorInput(bmsMainData_t *mdata, dataPipelineOutput_t *dpOutput, protectionOutput_t *pOutput, contactorInput_t *cInput, contactorOutput_t *cOutput, ExtU_Contactors_T *cInputMBD, stateMachineOutput_t *smOutput);
bmsStatus_t contactorOutput (bmsMainData_t *mdata, contactorOutput_t *cOutput, ExtY_Contactors_T *cOutputMBD);

#ifdef EKF
bmsStatus_t coulombCountingEKFInit(ccEKFInput_t *ccEKFInput, ExtU_EKF_T *ccEKFInputMBD);
bmsStatus_t coulombCountingEKFInput(bmsMainData_t *mdata, dataPipelineOutput_t *dpOutput, protectionOutput_t *pOutput, ccEKFInput_t *ccEKFInput, ExtU_EKF_T *ccEKFInputMBD);
bmsStatus_t coulombCountingEKFOutput(bmsMainData_t *mdata, ccEKFOutput_t *ccEKFOutput, ExtY_EKF_T *ccEKFOutputMBD);
#else
bmsStatus_t coulombCountingInit(coulombCountingInput_t *ccInput, ExtU_SOCEstimation_T *ccInputMBD, coulombCountingOutput_t *ccOutputData);
bmsStatus_t coulombCountingInput(bmsMainData_t *mdata, dataPipelineOutput_t *dpOutput, chargingOutputData_t *chargerOutput, protectionOutput_t *pOutput, coulombCountingInput_t *ccInput, ExtU_SOCEstimation_T *ccInputMBD);
bmsStatus_t coulombCountingOutput(bmsMainData_t *mdata, coulombCountingOutput_t *ccOutput, coulombCountingOutput_t *ccOutputEEPROM, coulombCountingOutput_t *ccOutputFLASH, ExtY_SOCEstimation_T *ccOutputMBD);
#endif


bmsStatus_t stateMachineInit(thresholdData_t *tData, dataPipelineInput_t *dpInput, ExtU_HighLevelStatemachine_T *smInputMBD);
bmsStatus_t stateMachineInput(dataPipelineOutput_t *dpOutput, protectionOutput_t *pOutput, contactorOutput_t *cOutput, ExtU_HighLevelStatemachine_T *smInputMBD);
bmsStatus_t stateMachineOutput(stateMachineOutput_t *smOutput, ExtY_HighLevelStatemachine_T *smOutputMBD, bmsMainData_t *mdata);

bmsStatus_t chargingInit(thresholdData_t *tData, ExtU_ChargingAlgorithm_T *chargingInputMBD);
bmsStatus_t chargingInput(ExtU_DataPipeline_T *dpInputMBD, ExtY_DataPipeline_T *dpOutputMBD, ExtU_ChargingAlgorithm_T *chargingInputMBD);
bmsStatus_t chargingOutput(chargingOutputData_t *chargerOutput, ExtY_ChargingAlgorithm_T *chargingOutputMBD);

void writeSoCToFlash(coulombCountingOutput_t *ccOutput, coulombCountingOutput_t *ccOutputEEPROM, coulombCountingOutput_t *ccOutputFlash);
void shuntdownRoutine(void);

float calculateChargingCurrent(dataPipelineOutput_t *dpOutput, coulombCountingOutput_t *ccOutput, chargingOutputData_t *chargerOutput, thresholdData_t *tData);
bmsStatus_t calculateDynamicCurrentLimits(bmsMainData_t *mdata, dataPipelineInput_t *dpInput, dataPipelineOutput_t *dpOutput, DynamicLimitsTable_t *ldata, ExtY_DataPipeline_T *dpOutputMBD);

#ifdef DEBUG_CAN
    #ifdef EKF
        bmsStatus_t sendCoulombCountingEKFDataOnCAN(ccEKFInput_t *ccEKFInput, ccEKFOutput_t *ccEKFOutput);
    #else
        bmsStatus_t sendCoulombCountingDataOnCAN(coulombCountingInput_t *ccInput, coulombCountingOutput_t *ccOutput);
    #endif
bmsStatus_t sendContactorDataOnCAN(thresholdData_t *tData, contactorInput_t *cInput, contactorOutput_t *cOutput);
bmsStatus_t sendCellBalancingDataOnCAN(thresholdData_t *tData, cellBalancingInput_t *cbInput, cellBalancingOutput_t *cbOutput);
bmsStatus_t sendProtectionDataOnCAN(thresholdData_t *tData, protectionInput_t *pInput, protectionOutput_t *pOutput, coulombCountingOutput_t *ccOutput);
bmsStatus_t sendBMSDataOnCAN(bmsMainData_t *mdata, dataPipelineOutput_t *dpOutput, dataPipelineInput_t *dpInput);
float SoCManipulation(bmsMainData_t *mdata, ExtY_SOCEstimation_T *ccOutputMBD);
uint8_t fetchMarvelState(bmsMainData_t *mdata,  contactorOutput_t *cOutput,  contactorInput_t *cInput);
uint8_t publishErrorCodes(contactorOutput_t *cOutput, protectionOutput_t *pOutput);
float activatePseudoLatching(coulombCountingOutput_t *ccOutput, coulombCountingInput_t *ccInput, float requestedCurrent);
void transmissionNotify(TX_QUEUE *queue_ptr);
bmsStatus_t publishMarvelUniqueID(void);
bmsStatus_t publishMarvelGitSha(void);

bmsStatus_t fetchSoCFromLookupTable(dataPipelineOutput_t *dpOutput);
float advancedLatchingRoutine(coulombCountingInput_t *ccInput, coulombCountingOutput_t *ccOutput);
uint8_t findMaxAllowedImbalance(uint8_t soc, chargingOutputData_t *chargerOutput);


#endif


#endif
/* End of File ---------------------------------------------------------------*/

