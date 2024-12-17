/**
 * @file bmsCANMatrix.h
 * @brief 
 * @version 0.1
 * @date 2022-04-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef BMSCANMATRIX_H_
#define BMSCANMATRIX_H_

/* Standard C includes -------------------------------------------------------*/
#include "stdint.h"
/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/


#ifdef CAN_CHARGER_EXTENDED_ID
    #define CAN_TX_ID_CHARGER     0x1806E5F4
#else
    #define CAN_TX_ID_CHARGER     0x107
#endif

#define CAN_TX_ID_001         0x110
#define CAN_TX_ID_002         0x109
#define CAN_TX_ID_003         0x112
#define CAN_TX_ID_005         0x113
#define CAN_TX_ID_006         0x114
#define CAN_TX_ID_007         0x115
#define CAN_TX_ID_008         0x116
#define CAN_TX_ID_009         0x117
#define CAN_TX_ID_010         0x118
#define CAN_TX_ID_011         0x12A
#define CAN_TX_ID_023         0x108
#define CAN_TX_ID_012         0x11C
#define CAN_TX_ID_013         0x258
#define CAN_TX_ID_014         0x259


#define CAN_TX_ID_004         0x11A
#define CAN_TX_ID_015         0x156
#define CAN_TX_ID_016         0x119
#define CAN_TX_ID_017         0x120
#define CAN_TX_ID_018         0x121
#define CAN_TX_ID_019         0x122
#define CAN_TX_ID_020         0x123
#define CAN_TX_ID_021         0x124
#define CAN_TX_ID_022         0x12B

#define CAN_TX_ID_023         0x108

#define CAN_TX_ID_024         0x12C
#define CAN_TX_ID_026         0x6F1
#define CAN_TX_ID_027         0x6F2
#define CAN_TX_ID_028         0x6F3
#define CAN_TX_ID_029         0x7B1
#define CAN_TX_ID_030         0x106
#define CAN_TX_ID_031         0x12D
#define CAN_TX_ID_033         0x12E
#define CAN_TX_ID_032         0x7A1
#define CAN_TX_ID_025         0x7A2
#define CAN_TX_ID_034         0X1A1  //HF ID

#define MARVEL_OFF_READY      CAN_TX_ID_030
#define HARD_FAULT_TX_ID      CAN_TX_ID_034

/* Reception CAN IDs*/
#define CAN_RX_ID_001                           0x103
#define CAN_RX_ID_002                           0x602
#define CAN_RX_ID_003                           0x105
#define CAN_RX_ID_006                           0x6FA
#define CAN_RX_ID_007                           0x6FB
#define CAN_RX_ID_008                           0x900
#define CAN_RX_ID_009                           0x901
#define CAN_RX_ID_010                           0x1FE
#define CAN_RX_ID_011                           0x4FA
#define CAN_RX_ID_012                           0x400
#define CAN_RX_ID_013                           0x1840F400
#define CAN_RX_ID_014                           0x1A0
#define CAN_RX_ID_015                           0x1B0

#define FLASH_CONFIG_CAN_RX_ID                  CAN_RX_ID_001
#define VEHICLE_STATE_CAN_RX_ID                 CAN_RX_ID_002
#define EEPROM_RESET_CAN_RX_ID                  CAN_RX_ID_003
#define STARK_VEHICLE_STATE                     CAN_RX_ID_012  
#define STARK_POWEROFF_CMD                      CAN_RX_ID_013
#define HARD_FAULT_FLOW_CONTROL_ID              CAN_RX_ID_014
#define MERLYN_INFO_RX_CMD                      CAN_RX_ID_015                      

#define IS_CAN_RX_ID(ID)                        (((ID) == CAN_RX_ID_001) || ((ID) == CAN_RX_ID_002) ||\
                                                 ((ID) == CAN_RX_ID_006) || ((ID) == CAN_RX_ID_007) ||\
                                                 ((ID) == CAN_RX_ID_008) || ((ID) == CAN_RX_ID_009) ||\
                                                 ((ID) == CAN_RX_ID_010) || ((ID) == CAN_RX_ID_012)||\
                                                 ((ID) == CAN_RX_ID_013) || ((ID) == CAN_RX_ID_014)||\
                                                 ((ID) == CAN_RX_ID_015))

/* Reception CAN Masks*/
#define CAN_RX_MASK                             0x7C0
#define IS_CAN_RX_MASK(MASK)                    (((MASK) == CAN_RX_MASK))

/* Scaling Factors*/
#define CMU_TEMP_MESSAGE_SCALING                1
#define CMU_CELL_VOLTAGE_MESSAGE_SCALING        0.1
#define PACK_CURRENT_MESSAGE_SCALING            0.00001

/* Private typedef -----------------------------------------------------------*/
typedef struct canDataTypeStd
{
    uint16_t messageID;
    uint8_t messageDLC;
    uint8_t messageStartByte;
    float messageScaling;
}CANdataStd;

typedef struct canDataTypeExt
{
    uint32_t messageID;
    uint8_t messageDLC;
    uint8_t messageStartByte;
    float messageScaling;
}CANdataExt;



/* External variables --------------------------------------------------------*/
//Backward Compatilble CAN
extern CANdataStd       CAN_flagsPreChargeActive;
extern CANdataStd       CAN_flagsBalancingActive;
extern CANdataStd       CAN_flagsFullCharge;

extern CANdataStd       CAN_balancingLimit;
extern CANdataStd       CAN_packCurrent;
extern CANdataStd       CAN_stateOfCharge;
extern CANdataStd       CAN_stateOfHealth;
extern CANdataStd       CAN_bmsState;
extern CANdataStd       CAN_packVoltage;
extern CANdataStd       CAN_packCapacity;
extern CANdataStd       CAN_dynamicLimitIN;
extern CANdataStd       CAN_dynamicLimitOUT;
extern CANdataStd       CAN_InstantaneousLimitIN;
extern CANdataStd       CAN_InstantaneousLimitOUT;
extern CANdataStd       CAN_FirmwareVersion;
extern CANdataStd       CAN_ConfigID;
extern CANdataStd       CAN_packCapacityDesign;
extern CANdataStd       CAN_packCapacityFull;
extern CANdataStd       CAN_balancingBitMask_CMU1;
extern CANdataStd       CAN_balancingBitMask_CMU2;
extern CANdataStd       CAN_timeBaseResetCounter;
extern CANdataStd       CAN_mcuResetCounter;
extern CANdataStd       CAN_2949CRCCounter;
extern CANdataStd       CAN_6815CRCCounter;

extern CANdataStd      CAN_pIn_voltageMin;
extern CANdataStd      CAN_pIn_voltageMax;
extern CANdataStd      CAN_pIn_voltageDelta;
extern CANdataStd       CAN_ccOut_cycleCount;
extern CANdataStd      CAN_cIn_contactorCommand;

#ifdef CAN_MATRIX_V_0_3
extern CANdataStd       CAN_maxAllowedCellVoltage;
extern CANdataStd       CAN_maxAllowedPackVoltage;
extern CANdataStd       CAN_maxAllowedCellTemp;
extern CANdataStd       CAN_maxAllowedCurrent;
#endif

extern CANdataStd       CAN_MCUTEMP1;  //MCU grp
extern CANdataStd       CAN_MCUTEMP2;
extern CANdataStd       CAN_MCUTEMP3;
extern CANdataStd       CAN_MCUTEMP4;
extern CANdataStd       CAN_cmu1Temp1;  //CMU grp
extern CANdataStd       CAN_cmu1Temp2;
extern CANdataStd       CAN_cmu1Temp3;
extern CANdataStd       CAN_cmu1Temp4;
extern CANdataStd       CAN_cmu3Temp1;
extern CANdataStd       CAN_cmu3Temp2;
extern CANdataStd       CAN_cmu3Temp3;
extern CANdataStd       CAN_cmu3Temp4;
extern CANdataStd       CAN_cmu4Temp1;
extern CANdataStd       CAN_cmu4Temp2;
extern CANdataStd       CAN_cmu4Temp3;
extern CANdataStd       CAN_cmu4Temp4;

// CMU1
extern CANdataStd       CAN_cmu1Cell1;
extern CANdataStd       CAN_cmu1Cell2;
extern CANdataStd       CAN_cmu1Cell3;
extern CANdataStd       CAN_cmu1Cell4;
extern CANdataStd       CAN_cmu1Cell5;
extern CANdataStd       CAN_cmu1Cell6;
extern CANdataStd       CAN_cmu1Cell7;
extern CANdataStd       CAN_cmu1Cell8;
extern CANdataStd       CAN_cmu1Cell9;
extern CANdataStd       CAN_cmu1Cell10;
extern CANdataStd       CAN_cmu1Cell11;
extern CANdataStd       CAN_cmu1Cell12;
extern CANdataStd       CAN_cmu1Cell13;
extern CANdataStd       CAN_cmu1Cell14;
extern CANdataStd       CAN_cmu1Cell15;
extern CANdataStd       CAN_cmu1Cell16;
extern CANdataStd       CAN_cmu1Cell17;
extern CANdataStd       CAN_cmu1Cell18;

// CMU 2
extern CANdataStd       CAN_cmu2Cell1;
extern CANdataStd       CAN_cmu2Cell2;
extern CANdataStd       CAN_cmu2Cell3;
extern CANdataStd       CAN_cmu2Cell4;
extern CANdataStd       CAN_cmu2Cell5;
extern CANdataStd       CAN_cmu2Cell6;
extern CANdataStd       CAN_cmu2Cell7;
extern CANdataStd       CAN_cmu2Cell8;
extern CANdataStd       CAN_cmu2Cell9;
extern CANdataStd       CAN_cmu2Cell10;
extern CANdataStd       CAN_cmu2Cell11;
extern CANdataStd       CAN_cmu2Cell12;
extern CANdataStd       CAN_cmu2Cell13;
extern CANdataStd       CAN_cmu2Cell14;
extern CANdataStd       CAN_cmu2Cell15;
extern CANdataStd       CAN_cmu2Cell16;
extern CANdataStd       CAN_cmu2Cell17;
extern CANdataStd       CAN_cmu2Cell18;


// CMU 3
extern CANdataStd       CAN_cmu3Cell1;
extern CANdataStd       CAN_cmu3Cell2;
extern CANdataStd       CAN_cmu3Cell3;
extern CANdataStd       CAN_cmu3Cell4;
extern CANdataStd       CAN_cmu3Cell5;
extern CANdataStd       CAN_cmu3Cell6;
extern CANdataStd       CAN_cmu3Cell7;
extern CANdataStd       CAN_cmu3Cell8;
extern CANdataStd       CAN_cmu3Cell9;
extern CANdataStd       CAN_cmu3Cell10;
extern CANdataStd       CAN_cmu3Cell11;
extern CANdataStd       CAN_cmu3Cell12;
extern CANdataStd       CAN_cmu3Cell13;
extern CANdataStd       CAN_cmu3Cell14;
extern CANdataStd       CAN_cmu3Cell15;
extern CANdataStd       CAN_cmu3Cell16;
extern CANdataStd       CAN_cmu3Cell17;
extern CANdataStd       CAN_cmu3Cell18;


#ifdef DEBUG_CAN

    //coulomb counting
    #define CAN_DEBUG_ID_001         0x320
    #define CAN_DEBUG_ID_002         0x321
    #define CAN_DEBUG_ID_003         0x322

    //contactor 
    #define CAN_DEBUG_ID_004         0x323
    #define CAN_DEBUG_ID_005         0x324
    #define CAN_DEBUG_ID_006         0x325
    #define CAN_DEBUG_ID_007         0x326

    //cell balancing
    #define CAN_DEBUG_ID_008         0x327
    #define CAN_DEBUG_ID_009         0x328
    #define CAN_DEBUG_ID_010         0x329
    #define CAN_DEBUG_ID_011         0x32A

    //protection in - voltages
    #define CAN_DEBUG_ID_012         0x32B
    #define CAN_DEBUG_ID_013         0x32C
    #define CAN_DEBUG_ID_014         0x32D
    #define CAN_DEBUG_ID_015         0x32E
    #define CAN_DEBUG_ID_016         0x32F

    //protection in - temp
    #define CAN_DEBUG_ID_017         0x330
    #define CAN_DEBUG_ID_018         0x331
    #define CAN_DEBUG_ID_019         0x332
    #define CAN_DEBUG_ID_020         0x333


    //protection in - current
    #define CAN_DEBUG_ID_021         0x334

    #define CAN_DEBUG_ID_022         0x335
    #define CAN_DEBUG_ID_023         0x336
    #define CAN_DEBUG_ID_024         0x337
    #define CAN_DEBUG_ID_025         0x338
    #define CAN_DEBUG_ID_026         0x339
    #define CAN_DEBUG_ID_027         0x33A
    #define CAN_DEBUG_ID_028         0x33B
    #define CAN_DEBUG_ID_029         0x33C
    #define CAN_DEBUG_ID_030         0x33D
    #define CAN_DEBUG_ID_031         0x33E
    #define CAN_DEBUG_ID_032         0x33F

    //IR
    #define CAN_DEBUG_ID_044         0x340
    #define CAN_DEBUG_ID_045         0x343  //IR status

    //cc in
    extern CANdataStd       CAN_ccIn_VehicleRest_Time;
    extern CANdataStd       CAN_ccIn_Max_PackCapacity;
    extern CANdataStd       CAN_ccIn_VehicleRest_DisCharge_Current;
    extern CANdataStd       CAN_ccIn_VehicleRest_Charge_Current;

    extern CANdataStd       CAN_ccIn_pOut_OVThreshold;
    extern CANdataStd       CAN_ccIn_Voltage;
    extern CANdataStd       CAN_ccIn_Current;

    //cc out
    extern CANdataStd       CAN_ccOut_Initial_Capacity_Ah; 
    extern CANdataStd       CAN_ccOut_SOC_Pct; 
    extern CANdataStd       CAN_ccOut_Total_Capacity_Discharged_Ah;
    extern CANdataStd       CAN_ccOut_Total_Capacity_Remaining_Ah; 
    extern CANdataStd       CAN_ccOut_Coulomb_Counting_State;

    extern CANdataStd       CAN_ccOut_EKF_SOC_Pct; 
    extern CANdataStd       CAN_pOut_overCurrentProtection;
    extern CANdataStd       CAN_ccOut_Voltage_EKF;
    extern CANdataStd       CAN_ccOut_Terminal_Voltage;
    //c in
    extern CANdataStd      CAN_cIn_NegContactor_FB;
    extern CANdataStd      CAN_cIn_PosContactor_FB;
    extern CANdataStd      CAN_cIn_PreChargeContactor_FB;


    extern CANdataStd      CAN_cIn_MaxImbalance;

    extern CANdataStd      CAN_cIn_ThermalRunaway;
    extern CANdataStd      CAN_cIn_OCC;
    extern CANdataStd      CAN_cIn_OCD;
    extern CANdataStd      CAN_cIn_OTC;
    extern CANdataStd      CAN_cIn_OTD;
    extern CANdataStd      CAN_cIn_UTC;
    extern CANdataStd      CAN_cIn_UTD;
    extern CANdataStd      CAN_cIn_OV;
    extern CANdataStd      CAN_cIn_UV;

    extern CANdataStd      CAN_cIn_contactorCutoffTime;
    extern CANdataStd      CAN_cIn_preChargeContactorCurrent;
    extern CANdataStd      CAN_cIn_contactorResponseTime_msec;

    extern CANdataStd      CAN_cIn_communicationFlag;
    extern CANdataStd      CAN_cIn_shortCircuitFlag;
    extern CANdataStd      CAN_cIn_permanentFailFlag;
    extern CANdataStd      CAN_cIn_contactorErrorFlag;

    //contactor out
    extern CANdataStd      CAN_cOut_NegContactorState;
    extern CANdataStd      CAN_cOut_PosContactorState;
    extern CANdataStd      CAN_cOut_PreChargeContactorState;

    //cb in 
    extern CANdataStd      CAN_cbIn_balancingMinThreshold;
    extern CANdataStd      CAN_cbIn_balancingMaxAllowedImbalance;
    extern CANdataStd      CAN_cbIn_balancingOVProtectionThreshold;
    extern CANdataStd      CAN_cbIn_balancingStateRestPeroid;
    extern CANdataStd      CAN_cbIn_cellsInSeries;

    //cb out
    extern CANdataStd      CAN_cbOut_balancingState;

    //protection input
    extern CANdataStd      CAN_pIn_voltage[20];
    extern CANdataStd      CAN_pIn_temperatureMin;
    extern CANdataStd      CAN_pIn_temperatureMax;
    extern CANdataStd      CAN_pIn_temperatureDelta;

    extern CANdataStd      CAN_pIn_current;
    extern CANdataStd      CAN_pIn_OCC_WarningCurrent;
    extern CANdataStd      CAN_pIn_OCC_ErrorCurrent;

    extern CANdataStd      CAN_pIn_eFuseChargingLimit_A2s;
    extern CANdataStd      CAN_pIn_eFuseDischargingLimit_A2s;
    extern CANdataStd      CAN_pIn_calculatedi2tFuseValue;
    extern CANdataStd      CAN_pOut_activeErrors;

    extern CANdataStd       CAN_ccOut_initialCapacity;
    extern CANdataStd       CAN_ccOut_totalCapacityRemaining;
    extern CANdataStd       CAN_ccOut_totalCapacityDischarge;
    extern CANdataStd       CAN_ccOut_SOCPercentage;
    extern CANdataStd       CAN_ccOut_coulombCountingState;
    extern CANdataStd       CAN_ccOut_MaxUsableCapacity_mAh;
    extern CANdataStd       CAN_ccOut_TotalCapacityExchange_mAh;
    extern CANdataStd       CAN_ccOut_SOH_cpct;
    extern CANdataStd       CAN_ccOut_PackEnergy;

#endif

void initializeDebugCANMatrix(void); 

#ifdef DEBUG_CAN
    #define CAN_DEBUG_IDS(ID) \
        ((ID) == CAN_DEBUG_ID_002) || ((ID) == CAN_DEBUG_ID_004) || \
        ((ID) == CAN_DEBUG_ID_005) || ((ID) == CAN_DEBUG_ID_006) || \
        ((ID) == CAN_DEBUG_ID_007) || ((ID) == CAN_DEBUG_ID_009) || \
        ((ID) == CAN_DEBUG_ID_017) || ((ID) == CAN_DEBUG_ID_018) || \
        ((ID) == CAN_DEBUG_ID_019) || ((ID) == CAN_DEBUG_ID_020) || \
        ((ID) == CAN_DEBUG_ID_029) || ((ID) == CAN_DEBUG_ID_030) || \
        ((ID) == CAN_DEBUG_ID_031) || ((ID) == CAN_DEBUG_ID_032) || \
        ((ID) == CAN_DEBUG_ID_045) || ((ID) == CAN_DEBUG_ID_044) 
#else
#define CAN_DEBUG_IDS
#endif

#define IS_CAN_TX_ID(ID)               (((ID) == CAN_TX_ID_001) || ((ID) == CAN_TX_ID_002) || \
                                        ((ID) == CAN_TX_ID_003) || ((ID) == CAN_TX_ID_004) || \
                                        ((ID) == CAN_TX_ID_005) || ((ID) == CAN_TX_ID_006) || \
                                        ((ID) == CAN_TX_ID_007) || ((ID) == CAN_TX_ID_008) || \
                                        ((ID) == CAN_TX_ID_009) || ((ID) == CAN_TX_ID_010) || \
                                        ((ID) == CAN_TX_ID_011) || ((ID) == CAN_TX_ID_012) || \
                                        ((ID) == CAN_TX_ID_013) || ((ID) == CAN_TX_ID_014) || \
                                        ((ID) == CAN_TX_ID_015) || ((ID) == CAN_TX_ID_022) || \
                                        ((ID) == CAN_TX_ID_023) || ((ID) == CAN_TX_ID_CHARGER) || \
                                        ((ID) == CAN_TX_ID_024) || ((ID) == CAN_TX_ID_025) || \
                                        ((ID) == CAN_TX_ID_026) || ((ID) == CAN_TX_ID_027) || \
                                        ((ID) == CAN_TX_ID_028) || ((ID) == CAN_TX_ID_029) || \
                                        ((ID) == CAN_TX_ID_030) || ((ID) == CAN_TX_ID_031) || \
                                        ((ID) == CAN_TX_ID_032) || ((ID) == CAN_TX_ID_033) || \
                                        ((ID) == CAN_TX_ID_034) ||\
                                        CAN_DEBUG_IDS(ID) \
                                        )

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
bmsStatus_t initializeCANMatrix(uint8_t canPort);

/* End of File ---------------------------------------------------------------*/
#endif

