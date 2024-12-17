/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "threadxMain.h"
#include "bmsConfiguration.h"
#include "onBoardEEPROM.h"
#include "onBoardMemory.h"
#include "bmsCANMatrix.h"
#include "canControls.h"
/* Self include --------------------------------------------------------------*/
#include "pvd.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief MCU voltage detection
 * @retval bms Status
 */
bmsStatus_t initializePVD(void)
{
    bmsStatus_t   status = BMS_ERROR;
    PWR_PVDTypeDef sConfigPVD;

    /* Peripheral interrupt init */
    /* PVD_PVM_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PVD_PVM_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(PVD_PVM_IRQn);

    /** PVD Configuration*/
    sConfigPVD.PVDLevel = PWR_CR2_PLS_LEV6;
    sConfigPVD.Mode = PWR_PVD_MODE_IT_RISING_FALLING;
    HAL_PWR_ConfigPVD(&sConfigPVD);

    /** Enable the PVD Output*/
    HAL_PWR_EnablePVD();

    status = BMS_OK;
    return status;
}

/* Function definition -------------------------------------------------------*/

void HAL_PWR_PVDCallback(void)
{
   uint8_t tempBuff[8] = {};
   // write the exception to EEPROM and can also notify server || publish on CAN

    tempBuff[0] = MARVEL_DEVICE_ID;
    tempBuff[1] = HW_MAJOR_VERSION;
    tempBuff[2] = HW_MINOR_VERSION;
    tempBuff[3] = 0;
    tempBuff[4] = 0;
    tempBuff[5] = 0;
    tempBuff[6] = 0;
    tempBuff[7] = 1;

    sendCAN(communicationCAN, STANDARD_CAN_ID, CAN_DEBUG_ID_042, tempBuff, 8);
}

/* End of File ---------------------------------------------------------------*/