/*
 * Contactors.c
 *
 * Code generation for model "Contactors".
 *
 * Model version              : 4.1471
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Thu Jul 25 17:57:01 2024
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: All passed
 */

#include "Contactors.h"
#include "rtwtypes.h"
#include "Contactors_types.h"
#include <string.h>

/* Named constants for Chart: '<S1>/Contactor_StateMachine' */
#define Con_IN_ChangeState01_to_Opening ((uint8_T)1U)
#define Con_IN_PreCharging_Substate_New ((uint8_T)11U)
#define Con_IN_PreCharging_Substate_Old ((uint8_T)12U)
#define Contacto_IN_NegContact_Check_02 ((uint8_T)9U)
#define Contactors_IN_Closed_Substate  ((uint8_T)1U)
#define Contactors_IN_IntState         ((uint8_T)2U)
#define Contactors_IN_IntState1        ((uint8_T)2U)
#define Contactors_IN_IntState10       ((uint8_T)2U)
#define Contactors_IN_IntState11       ((uint8_T)3U)
#define Contactors_IN_IntState12       ((uint8_T)4U)
#define Contactors_IN_IntState13       ((uint8_T)5U)
#define Contactors_IN_IntState14       ((uint8_T)3U)
#define Contactors_IN_IntState14_i     ((uint8_T)6U)
#define Contactors_IN_IntState15       ((uint8_T)4U)
#define Contactors_IN_IntState15_o     ((uint8_T)7U)
#define Contactors_IN_IntState16       ((uint8_T)5U)
#define Contactors_IN_IntState16_o     ((uint8_T)8U)
#define Contactors_IN_IntState2        ((uint8_T)3U)
#define Contactors_IN_IntState3        ((uint8_T)4U)
#define Contactors_IN_IntState4        ((uint8_T)5U)
#define Contactors_IN_IntState5        ((uint8_T)6U)
#define Contactors_IN_IntState6        ((uint8_T)7U)
#define Contactors_IN_IntState7        ((uint8_T)8U)
#define Contactors_IN_IntState8        ((uint8_T)6U)
#define Contactors_IN_IntState8_n      ((uint8_T)9U)
#define Contactors_IN_IntState9        ((uint8_T)7U)
#define Contactors_IN_IntState9_d      ((uint8_T)9U)
#define Contactors_IN_NO_ACTIVE_CHILD  ((uint8_T)0U)
#define Contactors_IN_NegContact_Check ((uint8_T)10U)
#define Contactors_IN_Open_Substate    ((uint8_T)8U)
#define Contactors_IN_PermanentFail    ((uint8_T)10U)
#define Contactors_IN_PosContact_Check ((uint8_T)11U)
#define Contactors_IN_PreCharge_Check01 ((uint8_T)12U)
#define Contactors_IN_PreCharge_Check02 ((uint8_T)13U)
#define Contactors_IN_PreCharge_Check03 ((uint8_T)14U)

/* Block signals (default storage) */
B_Contactors_T Contactors_B;

/* Block states (default storage) */
DW_Contactors_T Contactors_DW;

/* External inputs (root inport signals with default storage) */
ExtU_Contactors_T Contactors_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Contactors_T Contactors_Y;

/* Real-time model */
static RT_MODEL_Contactors_T Contactors_M_;
RT_MODEL_Contactors_T *const Contactors_M = &Contactors_M_;

/* Forward declaration for local functions */
static void exit_internal_PreCharging_Sub_d(void);
static void Contac_PreCharging_Substate_Old(void);
static void Contactors_Open_Substate(void);
static void exit_internal_PreCharging_Subst(void);
static void Contac_PreCharging_Substate_New(void);
const ProtectionState_Out Contactors_rtZProtectionState_O = { NoError,/* ThermalRunaway */
  NoError,                             /* TemperatureGradient */
  NoError,                             /* HighImbalanceFlag */
  NoError,                             /* ShortCircuitDetect */
  NoError,                             /* SuddenVoltageDrop */
  NoError,                             /* OV */
  NoError,                             /* UV */
  NoError,                             /* OCC */
  NoError,                             /* OCD */
  0.0F,                                /* i2t_Calculated_A2sec */
  NoError,                             /* eFuseChargeFlag */
  NoError,                             /* eFuseDischargeFlag */
  NoErr,                               /* TempState1 */
  NoErr,                               /* TempState2 */
  NoError,                             /* FlagGroup1 */
  NoError,                             /* FlagGroup2 */
  NoError,                             /* TempOverallState */
  NoError,                             /* ErrorDetect */
  NoError,                             /* DCLI_CurrentFlag */
  NoError                              /* DCLO_CurrentFlag */
};

/* Function for Chart: '<S1>/Contactor_StateMachine' */
static void exit_internal_PreCharging_Sub_d(void)
{
  Contactors_DW.is_PreCharging_Substate_Old = Contactors_IN_NO_ACTIVE_CHILD;
}

/* Function for Chart: '<S1>/Contactor_StateMachine' */
static void Contac_PreCharging_Substate_Old(void)
{
  /* Inport: '<Root>/ProtectionOutput' incorporates:
   *  Inport: '<Root>/DataPipeline'
   */
  /* :  sf_internal_predicateOutput = ProtectionOutput.ThermalRunaway== ProtectionFlags.Error|| ProtectionOutput.ShortCircuitDetect== ProtectionFlags.Error || DataPipeline.VCU.EmergencyDisconnectEn==1; */
  if ((Contactors_U.ProtectionOutput.ThermalRunaway == Error) ||
      (Contactors_U.ProtectionOutput.ShortCircuitDetect == Error) ||
      Contactors_U.DataPipeline.VCU.EmergencyDisconnectEn) {
    exit_internal_PreCharging_Sub_d();
    Contactors_DW.is_ContactorOperations = Contactors_IN_PermanentFail;

    /* Update for Outport: '<Root>/NegContactors' */
    /* :  NegContactors = ContactorsFlags.Open; */
    Contactors_Y.NegContactors = Open;

    /* Update for Outport: '<Root>/PosContactors' */
    /* :  PosContactors = ContactorsFlags.Open; */
    Contactors_Y.PosContactors = Open;

    /* Update for Outport: '<Root>/PreChargeContactors' */
    /* :  PreChargeContactors = ContactorsFlags.Open; */
    Contactors_Y.PreChargeContactors = Open;

    /* Outport: '<Root>/PermanentFaiil' */
    /* :  PermanentFail = ProtectionFlags.Error; */
    Contactors_Y.PermanentFaiil = Error;

    /* Outport: '<Root>/ContactorsState' */
    /* :  ContactorsState = ContState.PermanentFailure; */
    Contactors_Y.ContactorsState = PermanentFailure;
  } else {
    /* :  sf_internal_predicateOutput = duration(Flags_detection == 0,msec) > Thresholds.PreToOpeningSubstate_msec; */
    if (Contactors_B.Flags_Detection) {
      Contactors_DW.durationCounter_1 = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Contactors_DW.durationCounter_1 * 100 >
        Contactors_U.Thresholds.PreToOpeningSubstate_msec) {
      exit_internal_PreCharging_Sub_d();
      Contactors_DW.is_ContactorOperations = Contacto_IN_NegContact_Check_02;

      /* Update for Outport: '<Root>/NegContactors' */
      /* :  NegContactors = ContactorsFlags.Open; */
      Contactors_Y.NegContactors = Open;

      /* Update for Outport: '<Root>/PosContactors' */
      /* :  PosContactors = ContactorsFlags.Open; */
      Contactors_Y.PosContactors = Open;

      /* Outport: '<Root>/ContactorsState' */
      /* :  ContactorsState = ContState.OpeningSubstate; */
      Contactors_Y.ContactorsState = OpeningSubstate;
    } else {
      switch (Contactors_DW.is_PreCharging_Substate_Old) {
       case Con_IN_ChangeState01_to_Opening:
        Contactors_DW.is_PreCharging_Substate_Old =
          Contactors_IN_NO_ACTIVE_CHILD;
        Contactors_DW.is_ContactorOperations = Contacto_IN_NegContact_Check_02;

        /* Update for Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Open; */
        Contactors_Y.NegContactors = Open;

        /* Update for Outport: '<Root>/PosContactors' */
        /* :  PosContactors = ContactorsFlags.Open; */
        Contactors_Y.PosContactors = Open;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.OpeningSubstate; */
        Contactors_Y.ContactorsState = OpeningSubstate;
        break;

       case Contactors_IN_IntState10:
        /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.ContCutoffTime_msec; */
        if (Contactors_B.PreIntegratedTime >
            Contactors_U.Thresholds.ContCutoffTime_msec) {
          /* :  PreTimeReset=0; */
          Contactors_B.PreTimeReset = 0.0;
          Contactors_DW.is_PreCharging_Substate_Old =
            Con_IN_ChangeState01_to_Opening;

          /* Inport: '<Root>/NegContactors_FB' */
          /* :  sf_internal_predicateOutput = NegContactors_FB == 1; */
        } else if ((int32_T)Contactors_U.NegContactors_FB == 1) {
          /* :  PreTimeReset=0; */
          Contactors_B.PreTimeReset = 0.0;
          Contactors_DW.is_PreCharging_Substate_Old =
            Contactors_IN_NegContact_Check;

          /* Update for Outport: '<Root>/NegContactors' */
          /* :  NegContactors = ContactorsFlags.Closed; */
          Contactors_Y.NegContactors = Closed;

          /* Outport: '<Root>/ContactorsState' */
          /* :  ContactorsState = ContState.PreCharging_NegContactorClosed; */
          Contactors_Y.ContactorsState = PreCharging_NegContactorClosed;
        }
        break;

       case Contactors_IN_IntState11:
        /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.ContCutoffTime_msec; */
        if (Contactors_B.PreIntegratedTime >
            Contactors_U.Thresholds.ContCutoffTime_msec) {
          /* :  PreTimeReset=0; */
          Contactors_B.PreTimeReset = 0.0;
          Contactors_DW.is_PreCharging_Substate_Old =
            Con_IN_ChangeState01_to_Opening;

          /* Inport: '<Root>/PreChargeContactors_FB' */
          /* :  sf_internal_predicateOutput = PreChargeContactors_FB ==1; */
        } else if ((int32_T)Contactors_U.PreChargeContactors_FB == 1) {
          /* :  PreTimeReset=0; */
          Contactors_DW.is_PreCharging_Substate_Old =
            Contactors_IN_PreCharge_Check01;

          /* :  PreTimeReset=1; */
          Contactors_B.PreTimeReset = 1.0;

          /* Update for Outport: '<Root>/PreChargeContactors' */
          /* :  PreChargeContactors = ContactorsFlags.Closed; */
          Contactors_Y.PreChargeContactors = Closed;

          /* Outport: '<Root>/ContactorsState' */
          /* :  ContactorsState = ContState.PreCharging_PreChargeClosed; */
          Contactors_Y.ContactorsState = PreCharging_PreChargeClosed;
        }
        break;

       case Contactors_IN_IntState12:
        /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.PreChargeRecoveryTimeout_msec; */
        if (Contactors_B.PreIntegratedTime >
            Contactors_U.Thresholds.PreChargeRecoveryTimeout_msec) {
          /* :  PreTimeReset=0; */
          Contactors_DW.is_PreCharging_Substate_Old =
            Contactors_IN_PreCharge_Check01;

          /* :  PreTimeReset=1; */
          Contactors_B.PreTimeReset = 1.0;

          /* Update for Outport: '<Root>/PreChargeContactors' */
          /* :  PreChargeContactors = ContactorsFlags.Closed; */
          Contactors_Y.PreChargeContactors = Closed;

          /* Outport: '<Root>/ContactorsState' */
          /* :  ContactorsState = ContState.PreCharging_PreChargeClosed; */
          Contactors_Y.ContactorsState = PreCharging_PreChargeClosed;

          /* :  sf_internal_predicateOutput = PreChargeRetryCheck > Thresholds.PreChargeRetryLimit; */
        } else if (Contactors_DW.PreChargeRetryCheck >
                   Contactors_U.Thresholds.PreChargeRetryLimit) {
          /* :  PreTimeReset=0; */
          Contactors_B.PreTimeReset = 0.0;
          Contactors_DW.is_PreCharging_Substate_Old =
            Contactors_IN_PreCharge_Check03;

          /* Update for Outport: '<Root>/PreChargeContactors' */
          /* :  PreChargeContactors = ContactorsFlags.Open; */
          Contactors_Y.PreChargeContactors = Open;

          /* Outport: '<Root>/ContactorsState' */
          /* :  ContactorsState = ContState.PreCharging_PreChargeRetry; */
          Contactors_Y.ContactorsState = PreCharging_PreChargeRetry;
        }
        break;

       case Contactors_IN_IntState13:
        {
          /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.PreChargeTimeout_msec; */
          if (Contactors_B.PreIntegratedTime >
              Contactors_U.Thresholds.PreChargeTimeout_msec) {
            /* :  PreTimeReset=0; */
            Contactors_B.PreTimeReset = 0.0;
            Contactors_DW.is_PreCharging_Substate_Old =
              Contactors_IN_PosContact_Check;

            /* Update for Outport: '<Root>/PosContactors' */
            /* :  PosContactors = ContactorsFlags.Closed; */
            Contactors_Y.PosContactors = Closed;

            /* Outport: '<Root>/ContactorsState' */
            /* :  ContactorsState = ContState.PreCharging_PosContactorClosed; */
            Contactors_Y.ContactorsState = PreCharging_PosContactorClosed;
          } else {
            int32_T tmp;

            /* :  sf_internal_predicateOutput = abs(DataPipeline.Current_mA) > Thresholds.ContCurrent_mA; */
            if (Contactors_U.DataPipeline.Current_mA < 0) {
              if (Contactors_U.DataPipeline.Current_mA <= MIN_int32_T) {
                tmp = MAX_int32_T;
              } else {
                tmp = -Contactors_U.DataPipeline.Current_mA;
              }
            } else {
              tmp = Contactors_U.DataPipeline.Current_mA;
            }

            if (tmp > Contactors_U.Thresholds.ContCurrent_mA) {
              /* :  PreTimeReset=0; */
              Contactors_DW.is_PreCharging_Substate_Old =
                Contactors_IN_PreCharge_Check01;

              /* :  PreTimeReset=1; */
              Contactors_B.PreTimeReset = 1.0;

              /* Update for Outport: '<Root>/PreChargeContactors' */
              /* :  PreChargeContactors = ContactorsFlags.Closed; */
              Contactors_Y.PreChargeContactors = Closed;

              /* Outport: '<Root>/ContactorsState' */
              /* :  ContactorsState = ContState.PreCharging_PreChargeClosed; */
              Contactors_Y.ContactorsState = PreCharging_PreChargeClosed;
            }
          }
        }
        break;

       case Contactors_IN_IntState14_i:
        /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.ContCutoffTime_msec; */
        if (Contactors_B.PreIntegratedTime >
            Contactors_U.Thresholds.ContCutoffTime_msec) {
          /* :  PreTimeReset=0; */
          Contactors_B.PreTimeReset = 0.0;
          Contactors_DW.is_PreCharging_Substate_Old =
            Contactors_IN_PreCharge_Check02;

          /* Update for Outport: '<Root>/PreChargeContactors' */
          /* :  PreChargeContactors = ContactorsFlags.Open; */
          Contactors_Y.PreChargeContactors = Open;

          /* Outport: '<Root>/ContactorsState' */
          /* :  ContactorsState = ContState.PreCharging_PreChargeOpen; */
          Contactors_Y.ContactorsState = PreCharging_PreChargeOpen;

          /* Inport: '<Root>/PosContactors_FB' */
          /* :  sf_internal_predicateOutput = PosContactors_FB == 0; */
        } else if ((int32_T)Contactors_U.PosContactors_FB == 0) {
          /* :  PreTimeReset=0; */
          Contactors_B.PreTimeReset = 0.0;
          Contactors_DW.is_PreCharging_Substate_Old =
            Contactors_IN_PosContact_Check;

          /* Update for Outport: '<Root>/PosContactors' */
          /* :  PosContactors = ContactorsFlags.Closed; */
          Contactors_Y.PosContactors = Closed;

          /* Outport: '<Root>/ContactorsState' */
          /* :  ContactorsState = ContState.PreCharging_PosContactorClosed; */
          Contactors_Y.ContactorsState = PreCharging_PosContactorClosed;
        }
        break;

       case Contactors_IN_IntState15_o:
        /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.ContCutoffTime_msec; */
        if (Contactors_B.PreIntegratedTime >
            Contactors_U.Thresholds.ContCutoffTime_msec) {
          Contactors_DW.is_PreCharging_Substate_Old =
            Con_IN_ChangeState01_to_Opening;

          /* Inport: '<Root>/PosContactors_FB' */
          /* :  sf_internal_predicateOutput = PosContactors_FB == 0; */
        } else if ((int32_T)Contactors_U.PosContactors_FB == 0) {
          /* :  PreTimeReset=0; */
          Contactors_B.PreTimeReset = 0.0;
          Contactors_DW.is_PreCharging_Substate_Old =
            Contactors_IN_PosContact_Check;

          /* Update for Outport: '<Root>/PosContactors' */
          /* :  PosContactors = ContactorsFlags.Closed; */
          Contactors_Y.PosContactors = Closed;

          /* Outport: '<Root>/ContactorsState' */
          /* :  ContactorsState = ContState.PreCharging_PosContactorClosed; */
          Contactors_Y.ContactorsState = PreCharging_PosContactorClosed;
        }
        break;

       case Contactors_IN_IntState16_o:
        /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.ContCutoffTime_msec; */
        if (Contactors_B.PreIntegratedTime >
            Contactors_U.Thresholds.ContCutoffTime_msec) {
          /* :  PreTimeReset=0; */
          Contactors_B.PreTimeReset = 0.0;
          Contactors_DW.is_PreCharging_Substate_Old =
            Con_IN_ChangeState01_to_Opening;

          /* Inport: '<Root>/PreChargeContactors_FB' */
          /* :  sf_internal_predicateOutput = PreChargeContactors_FB ==0; */
        } else if ((int32_T)Contactors_U.PreChargeContactors_FB == 0) {
          /* :  PreTimeReset=0; */
          Contactors_B.PreTimeReset = 0.0;
          Contactors_DW.is_PreCharging_Substate_Old =
            Contactors_IN_PreCharge_Check02;

          /* Update for Outport: '<Root>/PreChargeContactors' */
          /* :  PreChargeContactors = ContactorsFlags.Open; */
          Contactors_Y.PreChargeContactors = Open;

          /* Outport: '<Root>/ContactorsState' */
          /* :  ContactorsState = ContState.PreCharging_PreChargeOpen; */
          Contactors_Y.ContactorsState = PreCharging_PreChargeOpen;
        }
        break;

       case Contactors_IN_IntState9_d:
        /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.ContCutoffTime_msec; */
        if (Contactors_B.PreIntegratedTime >
            Contactors_U.Thresholds.ContCutoffTime_msec) {
          /* :  PreTimeReset=0; */
          Contactors_DW.is_PreCharging_Substate_Old =
            Contactors_IN_PreCharge_Check01;

          /* :  PreTimeReset=1; */
          Contactors_B.PreTimeReset = 1.0;

          /* Update for Outport: '<Root>/PreChargeContactors' */
          /* :  PreChargeContactors = ContactorsFlags.Closed; */
          Contactors_Y.PreChargeContactors = Closed;

          /* Outport: '<Root>/ContactorsState' */
          /* :  ContactorsState = ContState.PreCharging_PreChargeClosed; */
          Contactors_Y.ContactorsState = PreCharging_PreChargeClosed;

          /* Inport: '<Root>/NegContactors_FB' */
          /* :  sf_internal_predicateOutput = NegContactors_FB == 0 || BMS_State==4; */
        } else if ((int32_T)Contactors_U.NegContactors_FB == 0) {
          /* :  PreTimeReset=0; */
          Contactors_B.PreTimeReset = 0.0;
          Contactors_DW.is_PreCharging_Substate_Old =
            Contactors_IN_NegContact_Check;

          /* Update for Outport: '<Root>/NegContactors' */
          /* :  NegContactors = ContactorsFlags.Closed; */
          Contactors_Y.NegContactors = Closed;

          /* Outport: '<Root>/ContactorsState' */
          /* :  ContactorsState = ContState.PreCharging_NegContactorClosed; */
          Contactors_Y.ContactorsState = PreCharging_NegContactorClosed;
        }
        break;

       case Contactors_IN_NegContact_Check:
        /* Update for Outport: '<Root>/NegContactors' */
        Contactors_Y.NegContactors = Closed;

        /* Outport: '<Root>/ContactorsState' */
        Contactors_Y.ContactorsState = PreCharging_NegContactorClosed;

        /* Inport: '<Root>/NegContactors_FB' */
        /* :  sf_internal_predicateOutput = NegContactors_FB == 1 && BMS_State~=4; */
        if ((int32_T)Contactors_U.NegContactors_FB == 1) {
          /* :  PreTimeReset = 1; */
          Contactors_B.PreTimeReset = 1.0;
          Contactors_DW.is_PreCharging_Substate_Old = Contactors_IN_IntState9_d;

          /* :  sf_internal_predicateOutput = NegContactors_FB == 0; */
        } else if ((int32_T)Contactors_U.NegContactors_FB == 0) {
          /* :  PreTimeReset=1; */
          Contactors_B.PreTimeReset = 1.0;
          Contactors_DW.is_PreCharging_Substate_Old = Contactors_IN_IntState10;
        }
        break;

       case Contactors_IN_PosContact_Check:
        /* Update for Outport: '<Root>/PosContactors' */
        Contactors_Y.PosContactors = Closed;

        /* Outport: '<Root>/ContactorsState' */
        Contactors_Y.ContactorsState = PreCharging_PosContactorClosed;

        /* Inport: '<Root>/PosContactors_FB' */
        /* :  sf_internal_predicateOutput = PosContactors_FB == 0; */
        if ((int32_T)Contactors_U.PosContactors_FB == 0) {
          /* :  PreTimeReset=1; */
          Contactors_B.PreTimeReset = 1.0;
          Contactors_DW.is_PreCharging_Substate_Old = Contactors_IN_IntState15_o;

          /* :  sf_internal_predicateOutput = PosContactors_FB == 1; */
        } else if ((int32_T)Contactors_U.PosContactors_FB == 1) {
          /* :  PreTimeReset=1; */
          Contactors_B.PreTimeReset = 1.0;
          Contactors_DW.is_PreCharging_Substate_Old = Contactors_IN_IntState14_i;
        }
        break;

       case Contactors_IN_PreCharge_Check01:
        {
          int32_T tmp;

          /* Update for Outport: '<Root>/PreChargeContactors' */
          Contactors_Y.PreChargeContactors = Closed;

          /* Outport: '<Root>/ContactorsState' */
          Contactors_Y.ContactorsState = PreCharging_PreChargeClosed;

          /* :  sf_internal_predicateOutput = abs(DataPipeline.Current_mA) <= Thresholds.ContCurrent_mA; */
          if (Contactors_U.DataPipeline.Current_mA < 0) {
            if (Contactors_U.DataPipeline.Current_mA <= MIN_int32_T) {
              tmp = MAX_int32_T;
            } else {
              tmp = -Contactors_U.DataPipeline.Current_mA;
            }
          } else {
            tmp = Contactors_U.DataPipeline.Current_mA;
          }

          if (tmp <= Contactors_U.Thresholds.ContCurrent_mA) {
            /* :  PreTimeReset=1; */
            Contactors_B.PreTimeReset = 1.0;
            Contactors_DW.is_PreCharging_Substate_Old = Contactors_IN_IntState13;

            /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.PreChargeRetryTimeout_msec; */
          } else if (Contactors_B.PreIntegratedTime >
                     Contactors_U.Thresholds.PreChargeRetryTimeout_msec) {
            /* :  PreChargeRetryCheck = PreChargeRetryCheck + 1; */
            tmp = (int32_T)(Contactors_DW.PreChargeRetryCheck + 1U);
            if (Contactors_DW.PreChargeRetryCheck + 1U > 255U) {
              tmp = 255;
            }

            Contactors_DW.PreChargeRetryCheck = (uint8_T)tmp;

            /* :  PreTimeReset=0; */
            Contactors_B.PreTimeReset = 0.0;
            Contactors_DW.is_PreCharging_Substate_Old =
              Contactors_IN_PreCharge_Check03;

            /* Update for Outport: '<Root>/PreChargeContactors' */
            /* :  PreChargeContactors = ContactorsFlags.Open; */
            Contactors_Y.PreChargeContactors = Open;

            /* Outport: '<Root>/ContactorsState' */
            /* :  ContactorsState = ContState.PreCharging_PreChargeRetry; */
            Contactors_Y.ContactorsState = PreCharging_PreChargeRetry;

            /* Inport: '<Root>/PreChargeContactors_FB' */
            /* :  sf_internal_predicateOutput = PreChargeContactors_FB ==0; */
          } else if ((int32_T)Contactors_U.PreChargeContactors_FB == 0) {
            /* :  PreTimeReset=1; */
            Contactors_B.PreTimeReset = 1.0;
            Contactors_DW.is_PreCharging_Substate_Old = Contactors_IN_IntState11;
          }
        }
        break;

       case Contactors_IN_PreCharge_Check02:
        /* Update for Outport: '<Root>/PreChargeContactors' */
        Contactors_Y.PreChargeContactors = Open;

        /* Outport: '<Root>/ContactorsState' */
        Contactors_Y.ContactorsState = PreCharging_PreChargeOpen;

        /* Inport: '<Root>/PreChargeContactors_FB' */
        /* :  sf_internal_predicateOutput = PreChargeContactors_FB == 0; */
        if ((int32_T)Contactors_U.PreChargeContactors_FB == 0) {
          /* :  PreTimeReset=1; */
          Contactors_B.PreTimeReset = 1.0;
          Contactors_DW.is_PreCharging_Substate_Old =
            Contactors_IN_NO_ACTIVE_CHILD;
          Contactors_DW.is_ContactorOperations = Contactors_IN_IntState14;

          /* :  sf_internal_predicateOutput = PreChargeContactors_FB ==1; */
        } else if ((int32_T)Contactors_U.PreChargeContactors_FB == 1) {
          /* :  PreTimeReset=1; */
          Contactors_B.PreTimeReset = 1.0;
          Contactors_DW.is_PreCharging_Substate_Old = Contactors_IN_IntState16_o;
        }
        break;

       default:
        /* Update for Outport: '<Root>/PreChargeContactors' */
        /* case IN_PreCharge_Check03: */
        Contactors_Y.PreChargeContactors = Open;

        /* Outport: '<Root>/ContactorsState' */
        Contactors_Y.ContactorsState = PreCharging_PreChargeRetry;

        /* :  sf_internal_predicateOutput = PreChargeRetryCheck > Thresholds.PreChargeRetryLimit; */
        if (Contactors_DW.PreChargeRetryCheck >
            Contactors_U.Thresholds.PreChargeRetryLimit) {
          /* :  PreTimeReset=1; */
          Contactors_B.PreTimeReset = 1.0;
          Contactors_DW.is_PreCharging_Substate_Old =
            Contactors_IN_NO_ACTIVE_CHILD;
          Contactors_DW.is_ContactorOperations = Contactors_IN_IntState15;

          /* :  sf_internal_predicateOutput = PreChargeRetryCheck <= Thresholds.PreChargeRetryLimit; */
        } else if (Contactors_DW.PreChargeRetryCheck <=
                   Contactors_U.Thresholds.PreChargeRetryLimit) {
          /* :  PreTimeReset=1; */
          Contactors_B.PreTimeReset = 1.0;
          Contactors_DW.is_PreCharging_Substate_Old = Contactors_IN_IntState12;
        }
        break;
      }
    }

    /* End of Inport: '<Root>/Thresholds' */
  }

  /* End of Inport: '<Root>/ProtectionOutput' */
}

/* Function for Chart: '<S1>/Contactor_StateMachine' */
static void Contactors_Open_Substate(void)
{
  /* Update for Outport: '<Root>/NegContactors' */
  Contactors_Y.NegContactors = Open;

  /* Update for Outport: '<Root>/PosContactors' */
  Contactors_Y.PosContactors = Open;

  /* Update for Outport: '<Root>/PreChargeContactors' */
  Contactors_Y.PreChargeContactors = Open;

  /* Outport: '<Root>/ContactorsWeldWarning' */
  Contactors_Y.ContactorsWeldWarning = None;

  /* Outport: '<Root>/ContactorsState' */
  Contactors_Y.ContactorsState = OpenSubstate;

  /* Inport: '<Root>/DataPipeline' */
  /* :  sf_internal_predicateOutput = Flags_detection == 1 && DataPipeline.PreChargeLogic == 1; */
  if (Contactors_B.Flags_Detection && Contactors_U.DataPipeline.PreChargeLogic)
  {
    /* :  PreTimeReset = 1; */
    Contactors_B.PreTimeReset = 1.0;
    Contactors_DW.is_ContactorOperations = Contactors_IN_IntState;

    /* Inport: '<Root>/ProtectionOutput' */
    /* :  sf_internal_predicateOutput = ProtectionOutput.ThermalRunaway== ProtectionFlags.Error|| ProtectionOutput.ShortCircuitDetect== ProtectionFlags.Error || DataPipeline.VCU.EmergencyDisconnectEn==1; */
  } else if ((Contactors_U.ProtectionOutput.ThermalRunaway == Error) ||
             (Contactors_U.ProtectionOutput.ShortCircuitDetect == Error) ||
             Contactors_U.DataPipeline.VCU.EmergencyDisconnectEn) {
    Contactors_DW.is_ContactorOperations = Contactors_IN_PermanentFail;

    /* Update for Outport: '<Root>/NegContactors' */
    /* :  NegContactors = ContactorsFlags.Open; */
    Contactors_Y.NegContactors = Open;

    /* Update for Outport: '<Root>/PosContactors' */
    /* :  PosContactors = ContactorsFlags.Open; */
    Contactors_Y.PosContactors = Open;

    /* Update for Outport: '<Root>/PreChargeContactors' */
    /* :  PreChargeContactors = ContactorsFlags.Open; */
    Contactors_Y.PreChargeContactors = Open;

    /* Outport: '<Root>/PermanentFaiil' */
    /* :  PermanentFail = ProtectionFlags.Error; */
    Contactors_Y.PermanentFaiil = Error;

    /* Outport: '<Root>/ContactorsState' */
    /* :  ContactorsState = ContState.PermanentFailure; */
    Contactors_Y.ContactorsState = PermanentFailure;

    /* :  sf_internal_predicateOutput = Flags_detection == 1 && DataPipeline.PreChargeLogic == 0; */
  } else if (Contactors_B.Flags_Detection &&
             (!Contactors_U.DataPipeline.PreChargeLogic)) {
    /* :  PreTimeReset=1; */
    Contactors_B.PreTimeReset = 1.0;
    Contactors_DW.is_ContactorOperations = Contactors_IN_IntState9;

    /* End of Inport: '<Root>/ProtectionOutput' */
  }

  /* End of Inport: '<Root>/DataPipeline' */
}

/* Function for Chart: '<S1>/Contactor_StateMachine' */
static void exit_internal_PreCharging_Subst(void)
{
  Contactors_DW.is_PreCharging_Substate_New = Contactors_IN_NO_ACTIVE_CHILD;
}

/* Function for Chart: '<S1>/Contactor_StateMachine' */
static void Contac_PreCharging_Substate_New(void)
{
  /* :  sf_internal_predicateOutput = duration(Flags_detection == 0,msec) > Thresholds.PreToOpeningSubstate_msec; */
  if (Contactors_B.Flags_Detection) {
    Contactors_DW.durationCounter_1_e = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  if (Contactors_DW.durationCounter_1_e * 100 >
      Contactors_U.Thresholds.PreToOpeningSubstate_msec) {
    exit_internal_PreCharging_Subst();
    Contactors_DW.is_ContactorOperations = Contacto_IN_NegContact_Check_02;

    /* Update for Outport: '<Root>/NegContactors' */
    /* :  NegContactors = ContactorsFlags.Open; */
    Contactors_Y.NegContactors = Open;

    /* Update for Outport: '<Root>/PosContactors' */
    /* :  PosContactors = ContactorsFlags.Open; */
    Contactors_Y.PosContactors = Open;

    /* Outport: '<Root>/ContactorsState' */
    /* :  ContactorsState = ContState.OpeningSubstate; */
    Contactors_Y.ContactorsState = OpeningSubstate;

    /* Inport: '<Root>/ProtectionOutput' incorporates:
     *  Inport: '<Root>/DataPipeline'
     */
    /* :  sf_internal_predicateOutput = ProtectionOutput.ThermalRunaway== ProtectionFlags.Error|| ProtectionOutput.ShortCircuitDetect== ProtectionFlags.Error || DataPipeline.VCU.EmergencyDisconnectEn==1; */
  } else if ((Contactors_U.ProtectionOutput.ThermalRunaway == Error) ||
             (Contactors_U.ProtectionOutput.ShortCircuitDetect == Error) ||
             Contactors_U.DataPipeline.VCU.EmergencyDisconnectEn) {
    exit_internal_PreCharging_Subst();
    Contactors_DW.is_ContactorOperations = Contactors_IN_PermanentFail;

    /* Update for Outport: '<Root>/NegContactors' */
    /* :  NegContactors = ContactorsFlags.Open; */
    Contactors_Y.NegContactors = Open;

    /* Update for Outport: '<Root>/PosContactors' */
    /* :  PosContactors = ContactorsFlags.Open; */
    Contactors_Y.PosContactors = Open;

    /* Update for Outport: '<Root>/PreChargeContactors' */
    /* :  PreChargeContactors = ContactorsFlags.Open; */
    Contactors_Y.PreChargeContactors = Open;

    /* Outport: '<Root>/PermanentFaiil' */
    /* :  PermanentFail = ProtectionFlags.Error; */
    Contactors_Y.PermanentFaiil = Error;

    /* Outport: '<Root>/ContactorsState' */
    /* :  ContactorsState = ContState.PermanentFailure; */
    Contactors_Y.ContactorsState = PermanentFailure;
  } else {
    switch (Contactors_DW.is_PreCharging_Substate_New) {
     case Con_IN_ChangeState01_to_Opening:
      Contactors_DW.is_PreCharging_Substate_New = Contactors_IN_NO_ACTIVE_CHILD;
      Contactors_DW.is_ContactorOperations = Contacto_IN_NegContact_Check_02;

      /* Update for Outport: '<Root>/NegContactors' */
      /* :  NegContactors = ContactorsFlags.Open; */
      Contactors_Y.NegContactors = Open;

      /* Update for Outport: '<Root>/PosContactors' */
      /* :  PosContactors = ContactorsFlags.Open; */
      Contactors_Y.PosContactors = Open;

      /* Outport: '<Root>/ContactorsState' */
      /* :  ContactorsState = ContState.OpeningSubstate; */
      Contactors_Y.ContactorsState = OpeningSubstate;
      break;

     case Contactors_IN_IntState1:
      /* Inport: '<Root>/NegContactors_FB' */
      /* :  sf_internal_predicateOutput = NegContactors_FB == 0; */
      if ((int32_T)Contactors_U.NegContactors_FB == 0) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.is_PreCharging_Substate_New =
          Contactors_IN_NegContact_Check;

        /* Update for Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Closed; */
        Contactors_Y.NegContactors = Closed;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_NegContactorClosed; */
        Contactors_Y.ContactorsState = PreCharging_NegContactorClosed;

        /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.ContCutoffTime_msec; */
      } else if (Contactors_B.PreIntegratedTime >
                 Contactors_U.Thresholds.ContCutoffTime_msec) {
        /* :  PreTimeReset=0; */
        Contactors_DW.is_PreCharging_Substate_New =
          Contactors_IN_PreCharge_Check01;

        /* :  PreTimeReset = 1; */
        Contactors_B.PreTimeReset = 1.0;

        /* Update for Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Closed; */
        Contactors_Y.PreChargeContactors = Closed;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_PreChargeClosed; */
        Contactors_Y.ContactorsState = PreCharging_PreChargeClosed;
      }
      break;

     case Contactors_IN_IntState2:
      /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.PreChargeRecoveryTimeout_msec; */
      if (Contactors_B.PreIntegratedTime >
          Contactors_U.Thresholds.PreChargeRecoveryTimeout_msec) {
        /* :  PreTimeReset=0 */
        Contactors_DW.is_PreCharging_Substate_New =
          Contactors_IN_PreCharge_Check01;

        /* :  PreTimeReset = 1; */
        Contactors_B.PreTimeReset = 1.0;

        /* Update for Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Closed; */
        Contactors_Y.PreChargeContactors = Closed;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_PreChargeClosed; */
        Contactors_Y.ContactorsState = PreCharging_PreChargeClosed;

        /* :  sf_internal_predicateOutput = PreChargeRetryCheck > Thresholds.PreChargeRetryLimit; */
      } else if (Contactors_DW.PreChargeRetryCheck >
                 Contactors_U.Thresholds.PreChargeRetryLimit) {
        Contactors_DW.is_PreCharging_Substate_New =
          Contactors_IN_PreCharge_Check03;

        /* :  PreTimeReset=1; */
        Contactors_B.PreTimeReset = 1.0;

        /* Update for Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_PreChargeRetry; */
        Contactors_Y.ContactorsState = PreCharging_PreChargeRetry;
      }
      break;

     case Contactors_IN_IntState3:
      {
        /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.PreChargeTimeout_msec; */
        if (Contactors_B.PreIntegratedTime >
            Contactors_U.Thresholds.PreChargeTimeout_msec) {
          /* :  PreTimeReset=0; */
          Contactors_B.PreTimeReset = 0.0;
          Contactors_DW.is_PreCharging_Substate_New =
            Contactors_IN_PosContact_Check;

          /* Update for Outport: '<Root>/PosContactors' */
          /* :  PosContactors = ContactorsFlags.Closed; */
          Contactors_Y.PosContactors = Closed;

          /* Outport: '<Root>/ContactorsState' */
          /* :  ContactorsState = ContState.PreCharging_PosContactorClosed; */
          Contactors_Y.ContactorsState = PreCharging_PosContactorClosed;
        } else {
          int32_T tmp;

          /* :  sf_internal_predicateOutput = abs(DataPipeline.Current_mA) > Thresholds.PreChargeEndMaxCurrent_mA || VoltageDelta_mV > Thresholds.PreChargeVoltageDelta_mV; */
          if (Contactors_U.DataPipeline.Current_mA < 0) {
            if (Contactors_U.DataPipeline.Current_mA <= MIN_int32_T) {
              tmp = MAX_int32_T;
            } else {
              tmp = -Contactors_U.DataPipeline.Current_mA;
            }
          } else {
            tmp = Contactors_U.DataPipeline.Current_mA;
          }

          if ((tmp > Contactors_U.Thresholds.PreChargeEndMaxCurrent_mA) ||
              (Contactors_B.Add >
               Contactors_U.Thresholds.PreChargeVoltageDelta_mV)) {
            /* :  PreTimeReset=0; */
            Contactors_DW.is_PreCharging_Substate_New =
              Contactors_IN_PreCharge_Check01;

            /* :  PreTimeReset = 1; */
            Contactors_B.PreTimeReset = 1.0;

            /* Update for Outport: '<Root>/PreChargeContactors' */
            /* :  PreChargeContactors = ContactorsFlags.Closed; */
            Contactors_Y.PreChargeContactors = Closed;

            /* Outport: '<Root>/ContactorsState' */
            /* :  ContactorsState = ContState.PreCharging_PreChargeClosed; */
            Contactors_Y.ContactorsState = PreCharging_PreChargeClosed;
          }
        }
      }
      break;

     case Contactors_IN_IntState4:
      /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.ContCutoffTime_msec; */
      if (Contactors_B.PreIntegratedTime >
          Contactors_U.Thresholds.ContCutoffTime_msec) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.is_PreCharging_Substate_New =
          Con_IN_ChangeState01_to_Opening;

        /* Inport: '<Root>/NegContactors_FB' */
        /* :  sf_internal_predicateOutput = NegContactors_FB==1; */
      } else if ((int32_T)Contactors_U.NegContactors_FB == 1) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.is_PreCharging_Substate_New =
          Contactors_IN_NegContact_Check;

        /* Update for Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Closed; */
        Contactors_Y.NegContactors = Closed;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_NegContactorClosed; */
        Contactors_Y.ContactorsState = PreCharging_NegContactorClosed;
      }
      break;

     case Contactors_IN_IntState5:
      /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.ContCutoffTime_msec; */
      if (Contactors_B.PreIntegratedTime >
          Contactors_U.Thresholds.ContCutoffTime_msec) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.is_PreCharging_Substate_New =
          Con_IN_ChangeState01_to_Opening;

        /* Inport: '<Root>/PreChargeContactors_FB' */
        /* :  sf_internal_predicateOutput = PreChargeContactors_FB==1; */
      } else if ((int32_T)Contactors_U.PreChargeContactors_FB == 1) {
        /* :  PreTimeReset=0; */
        Contactors_DW.is_PreCharging_Substate_New =
          Contactors_IN_PreCharge_Check01;

        /* :  PreTimeReset = 1; */
        Contactors_B.PreTimeReset = 1.0;

        /* Update for Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Closed; */
        Contactors_Y.PreChargeContactors = Closed;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_PreChargeClosed; */
        Contactors_Y.ContactorsState = PreCharging_PreChargeClosed;
      }
      break;

     case Contactors_IN_IntState6:
      /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.ContCutoffTime_msec; */
      if (Contactors_B.PreIntegratedTime >
          Contactors_U.Thresholds.ContCutoffTime_msec) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.is_PreCharging_Substate_New =
          Con_IN_ChangeState01_to_Opening;

        /* Inport: '<Root>/PosContactors_FB' */
        /* :  sf_internal_predicateOutput = PosContactors_FB == 1; */
      } else if ((int32_T)Contactors_U.PosContactors_FB == 1) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.is_PreCharging_Substate_New =
          Contactors_IN_PosContact_Check;

        /* Update for Outport: '<Root>/PosContactors' */
        /* :  PosContactors = ContactorsFlags.Closed; */
        Contactors_Y.PosContactors = Closed;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_PosContactorClosed; */
        Contactors_Y.ContactorsState = PreCharging_PosContactorClosed;
      }
      break;

     case Contactors_IN_IntState7:
      /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.ContCutoffTime_msec; */
      if (Contactors_B.PreIntegratedTime >
          Contactors_U.Thresholds.ContCutoffTime_msec) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.is_PreCharging_Substate_New =
          Con_IN_ChangeState01_to_Opening;

        /* Inport: '<Root>/PreChargeContactors_FB' */
        /* :  sf_internal_predicateOutput = PreChargeContactors_FB==0; */
      } else if ((int32_T)Contactors_U.PreChargeContactors_FB == 0) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.is_PreCharging_Substate_New =
          Contactors_IN_PreCharge_Check02;

        /* Update for Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_PreChargeOpen; */
        Contactors_Y.ContactorsState = PreCharging_PreChargeOpen;
      }
      break;

     case Contactors_IN_IntState8_n:
      /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.ContCutoffTime_msec; */
      if (Contactors_B.PreIntegratedTime >
          Contactors_U.Thresholds.ContCutoffTime_msec) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.is_PreCharging_Substate_New =
          Contactors_IN_PreCharge_Check02;

        /* Update for Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_PreChargeOpen; */
        Contactors_Y.ContactorsState = PreCharging_PreChargeOpen;

        /* Inport: '<Root>/PosContactors_FB' */
        /* :  sf_internal_predicateOutput = PosContactors_FB==0; */
      } else if ((int32_T)Contactors_U.PosContactors_FB == 0) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.is_PreCharging_Substate_New =
          Contactors_IN_PosContact_Check;

        /* Update for Outport: '<Root>/PosContactors' */
        /* :  PosContactors = ContactorsFlags.Closed; */
        Contactors_Y.PosContactors = Closed;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_PosContactorClosed; */
        Contactors_Y.ContactorsState = PreCharging_PosContactorClosed;
      }
      break;

     case Contactors_IN_NegContact_Check:
      /* Update for Outport: '<Root>/NegContactors' */
      Contactors_Y.NegContactors = Closed;

      /* Outport: '<Root>/ContactorsState' */
      Contactors_Y.ContactorsState = PreCharging_NegContactorClosed;

      /* Inport: '<Root>/NegContactors_FB' */
      /* :  sf_internal_predicateOutput = NegContactors_FB == 0; */
      if ((int32_T)Contactors_U.NegContactors_FB == 0) {
        /* :  PreTimeReset=1; */
        Contactors_B.PreTimeReset = 1.0;
        Contactors_DW.is_PreCharging_Substate_New = Contactors_IN_IntState4;

        /* :  sf_internal_predicateOutput = NegContactors_FB == 1; */
      } else if ((int32_T)Contactors_U.NegContactors_FB == 1) {
        /* :  PreTimeReset = 1; */
        Contactors_B.PreTimeReset = 1.0;
        Contactors_DW.is_PreCharging_Substate_New = Contactors_IN_IntState1;
      }
      break;

     case Contactors_IN_PosContact_Check:
      /* Update for Outport: '<Root>/PosContactors' */
      Contactors_Y.PosContactors = Closed;

      /* Outport: '<Root>/ContactorsState' */
      Contactors_Y.ContactorsState = PreCharging_PosContactorClosed;

      /* Inport: '<Root>/PosContactors_FB' */
      /* :  sf_internal_predicateOutput = PosContactors_FB == 1; */
      if ((int32_T)Contactors_U.PosContactors_FB == 1) {
        /* :  PreTimeReset=1; */
        Contactors_B.PreTimeReset = 1.0;
        Contactors_DW.is_PreCharging_Substate_New = Contactors_IN_IntState8_n;

        /* :  sf_internal_predicateOutput = PosContactors_FB == 0; */
      } else if ((int32_T)Contactors_U.PosContactors_FB == 0) {
        /* :  PreTimeReset=1; */
        Contactors_B.PreTimeReset = 1.0;
        Contactors_DW.is_PreCharging_Substate_New = Contactors_IN_IntState6;
      }
      break;

     case Contactors_IN_PreCharge_Check01:
      {
        /* Update for Outport: '<Root>/PreChargeContactors' */
        Contactors_Y.PreChargeContactors = Closed;

        /* Outport: '<Root>/ContactorsState' */
        Contactors_Y.ContactorsState = PreCharging_PreChargeClosed;

        /* Inport: '<Root>/PreChargeContactors_FB' */
        /* :  sf_internal_predicateOutput = PreChargeContactors_FB ==0; */
        if ((int32_T)Contactors_U.PreChargeContactors_FB == 0) {
          /* :  PreTimeReset=1; */
          Contactors_B.PreTimeReset = 1.0;
          Contactors_DW.is_PreCharging_Substate_New = Contactors_IN_IntState5;
        } else {
          int32_T tmp;

          /* :  sf_internal_predicateOutput = abs(DataPipeline.Current_mA) < Thresholds.PreChargeEndMaxCurrent_mA && VoltageDelta_mV < Thresholds.PreChargeVoltageDelta_mV; */
          if (Contactors_U.DataPipeline.Current_mA < 0) {
            if (Contactors_U.DataPipeline.Current_mA <= MIN_int32_T) {
              tmp = MAX_int32_T;
            } else {
              tmp = -Contactors_U.DataPipeline.Current_mA;
            }
          } else {
            tmp = Contactors_U.DataPipeline.Current_mA;
          }

          if ((tmp < Contactors_U.Thresholds.PreChargeEndMaxCurrent_mA) &&
              (Contactors_B.Add <
               Contactors_U.Thresholds.PreChargeVoltageDelta_mV)) {
            /* :  PreTimeReset=1; */
            Contactors_B.PreTimeReset = 1.0;
            Contactors_DW.is_PreCharging_Substate_New = Contactors_IN_IntState3;

            /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.PreChargeRetryTimeout_msec; */
          } else if (Contactors_B.PreIntegratedTime >
                     Contactors_U.Thresholds.PreChargeRetryTimeout_msec) {
            /* :  PreChargeRetryCheck = PreChargeRetryCheck + 1; */
            tmp = (int32_T)(Contactors_DW.PreChargeRetryCheck + 1U);
            if (Contactors_DW.PreChargeRetryCheck + 1U > 255U) {
              tmp = 255;
            }

            Contactors_DW.PreChargeRetryCheck = (uint8_T)tmp;

            /* :  PreTimeReset=0; */
            Contactors_DW.is_PreCharging_Substate_New =
              Contactors_IN_PreCharge_Check03;

            /* :  PreTimeReset=1; */
            Contactors_B.PreTimeReset = 1.0;

            /* Update for Outport: '<Root>/PreChargeContactors' */
            /* :  PreChargeContactors = ContactorsFlags.Open; */
            Contactors_Y.PreChargeContactors = Open;

            /* Outport: '<Root>/ContactorsState' */
            /* :  ContactorsState = ContState.PreCharging_PreChargeRetry; */
            Contactors_Y.ContactorsState = PreCharging_PreChargeRetry;
          }
        }
      }
      break;

     case Contactors_IN_PreCharge_Check02:
      /* Update for Outport: '<Root>/PreChargeContactors' */
      Contactors_Y.PreChargeContactors = Open;

      /* Outport: '<Root>/ContactorsState' */
      Contactors_Y.ContactorsState = PreCharging_PreChargeOpen;

      /* Inport: '<Root>/PreChargeContactors_FB' */
      /* :  sf_internal_predicateOutput = PreChargeContactors_FB == 0; */
      if ((int32_T)Contactors_U.PreChargeContactors_FB == 0) {
        /* :  PreTimeReset=1; */
        Contactors_B.PreTimeReset = 1.0;
        Contactors_DW.is_PreCharging_Substate_New =
          Contactors_IN_NO_ACTIVE_CHILD;
        Contactors_DW.is_ContactorOperations = Contactors_IN_IntState8;

        /* :  sf_internal_predicateOutput = PreChargeContactors_FB ==1; */
      } else if ((int32_T)Contactors_U.PreChargeContactors_FB == 1) {
        /* :  PreTimeReset=1; */
        Contactors_B.PreTimeReset = 1.0;
        Contactors_DW.is_PreCharging_Substate_New = Contactors_IN_IntState7;
      }
      break;

     default:
      /* Update for Outport: '<Root>/PreChargeContactors' */
      /* case IN_PreCharge_Check03: */
      Contactors_Y.PreChargeContactors = Open;

      /* Outport: '<Root>/ContactorsState' */
      Contactors_Y.ContactorsState = PreCharging_PreChargeRetry;

      /* :  sf_internal_predicateOutput = PreChargeRetryCheck > Thresholds.PreChargeRetryLimit; */
      if (Contactors_DW.PreChargeRetryCheck >
          Contactors_U.Thresholds.PreChargeRetryLimit) {
        /* :  PreTimeReset=1; */
        Contactors_B.PreTimeReset = 1.0;
        Contactors_DW.is_PreCharging_Substate_New =
          Contactors_IN_NO_ACTIVE_CHILD;
        Contactors_DW.is_ContactorOperations = Contactors_IN_IntState16;

        /* :  sf_internal_predicateOutput = PreChargeRetryCheck <= Thresholds.PreChargeRetryLimit; */
      } else if (Contactors_DW.PreChargeRetryCheck <=
                 Contactors_U.Thresholds.PreChargeRetryLimit) {
        /* :  PreTimeReset=1; */
        Contactors_B.PreTimeReset = 1.0;
        Contactors_DW.is_PreCharging_Substate_New = Contactors_IN_IntState2;
      }
      break;
    }

    /* End of Inport: '<Root>/ProtectionOutput' */
  }

  /* End of Inport: '<Root>/Thresholds' */
}

/* Model step function */
void Contactors_step(void)
{
  int32_T i;
  int32_T rtb_SumofElements;
  boolean_T rtb_NOT;
  ProtectionFlags tmp;
  ProtectionFlags tmp_0;
  ProtectionFlags tmp_1;

  /* Logic: '<S1>/NOT' incorporates:
   *  UnitDelay: '<S1>/Unit Delay'
   */
  rtb_NOT = (Contactors_DW.UnitDelay_DSTATE == 0.0);

  /* DiscreteIntegrator: '<S1>/Discrete-Time Integrator' */
  if (rtb_NOT || (Contactors_DW.DiscreteTimeIntegrator_PrevRese != 0)) {
    Contactors_DW.DiscreteTimeIntegrator_DSTATE = 0.0;
  }

  /* DiscreteIntegrator: '<S1>/Discrete-Time Integrator' */
  Contactors_B.PreIntegratedTime = Contactors_DW.DiscreteTimeIntegrator_DSTATE;

  /* Sum: '<S1>/Sum of Elements' incorporates:
   *  Inport: '<Root>/DataPipeline'
   */
  rtb_SumofElements = 0;
  for (i = 0; i < 20; i++) {
    rtb_SumofElements += Contactors_U.DataPipeline.VoltageSenseBus.Voltages_mV[i];
  }

  /* End of Sum: '<S1>/Sum of Elements' */

  /* Sum: '<S1>/Add' incorporates:
   *  Inport: '<Root>/TerminalVoltage_mV'
   */
  Contactors_B.Add = rtb_SumofElements - Contactors_U.TerminalVoltage_mV;

  /* Switch: '<S1>/Switch' incorporates:
   *  Inport: '<Root>/ProtectionOutput'
   *  Inport: '<Root>/Thresholds'
   *  Switch: '<S1>/Switch1'
   */
  if (Contactors_U.Thresholds.AdditionalProtectionEn) {
    tmp = Contactors_U.ProtectionOutput.TemperatureGradient;
    tmp_0 = Contactors_U.ProtectionOutput.SuddenVoltageDrop;
  } else {
    tmp = Contactors_ConstB.DataTypeConversion9;
    tmp_0 = Contactors_ConstB.DataTypeConversion9;
  }

  /* Switch: '<S1>/Switch2' incorporates:
   *  Inport: '<Root>/ProtectionOutput'
   *  Inport: '<Root>/Thresholds'
   */
  if (Contactors_U.Thresholds.HighImbalanceFlagEn) {
    tmp_1 = Contactors_U.ProtectionOutput.HighImbalanceFlag;
  } else {
    tmp_1 = Contactors_ConstB.DataTypeConversion9;
  }

  /* Logic: '<S1>/AND' incorporates:
   *  Constant: '<S10>/Constant'
   *  Constant: '<S11>/Constant'
   *  Constant: '<S12>/Constant'
   *  Constant: '<S13>/Constant'
   *  Constant: '<S14>/Constant'
   *  Constant: '<S15>/Constant'
   *  Constant: '<S16>/Constant'
   *  Constant: '<S17>/Constant'
   *  Constant: '<S18>/Constant'
   *  Constant: '<S19>/Constant'
   *  Constant: '<S20>/Constant'
   *  Constant: '<S21>/Constant'
   *  Constant: '<S22>/Constant'
   *  Constant: '<S24>/Constant'
   *  Constant: '<S25>/Constant'
   *  Constant: '<S26>/Constant'
   *  Constant: '<S27>/Constant'
   *  Constant: '<S28>/Constant'
   *  Constant: '<S29>/Constant'
   *  Constant: '<S2>/Constant'
   *  Constant: '<S30>/Constant'
   *  Constant: '<S31>/Constant'
   *  Constant: '<S32>/Constant'
   *  Constant: '<S3>/Constant'
   *  Constant: '<S4>/Constant'
   *  Constant: '<S5>/Constant'
   *  Constant: '<S6>/Constant'
   *  Constant: '<S7>/Constant'
   *  Constant: '<S8>/Constant'
   *  Constant: '<S9>/Constant'
   *  DataTypeConversion: '<S1>/Data Type Conversion'
   *  DataTypeConversion: '<S1>/Data Type Conversion1'
   *  DataTypeConversion: '<S1>/Data Type Conversion10'
   *  DataTypeConversion: '<S1>/Data Type Conversion11'
   *  DataTypeConversion: '<S1>/Data Type Conversion12'
   *  DataTypeConversion: '<S1>/Data Type Conversion13'
   *  DataTypeConversion: '<S1>/Data Type Conversion14'
   *  DataTypeConversion: '<S1>/Data Type Conversion15'
   *  DataTypeConversion: '<S1>/Data Type Conversion16'
   *  DataTypeConversion: '<S1>/Data Type Conversion17'
   *  DataTypeConversion: '<S1>/Data Type Conversion18'
   *  DataTypeConversion: '<S1>/Data Type Conversion19'
   *  DataTypeConversion: '<S1>/Data Type Conversion2'
   *  DataTypeConversion: '<S1>/Data Type Conversion20'
   *  DataTypeConversion: '<S1>/Data Type Conversion21'
   *  DataTypeConversion: '<S1>/Data Type Conversion22'
   *  DataTypeConversion: '<S1>/Data Type Conversion23'
   *  DataTypeConversion: '<S1>/Data Type Conversion24'
   *  DataTypeConversion: '<S1>/Data Type Conversion25'
   *  DataTypeConversion: '<S1>/Data Type Conversion3'
   *  DataTypeConversion: '<S1>/Data Type Conversion4'
   *  DataTypeConversion: '<S1>/Data Type Conversion5'
   *  DataTypeConversion: '<S1>/Data Type Conversion6'
   *  DataTypeConversion: '<S1>/Data Type Conversion7'
   *  DataTypeConversion: '<S1>/Data Type Conversion8'
   *  Inport: '<Root>/Communication_Flag'
   *  Inport: '<Root>/ContactorCommand'
   *  Inport: '<Root>/ContactorError_Flag'
   *  Inport: '<Root>/PermanentFail_FB'
   *  Inport: '<Root>/ProtectionOutput'
   *  Inport: '<Root>/SC_Flag'
   *  Logic: '<S1>/AND2'
   *  Logic: '<S1>/NAND'
   *  Logic: '<S1>/NOR'
   *  Logic: '<S1>/OR'
   *  Memory: '<S1>/Memory'
   *  Memory: '<S1>/Memory1'
   *  Memory: '<S1>/Memory2'
   *  RelationalOperator: '<S10>/Compare'
   *  RelationalOperator: '<S11>/Compare'
   *  RelationalOperator: '<S12>/Compare'
   *  RelationalOperator: '<S13>/Compare'
   *  RelationalOperator: '<S14>/Compare'
   *  RelationalOperator: '<S15>/Compare'
   *  RelationalOperator: '<S16>/Compare'
   *  RelationalOperator: '<S17>/Compare'
   *  RelationalOperator: '<S18>/Compare'
   *  RelationalOperator: '<S19>/Compare'
   *  RelationalOperator: '<S20>/Compare'
   *  RelationalOperator: '<S21>/Compare'
   *  RelationalOperator: '<S22>/Compare'
   *  RelationalOperator: '<S24>/Compare'
   *  RelationalOperator: '<S25>/Compare'
   *  RelationalOperator: '<S26>/Compare'
   *  RelationalOperator: '<S27>/Compare'
   *  RelationalOperator: '<S28>/Compare'
   *  RelationalOperator: '<S29>/Compare'
   *  RelationalOperator: '<S2>/Compare'
   *  RelationalOperator: '<S30>/Compare'
   *  RelationalOperator: '<S31>/Compare'
   *  RelationalOperator: '<S32>/Compare'
   *  RelationalOperator: '<S3>/Compare'
   *  RelationalOperator: '<S4>/Compare'
   *  RelationalOperator: '<S5>/Compare'
   *  RelationalOperator: '<S6>/Compare'
   *  RelationalOperator: '<S7>/Compare'
   *  RelationalOperator: '<S8>/Compare'
   *  RelationalOperator: '<S9>/Compare'
   *  Switch: '<S1>/Switch'
   *  Switch: '<S1>/Switch1'
   *  Switch: '<S1>/Switch2'
   */
  Contactors_B.Flags_Detection = (((int32_T)
    Contactors_U.ProtectionOutput.DCLI_CurrentFlag != 2) && ((int32_T)
    Contactors_U.ProtectionOutput.DCLO_CurrentFlag != 2) && ((int32_T)
    Contactors_U.ProtectionOutput.OCC != 2) && ((int32_T)
    Contactors_U.ProtectionOutput.OCD != 2) && ((int32_T)
    Contactors_U.ProtectionOutput.UV != 2) && ((int32_T)
    Contactors_U.ProtectionOutput.OV != 2) && ((int32_T)
    Contactors_U.ProtectionOutput.eFuseChargeFlag < 2) && ((int32_T)
    Contactors_U.ProtectionOutput.eFuseDischargeFlag < 2) && ((int32_T)
    Contactors_U.ProtectionOutput.TempOverallState != 2) && ((int32_T)
    Contactors_U.ProtectionOutput.ShortCircuitDetect != 2) && ((int32_T)
    Contactors_U.ProtectionOutput.ThermalRunaway != 2) && ((int32_T)tmp_1 != 2) &&
    ((int32_T)tmp != 2) && ((int32_T)tmp_0 != 2) && (Contactors_U.SC_Flag != 2) &&
    (Contactors_U.Communication_Flag != 2) && (Contactors_U.ContactorError_Flag
    != 2) && ((int32_T)Contactors_U.PermanentFail_Flag != 2) &&
    (Contactors_U.ContactorCommand == 1) && ((Contactors_U.ContactorCommand != 0)
    || ((int32_T)Contactors_DW.Memory_PreviousInput != 0) || ((int32_T)
    Contactors_DW.Memory1_PreviousInput != 0) || ((int32_T)
    Contactors_DW.Memory2_PreviousInput != 0) || (((int32_T)
    Contactors_U.ProtectionOutput.OCC != 1) && ((int32_T)
    Contactors_U.ProtectionOutput.OCD != 1) && ((int32_T)
    Contactors_U.ProtectionOutput.eFuseChargeFlag != 1) && ((int32_T)
    Contactors_U.ProtectionOutput.eFuseDischargeFlag != 1) && ((int32_T)
    Contactors_U.ProtectionOutput.UV != 1) && ((int32_T)
    Contactors_U.ProtectionOutput.OV != 1) && ((int32_T)
    Contactors_U.ProtectionOutput.TempOverallState != 1))));

  /* Chart: '<S1>/Contactor_StateMachine' incorporates:
   *  Inport: '<Root>/DataPipeline'
   *  Inport: '<Root>/PermanentFailRecoveryTrigger'
   *  Inport: '<Root>/PreChargeContactors_FB'
   *  Inport: '<Root>/ProtectionOutput'
   *  Inport: '<Root>/Thresholds'
   */
  if (Contactors_DW.is_active_c1_Contactors == 0U) {
    Contactors_DW.is_active_c1_Contactors = 1U;
    Contactors_DW.is_ContactorOperations = Contactors_IN_Open_Substate;

    /* Outport: '<Root>/NegContactors' */
    /* :  NegContactors = ContactorsFlags.Open; */
    Contactors_Y.NegContactors = Open;

    /* Outport: '<Root>/PosContactors' */
    /* :  PosContactors = ContactorsFlags.Open; */
    Contactors_Y.PosContactors = Open;

    /* Outport: '<Root>/PreChargeContactors' */
    /* :  PreChargeContactors = ContactorsFlags.Open; */
    Contactors_Y.PreChargeContactors = Open;

    /* Outport: '<Root>/ContactorsWeldWarning' */
    /* :  ContactorsWeldWarning = ContactorsFlags.None; */
    Contactors_Y.ContactorsWeldWarning = None;

    /* Outport: '<Root>/ContactorsState' */
    /* :  ContactorsState = ContState.OpenSubstate; */
    Contactors_Y.ContactorsState = OpenSubstate;
  } else {
    switch (Contactors_DW.is_ContactorOperations) {
     case Contactors_IN_Closed_Substate:
      /* Outport: '<Root>/NegContactors' */
      Contactors_Y.NegContactors = Closed;

      /* Outport: '<Root>/PosContactors' */
      Contactors_Y.PosContactors = Closed;

      /* Outport: '<Root>/PreChargeContactors' */
      Contactors_Y.PreChargeContactors = Open;

      /* Outport: '<Root>/ContactorsWeldWarning' */
      Contactors_Y.ContactorsWeldWarning = None;

      /* Outport: '<Root>/ContactorsState' */
      Contactors_Y.ContactorsState = ClosedSubstate;

      /* :  sf_internal_predicateOutput = ProtectionOutput.ThermalRunaway== ProtectionFlags.Error|| ProtectionOutput.ShortCircuitDetect== ProtectionFlags.Error || DataPipeline.VCU.EmergencyDisconnectEn==1; */
      if ((Contactors_U.ProtectionOutput.ThermalRunaway == Error) ||
          (Contactors_U.ProtectionOutput.ShortCircuitDetect == Error) ||
          Contactors_U.DataPipeline.VCU.EmergencyDisconnectEn) {
        Contactors_DW.is_ContactorOperations = Contactors_IN_PermanentFail;

        /* Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Open; */
        Contactors_Y.NegContactors = Open;

        /* Outport: '<Root>/PosContactors' */
        /* :  PosContactors = ContactorsFlags.Open; */
        Contactors_Y.PosContactors = Open;

        /* Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* Outport: '<Root>/PermanentFaiil' */
        /* :  PermanentFail = ProtectionFlags.Error; */
        Contactors_Y.PermanentFaiil = Error;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PermanentFailure; */
        Contactors_Y.ContactorsState = PermanentFailure;
      } else {
        /* :  sf_internal_predicateOutput = duration(Flags_detection == 0, msec) > Thresholds.FlagDebounceTime_msec; */
        if (Contactors_B.Flags_Detection) {
          Contactors_DW.durationCounter_1_i = 0;
        }

        if (Contactors_DW.durationCounter_1_i * 100 >
            Contactors_U.Thresholds.FlagDebounceTime_msec) {
          Contactors_DW.is_ContactorOperations = Contacto_IN_NegContact_Check_02;

          /* Outport: '<Root>/NegContactors' */
          /* :  NegContactors = ContactorsFlags.Open; */
          Contactors_Y.NegContactors = Open;

          /* Outport: '<Root>/PosContactors' */
          /* :  PosContactors = ContactorsFlags.Open; */
          Contactors_Y.PosContactors = Open;

          /* Outport: '<Root>/ContactorsState' */
          /* :  ContactorsState = ContState.OpeningSubstate; */
          Contactors_Y.ContactorsState = OpeningSubstate;
        }
      }
      break;

     case Contactors_IN_IntState:
      /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.OpenToPreChargeStateTime_msec; */
      if (Contactors_B.PreIntegratedTime >
          Contactors_U.Thresholds.OpenToPreChargeStateTime_msec) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.durationCounter_1_e = 0;
        Contactors_DW.is_ContactorOperations = Con_IN_PreCharging_Substate_New;
        Contactors_DW.is_PreCharging_Substate_New =
          Contactors_IN_NegContact_Check;

        /* Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Closed; */
        Contactors_Y.NegContactors = Closed;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_NegContactorClosed; */
        Contactors_Y.ContactorsState = PreCharging_NegContactorClosed;

        /* :  sf_internal_predicateOutput = Flags_detection == 0 || DataPipeline.PreChargeLogic == 0; */
      } else if ((!Contactors_B.Flags_Detection) ||
                 (!Contactors_U.DataPipeline.PreChargeLogic)) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.is_ContactorOperations = Contactors_IN_Open_Substate;

        /* Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Open; */
        Contactors_Y.NegContactors = Open;

        /* Outport: '<Root>/PosContactors' */
        /* :  PosContactors = ContactorsFlags.Open; */
        Contactors_Y.PosContactors = Open;

        /* Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* Outport: '<Root>/ContactorsWeldWarning' */
        /* :  ContactorsWeldWarning = ContactorsFlags.None; */
        Contactors_Y.ContactorsWeldWarning = None;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.OpenSubstate; */
        Contactors_Y.ContactorsState = OpenSubstate;
      }
      break;

     case Contactors_IN_IntState14:
      /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.PreToClosed_Time_msec; */
      if (Contactors_B.PreIntegratedTime >
          Contactors_U.Thresholds.PreToClosed_Time_msec) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.durationCounter_1_i = 0;
        Contactors_DW.is_ContactorOperations = Contactors_IN_Closed_Substate;

        /* Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Closed; */
        Contactors_Y.NegContactors = Closed;

        /* Outport: '<Root>/PosContactors' */
        /* :  PosContactors = ContactorsFlags.Closed; */
        Contactors_Y.PosContactors = Closed;

        /* Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* Outport: '<Root>/ContactorsWeldWarning' */
        /* :  ContactorsWeldWarning = ContactorsFlags.None */
        Contactors_Y.ContactorsWeldWarning = None;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.ClosedSubstate; */
        Contactors_Y.ContactorsState = ClosedSubstate;

        /* :  sf_internal_predicateOutput = PreChargeContactors_FB == 1; */
      } else if ((int32_T)Contactors_U.PreChargeContactors_FB == 1) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.durationCounter_1 = 0;
        Contactors_DW.is_ContactorOperations = Con_IN_PreCharging_Substate_Old;
        Contactors_DW.is_PreCharging_Substate_Old =
          Contactors_IN_PreCharge_Check02;

        /* Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_PreChargeOpen; */
        Contactors_Y.ContactorsState = PreCharging_PreChargeOpen;
      }
      break;

     case Contactors_IN_IntState15:
      /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.PreChargeTimeout_msec; */
      if (Contactors_B.PreIntegratedTime >
          Contactors_U.Thresholds.PreChargeTimeout_msec) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.is_ContactorOperations = Contactors_IN_PermanentFail;

        /* Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Open; */
        Contactors_Y.NegContactors = Open;

        /* Outport: '<Root>/PosContactors' */
        /* :  PosContactors = ContactorsFlags.Open; */
        Contactors_Y.PosContactors = Open;

        /* Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* Outport: '<Root>/PermanentFaiil' */
        /* :  PermanentFail = ProtectionFlags.Error; */
        Contactors_Y.PermanentFaiil = Error;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PermanentFailure; */
        Contactors_Y.ContactorsState = PermanentFailure;

        /* :  sf_internal_predicateOutput = PreChargeRetryCheck < Thresholds.PreChargeRetryLimit; */
      } else if (Contactors_DW.PreChargeRetryCheck <
                 Contactors_U.Thresholds.PreChargeRetryLimit) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.durationCounter_1 = 0;
        Contactors_DW.is_ContactorOperations = Con_IN_PreCharging_Substate_Old;
        Contactors_DW.is_PreCharging_Substate_Old =
          Contactors_IN_PreCharge_Check03;

        /* Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_PreChargeRetry; */
        Contactors_Y.ContactorsState = PreCharging_PreChargeRetry;
      }
      break;

     case Contactors_IN_IntState16:
      /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.PreChargeTimeout_msec; */
      if (Contactors_B.PreIntegratedTime >
          Contactors_U.Thresholds.PreChargeTimeout_msec) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.is_ContactorOperations = Contactors_IN_PermanentFail;

        /* Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Open; */
        Contactors_Y.NegContactors = Open;

        /* Outport: '<Root>/PosContactors' */
        /* :  PosContactors = ContactorsFlags.Open; */
        Contactors_Y.PosContactors = Open;

        /* Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* Outport: '<Root>/PermanentFaiil' */
        /* :  PermanentFail = ProtectionFlags.Error; */
        Contactors_Y.PermanentFaiil = Error;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PermanentFailure; */
        Contactors_Y.ContactorsState = PermanentFailure;

        /* :  sf_internal_predicateOutput = PreChargeRetryCheck < Thresholds.PreChargeRetryLimit; */
      } else if (Contactors_DW.PreChargeRetryCheck <
                 Contactors_U.Thresholds.PreChargeRetryLimit) {
        /* :  PreTimeReset=0; */
        Contactors_DW.durationCounter_1_e = 0;
        Contactors_DW.is_ContactorOperations = Con_IN_PreCharging_Substate_New;
        Contactors_DW.is_PreCharging_Substate_New =
          Contactors_IN_PreCharge_Check03;

        /* :  PreTimeReset=1; */
        Contactors_B.PreTimeReset = 1.0;

        /* Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_PreChargeRetry; */
        Contactors_Y.ContactorsState = PreCharging_PreChargeRetry;
      }
      break;

     case Contactors_IN_IntState8:
      /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.PreToClosed_Time_msec; */
      if (Contactors_B.PreIntegratedTime >
          Contactors_U.Thresholds.PreToClosed_Time_msec) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.durationCounter_1_i = 0;
        Contactors_DW.is_ContactorOperations = Contactors_IN_Closed_Substate;

        /* Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Closed; */
        Contactors_Y.NegContactors = Closed;

        /* Outport: '<Root>/PosContactors' */
        /* :  PosContactors = ContactorsFlags.Closed; */
        Contactors_Y.PosContactors = Closed;

        /* Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* Outport: '<Root>/ContactorsWeldWarning' */
        /* :  ContactorsWeldWarning = ContactorsFlags.None */
        Contactors_Y.ContactorsWeldWarning = None;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.ClosedSubstate; */
        Contactors_Y.ContactorsState = ClosedSubstate;

        /* :  sf_internal_predicateOutput = PreChargeContactors_FB==1; */
      } else if ((int32_T)Contactors_U.PreChargeContactors_FB == 1) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.durationCounter_1_e = 0;
        Contactors_DW.is_ContactorOperations = Con_IN_PreCharging_Substate_New;
        Contactors_DW.is_PreCharging_Substate_New =
          Contactors_IN_PreCharge_Check02;

        /* Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_PreChargeOpen; */
        Contactors_Y.ContactorsState = PreCharging_PreChargeOpen;
      }
      break;

     case Contactors_IN_IntState9:
      /* :  sf_internal_predicateOutput = PreIntegratedTime>Thresholds.OpenToPreChargeStateTime_msec; */
      if (Contactors_B.PreIntegratedTime >
          Contactors_U.Thresholds.OpenToPreChargeStateTime_msec) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.durationCounter_1 = 0;
        Contactors_DW.is_ContactorOperations = Con_IN_PreCharging_Substate_Old;
        Contactors_DW.is_PreCharging_Substate_Old =
          Contactors_IN_NegContact_Check;

        /* Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Closed; */
        Contactors_Y.NegContactors = Closed;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.PreCharging_NegContactorClosed; */
        Contactors_Y.ContactorsState = PreCharging_NegContactorClosed;

        /* :  sf_internal_predicateOutput = Flags_detection==0 || DataPipeline.PreChargeLogic==1; */
      } else if ((!Contactors_B.Flags_Detection) ||
                 Contactors_U.DataPipeline.PreChargeLogic) {
        /* :  PreTimeReset=0; */
        Contactors_B.PreTimeReset = 0.0;
        Contactors_DW.is_ContactorOperations = Contactors_IN_Open_Substate;

        /* Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Open; */
        Contactors_Y.NegContactors = Open;

        /* Outport: '<Root>/PosContactors' */
        /* :  PosContactors = ContactorsFlags.Open; */
        Contactors_Y.PosContactors = Open;

        /* Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* Outport: '<Root>/ContactorsWeldWarning' */
        /* :  ContactorsWeldWarning = ContactorsFlags.None; */
        Contactors_Y.ContactorsWeldWarning = None;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.OpenSubstate; */
        Contactors_Y.ContactorsState = OpenSubstate;
      }
      break;

     case Contactors_IN_Open_Substate:
      Contactors_Open_Substate();
      break;

     case Contacto_IN_NegContact_Check_02:
      Contactors_DW.is_ContactorOperations = Contactors_IN_Open_Substate;

      /* Outport: '<Root>/NegContactors' */
      /* :  NegContactors = ContactorsFlags.Open; */
      Contactors_Y.NegContactors = Open;

      /* Outport: '<Root>/PosContactors' */
      /* :  PosContactors = ContactorsFlags.Open; */
      Contactors_Y.PosContactors = Open;

      /* Outport: '<Root>/PreChargeContactors' */
      /* :  PreChargeContactors = ContactorsFlags.Open; */
      Contactors_Y.PreChargeContactors = Open;

      /* Outport: '<Root>/ContactorsWeldWarning' */
      /* :  ContactorsWeldWarning = ContactorsFlags.None; */
      Contactors_Y.ContactorsWeldWarning = None;

      /* Outport: '<Root>/ContactorsState' */
      /* :  ContactorsState = ContState.OpenSubstate; */
      Contactors_Y.ContactorsState = OpenSubstate;
      break;

     case Contactors_IN_PermanentFail:
      /* Outport: '<Root>/NegContactors' */
      Contactors_Y.NegContactors = Open;

      /* Outport: '<Root>/PosContactors' */
      Contactors_Y.PosContactors = Open;

      /* Outport: '<Root>/PreChargeContactors' */
      Contactors_Y.PreChargeContactors = Open;

      /* Outport: '<Root>/PermanentFaiil' */
      Contactors_Y.PermanentFaiil = Error;

      /* Outport: '<Root>/ContactorsState' */
      Contactors_Y.ContactorsState = PermanentFailure;

      /* :  sf_internal_predicateOutput = PermanentFailRecoveryTrigger == 1; */
      if (Contactors_U.PermanentFailRecoveryTrigger) {
        /* :  PreChargeRetryCheck=0 */
        Contactors_DW.PreChargeRetryCheck = 0U;
        Contactors_DW.is_ContactorOperations = Contactors_IN_Open_Substate;

        /* Outport: '<Root>/NegContactors' */
        /* :  NegContactors = ContactorsFlags.Open; */
        Contactors_Y.NegContactors = Open;

        /* Outport: '<Root>/PosContactors' */
        /* :  PosContactors = ContactorsFlags.Open; */
        Contactors_Y.PosContactors = Open;

        /* Outport: '<Root>/PreChargeContactors' */
        /* :  PreChargeContactors = ContactorsFlags.Open; */
        Contactors_Y.PreChargeContactors = Open;

        /* Outport: '<Root>/ContactorsWeldWarning' */
        /* :  ContactorsWeldWarning = ContactorsFlags.None; */
        Contactors_Y.ContactorsWeldWarning = None;

        /* Outport: '<Root>/ContactorsState' */
        /* :  ContactorsState = ContState.OpenSubstate; */
        Contactors_Y.ContactorsState = OpenSubstate;
      }
      break;

     case Con_IN_PreCharging_Substate_New:
      Contac_PreCharging_Substate_New();
      break;

     default:
      /* case IN_PreCharging_Substate_Old: */
      Contac_PreCharging_Substate_Old();
      break;
    }
  }

  if (!Contactors_B.Flags_Detection) {
    Contactors_DW.durationCounter_1++;
    Contactors_DW.durationCounter_1_e++;
    Contactors_DW.durationCounter_1_i++;
  } else {
    Contactors_DW.durationCounter_1 = 0;
    Contactors_DW.durationCounter_1_e = 0;
    Contactors_DW.durationCounter_1_i = 0;
  }

  /* End of Chart: '<S1>/Contactor_StateMachine' */

  /* Update for UnitDelay: '<S1>/Unit Delay' */
  Contactors_DW.UnitDelay_DSTATE = Contactors_B.PreTimeReset;

  /* Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' incorporates:
   *  Inport: '<Root>/PreCharge_LoopTime_msec'
   */
  Contactors_DW.DiscreteTimeIntegrator_DSTATE += Contactors_U.PreCharge_LoopTime_msec;
  Contactors_DW.DiscreteTimeIntegrator_PrevRese = (int8_T)rtb_NOT;

  /* Update for Memory: '<S1>/Memory' incorporates:
   *  Outport: '<Root>/NegContactors'
   */
  Contactors_DW.Memory_PreviousInput = Contactors_Y.NegContactors;

  /* Update for Memory: '<S1>/Memory1' incorporates:
   *  Outport: '<Root>/PosContactors'
   */
  Contactors_DW.Memory1_PreviousInput = Contactors_Y.PosContactors;

  /* Update for Memory: '<S1>/Memory2' incorporates:
   *  Outport: '<Root>/PreChargeContactors'
   */
  Contactors_DW.Memory2_PreviousInput = Contactors_Y.PreChargeContactors;
}

/* Model initialize function */
void Contactors_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(Contactors_M, (NULL));

  /* block I/O */
  (void) memset(((void *) &Contactors_B), 0,
                sizeof(B_Contactors_T));

  /* states (dwork) */
  (void) memset((void *)&Contactors_DW, 0,
                sizeof(DW_Contactors_T));

  /* external inputs */
  (void)memset(&Contactors_U, 0, sizeof(ExtU_Contactors_T));
  Contactors_U.BMS_State = MARVEL_READY;
  Contactors_U.ProtectionOutput = Contactors_rtZProtectionState_O;
  Contactors_U.PermanentFail_Flag = NoError;
  Contactors_U.PosContactors_FB = Open;
  Contactors_U.PreChargeContactors_FB = Open;
  Contactors_U.NegContactors_FB = Open;

  /* external outputs */
  Contactors_Y.NegContactors = Open;
  Contactors_Y.PreChargeContactors = Open;
  Contactors_Y.PosContactors = Open;
  Contactors_Y.ContactorsWeldWarning = Open;
  Contactors_Y.PermanentFaiil = NoError;
  Contactors_Y.ContactorsState = OpenSubstate;

  /* InitializeConditions for UnitDelay: '<S1>/Unit Delay' */
  Contactors_DW.UnitDelay_DSTATE = 0.0;

  /* InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' */
  Contactors_DW.DiscreteTimeIntegrator_DSTATE = 0.0;
  Contactors_DW.DiscreteTimeIntegrator_PrevRese = 0;

  /* InitializeConditions for Memory: '<S1>/Memory' */
  Contactors_DW.Memory_PreviousInput = Open;

  /* InitializeConditions for Memory: '<S1>/Memory1' */
  Contactors_DW.Memory1_PreviousInput = Open;

  /* InitializeConditions for Memory: '<S1>/Memory2' */
  Contactors_DW.Memory2_PreviousInput = Open;

  /* SystemInitialize for Chart: '<S1>/Contactor_StateMachine' */
  Contactors_DW.is_ContactorOperations = Contactors_IN_NO_ACTIVE_CHILD;
  Contactors_DW.is_PreCharging_Substate_New = Contactors_IN_NO_ACTIVE_CHILD;
  Contactors_DW.is_PreCharging_Substate_Old = Contactors_IN_NO_ACTIVE_CHILD;
  Contactors_DW.is_active_c1_Contactors = 0U;
  Contactors_DW.PreChargeRetryCheck = 0U;
  Contactors_B.PreTimeReset = 0.0;

  /* SystemInitialize for Outport: '<Root>/NegContactors' incorporates:
   *  Chart: '<S1>/Contactor_StateMachine'
   */
  Contactors_Y.NegContactors = Open;

  /* SystemInitialize for Outport: '<Root>/PosContactors' incorporates:
   *  Chart: '<S1>/Contactor_StateMachine'
   */
  Contactors_Y.PosContactors = Open;

  /* SystemInitialize for Outport: '<Root>/PreChargeContactors' incorporates:
   *  Chart: '<S1>/Contactor_StateMachine'
   */
  Contactors_Y.PreChargeContactors = Open;

  /* SystemInitialize for Outport: '<Root>/ContactorsWeldWarning' incorporates:
   *  Chart: '<S1>/Contactor_StateMachine'
   */
  Contactors_Y.ContactorsWeldWarning = Open;

  /* SystemInitialize for Outport: '<Root>/PermanentFaiil' incorporates:
   *  Chart: '<S1>/Contactor_StateMachine'
   */
  Contactors_Y.PermanentFaiil = NoError;

  /* SystemInitialize for Outport: '<Root>/ContactorsState' incorporates:
   *  Chart: '<S1>/Contactor_StateMachine'
   */
  Contactors_Y.ContactorsState = OpenSubstate;
}

/* Model terminate function */
void Contactors_terminate(void)
{
  /* (no terminate code required) */
}
