#ifndef __FIRMWARE_UPGRADE_APP_H__
#define __FIRMWARE_UPGRADE_APP_H__

#include <stdint.h>
#include "stdbool.h"
#include <string.h>
#include <stdlib.h>

#define BIT0  (1 << 0)
#define BIT1  (1 << 1)
#define BIT2  (1 << 2)
#define BIT3  (1 << 3)
#define BIT4  (1 << 4)
#define BIT5  (1 << 5)
#define BIT6  (1 << 6)
#define BIT7  (1 << 7)
#define BIT8  (1 << 8)
#define BIT9  (1 << 9)
#define BIT10 (1 << 10)
#define BIT11 (1 << 11)
#define BIT12 (1 << 12)
#define BIT13 (1 << 13)
#define BIT14 (1 << 14)
#define BIT15 (1 << 15)
#define BIT16 (1 << 16)
#define BIT17 (1 << 17)
#define BIT18 (1 << 18)
#define BIT19 (1 << 19)
#define BIT20 (1 << 20)
#define BIT21 (1 << 21)
#define BIT22 (1 << 22)
#define BIT23 (1 << 23)
#define BIT24 (1 << 24)
#define BIT25 (1 << 25)
#define BIT26 (1 << 26)
#define BIT27 (1 << 27)
#define BIT28 (1 << 28)
#define BIT29 (1 << 29)
#define BIT30 (1 << 30)
#define BIT31 (1 << 31)


#define STARK_ID    0
#define MARK_ID     1 
#define MARVEL_ID   2
#define LAPTOP_ID   9
#define XAVIER_ID   4

#define MAX_RETRY           10
#define OTA_SLEEP_TIMEOUT   20

#define tx_flash_writeProgress  0x6FB
#define FIRMWARE_UPGRADE_COMM   0x6FA
#define rx_CAN_TP_Stark         0x1FE
#define FUPFG_REQUEST           1
#define FUPG_STATUS             0  

#define RECEIVE_TIMEOUT         0x1000

#define DATA_PACKET_SIZE        512
#define NUM_TIMEOUTS            15



#define STARK_CAN_TP_RX     BIT0
#define START_OTA 			BIT1
#define COMPLETE_FOTA       BIT2
#define PAUSE_OTA           BIT3
#define RESUME_OTA          BIT4
#define COMPLETE_COTA       BIT13
#define FAILED_OTA          BIT10
#define OTA_TX_COMPLETE     BIT15

#define BUFFER_SIZE         24      // LCM of 8 and 3 


#define NA                                1 //not applicable
#define BASE_10                           10

typedef enum {
    FAILED = 0,
    SUCCESS_MESS,
    CAN_TP_TIMEOUT,
    FILE_CREATE_ERROR,
    CRC_ERROR,
    FILE_WRITE_ERROR,
    TRANSFER_COMPLETE,
    OTA_ALREADY_PROGRESS,
    VERIFICATION_FAILED,
    FILE_NOT_PRESENT,
    DOWNLOAD_FAIL,
} status_reponse_codes;

typedef enum {
    TRANSFERED_FAILED = 0,
    DOWNLOAD_BIN,
    RECEIVE_BIN,
    DOWNLOAD_CONF,
    RECEIVE_CONF,
    PERFORM_UPGRADE,
    PAUSE_UPGRADE,
    RESUME_UPGRADE,
    DOWNLOAD_CERTS,
    RECEIVE_CERTS,
    UPDATE_CONFIG,
    RECEIVE_MANIFEST
} request_codes;


typedef enum {
    UPGRADE_INIT = (uint8_t)0,
    UPGRADE_RECEIVE_FILE_SIZE = (uint8_t)1, 
    UPGRADE_RECEIVE_DATA = (uint8_t)2,
    UPGRADE_PAUSE = (uint8_t)3,
    UPGRADE_RESUME = (uint8_t)4,
    UPGRADE_COMPLETE =  (uint8_t)5,
    UPGRADE_FAILED = (uint8_t)6,
} firmwareUpgradeState;

typedef struct {
    uint8_t buffer[BUFFER_SIZE];    // Buffer for partial 8-byte block
    size_t bufferIndex;    // Current index in the buffer
} FlashWriteState;

typedef enum {
    COTA_MESSAGE = (uint8_t) 1,
    FOTA_MESSAGE = (uint8_t) 0,
} FILE_TYPES;

typedef enum {
    COTA = (uint8_t) 1,
    FOTA = (uint8_t) 0,
} upgradeType;

typedef struct __attribute__((packed)){
    uint8_t device_id;
    uint32_t offset;
} flashWriteProgress;

extern upgradeType uptype;

void handleUpgradeComplete();
bmsStatus_t upgradeFirmware(void);
void handleUpgradeComplete(void);
bmsStatus_t updateMarvelConfiguration(uint64_t *configData);
bmsStatus_t handlePauseUpgrade(void);
bmsStatus_t handleReceiveData(void);
bmsStatus_t handleUpgradeInit(void);
uint8_t dataParserCOTA(uint64_t *configData, char * rawConfigData);
bool parseStringFromServer(char *rest, uint64_t * data, uint32_t len);
bmsStatus_t base64DecodeStream(const char *input, size_t inputLength, 
                            bmsStatus_t (*flashWrite)(uint32_t, __IO uint64_t *, uint16_t),
                            FlashWriteState *state, uint8_t lastDataPacket, uint32_t *address);
void parseJSON_chunks(const char *jsonChunk, size_t chunkLength, char *csvData, 
                      uint32_t startAddress, bmsStatus_t (*flashWrite)(uint32_t, __IO uint64_t *, uint16_t),
                      uint8_t *binaryUnderParsing, uint8_t *csvUnderParsing, size_t *csvOffset);

char *strtok_r(char * __restrict__ _Str, const char * __restrict__ _Delim, char ** __restrict__ __last);

#endif