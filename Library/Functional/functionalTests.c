/**
 * @file functionalTests.c
 * @brief
 * @version 0.1
 * @date 2022-02-25
 *
 * @copyright Copyright (c) 2022
 *
 */

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/

/* Self include --------------------------------------------------------------*/
#include "functionalTests.h"
#include "conversion.h"
/* Type definiton ------------------------------------------------------------*/

#define TEST_FASTCHARGING

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
uint16_t waitCount = 0;
uint8_t delayCount = 0;

bool Latchguard = false;
uint16_t latchStayCount = 0;
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
void functionalTests(int testCase, bmsMainData_t *mdata)
{
//dynamic Test cases
    switch (testCase)
    {
        case overVoltageTest:
            // simulate error condition
            if (waitCount > 10 && waitCount < 70)
            {
                mdata->cmuData.cmuVolatges.cellVoltages[1][2] = 4500;
            }
            else if (waitCount > 70)
            {
                mdata->cmuData.cmuVolatges.cellVoltages[1][2] = 3200;
            }
            waitCount++;
            break;

        case underVoltageTest:
            // simulate error condition
            if (waitCount > 10 && waitCount < 70)
            {
                mdata->cmuData.cmuVolatges.cellVoltages[1][2] = 2800;
            }
            else if (waitCount > 70)
            {
                mdata->cmuData.cmuVolatges.cellVoltages[1][2] = 3200;
            }
            waitCount++;
            break;

        case overCurrentChargeTest:
            // simulate error condition
            if (waitCount > 10 && waitCount < 30)
            {
                mdata->ccmData.packCurrent = 100;
            }
            else if (waitCount > 30)
            {
                mdata->ccmData.packCurrent = 0;
            }
            waitCount++;
            break;

        case overCurrentDischargeTest:
            // simulate error condition
            if (waitCount > 10 && waitCount < 90)
            {
                mdata->ccmData.packCurrent = -250;
            }
            else if (waitCount > 90)
            {
                mdata->ccmData.packCurrent = 0;
            }
            waitCount++;
            break;

        case overTemperatureChargeTest:
            // simulate error condition
            if (waitCount > 10 && waitCount < 70)
            {
                mdata->ccmData.packCurrent = 40;
                mdata->cmuData.cmuTemperatures.externalNTC[0][2] = 57;
            }
            else if (waitCount > 70)
            {
                mdata->cmuData.cmuTemperatures.externalNTC[0][2] = 28;
            }
            waitCount++;
            break;

        case overTemperatureDischargeTest:
            // simulate error condition
            if (waitCount > 10 && waitCount < 70)
            {
                mdata->ccmData.packCurrent = -40;
                mdata->cmuData.cmuTemperatures.externalNTC[0][2] = 57;
            }
            else if (waitCount > 70)
            {
                mdata->cmuData.cmuTemperatures.externalNTC[0][2] = 28;
            }
            waitCount++;
            break;

        case underTemperatureChargeTest:
            // simulate error condition
            if (waitCount > 10 && waitCount < 70)
            {
                mdata->ccmData.packCurrent = 40;
                mdata->cmuData.cmuTemperatures.externalNTC[0][2] = 2;
            }
            else if (waitCount > 70)
            {
                mdata->cmuData.cmuTemperatures.externalNTC[0][2] = 28;
            }     
            waitCount++;
            break;

        case underTemperatureDischargeTest:
            // simulate error condition
            if (waitCount > 10 && waitCount < 70)
            {
                mdata->ccmData.packCurrent = -40;
                mdata->cmuData.cmuTemperatures.externalNTC[0][2] = 2;
            }
            else if (waitCount > 70)
            {
                mdata->cmuData.cmuTemperatures.externalNTC[0][2] = 28;
            }       
            waitCount++;
            break;

        case retryTest:
            //simulate error condition
            if (waitCount > 10){
                mdata->cmuData.cmuVolatges.cellVoltages[1][2] = 5000;
                if (delayCount > 20){waitCount = 0;}}
            waitCount++;
            break;


        case thermalRunaway:
            //simulate error condition
            if (waitCount > 20 && waitCount < 70) 
            {

                mdata->cmuData.cmuTemperatures.externalNTC[0][2] = 90;

            }
            waitCount++;
            break;
        
        case suddenVoltageDrop:
            //simulate error condition
            if (waitCount > 20 && waitCount < 70) 
            {

                mdata->cmuData.cmuVolatges.cellVoltages[1][2] = 4400;

            }
            waitCount++;
            break;

        case temperatureGradient:
            //simulate error condition
        if (waitCount > 20 && waitCount < 70) 
            {

                mdata->cmuData.cmuTemperatures.externalNTC[0][2] = 40;

            }
            waitCount++;
            break;

        case highImbalanceFlag:
            //simulate error condition
            if (waitCount > 20 && waitCount < 70) 
            {

                mdata->cmuData.cmuVolatges.cellVoltages[1][2] = 3420;
                mdata->cmuData.cmuVolatges.cellVoltages[1][2] = 3120;

            }
            waitCount++;
            break;
        

        case shortCircuitDetect:
            //simulate error condition
            if (waitCount > 20 && waitCount < 70) 
            {

                mdata->ccmData.packCurrent = -400000;

            }
            waitCount++;
            break;


    //Static Test Cases

        case overVoltageTestStatic:
            // simulate error condition
            mdata->cmuData.cmuVolatges.cellVoltages[1][2] = 4500;
            if (waitCount > 20)
            {
                mdata->cmuData.cmuVolatges.cellVoltages[1][2] = 3200;
            }
            waitCount++;
            break;  

        case underVoltageTestStatic:
            // simulate error condition
            mdata->cmuData.cmuVolatges.cellVoltages[1][2] = 2500;
            if (waitCount > 20)
            {
                mdata->cmuData.cmuVolatges.cellVoltages[1][2] = 3200;
            }
            waitCount++;
            break; 

        case overCurrentChargeTestStatic:
            // simulate error condition
            mdata->ccmData.packCurrent = 100;
            if (waitCount > 30 && waitCount < 30)
            {
                mdata->ccmData.packCurrent = 0;
            }
            waitCount++;
            break;       

        case overCurrentDischargeTestStatic:
            // simulate error condition
            mdata->ccmData.packCurrent = -100;
            if (waitCount > 30)
            {
                mdata->ccmData.packCurrent = 0;
            }
            waitCount++;
            break;  

        case overTemperatureChargeTestStatic:
            // simulate error condition
            mdata->ccmData.packCurrent = 40;
            mdata->cmuData.cmuTemperatures.externalNTC[0][2] = 57;
            if (waitCount > 30)
            {
                mdata->cmuData.cmuTemperatures.externalNTC[0][2] = 28;
            }
            waitCount++;
            break;

        case overTemperatureDischargeTestStatic:
            // simulate error condition
            mdata->ccmData.packCurrent = -40;
            mdata->cmuData.cmuTemperatures.onBoardNTC[0][2] = 57;
            if (waitCount > 30)
            {
                mdata->cmuData.cmuTemperatures.externalNTC[0][2] = 28;
            }
            waitCount++;
            break;    

        case underTemperatureChargeTestStatic:
            // simulate error condition
            mdata->ccmData.packCurrent = 40;
            mdata->cmuData.cmuTemperatures.externalNTC[0][2] = 0;
            if (waitCount > 30)
            {
                mdata->cmuData.cmuTemperatures.externalNTC[0][2] = 28;
            }
            waitCount++;
            break;  

        case underTemperatureDischargeTestStatic:
            // simulate error condition
            mdata->ccmData.packCurrent = -40;
            mdata->cmuData.cmuTemperatures.externalNTC[0][2] = 2;
            if (waitCount > 30)
            {
                mdata->cmuData.cmuTemperatures.externalNTC[0][2] = 28;
            }
            waitCount++;
            break;    

        case socLatching:
            if (waitCount > 20)
            {

#ifdef TEST_FASTCHARGING                
                for(uint8_t cellIndex = 0; cellIndex < MAX_CELL_PER_SLAVE; cellIndex++)
                {
                    mdata->cmuData.cmuVolatges.cellVoltages[0][cellIndex] = 3570;
                    mdata->cmuData.cmuVolatges.cellVoltages[1][cellIndex] = 3560;
                }
#else
                for(uint8_t cellIndex = 0; cellIndex < MAX_CELL_PER_SLAVE; cellIndex++)
                {
                    mdata->cmuData.cmuVolatges.cellVoltages[0][cellIndex] = 3400;
                    mdata->cmuData.cmuVolatges.cellVoltages[1][cellIndex] = 3390;
                }
#endif
	            
                mdata->ccmData.packCurrent = 10;
            }
            waitCount++;
            break;

        case highImbalanceError:
            if (waitCount > 10)
            {
                mdata->cmuData.cmuVolatges.cellVoltages[0][0] = 3100;
                mdata->cmuData.cmuVolatges.cellVoltages[1][0] = 3400;
            }
            waitCount++;
            break;

        
    }
}

void functionalTestsInputParam(int testCase, dataPipelineInput_t *dpInput, dataPipelineOutput_t *dpOutput)
{
    switch (testCase)
    {
    case chargerEnable:
        chargerSim(dpInput);
        break;

    case voltageBus:
        voltageSim(dpOutput);
        break;

    case fullChargeLatchSim: //TO be tested
        Latchguard = true;
        latchingCondition(dpInput);
        break;
    }
    
}

void chargerSim(dataPipelineInput_t  *dpInput)
{
    dpInput->SlowCharge            = true;
    dpInput->FastCharge            = false;
    dpInput->isChargerConnected    = true; 
}

void voltageSim(dataPipelineOutput_t  *dpOutput)
{
    //UV
    if(Latchguard == false)
    {
        dpOutput->voltageSense.voltageMin   = 2650;
        dpOutput->voltageSense.voltageMax   = 2750;
        dpOutput->voltageSense.voltageDelta = 100;
        //TODO Nominal and OV
    }
    else if(Latchguard == true && latchStayCount == 0)
    {   
        //Latch Conditions
        dpOutput->voltageSense.voltageMin   = 3395;
        dpOutput->voltageSense.voltageMax   = 3400;
        dpOutput->voltageSense.voltageDelta = 5;
    }
    else
    {
        dpOutput->voltageSense.voltageMin   = 3240;
        dpOutput->voltageSense.voltageMax   = 3250;
        dpOutput->voltageSense.voltageDelta = 10;
    }

}

void latchingCondition(dataPipelineInput_t  *dpInput)
{   
    if(Latchguard == true && latchStayCount < 200)
    {
        dpInput->current = CONVERT_AMPERE_TO_MILLI_AMPERE(14);
        latchStayCount++;
    }
    else if(latchStayCount >= 200 && latchStayCount < 400)
    {
        dpInput->current = CONVERT_AMPERE_TO_MILLI_AMPERE(-100);
        latchStayCount++;
        Latchguard = false;
    }
    else if(latchStayCount >= 400 && latchStayCount < 700)
    {
        dpInput->current = CONVERT_AMPERE_TO_MILLI_AMPERE(100);
        latchStayCount++;
    }
    else
    {
        latchStayCount = 0;
        Latchguard = true;
    }
}

/* End of File ---------------------------------------------------------------*/