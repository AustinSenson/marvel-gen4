
/* Standard C includes -------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "stdint.h"
#include "stdint.h"

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "can_tp_app.h"
#include "threadxMain.h"
#include "canControls.h"
#include "memoryMap.h"
#include "ota.h"
#include "flashControls.h"
#include "onBoardFlash.h"
#include "bmsConfiguration.h"
#include "stm32l4xx_it.h"

/* Self include --------------------------------------------------------------*/
#include "firmwareUpgradeApp.h"


/* Private variables ---------------------------------------------------------*/

uint8_t serverConfig[2048];

// Firmware Upgrade Structures
upgradeType uptype;
firmwareUpgradeState upgradeState;
firmwareUpgradeState previousUpgradeState;
volatile flashWriteProgress writeProgress;

// Shims Structures
IsoTpShims firmwareUpgradeReceiveShim;
IsoTpReceiveHandle firmwareUpgradeReceiveHandle;
IsoTpMessage firmwareUpgradeReceivedMessage;

extern ULONG actualFlag;
extern TX_EVENT_FLAGS_GROUP systemEventFlag;
extern uint16_t otaTransmitter;

uint8_t targetID = STARK_ID;
uint32_t appAddress = 0;
uint16_t currentFrame = 0;

uint8_t response[5] = {MARVEL_ID, STARK_ID, FUPG_STATUS, 1, MARVEL_ID};

uint32_t crc32_for_byte(uint32_t r)
{
	for (int j = 0; j < 8; ++j) r = (r & 1 ? 0 : (uint32_t)0xEDB88320L) ^ r >> 1;
	return r ^ (uint32_t)0xFF000000L;
}


uint32_t crc32(const void *data, uint32_t n_bytes)
{
	uint32_t crc = 0;
	static uint32_t table[0x100];
	if (!*table)
		for (size_t i = 0; i < 0x100; ++i) table[i] = crc32_for_byte(i);
	for (size_t i = 0; i < n_bytes; ++i)
		crc = table[(uint8_t)crc ^ ((uint8_t *)data)[i]] ^ crc >> 8;

	return crc;
}

bmsStatus_t handleUpgradeInit(void)
{
    bmsStatus_t status = BMS_ERROR;
    response[1] = otaTransmitter;


    if (uptype == FOTA)
    {
        #ifdef APP1
                appAddress =  (uint32_t)&__app2_start__;
                status = clearApp2();
        #endif
        #ifdef APP2
                appAddress =  (uint32_t)&__app1_start__;
                status = clearApp1();
        #endif

        if(status != BMS_OK)
        {
            // changing state to upgrade failed
            upgradeState = UPGRADE_FAILED;
            response[3] = FILE_CREATE_ERROR;
            status = sendCAN(communicationCAN, S, FIRMWARE_UPGRADE_COMM, response, 5);

            return status;
        }
    }

    // changing state to start receiving binaries upon successful clearing of APP
    response[3] = SUCCESS_MESS;
    status = sendCAN(communicationCAN, S, FIRMWARE_UPGRADE_COMM, response, 5);
    upgradeState = UPGRADE_RECEIVE_DATA;

    return status;
}

bmsStatus_t handleReceiveData(void)
{
    bmsStatus_t status = BMS_ERROR;
    response[1] = otaTransmitter;
    ULONG actualFlag;

    static uint8_t messageBuffer[521]  = {0};
    uint64_t flashDoubleWords[32]      = {0};
    static uint16_t messageLength      = 0;
    static unsigned int receivedCRC    = 0;
    static unsigned int offset         = 0;
    static unsigned int calculatedCRC  = 0;
    uint32_t address                   = 0;
    uint8_t messageType                = 0;
    uint8_t retry                      = 0;
    uint64_t* doubleWord              = NULL;

    while (1)
    {
        if(tx_event_flags_get(&systemEventFlag, STARK_CAN_TP_RX | PAUSE_OTA | COMPLETE_FOTA | COMPLETE_COTA | FAILED_OTA, TX_OR, &actualFlag, RECEIVE_TIMEOUT) == TX_SUCCESS)
        {
            if(tx_event_flags_get(&systemEventFlag, FAILED_OTA, TX_OR_CLEAR, &actualFlag, TX_NO_WAIT) == TX_SUCCESS)
            {
                previousUpgradeState = upgradeState;
                upgradeState = UPGRADE_FAILED;
                return BMS_OK;
            }

            if(tx_event_flags_get(&systemEventFlag, COMPLETE_FOTA, TX_OR, &actualFlag, TX_NO_WAIT) == TX_SUCCESS)
            {
                upgradeState = UPGRADE_COMPLETE;
                return BMS_OK;
            }
            if(tx_event_flags_get(&systemEventFlag, COMPLETE_COTA, TX_OR, &actualFlag, TX_NO_WAIT) == TX_SUCCESS)
            {
                upgradeState = UPGRADE_COMPLETE;
                return BMS_OK;
            }

            if(tx_event_flags_get(&systemEventFlag, PAUSE_OTA, TX_OR_CLEAR, &actualFlag, TX_NO_WAIT) == TX_SUCCESS)
            {
                tx_event_flags_get(&systemEventFlag, STARK_CAN_TP_RX, TX_OR_CLEAR, &actualFlag, TX_NO_WAIT);
                tx_event_flags_get(&systemEventFlag, RESUME_OTA, TX_OR_CLEAR, &actualFlag, TX_NO_WAIT);         // redundant
                previousUpgradeState = upgradeState;
                upgradeState = UPGRADE_PAUSE;
                return BMS_OK;
            }            

            tx_event_flags_get(&systemEventFlag, STARK_CAN_TP_RX, TX_OR_CLEAR, &actualFlag, TX_NO_WAIT);
            memcpy(&messageType, firmwareUpgradeReceivedMessage.payload, 1);

            switch (messageType)
            {
                /* 
                    payload message Type  -> 1 byte, index 0
                    payload crc           -> 4 bytes from index 1
                    payload offset        -> 4 bytes from index 5    
                */

                case COTA_MESSAGE:
                    memcpy(&receivedCRC, firmwareUpgradeReceivedMessage.payload + 1, CRC_LENGTH_OTA); 
                    memcpy(&offset, firmwareUpgradeReceivedMessage.payload + 5, 4);
                    
                    if(receivedCRC == crc32(firmwareUpgradeReceivedMessage.payload+ 9,firmwareUpgradeReceivedMessage.size - 9))
                    {
                        memcpy(serverConfig+offset,firmwareUpgradeReceivedMessage.payload + 9,firmwareUpgradeReceivedMessage.size - 9);
                    }
                    else
                    {
                        response[3] = CRC_ERROR;
                        status = sendCAN(communicationCAN, S, FIRMWARE_UPGRADE_COMM, response, 5);
                        return status;
                    }

                    break;
                
                case FOTA_MESSAGE:
                    memcpy(messageBuffer, firmwareUpgradeReceivedMessage.payload + 9, firmwareUpgradeReceivedMessage.size - 9);
                    memcpy(&receivedCRC, firmwareUpgradeReceivedMessage.payload + 1, CRC_LENGTH_OTA); 
                    memcpy(&offset, firmwareUpgradeReceivedMessage.payload + 5, 4);

                    messageLength = firmwareUpgradeReceivedMessage.size - 9;
                    calculatedCRC = crc32(messageBuffer, messageLength);

                    if(calculatedCRC != receivedCRC)
                    {
                        response[3] = CRC_ERROR;
                        status = sendCAN(communicationCAN, S, FIRMWARE_UPGRADE_COMM, response, 5);
                        return status;
                    }

                    for(int data = 0; data < messageLength/8; data++)
                    {
                        doubleWord = (uint64_t*)&messageBuffer[data*8];
                        flashDoubleWords[data] = *doubleWord;
                    }

                    address = appAddress + offset;                                 
                    status = flashWriteOTA(address, flashDoubleWords, messageLength/8);
                    currentFrame++;
                    writeProgress.offset = offset;
                    response[3] = SUCCESS_MESS;
                    status = sendCAN(communicationCAN, S, FIRMWARE_UPGRADE_COMM, response, 5);
                    
                    break;
                
                default:
                    break;
            }
        }
        else
        {
            retry++;
            response[3] = CAN_TP_TIMEOUT;
            status = sendCAN(communicationCAN, S, FIRMWARE_UPGRADE_COMM, response, 5);
             

            if(retry > MAX_RETRY) 
            {   
                upgradeState = UPGRADE_FAILED; 
            }

        }
    }
    return status;  
}

bmsStatus_t handlePauseUpgrade(void)
{
    bmsStatus_t status = BMS_ERROR;
    ULONG actualFlag;

    if (tx_event_flags_get(&systemEventFlag, RESUME_OTA, TX_OR_CLEAR, &actualFlag, TX_WAIT_FOREVER) == TX_SUCCESS)
    {
        tx_event_flags_set(&systemEventFlag, ~PAUSE_OTA, TX_AND);               // clearing PAUSE_OTA flag
        response[3] = RESUME_UPGRADE;
        status = sendCAN(communicationCAN, S, FIRMWARE_UPGRADE_COMM, response, 5);
        upgradeState = previousUpgradeState;
    }
    return status;
}

void handleUpgradeComplete(void)
{
    ULONG actualFlag;
    response[1] = otaTransmitter;
    bmsStatus_t status = BMS_ERROR;

    if(tx_event_flags_get(&systemEventFlag, COMPLETE_FOTA, TX_OR_CLEAR, &actualFlag, TX_NO_WAIT) == TX_SUCCESS)
    {
        switchPartitionAndReset();
    }

    if(tx_event_flags_get(&systemEventFlag, COMPLETE_COTA, TX_OR_CLEAR, &actualFlag, TX_NO_WAIT) == TX_SUCCESS)
    {
        status = updateMarvelConfiguration((uint64_t *)&defaultFlashConfigData);
        if(status == BMS_OK)
        {
            systemReset();
        }
    }
}

void upgradeFirmware(void)
{
    ULONG actualFlag;
    if(tx_event_flags_get(&systemEventFlag, START_OTA, TX_OR_CLEAR, &actualFlag, TX_WAIT_FOREVER) == TX_SUCCESS)
    {
        upgradeState = UPGRADE_INIT;

        while (1)
        {
            switch (upgradeState)
            {
                case UPGRADE_INIT:
                    handleUpgradeInit();
                    break;

                case UPGRADE_RECEIVE_DATA:
                    handleReceiveData();
                    break;

                case UPGRADE_PAUSE:
                    handlePauseUpgrade();
                    break;

                case UPGRADE_COMPLETE:
                    handleUpgradeComplete();
                    break;

                case UPGRADE_FAILED:
                    // handle firmware Upgrade failed -> clearing all OTA flags followed by a system reset   

                    tx_event_flags_set(&systemEventFlag, ~STARK_CAN_TP_RX, TX_AND);
                    tx_event_flags_set(&systemEventFlag, ~START_OTA, TX_AND);
                    tx_event_flags_set(&systemEventFlag, ~COMPLETE_FOTA, TX_AND);
                    tx_event_flags_set(&systemEventFlag, ~PAUSE_OTA, TX_AND);
                    tx_event_flags_set(&systemEventFlag, ~RESUME_OTA, TX_AND);
                    tx_event_flags_set(&systemEventFlag, ~FAILED_OTA, TX_AND);

                    systemReset();
                    break;

                default:
                    // nothing to be done here
                    break;
            }
            tx_thread_sleep(0x20);
        }   
    }
}

bool parseStringFromServer(char *rawConfigData, uint64_t *data, uint32_t length)
{
    uint32_t index = 1;
    uint8_t tokenCount = 0;
    
    char *rest;
    char configTemp[2048] = {0};

    // Copy the raw config data to a temporary buffer
    strncpy(configTemp, rawConfigData, sizeof(configTemp) - 1);

    // Ensuring null termination
    configTemp[sizeof(configTemp) - 1] = '\0'; 

    // Start tokenizing the copied string
    char *token = strtok_r(configTemp, ",", &rest);

    // Config version
    data[CONFIG_VERSION] = atol(token);

    // Loop through each token and store it in the array
    while(token != NULL && index <= length)
    {
        token = strtok_r(NULL, ",", &rest);         // Get the next token
        data[index++] = strtoull(token, NULL, BASE_10);  // Convert the token to an integer and store it in the array
        tokenCount++;
    }

    return true;
}

uint8_t dataParserCOTA(uint64_t *configData, char * rawConfigData)
{
    bmsStatus_t status = BMS_ERROR;

    if (parseStringFromServer(rawConfigData, configData, FLASH_CONFIG_TOTAL_LENGTH - 1))
    {
        
        status = writeDataToFlash(FLASH_COTA_UPDATE, FLASH_CONFIG_ADDRESS, configData, FLASH_CONFIG_TOTAL_LENGTH, (uint8_t*)NA);       // have to see what to do with this received CRC
        return status;
    }
    else
    {
        status = BMS_COTA_UPDATE_ERROR;
        return status;
    }
}

bmsStatus_t updateMarvelConfiguration(uint64_t *configData)
{
    bmsStatus_t status = BMS_ERROR;

    status = dataParserCOTA(configData, ((char *)serverConfig));  

    return status;
}
