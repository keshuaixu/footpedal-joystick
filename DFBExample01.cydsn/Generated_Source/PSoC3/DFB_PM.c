/*******************************************************************************
* File Name: DFB_PM.c
* Version 1.40
*
* Description:
*  This file contains the setup, control and status commands to support
*  component operations in low power mode.
*
* Note:
*
********************************************************************************
* Copyright 2011-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "DFB.h"

static DFB_BACKUP_STRUCT DFB_backup = { 0u };


/*******************************************************************************
* Function Name: DFB_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the user configuration of the DFB non-retention registers. This routine
*  is called by DFB_Sleep() to save the component configuration before entering
*  sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void DFB_SaveConfig(void) 
{
    uint8 count;
    uint8 i = 0u;
	
    DFB_backup.sr = DFB_SR_REG;
    DFB_backup.sem = DFB_SEM_REG;

    /* ACU RAM */
    count = DFB_ACU_RAM_SIZE_CUR;
    while (count > 0u)
    {
        DFB_backup.acu[i] = CY_GET_REG32(& DFB_ACU_RAM_PTR[i]);
        count--;
        i++;
    }
	
    /* Take DFB RAM off the bus */
    DFB_RAM_DIR_REG = DFB_RAM_DIR_DFB;
}


/*******************************************************************************
* Function Name: DFB_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the user configuration of the DFB non-retention registers. This
*  routine is called by DFB_Wakeup() to restore the component configuration when
*  exiting sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void DFB_RestoreConfig(void) 
{
    uint8 count;
    uint8 i = 0u;

    DFB_SR_REG = DFB_backup.sr;
    DFB_SEM_REG = DFB_backup.sem;
    
    /* Power on DFB before initializing the RAMs */
    DFB_PM_ACT_CFG_REG |= DFB_PM_ACT_MSK;
    /* Enables DFB block in Alternate Active (Standby) mode */
    DFB_PM_STBY_CFG_REG |= DFB_PM_STBY_MSK;
    
    /* Put DFB RAM on the bus */
    DFB_RAM_DIR_REG = DFB_RAM_DIR_BUS;

    /* ACU RAM */
    count = DFB_ACU_RAM_SIZE_CUR;
    while (count > 0u)
    {
        CY_SET_REG32(& DFB_ACU_RAM_PTR[i], DFB_backup.acu[i]);
        count--;
        i++;
    }

    /* Take DFB RAM off the bus */
    DFB_RAM_DIR_REG = DFB_RAM_DIR_DFB;
}


/*******************************************************************************
* Function Name: DFB_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred routine to prepare the component for sleep. The
*  DFB_Sleep() routine saves the current component state. Then it
*  calls the DFB_Stop() function and calls DFB_SaveConfig() to save
*  the hardware configuration. Call the DFB_Sleep() function before
*  calling the CyPmSleep() or the CyPmHibernate() function. Refer to the PSoC
*  Creator System Reference Guide for more information about power management
*  functions.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void DFB_Sleep(void) 
{
    if (0u != (DFB_CR_REG & DFB_RUN_MASK))
    {
        DFB_backup.enableState = 1u;
    }
    else /* The DFB block is disabled */
    {
        DFB_backup.enableState = 0u;
    }

    /* Turn off the run bit, and put the DFB RAM on the bus */
    DFB_Pause();
    
    /* Save the configuration */
    DFB_SaveConfig();
    
    /* Disables the clock to the entire core of the block */
    DFB_CR_REG |= DFB_CORECLK_DISABLE;
}


/*******************************************************************************
* Function Name: DFB_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred routine to restore the component to the state when
*  DFB_Sleep() was called. The DFB_Wakeup() function
*  calls the DFB_RestoreConfig() function to restore the
*  configuration. If the component was enabled before the
*  DFB_Sleep() function was called, the DFB_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  Calling the DFB_Wakeup() function without first calling the
*  DFB_Sleep() or DFB_SaveConfig() function may
*  produce unexpected behavior.
*
*******************************************************************************/
void DFB_Wakeup(void) 
{
    DFB_RestoreConfig();

    if (DFB_backup.enableState != 0u)
    {
        /* Enables component's operation */
        DFB_Resume();
    } /* Left Run Bit in off state if component's block was disabled before */
    else
    {
        /* Clears any pending interrupts */
        DFB_SR_REG = 0xFFu;
    }
    /* Enables the clock to the entire core of the block */
    DFB_CR_REG &= (uint8) (~DFB_CORECLK_DISABLE);
}


/* [] END OF FILE */
