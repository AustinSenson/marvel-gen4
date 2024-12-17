
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

char serverConfig[2048];

// Firmware Upgrade Structures
upgradeType uptype;
firmwareUpgradeState upgradeState;
firmwareUpgradeState previousUpgradeState;
volatile flashWriteProgress writeProgress;

// Shims Structures
IsoTpShims firmwareUpgradeReceiveShim;
IsoTpReceiveHandle firmwareUpgradeReceiveHandle;
IsoTpMessage firmwareUpgradeReceivedMessage;

FlashWriteState bufferState;

extern ULONG actualFlag;
extern TX_EVENT_FLAGS_GROUP systemEventFlag;
extern uint16_t otaTransmitter;
extern bool appWriteInProgress;
extern bool sopPageErased;


uint32_t appAddress = 0;
uint32_t sopWriteAddress = 0;
uint16_t currentFrame = 0;
uint8_t csvUnderParsing = 0;
uint8_t binaryUnderParsing = 0;
uint8_t lastDataPacket = 0;
size_t csvOffset = 0;

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
    size_t rawConfigLength             = 0;
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
        if (tx_event_flags_get(&systemEventFlag, STARK_CAN_TP_RX | PAUSE_OTA | OTA_TX_COMPLETE | COMPLETE_FOTA | COMPLETE_COTA | FAILED_OTA, TX_OR, &actualFlag, RECEIVE_TIMEOUT) == TX_SUCCESS)
        {
            if (tx_event_flags_get(&systemEventFlag, FAILED_OTA, TX_OR_CLEAR, &actualFlag, TX_NO_WAIT) == TX_SUCCESS)
            {
                previousUpgradeState = upgradeState;
                upgradeState = UPGRADE_FAILED;
                return BMS_OK;
            }

            if (tx_event_flags_get(&systemEventFlag, COMPLETE_FOTA | COMPLETE_COTA | OTA_TX_COMPLETE, TX_OR, &actualFlag, TX_NO_WAIT) == TX_SUCCESS)
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

            if(tx_event_flags_get(&systemEventFlag, STARK_CAN_TP_RX, TX_OR_CLEAR, &actualFlag, TX_NO_WAIT) == TX_SUCCESS)
            {
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
                    memcpy(messageBuffer, firmwareUpgradeReceivedMessage.payload + 9, firmwareUpgradeReceivedMessage.size - 9);
                    memcpy(&offset, firmwareUpgradeReceivedMessage.payload + 5, 4);
                    rawConfigLength = firmwareUpgradeReceivedMessage.size - 9;

                    if(receivedCRC == crc32(firmwareUpgradeReceivedMessage.payload+ 9, firmwareUpgradeReceivedMessage.size - 9))
                    {
                        lastDataPacket = (firmwareUpgradeReceivedMessage.size < 512) ? 1 : 0;        //TODO revisit this
                        parseJSON_chunks((const char *)messageBuffer, rawConfigLength, serverConfig, SOP_DATA_ADDRESS, flashWrite, &binaryUnderParsing, &csvUnderParsing, &offset);

                        response[3] = SUCCESS_MESS;
                        status = sendCAN(communicationCAN, S, FIRMWARE_UPGRADE_COMM, response, 5);
                    }
                    else
                    {
                        response[3] = CRC_ERROR;
                        status = sendCAN(communicationCAN, S, FIRMWARE_UPGRADE_COMM, response, 5);
                        return status;
                    }

                    break;

                case FOTA_MESSAGE:
                    appWriteInProgress = true;
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

                    for(int data = 0; data < (messageLength / 8); data++)
                    {
                        doubleWord = (uint64_t*)&messageBuffer[data * 8];
                        flashDoubleWords[data] = *doubleWord;
                    }

                    address = appAddress + offset;
                    // status = flashWriteOTA(address, flashDoubleWords, (messageLength / 8));
                    status = flashWrite(address, flashDoubleWords, (messageLength / 8));
                    currentFrame++;
                    writeProgress.offset = offset;
                    response[3] = SUCCESS_MESS;
                    status = sendCAN(communicationCAN, S, FIRMWARE_UPGRADE_COMM, response, 5);

                    break;

                default:
                    break;
                }
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
    bmsStatus_t status = BMS_ERROR;
    ULONG actualFlag;
    response[1] = otaTransmitter;
    appWriteInProgress = false;

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
        else 
        {
            upgradeState = UPGRADE_FAILED;
        }
    }
}

bmsStatus_t upgradeFirmware(void)
{
    ULONG actualFlag;
    bmsStatus_t status = BMS_OK;

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


                tx_event_flags_set(&systemEventFlag, ~STARK_CAN_TP_RX, TX_AND);
                tx_event_flags_set(&systemEventFlag, ~START_OTA, TX_AND);
                tx_event_flags_set(&systemEventFlag, ~COMPLETE_FOTA, TX_AND);
                tx_event_flags_set(&systemEventFlag, ~PAUSE_OTA, TX_AND);
                tx_event_flags_set(&systemEventFlag, ~RESUME_OTA, TX_AND);
                tx_event_flags_set(&systemEventFlag, ~FAILED_OTA, TX_AND);
                tx_event_flags_set(&systemEventFlag, ~COMPLETE_COTA, TX_AND);
                status = BMS_ERROR;

                return status;
                break;

            default:
                // nothing to be done here
                break;
            }
            tx_thread_sleep(0x20);
        }
    }
    return status;
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

void parseJSON_chunks(const char *jsonChunk, size_t chunkLength, char *csvData, 
                      uint32_t startAddress, bmsStatus_t (*flashWrite)(uint32_t, __IO uint64_t *, uint16_t),
                      uint8_t *binaryUnderParsing, uint8_t *csvUnderParsing, size_t *csvOffset)
{
    // appWriteInProgress          = false;
    static const char *csvKey   = "\\\"csv_data\\\":";
    const char *csvStart        = NULL;
    static size_t csvLength     = 0;
    size_t binary_length_base64 = 0;
    static uint8_t binaryChunkCount    = 0;

    // Parse CSV Data (if not already parsing it)
    if (!*binaryUnderParsing && !*csvUnderParsing) 
    {
        csvStart = strstr(jsonChunk, csvKey);
        if (csvStart) 
        {
            csvStart += strlen(csvKey);

        // Skip leading escape sequences and quotes
        if(*csvStart == '\\' && *(csvStart + 1) == '"') 
        {
            csvStart += 2; // Skip both '\' and '"'
        }
            *csvUnderParsing = 1; // CSV parsing has started
        }
    }

    // If parsing CSV, accumulate data                                                                                                           
    if (*csvUnderParsing) 
    {
        if (!csvStart) 
        {
            csvStart = jsonChunk; // Continue from current packet
        }

        // Check if this chunk contains the end of the CSV data
        const char *csvEnd = strchr(csvStart, '"');
        if(csvEnd) 
        {
            // Copy the remaining part of the CSV data and stop parsing CSV
            csvLength = csvEnd - csvStart;
            strncpy(&csvData[*csvOffset], csvStart, csvLength);
            *csvOffset += csvLength;
            csvData[*csvOffset] = '\0';    // Null-terminate CSV
            *csvUnderParsing = 0;          // Done parsing CSV
        } 
        else 
        {
            // No end found, accumulate the entire chunk
            csvLength = chunkLength - (csvStart - jsonChunk);
            strncpy(&csvData[*csvOffset], csvStart, csvLength);
            *csvOffset += csvLength;
        }
    }

    // Parse Binary Data (continue as before)
    const char *binaryKey = "\\\"binary_data\\\":\\\"";
    const char *binaryStart = strstr(jsonChunk, binaryKey);

    if (binaryStart || *binaryUnderParsing) // Start execution only when binary section parsing has started
    {
        if (!*binaryUnderParsing) 
        {
            binaryStart += strlen(binaryKey);

            // Skip leading escape sequences and quotes
            while (*binaryStart == '\\' && *(binaryStart + 1) == '"') 
            {
                binaryStart += 2;    // Skip both '\' and '"'
            }

            *binaryUnderParsing = 1; // Binary parsing has started
        } 
        else 
        {
            binaryStart = jsonChunk; // Continue parsing from the current packet
        }

        binary_length_base64 = chunkLength - (binaryStart - jsonChunk);

        // if the second chunk after csv completion, subtract the csv length from the offset, else ignore
        if(!binaryChunkCount)       
        {
            sopWriteAddress = startAddress;
        }
        
        base64DecodeStream(binaryStart, binary_length_base64, flashWrite, &bufferState, lastDataPacket, &sopWriteAddress);
        binaryChunkCount++;
    }
}

bmsStatus_t base64DecodeStream(const char *input, size_t inputLength, 
                            bmsStatus_t (*flashWrite)(uint32_t, __IO uint64_t *, uint16_t),
                            FlashWriteState *state, 
                            uint8_t lastDataPacket, uint32_t *address)
{
    bmsStatus_t status = BMS_ERROR;
    static bool bufferNotEmpty = false; 
    static bool handleFinalWrite = false; 
    const char *base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    if(!bufferNotEmpty)
    {
        state->bufferIndex = 0; // Reset buffer index
    }

    for (size_t i = 0; i < inputLength;) 
    {
        if (i >= inputLength) break;
        uint32_t sextet_a = (input[i] == '=') ? 0 : strchr(base64_chars, input[i]) - base64_chars;
        i++;

        if (i >= inputLength) break;
        uint32_t sextet_b = (input[i] == '=') ? 0 : strchr(base64_chars, input[i]) - base64_chars;
        i++;

        if (i >= inputLength) break;
        uint32_t sextet_c = (input[i] == '=') ? 0 : strchr(base64_chars, input[i]) - base64_chars;
        i++;

        if (i >= inputLength) break;
        uint32_t sextet_d = (input[i] == '=') ? 0 : strchr(base64_chars, input[i]) - base64_chars;
        i++;

        // Process the sextets into a triple 
        uint32_t triple = (sextet_a << 18) + (sextet_b << 12) + (sextet_c << 6) + sextet_d;


        // Decode bytes into the state buffer
        state->buffer[state->bufferIndex++] = (triple >> 16) & 0xFF;
        state->buffer[state->bufferIndex++] = (triple >> 8) & 0xFF;
        state->buffer[state->bufferIndex++] = triple & 0xFF;

        // Write to flash if buffer is full

        /*
            should use LCM of 3 and 8
            buffer length should be 24
        
        */

        if (state->bufferIndex == BUFFER_SIZE) 
        {
            if(flashWrite)
            {
                status = flashWrite(*address, (__IO uint64_t *)state->buffer, 3);   //add retry and error handling here
                CHECK_STATUS(status);

                appWriteInProgress = true;
                state->bufferIndex = 0; // Reset buffer index
                *address += BUFFER_SIZE;

                handleFinalWrite = lastDataPacket ? true : false;
            }
        }
    }

    bufferNotEmpty = state->bufferIndex > 0 ? true : false;

    // Handle the last chunk
    if (handleFinalWrite && state->bufferIndex > 0) 
    {
        //TODO memset all the other values to 0xFF?

        if (flashWrite) 
        {
            status = flashWrite(*address, (__IO uint64_t *)state->buffer, 3);   
        }
        state->bufferIndex = 0; // Reset buffer index
    }

    return status;
}



