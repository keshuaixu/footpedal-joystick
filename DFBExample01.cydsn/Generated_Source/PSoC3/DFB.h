/*******************************************************************************
* File Name: DFB.h
* Version 1.40
*
* Description:
*  Contains the function prototypes, constants and register definition
*  of the DFB Component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2011-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_DFB_DFB_H) /* DFB Header File */
#define CY_DFB_DFB_H

#include "CyLib.h"

extern uint8 DFB_initVar;


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define DFB_OPTIMIZE_ASSEMBLY      (0u)
#define DFB_INIT_INTERRUPT_MODE    (0u)
#define DFB_INIT_DMA_MODE          (1u)
#define DFB_INIT_OUT1_SOURCE       (2u)
#define DFB_INIT_OUT2_SOURCE       (0u)

/* Sizes of current DFB program */
#define DFB_DA_RAM_SIZE_CUR        (0x01u)
#define DFB_DB_RAM_SIZE_CUR        (0x01u)
#define DFB_CSA_RAM_SIZE_CUR       (0x0Bu)
#define DFB_CSB_RAM_SIZE_CUR       (0x0Bu)
#define DFB_CFSM_RAM_SIZE_CUR      (0x04u)
#define DFB_ACU_RAM_SIZE_CUR       (0x01u)


/***************************************
*     Data Types Definitions
***************************************/

#if (DFB_OPTIMIZE_ASSEMBLY)
    extern const uint32 CYCODE DFB_cstoreA[DFB_CSA_RAM_SIZE_CUR];
    extern const uint32 CYCODE DFB_cstoreB[DFB_CSB_RAM_SIZE_CUR];
#else
    extern const uint32 CYCODE DFB_control[DFB_CSA_RAM_SIZE_CUR];
#endif /* DFB_OPTIMIZE_ASSEMBLY */

extern const uint32 CYCODE DFB_data_a[DFB_DA_RAM_SIZE_CUR];
extern const uint32 CYCODE DFB_data_b[DFB_DB_RAM_SIZE_CUR];
extern const uint32 CYCODE DFB_cfsm[DFB_CFSM_RAM_SIZE_CUR];
extern const uint32 CYCODE DFB_acu[DFB_ACU_RAM_SIZE_CUR];

/* Sleep Mode API Support */
typedef struct
{
    uint8  enableState;
    uint8  cr;
    uint8  sr;
    uint8  sem;
    uint32 acu[DFB_ACU_RAM_SIZE_CUR];
} DFB_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void   DFB_Init(void) ;
void   DFB_Enable(void) ;
void   DFB_Start(void) ;
void   DFB_Stop(void) ;
void   DFB_Pause(void) ;
void   DFB_Resume(void) ;
void   DFB_SetCoherency(uint8 coherencyKeyByte) ;
void   DFB_SetDalign(uint8 dalignKeyByte) ;
uint8  DFB_LoadDataRAMA(const int32 ptr[], uint32 addr[], uint8 size)
        ;
uint8  DFB_LoadDataRAMB(const int32 ptr[], uint32 addr[], uint8 size)
        ;
void   DFB_LoadInputValue(uint8 channel, int32 sample)
        ;
int32  DFB_GetOutputValue(uint8 channel) ;
void   DFB_SetInterruptMode(uint8 events) ;
uint8  DFB_GetInterruptSource(void) ;
void   DFB_ClearInterrupt(uint8 interruptMask) ;
void   DFB_SetDMAMode(uint8 events) ;
void   DFB_SetSemaphores(uint8 mask) CYREENTRANT;
uint8  DFB_GetSemaphores(void) ;
void   DFB_ClearSemaphores(uint8 mask) ;
void   DFB_SetOutput1Source(uint8 source) ;
void   DFB_SetOutput2Source(uint8 source) ;
void   DFB_Sleep(void) ;
void   DFB_Wakeup(void) ;
void   DFB_SaveConfig(void) ;
void   DFB_RestoreConfig(void) ;

/* Macros */
#define DFB_ClearInterruptSource(event) \
do { \
    DFB_SR_REG = (uint8) (((event) & DFB_EVENT_MASK) << DFB_SR_EVENT_SHIFT); \
} while (0)


/***************************************
*           API Constants
***************************************/

/* Channel Definitions */
#define DFB_CHANNEL_A              (1u)
#define DFB_CHANNEL_B              (0u)

/* Sizes of DFB memories */
#define DFB_DA_RAM_SIZE            (0x80u)
#define DFB_DB_RAM_SIZE            (0x80u)
#define DFB_CSA_RAM_SIZE           (0x40u)
#define DFB_CSB_RAM_SIZE           (0x40u)
#define DFB_CFSM_RAM_SIZE          (0x40u)
#define DFB_ACU_RAM_SIZE           (0x10u)

#define DFB_SUCCESS                (CYRET_SUCCESS)
#define DFB_DATA_OUT_OF_RANGE      (0xFEu)
#define DFB_ADDRESS_OUT_OF_RANGE   (0xFFu)


/***************************************
*             Registers
***************************************/

/* RAM memory map offsets */
#define DFB_DA_RAM_REG             (* (reg32 *) (DFB_DFB__DPA_SRAM_DATA))
#define DFB_DA_RAM_PTR             (  (reg32 *) (DFB_DFB__DPA_SRAM_DATA))
#define DFB_DB_RAM_REG             (* (reg32 *) (DFB_DFB__DPB_SRAM_DATA))
#define DFB_DB_RAM_PTR             (  (reg32 *) (DFB_DFB__DPB_SRAM_DATA))
#define DFB_CSA_RAM_REG            (* (reg32 *) (DFB_DFB__CSA_SRAM_DATA))
#define DFB_CSA_RAM_PTR            (  (reg32 *) (DFB_DFB__CSA_SRAM_DATA))
#define DFB_CSB_RAM_REG            (* (reg32 *) (DFB_DFB__CSB_SRAM_DATA))
#define DFB_CSB_RAM_PTR            (  (reg32 *) (DFB_DFB__CSB_SRAM_DATA))
#define DFB_CFSM_RAM_REG           (* (reg32 *) (DFB_DFB__FSM_SRAM_DATA))
#define DFB_CFSM_RAM_PTR           (  (reg32 *) (DFB_DFB__FSM_SRAM_DATA))
#define DFB_ACU_RAM_REG            (* (reg32 *) (DFB_DFB__ACU_SRAM_DATA))
#define DFB_ACU_RAM_PTR            (  (reg32 *) (DFB_DFB__ACU_SRAM_DATA))

#define DFB_SEM_REG                (* (reg8 *) DFB_DFB__SEMA)
#define DFB_SEM_PTR                (  (reg8 *) DFB_DFB__SEMA)
#define DFB_CR_REG                 (* (reg8 *) DFB_DFB__CR)
#define DFB_CR_PTR                 (  (reg8 *) DFB_DFB__CR)
#define DFB_SR_REG                 (* (reg8 *) DFB_DFB__SR)
#define DFB_SR_PTR                 (  (reg8 *) DFB_DFB__SR)
#define DFB_INT_CTRL_REG           (* (reg8 *) DFB_DFB__INT_CTRL)
#define DFB_INT_CTRL_PTR           (  (reg8 *) DFB_DFB__INT_CTRL)
#define DFB_DMA_CTRL_REG           (* (reg8 *) DFB_DFB__DMA_CTRL)
#define DFB_DMA_CTRL_PTR           (  (reg8 *) DFB_DFB__DMA_CTRL)
#define DFB_RAM_DIR_REG            (* (reg8 *) DFB_DFB__RAM_DIR)
#define DFB_RAM_DIR_PTR            (  (reg8 *) DFB_DFB__RAM_DIR)

#define DFB_DALIGN_REG             (* (reg8 *) DFB_DFB__DALIGN)
#define DFB_DALIGN_PTR             (  (reg8 *) DFB_DFB__DALIGN)
#define DFB_DSI_CTRL_REG           (* (reg8 *) DFB_DFB__DSI_CTRL)
#define DFB_DSI_CTRL_PTR           (  (reg8 *) DFB_DFB__DSI_CTRL)
#define DFB_HOLDA_REG              (* (reg8 *) DFB_DFB__HOLDA)
#define DFB_HOLDA_PTR              (  (reg8 *) DFB_DFB__HOLDA)
#define DFB_HOLDAH_REG             (* (reg8 *) DFB_DFB__HOLDAH)
#define DFB_HOLDAH_PTR             (  (reg8 *) DFB_DFB__HOLDAH)
#define DFB_HOLDAM_REG             (* (reg8 *) DFB_DFB__HOLDAM)
#define DFB_HOLDAM_PTR             (  (reg8 *) DFB_DFB__HOLDAM)
#define DFB_HOLDB_REG              (* (reg8 *) DFB_DFB__HOLDB)
#define DFB_HOLDB_PTR              (  (reg8 *) DFB_DFB__HOLDB)
#define DFB_HOLDBH_REG             (* (reg8 *) DFB_DFB__HOLDBH)
#define DFB_HOLDBH_PTR             (  (reg8 *) DFB_DFB__HOLDBH)
#define DFB_HOLDBM_REG             (* (reg8 *) DFB_DFB__HOLDBM)
#define DFB_HOLDBM_PTR             (  (reg8 *) DFB_DFB__HOLDBM)
#define DFB_PM_ACT_CFG_REG         (* (reg8 *) DFB_DFB__PM_ACT_CFG)
#define DFB_PM_ACT_CFG_PTR         (  (reg8 *) DFB_DFB__PM_ACT_CFG)
#define DFB_PM_STBY_CFG_REG        (* (reg8 *) DFB_DFB__PM_STBY_CFG)
#define DFB_PM_STBY_CFG_PTR        (  (reg8 *) DFB_DFB__PM_STBY_CFG)
#define DFB_RAM_EN_REG             (* (reg8 *) DFB_DFB__RAM_EN)
#define DFB_RAM_EN_PTR             (  (reg8 *) DFB_DFB__RAM_EN)
#define DFB_STAGEA_REG             (* (reg8 *) DFB_DFB__STAGEA)
#define DFB_STAGEA_PTR             (  (reg8 *) DFB_DFB__STAGEA)
#define DFB_STAGEAH_REG            (* (reg8 *) DFB_DFB__STAGEAH)
#define DFB_STAGEAH_PTR            (  (reg8 *) DFB_DFB__STAGEAH)
#define DFB_STAGEAM_REG            (* (reg8 *) DFB_DFB__STAGEAM)
#define DFB_STAGEAM_PTR            (  (reg8 *) DFB_DFB__STAGEAM)
#define DFB_STAGEB_REG             (* (reg8 *) DFB_DFB__STAGEB)
#define DFB_STAGEB_PTR             (  (reg8 *) DFB_DFB__STAGEB)
#define DFB_STAGEBH_REG            (* (reg8 *) DFB_DFB__STAGEBH)
#define DFB_STAGEBH_PTR            (  (reg8 *) DFB_DFB__STAGEBH)
#define DFB_STAGEBM_REG            (* (reg8 *) DFB_DFB__STAGEBM)
#define DFB_STAGEBM_PTR            (  (reg8 *) DFB_DFB__STAGEBM)
#define DFB_COHER_REG              (* (reg8 *) DFB_DFB__COHER)
#define DFB_COHER_PTR              (  (reg8 *) DFB_DFB__COHER)

#define DFB_DA_RAM_LAST_ELEM_PTR   (  (reg32 *) (DFB_DFB__DPB_SRAM_DATA - 4u))
#define DFB_DB_RAM_LAST_ELEM_PTR   (  (reg32 *) (DFB_DFB__CSA_SRAM_DATA - 4u))


/***************************************
*       Register Constants
***************************************/

#define DFB_PM_ACT_MSK             (DFB_DFB__PM_ACT_MSK)
#define DFB_PM_STBY_MSK            (DFB_DFB__PM_STBY_MSK)

#define DFB_RUN_MASK               (0x01u)
#define DFB_CORECLK_DISABLE        (0x04u)
#define DFB_RAM_DIR_BUS            (0x3Fu)
#define DFB_RAM_DIR_DFB            (0x00u)
#define DFB_RAM_DPA_DIR            (0x10u)
#define DFB_RAM_DPB_DIR            (0x20u)

/* Mask for bits within DFB Coherency Register */
#define DFB_STGA_KEY_SHIFT         (0x00u)
#define DFB_STGB_KEY_SHIFT         (0x02u)
#define DFB_HOLDA_KEY_SHIFT        (0x04u)
#define DFB_HOLDB_KEY_SHIFT        (0x06u)

#define DFB_STGA_KEY_LOW           ((uint8) (0x00u << DFB_STGA_KEY_SHIFT))
#define DFB_STGA_KEY_MID           ((uint8) (0x01u << DFB_STGA_KEY_SHIFT))
#define DFB_STGA_KEY_HIGH          ((uint8) (0x02u << DFB_STGA_KEY_SHIFT))
#define DFB_STGB_KEY_LOW           ((uint8) (0x00u << DFB_STGB_KEY_SHIFT))
#define DFB_STGB_KEY_MID           ((uint8) (0x01u << DFB_STGB_KEY_SHIFT))
#define DFB_STGB_KEY_HIGH          ((uint8) (0x02u << DFB_STGB_KEY_SHIFT))
#define DFB_HOLDA_KEY_LOW          ((uint8) (0x00u << DFB_HOLDA_KEY_SHIFT))
#define DFB_HOLDA_KEY_MID          ((uint8) (0x01u << DFB_HOLDA_KEY_SHIFT))
#define DFB_HOLDA_KEY_HIGH         ((uint8) (0x02u << DFB_HOLDA_KEY_SHIFT))
#define DFB_HOLDB_KEY_LOW          ((uint8) (0x00u << DFB_HOLDB_KEY_SHIFT))
#define DFB_HOLDB_KEY_MID          ((uint8) (0x01u << DFB_HOLDB_KEY_SHIFT))
#define DFB_HOLDB_KEY_HIGH         ((uint8) (0x02u << DFB_HOLDB_KEY_SHIFT))

/* Mask for bits within DFB Data Alignment Register */
#define DFB_STGA_DALIGN_SHIFT      (0x00u)
#define DFB_STGB_DALIGN_SHIFT      (0x01u)
#define DFB_HOLDA_DALIGN_SHIFT     (0x02u)
#define DFB_HOLDB_DALIGN_SHIFT     (0x03u)

#define DFB_STGA_DALIGN_LOW        ((uint8) (0x00u << DFB_STGA_DALIGN_SHIFT))
#define DFB_STGA_DALIGN_HIGH       ((uint8) (0x01u << DFB_STGA_DALIGN_SHIFT))
#define DFB_STGB_DALIGN_LOW        ((uint8) (0x00u << DFB_STGB_DALIGN_SHIFT))
#define DFB_STGB_DALIGN_HIGH       ((uint8) (0x01u << DFB_STGB_DALIGN_SHIFT))
#define DFB_HOLDA_DALIGN_LOW       ((uint8) (0x00u << DFB_HOLDA_DALIGN_SHIFT))
#define DFB_HOLDA_DALIGN_HIGH      ((uint8) (0x01u << DFB_HOLDA_DALIGN_SHIFT))
#define DFB_HOLDB_DALIGN_LOW       ((uint8) (0x00u << DFB_HOLDB_DALIGN_SHIFT))
#define DFB_HOLDB_DALIGN_HIGH      ((uint8) (0x01u << DFB_HOLDB_DALIGN_SHIFT))

/* Mask for bits within DFB Interrupt Control and Status Registers */
#define DFB_SEM_MASK               (0x07u)
#define DFB_SEM_ENABLE_SHIFT       (0x04u)
#define DFB_EVENT_MASK             (0x1Fu)
#define DFB_SR_EVENT_SHIFT         (0x03u)

#define DFB_HOLDA_SHIFT            (0x00u)
#define DFB_HOLDB_SHIFT            (0x01u)
#define DFB_SEMA0_SHIFT            (0x02u)
#define DFB_SEMA1_SHIFT            (0x03u)
#define DFB_SEMA2_SHIFT            (0x04u)

#define DFB_HOLDA                  ((uint8) (0x01u << DFB_HOLDA_SHIFT))
#define DFB_HOLDB                  ((uint8) (0x01u << DFB_HOLDB_SHIFT))
#define DFB_SEMA0                  ((uint8) (0x01u << DFB_SEMA0_SHIFT))
#define DFB_SEMA1                  ((uint8) (0x01u << DFB_SEMA1_SHIFT))
#define DFB_SEMA2                  ((uint8) (0x01u << DFB_SEMA2_SHIFT))

/* Mask for bits within DFB DMAREQ Control Register */
#define DFB_DMA_CTRL_MASK          (0x0Fu)
#define DFB_DMAREQ1_DISABLED       (0x00u)
#define DFB_DMAREQ1_HOLDA          (0x01u)
#define DFB_DMAREQ1_SEM0           (0x02u)
#define DFB_DMAREQ1_SEM1           (0x03u)
#define DFB_DMAREQ2_DISABLED       (0x00u)
#define DFB_DMAREQ2_HOLDB          (0x04u)
#define DFB_DMAREQ2_SEM0           (0x08u)
#define DFB_DMAREQ2_SEM1           (0x0Cu)

/* Mask for bits within DFB Semaphore Register */
#define DFB_SEMAPHORE0             (0x01u)
#define DFB_SEMAPHORE1             (0x02u)
#define DFB_SEMAPHORE2             (0x04u)

/* Mask for bits within Global Control Register */
#define DFB_DFB_RUN                (0x00u)
#define DFB_SEM0                   (0x01u)
#define DFB_SEM1                   (0x02u)
#define DFB_DFB_INTR               (0x03u)
#define DFB_SEM2                   (0x00u)
#define DFB_DPSIGN                 (0x04u)
#define DFB_DPTHRESH               (0x08u)
#define DFB_DPEQ                   (0x0Cu)
#define DFB_DFB_GBL1_OUT_MASK      (0xFCu)
#define DFB_DFB_GBL2_OUT_MASK      (0xF3u)

#endif /* End DFB Header File */


/* [] END OF FILE */

