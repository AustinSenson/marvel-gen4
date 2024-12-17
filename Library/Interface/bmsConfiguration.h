/**
 * @file bmsConfiguration.h
 * @brief 
 * @version 0.1
 * @date 2022-04-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef BMSCONFIGURATION_H_
#define BMSCONFIGURATION_H_

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/**************** Configuration Selection ------------------------------------------------------------*/
#define THREADX
#define LTC2949

#if CANDEBUG == 1
    #define DEBUG_CAN
#endif
// #define ENABLE_LTC2949_SHORT_CIRCUIT

#define CAN_MATRIX_V_0_3            // 3WL5 CAN v0.3
// #define CAN_CHARGER_EXTENDED_ID
// #define SOC_IN_FLASH
#define CHARGING_CURRENT_FROM_MBD


// #define EEPROM_TEST
// #define FUNCTIONALTESTS

/*************************** HARDWARE VERSION SELECTION **************************/

#define HARDWARE_VERSION_3_1_1

/*************************** Debug ***************************/

// #define WWDOG
// #define EKF
#define LTC2949_DEBUG
// #define DEBUG

/**************** Configuration Selection End ------------------------------------------------------------*/


/**************** Battery Parameters Selection ------------------------------------------------------------*/
#define HILOAD_ONE_LFP
#define CELL_VOLTAGES_LFP
#define GF100
// #define ERROR_CODE_COUNT

/**************** Battery Parameters End ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define MARVEL_HW_VER                   32
#define MARVEL_FW_VER                   10                     //will be read with a scaling of 0.1                        

#ifdef GF100
    #define MARVEL_CONFIG                   4
#else
    #define MARVEL_CONFIG                   1
#endif

#define MARVEL_DEVICE_ID                0x02
#define SW_MAJOR_VERSION                0x00
#define SW_MINOR_VERSION                0x0A                    // as an integer value 
#define SW_PATCH_VERSION                0x0E

#define HW_MAJOR_VERSION                0x03
#define HW_MINOR_VERSION                0x02
#define HW_PATCH_VERSION                0x01

#define CONFIG_MAJOR_VERSION            4                       // this will denote the default config of this firmware version
#define CONFIG_MINOR_VERSION            0
#define CONFIG_PATCH_VERSION            0

#define MINIMUM_REQUIRED_CONFIG         4

#define EEPROM_MAGIC                    69
#define READ_FROM_FLASH                 1
#define LOOKUPTABLE_TEMPS               4
#define LOOKUPTABLE_NUMBERS_SIZE        101
#define DEBOUNCE_TIME_ERROR_CODES       5                    // loop time of 100mSec -> 1 sec debounce time for error codes

// tasks looptime average scaling for MBD duration count error correction

#ifdef CELL_VOLTAGES_LFP
    //cell voltage
    #define CELL_MAX_VOLTAGE_THRESHOLD_mV            3650                                                 //Volts
    #define CELL_MIN_VOLTAGE_THRESHOLD_mV            3100                                                 //Volts
    #define CELL_BALANCING_START_VOLTAGE_mV          3100                                                 //Volts   
    #define CELL_MAX_TEMP_THRESHOLD_C                45                                                   //degC
    #define CELL_MIN_TEMP_THRESHOLD_C                00                                                   //degC    
    #define PACK_NOMINAL_VOLTAGE                     57.6

#endif

#ifdef HILOAD_ONE_LFP

    /*
       AFE1 -> ADBMS2950
       AFE2 -> ADBMS6832 --- daisy chained arrangement
    */
    #define ADBMS6832_DAISYCHAIN_END          ADBMS6832_START_INDEX + NUMBER_OF_CMU                                          
    #define ADBMS2950_DAISYCHAIN_INDEX          1
    #define ADBMS6832_START_INDEX               1          
    #define NUMBER_OF_CMU                       3      
    #define CELL_IN_SERIES                      30                                                  //20S
    
    //cell per slave
    #define MAX_CELL_PER_SLAVE                  18                                                  //CMU-ADBMS6815
    #define CELL_IN_SLAVE_1                     10 
    #define CELL_IN_SLAVE_2                     10 
    #define CELL_IN_SLAVE_3                     0 
    #define CELL_IN_SLAVE_4                     0 
    #define CELL_IN_SLAVE_5                     0 
    #define CELL_IN_SLAVE_6                     0 
    #define CELL_IN_SLAVE_7                     0 
    #define CELL_IN_SLAVE_8                     0 
    #define CELL_IN_SLAVE_9                     0 
    #define CELL_IN_SLAVE_10                    0 
    #define CELL_IN_SLAVE_11                    0 
    #define CELL_IN_SLAVE_12                    0 

    #define CELL_IMBALANCE_THRESHOLD_mV         10                                                  //milliVolts

    //current
    #define MAX_CHARGE_CURRENT_THRESHOLD_A      180
    #define MAX_DISCHARGE_CURRENT_THRESHOLD_A   200

    //capacity
    #define PACK_MAX_CAPACITY_Ah                200  
    #define PACK_MIN_CAPACITY_Ah                1                                                 //Ah
    #define CURRENT_LIMIT_DISCHARGE             205                                                 //Amp

#endif

/* configure CAN matix V2 parameters*/
#ifdef CAN_MATRIX_V_0_3
    #define MAX_ALLOWED_CELL_VOLTAGE    ((CELL_MAX_VOLTAGE_THRESHOLD_mV))
    #define MAX_ALLOWED_PACK_VOLTAGE    (((CELL_IN_SERIES) * (CELL_MAX_VOLTAGE_THRESHOLD_mV)) * 0.001)  //Volts
    #define MAX_ALLOWED_CELL_TEMP       CELL_MAX_TEMP_THRESHOLD_C
    #define MAX_ALLOWED_CURRENT         CURRENT_LIMIT_DISCHARGE
#endif

#define MAX_PACK_VOLTAGE_THRESHOLD      ((CELL_MAX_VOLTAGE_THRESHOLD_mV * CELL_IN_SERIES) * 0.001)         //Volts
#define MIN_PACK_VOLTAGE_THRESHOLD      ((CELL_MIN_VOLTAGE_THRESHOLD_mV * CELL_IN_SERIES) * 0.001)         //Volts

#define MAX_NUM_CMU                     12                                                  //144S

//ntc
#define EXTERNAL_NTC_PER_SLAVE          4                                                   //CMU-ADBMS6832
#define INTERNAL_NTC_PER_SLAVE          6                                                   //CMU-ADBMS6832
#define NTC_BETA_VALUE_AT_25C           3988                                                //TDK Sensor

//shunt
#define SHUNT_RESISTOR_uOhm             167                                                 //micro-Ohm   //50mV/300A

//timeouts
#define TIME_100ms                      100
#define TIME_200ms                      200
#define ERROR_TIMEOUT_ms                500
#define WARNING_TIMEOUT_ms              500
#define RECOVERY_TIMEOUT_ms             500

//board temp
#define BOARD_MAX_TEMP_THRESHOLD_C      90                                                   //degC
#define BOARD_MIN_TEMP_THRESHOLD_C      00 

//protection threshold
#define MAX_ALLOWED_TEMP_CHANGE_C       50
#define MAX_ALLOWED_TEMP_GRADIENT_C     50
#define HIGH_IMBALANCE_ERROR_mV         250

//balancing
#define BALANCING_OFF_TIME_ms           500
#define BALANCING_ON_TIME_ms            100

//contactor
#define CONTACTOR_CUT_OFF_TIME_ms       100
#define PRE_CHARGE_CURRENT_mA           200
#define PRECHARGE_TIMEOUT_VALUE         1250
#define PRECHARGE_TIMEOUT_RETRY_mSec    2500
#define PRECHARGE_RETRY_LIMIT_COUNT     2
    
//cc
#define VEHICLE_REST_TIME_m             30                         
#define VEHICLE_REST_CURRENT_A          5

//data pipeline
#define PSEUDO_LATCH_CURRENT                     15   
#define REQUESTED_CURRENT_SCALING                0.6
#define REQUESTED_CURRENT_SCALING_FASTCHARGING   0.6

/*************** FLASH CONFIG DATA RANGE MAX MIN VALUE START  ***************************************************************/

#define NUMBER_OF_CMU_MIN_VALUE								1
#define NUMBER_OF_CMU_MAX_VALUE								12

#define CELL_IN_SERIES_MIN_VALUE							1
#define CELL_IN_SERIES_MAX_VALUE							144

//cell per slave
#define MAX_CELL_PER_SLAVE_MIN_VALUE						1
#define MAX_CELL_PER_SLAVE_MAX_VALUE						24

#define CELL_IN_SLAVE_MIN_VALUE								1
#define CELL_IN_SLAVE_MAX_VALUE								24

//ntc
#define EXTERNAL_NTC_PER_SLAVE_MIN_VALUE					1
#define EXTERNAL_NTC_PER_SLAVE_MAX_VALUE					12

#define INTERNAL_NTC_PER_SLAVE_MIN_VALUE					1
#define INTERNAL_NTC_PER_SLAVE_MAX_VALUE					12

#define NTC_BETA_VALUE_AT_25C_MIN_VALUE						1
#define NTC_BETA_VALUE_AT_25C_MAX_VALUE						10000

//shunt
#define SHUNT_RESISTOR_uOhm_MIN_VALUE						1
#define SHUNT_RESISTOR_uOhm_MAX_VALUE						10000

//cell voltage
#define CELL_MAX_VOLTAGE_THRESHOLD_mV_MIN_VALUE				1
#define CELL_MAX_VOLTAGE_THRESHOLD_mV_MAX_VALUE				7000
#define CELL_MIN_VOLTAGE_THRESHOLD_mV_MIN_VALUE				1
#define CELL_MIN_VOLTAGE_THRESHOLD_mV_MAX_VALUE				7000
#define CELL_BALANCING_START_VOLTAGE_mV_MIN_VALUE			1
#define CELL_BALANCING_START_VOLTAGE_mV_MAX_VALUE			7000
#define CELL_IMBALANCE_THRESHOLD_mV_MIN_VALUE				1
#define CELL_IMBALANCE_THRESHOLD_mV_MAX_VALUE				5000
#define CELL_CHARGING_VOLTAGE                               3.6     //Volts

//current
#define MAX_CHARGE_CURRENT_THRESHOLD_A_MIN_VALUE			1
#define MAX_CHARGE_CURRENT_THRESHOLD_A_MAX_VALUE			1000
#define MAX_DISCHARGE_CURRENT_THRESHOLD_A_MIN_VALUE			1
#define MAX_DISCHARGE_CURRENT_THRESHOLD_A_MAX_VALUE			1000
#define EFUSE_MAX_TRIGGER_TIMEOUT                           60

#define IR_WAIT_SOC                                         40
#define IR_START_SOC                                        50
#define IR_CYCLE_COUNT_THRESHOLD                            5
#define SLOW_CHARGING_MAX_CURRENT_A                         45
#define FAST_CHARGING_MAX_CURRENT_A                         180
#define CHARGE_CURRENT_DETECTION_THRESHOLD_mA               500
#define SLOW_CHARGING_TARGET_mV                             3420
#define FAST_CHARGING_TARGET_mV                             3550
#define CV_TRANSITION_mV                                    3380

//scaled x100
#define FAST_CHARGING_SCALING_FACTOR                        120
#define SLOW_CHARGING_CV_SCALING_FACTOR                     60
#define SLOW_CHARGING_CC_SCALING_FACTOR                     103

#define OCC_ERROR_CURRENT_A       185
#define OCC_WARNING_CURRENT_A     180
#define OCD_ERROR_CURRENT_A       185
#define OCD_WARNING_CURRENT_A     180

//capacity
#define PACK_MAX_CAPACITY_Ah_MIN_VALUE						1
#define PACK_MAX_CAPACITY_Ah_MAX_VALUE						1000
#define PACK_MIN_CAPACITY_Ah_MIN_VALUE						1
#define PACK_MIN_CAPACITY_Ah_MAX_VALUE						1000
#define EEPROM_CAP_WRITE                                    1000

//cell temp
#define CELL_MAX_TEMP_THRESHOLD_C_MIN_VALUE					1
#define CELL_MAX_TEMP_THRESHOLD_C_MAX_VALUE					100
#define CELL_MIN_TEMP_THRESHOLD_C_MIN_VALUE					1
#define CELL_MIN_TEMP_THRESHOLD_C_MAX_VALUE					100

//board temp
#define BOARD_MAX_TEMP_THRESHOLD_C_MIN_VALUE				1
#define BOARD_MAX_TEMP_THRESHOLD_C_MAX_VALUE				100
#define BOARD_MIN_TEMP_THRESHOLD_C_MIN_VALUE				1
#define BOARD_MIN_TEMP_THRESHOLD_C_MAX_VALUE				100

//timeout
#define ERROR_TIMEOUT_ms_MIN_VALUE							1
#define ERROR_TIMEOUT_ms_MAX_VALUE							10000
#define WARNING_TIMEOUT_ms_MIN_VALUE						1
#define WARNING_TIMEOUT_ms_MAX_VALUE						10000
#define RECOVERY_TIMEOUT_ms_MIN_VALUE						1
#define RECOVERY_TIMEOUT_ms_MAX_VALUE						10000

//protection threshold
#define MAX_ALLOWED_TEMP_CHANGE_C_MIN_VALUE					1
#define MAX_ALLOWED_TEMP_CHANGE_C_MAX_VALUE					100
#define MAX_ALLOWED_TEMP_GRADIENT_C_MIN_VALUE				1
#define MAX_ALLOWED_TEMP_GRADIENT_C_MAX_VALUE				100
#define HIGH_IMBALANCE_ERROR_mV_MIN_VALUE					1
#define HIGH_IMBALANCE_ERROR_mV_MAX_VALUE					1000

//balancing
#define BALANCING_OFF_TIME_ms_MIN_VALUE						1
#define BALANCING_OFF_TIME_ms_MAX_VALUE						10000
#define BALANCING_ON_TIME_ms_MIN_VALUE						1
#define BALANCING_ON_TIME_ms_MAX_VALUE						10000

//contactor
#define CONTACTOR_CUT_OFF_TIME_ms_MIN_VALUE					1
#define CONTACTOR_CUT_OFF_TIME_ms_MAX_VALUE					10000
#define PRE_CHARGE_CURRENT_mA_MIN_VALUE						1
#define PRE_CHARGE_CURRENT_mA_MAX_VALUE						10000   //10 A
	
//cc
#define VEHICLE_REST_TIME_m_MIN_VALUE						1
#define VEHICLE_REST_TIME_m_MAX_VALUE						300     //5 hours
#define VEHICLE_REST_CURRENT_A_MIN_VALUE					1
#define VEHICLE_REST_CURRENT_A_MAX_VALUE					20

//data pipeline
#define CHARGE_CURRENT_DETECTION_THRESHOLD_A_MIN_VALUE		1
#define CHARGE_CURRENT_DETECTION_THRESHOLD_A_MAX_VALUE		20

//ARC REACTOR
#define AR_CARTRAGE_TEMP                                    0
#define AR_COOLING_TEMP                                     0
#define AR_COOLING_TEMP_CHARGING                            0
#define AR_ENABLE                                           0
#define AR_HEATING_TEMP                                     0
#define AR_HEATING_TEMP_CHARGING                            0
#define AR_COOLING_TYPE                                     0

/*************** FLASH CONFIG DATA RANGE MAX MIN VALUE END  ***************************************************************/

//-- 
#define BLEEDING_CURRENT_mA                                 200
#define CC_TIMEOUT_mS                                       1000
#define DEBOUNCING_TIMEOUT_mS                               250       // Recalib timeout
#define SENSOR_ACCURACY                                     100       //percentage
#define SENSOR_OFFSET_mA                                    0
#define CAPACITY_DELTA_CHECK_mAh                            10   
#define EQUIVALENT_CYCLE_CAPACITY                           360000  
#define INSTALLED_CAPACITY                                  180000

/*************** RETRY VALUE START  ***************************************************************************************/

#define RETRY(RETRY_MAX_COUNT, RETRY_STATUS, CODE_BLOCK) \
    do { \
        uint8_t retryCount; \
        for (retryCount = 0; retryCount < RETRY_MAX_COUNT; retryCount++) \
        { \
            RETRY_STATUS = CODE_BLOCK; \
            if (RETRY_STATUS == BMS_OK) \
                break; \
        } \
    } while (0)


#define RETRY_PRIORITY_ZERO         5
#define RETRY_PRIORITY_ONE          4
#define RETRY_PRIORITY_TWO          3
#define RETRY_PRIORITY_THREE        2
#define RETRY_PRIORITY_FOUR         1
#define RETRY_PRIORITY_FIVE         0

#define RETRY_CRC_ERROR             RETRY_PRIORITY_ZERO
#define RETRY_CMU_READ              RETRY_PRIORITY_ZERO
#define RETRY_CCM_READ              RETRY_PRIORITY_ZERO
#define RETRY_EEPROM                RETRY_PRIORITY_ZERO
#define RETRY_FLASH_READ            RETRY_PRIORITY_ZERO
#define RETRY_FLASH_WRITE           RETRY_PRIORITY_ZERO
#define RETRY_CAN_COMMUNICATION     RETRY_PRIORITY_ZERO
#define RETRY_OTA                   RETRY_PRIORITY_ZERO

#define RETRY_CAN_INIT              RETRY_PRIORITY_ZERO
#define RETRY_CMU_INIT              RETRY_PRIORITY_ZERO
#define RETRY_CCM_INIT              RETRY_PRIORITY_ZERO

#define RETRY_SHORT_CIRCUIT         RETRY_PRIORITY_FOUR

/*************** RETRY VALUE END  ***************************************************************/


/*************** ADC PARAM START  ***************************************************************/

    #define NUM_CHANNELS 4
    #define ADCMaxRes 4096.0
    #define ADCMaxVout 3.3
    #define ADCVOLMULTIPLIER (ADCMaxVout / ADCMaxRes)
    
/*************** ADC PARAM END  ***************************************************************/


/*************** BALANCING DERATING START  ***************************************************************/

#define BALANCING_DERATING_START_TEMP_C         80
#define BALANCING_DERATING_END_TEMP_C           90

#define BALANCING_MAX_ON_TIME_ms                400
#define BALANCING_MIN_ON_TIME_ms                100

#define BALANCING_MAX_OFF_TIME_ms               200
#define BALANCING_MIN_OFF_TIME_ms               100

/*************** BALANCING DERATING START  ***************************************************************/

#define CHECK_STATUS(x)  if(x!=0)return(x)

/* Private typedef -----------------------------------------------------------*/
/**
  * @brief  BMS Status structure definition
  */
typedef enum bmsStatus
{
  BMS_OK = 0,
  BMS_ERROR,
  BMS_BUSY,
  BMS_TIMEOUT,
  BMS_DYNAMIC_MEM_ALLOC_ERROR,
  BMS_CMU_PEC10_ERROR,
  BMS_CMU_NOT_READY,
  BMS_CCM_PEC15_ERROR,
  BMS_CCM_TIMEOUT,
  BMS_EEPROM_WRITE_FAILED,
  BMS_EEPROM_READ_FAILED,
  BMS_FLASH_CRC_NOT_EQUAL,
  BMS_FLASH_EXCEPTION_DATA_NOT_FOUND,
  BMS_FLASH_CONFIG_DATA_OUT_OF_RANGE,
  BMS_FLASH_CONFIG_DATA_OUT_OF_INDEX,
  BMS_FLASH_CONFIG_RECEIVED_DATA_IS_SAME,
  BMS_CONFIG_MINIMUM_VERSION_UNSATISFIED,
  BMS_FUNCTION_PARAMETER_ERROR,
  BMS_CAN_FUNCTION_PARAMETER_ERROR,
  BMS_SPI_FUNCTION_PARAMETER_ERROR,
  BMS_I2C_FUNCTION_PARAMETER_ERROR,
  BMS_UART_FUNCTION_PARAMETER_ERROR,
  BMS_GPIO_FUNCTION_PARAMETER_ERROR,
  BMS_TIMER_FUNCTION_PARAMETER_ERROR,
  BMS_WATCH_DOG_FUNCTION_PARAMETER_ERROR,
  BMS_EEPROM_FUNCTION_PARAMETER_ERROR,
  BMS_FLASH_FUNCTION_PARAMETER_ERROR,
  BMS_MEMORY_FUNCTION_PARAMETER_ERROR,
  BMS_CRC_FUNCTION_PARAMETER_ERROR,
  BMS_CONVERSION_FUNCTION_PARAMETER_ERROR,
  BMS_CMU_FUNCTION_PARAMETER_ERROR,
  BMS_CCM_FUNCTION_PARAMETER_ERROR,
  BMS_FILTER_FUNCTION_PARAMETER_ERROR,
  BMS_DATA_PROCESSING_FUNCTION_PARAMETER_ERROR,
  BMS_COULOMB_COUNTING_FUNCTION_PARAMETER_ERROR,
  BMS_CC_EKF_FUNCTION_PARAMETER_ERROR,
  BMS_THRESHOLD_FUNCTION_PARAMETER_ERROR,
  BMS_CELL_BALANCING_FUNCTION_PARAMETER_ERROR,
  BMS_STATE_MACHINE_FUNCTION_PARAMETER_ERROR,
  BMS_CHARGING_FUNCTION_PARAMETER_ERROR,
  BMS_CONTACTOR_FUNCTION_PARAMETER_ERROR,
  BMS_PROTECTION_FUNCTION_PARAMETER_ERROR,
  BMS_DATA_PIPELINE_FUNCTION_PARAMETER_ERROR,
  BMS_COTA_UPDATE_ERROR,
  BMS_HARDFAULT_ERROR
} bmsStatus_t;

typedef enum
{
    OTC_ERROR_TEMPERATURE_INDEX,
    OTC_WARNING_TEMPERATURE_INDEX,
    OTC_RECOVERY_TEMPERATURE_INDEX,

    OTD_ERROR_TEMPERATURE_INDEX,
    OTD_WARNING_TEMPERATURE_INDEX,
    OTD_RECOVERY_TEMPERATURE_INDEX,

    UTC_ERROR_TEMPERATURE_INDEX,
    UTC_WARNING_TEMPERATURE_INDEX,
    UTC_RECOVERY_TEMPERATURE_INDEX,

    UTD_ERROR_TEMPERATURE_INDEX,
    UTD_WARNING_TEMPERATURE_INDEX,
    UTD_RECOVERY_TEMPERATURE_INDEX

}temperatureLimitsIndex_t;

/*-------------------------  Group 1  -----------------------------------------------------------*/

#define OTC_ERROR_TEMPERATURE_GROUP_1           55
#define OTC_WARNING_TEMPERATURE_GROUP_1         52
#define OTC_RECOVERY_TEMPERATURE_GROUP_1        50

#define OTD_ERROR_TEMPERATURE_GROUP_1           55
#define OTD_WARNING_TEMPERATURE_GROUP_1         52
#define OTD_RECOVERY_TEMPERATURE_GROUP_1        50

#define UTC_ERROR_TEMPERATURE_GROUP_1           0
#define UTC_WARNING_TEMPERATURE_GROUP_1         2
#define UTC_RECOVERY_TEMPERATURE_GROUP_1        5

#define UTD_ERROR_TEMPERATURE_GROUP_1           0
#define UTD_WARNING_TEMPERATURE_GROUP_1         2
#define UTD_RECOVERY_TEMPERATURE_GROUP_1        5


/*-------------------------  Group 2  -----------------------------------------------------------*/

#define OTC_ERROR_TEMPERATURE_GROUP_2           95
#define OTC_WARNING_TEMPERATURE_GROUP_2         92
#define OTC_RECOVERY_TEMPERATURE_GROUP_2        90

#define OTD_ERROR_TEMPERATURE_GROUP_2           95
#define OTD_WARNING_TEMPERATURE_GROUP_2         92
#define OTD_RECOVERY_TEMPERATURE_GROUP_2        90

#define UTC_ERROR_TEMPERATURE_GROUP_2           0
#define UTC_WARNING_TEMPERATURE_GROUP_2         2
#define UTC_RECOVERY_TEMPERATURE_GROUP_2        5

#define UTD_ERROR_TEMPERATURE_GROUP_2           0
#define UTD_WARNING_TEMPERATURE_GROUP_2         2
#define UTD_RECOVERY_TEMPERATURE_GROUP_2        5

#define EEPROM_WARNING_THRESHOLD                5
#define EEPROM_CORRUPTION_THRESHOLD             9

#define HARD_FAULT_SIZE                         16
#define CC_BUFFER_SIZE                          12            
#define HARD_FAULT_PAYLOAD                      HARD_FAULT_SIZE * sizeof(uint64_t)            

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

#endif
/* End of File ---------------------------------------------------------------*/
