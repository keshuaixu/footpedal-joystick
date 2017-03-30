/*******************************************************************************
* File Name: Isr.c  
* Version 1.70
*
*  Description:
*   API for controlling the state of an interrupt.
*
*
*  Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include <cydevice_trm.h>
#include <CyLib.h>
#include <Isr.h>
#include "cyapicallbacks.h"

#if !defined(Isr__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START Isr_intc` */

/* `#END` */


/*******************************************************************************
* Function Name: Isr_Start
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it. This function disables the interrupt, 
*  sets the default interrupt vector, sets the priority from the value in the
*  Design Wide Resources Interrupt Editor, then enables the interrupt to the 
*  interrupt controller.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void Isr_Start(void) 
{
    /* For all we know the interrupt is active. */
    Isr_Disable();

    /* Set the ISR to point to the Isr Interrupt. */
    Isr_SetVector(&Isr_Interrupt);

    /* Set the priority. */
    Isr_SetPriority((uint8)Isr_INTC_PRIOR_NUMBER);

    /* Enable it. */
    Isr_Enable();
}


/*******************************************************************************
* Function Name: Isr_StartEx
********************************************************************************
*
* Summary:
*  Sets up the interrupt and enables it. This function disables the interrupt,
*  sets the interrupt vector based on the address passed in, sets the priority 
*  from the value in the Design Wide Resources Interrupt Editor, then enables 
*  the interrupt to the interrupt controller.
*  
*  When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*  used to provide consistent definition across compilers:
*  
*  Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*   Function prototype example:
*   CY_ISR_PROTO(MyISR);
*
* Parameters:  
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void Isr_StartEx(cyisraddress address) 
{
    /* For all we know the interrupt is active. */
    Isr_Disable();

    /* Set the ISR to point to the Isr Interrupt. */
    Isr_SetVector(address);

    /* Set the priority. */
    Isr_SetPriority((uint8)Isr_INTC_PRIOR_NUMBER);

    /* Enable it. */
    Isr_Enable();
}


/*******************************************************************************
* Function Name: Isr_Stop
********************************************************************************
*
* Summary:
*   Disables and removes the interrupt.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void Isr_Stop(void) 
{
    /* Disable this interrupt. */
    Isr_Disable();
}


/*******************************************************************************
* Function Name: Isr_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for Isr.
*
*   Add custom code between the START and END comments to keep the next version
*   of this file from over-writing your code.
*
*   Note You may use either the default ISR by using this API, or you may define
*   your own separate ISR through ISR_StartEx().
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
CY_ISR(Isr_Interrupt)
{
    #ifdef Isr_INTERRUPT_INTERRUPT_CALLBACK
        Isr_Interrupt_InterruptCallback();
    #endif /* Isr_INTERRUPT_INTERRUPT_CALLBACK */

    /*  Place your Interrupt code here. */
    /* `#START Isr_Interrupt` */

    /* `#END` */

    /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3)
        #if((CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2) && (Isr__ES2_PATCH ))      
            Isr_ISR_PATCH();
        #endif /* CYDEV_CHIP_REVISION_USED */
    #endif /* (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) */
}


/*******************************************************************************
* Function Name: Isr_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling Isr_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use Isr_StartEx instead.
* 
*   When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*   used to provide consistent definition across compilers:
*
*   Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*
*   Function prototype example:
*     CY_ISR_PROTO(MyISR);
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void Isr_SetVector(cyisraddress address) 
{
    CY_SET_REG16(Isr_INTC_VECTOR, (uint16) address);
}


/*******************************************************************************
* Function Name: Isr_GetVector
********************************************************************************
*
* Summary:
*   Gets the "address" of the current ISR vector for the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress Isr_GetVector(void) 
{
    return (cyisraddress) CY_GET_REG16(Isr_INTC_VECTOR);
}


/*******************************************************************************
* Function Name: Isr_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. 
*
*   Note calling Isr_Start or Isr_StartEx will 
*   override any effect this API would have had. This API should only be called
*   after Isr_Start or Isr_StartEx has been called. 
*   To set the initial priority for the component, use the Design-Wide Resources
*   Interrupt Editor.
*
*   Note This API has no effect on Non-maskable interrupt NMI).
*
* Parameters:
*   priority: Priority of the interrupt, 0 being the highest priority
*             PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*             PSoC 4: Priority is from 0 to 3.
*
* Return:
*   None
*
*******************************************************************************/
void Isr_SetPriority(uint8 priority) 
{
    *Isr_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: Isr_GetPriority
********************************************************************************
*
* Summary:
*   Gets the Priority of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Priority of the interrupt, 0 being the highest priority
*    PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*    PSoC 4: Priority is from 0 to 3.
*
*******************************************************************************/
uint8 Isr_GetPriority(void) 
{
    uint8 priority;


    priority = *Isr_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: Isr_Enable
********************************************************************************
*
* Summary:
*   Enables the interrupt to the interrupt controller. Do not call this function
*   unless ISR_Start() has been called or the functionality of the ISR_Start() 
*   function, which sets the vector and the priority, has been called.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void Isr_Enable(void) 
{
    /* Enable the general interrupt. */
    *Isr_INTC_SET_EN = Isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: Isr_GetState
********************************************************************************
*
* Summary:
*   Gets the state (enabled, disabled) of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   1 if enabled, 0 if disabled.
*
*******************************************************************************/
uint8 Isr_GetState(void) 
{
    /* Get the state of the general interrupt. */
    return ((*Isr_INTC_SET_EN & (uint8)Isr__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: Isr_Disable
********************************************************************************
*
* Summary:
*   Disables the Interrupt in the interrupt controller.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void Isr_Disable(void) 
{
    /* Disable the general interrupt. */
    *Isr_INTC_CLR_EN = Isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: Isr_SetPending
********************************************************************************
*
* Summary:
*   Causes the Interrupt to enter the pending state, a software method of
*   generating the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
* Side Effects:
*   If interrupts are enabled and the interrupt is set up properly, the ISR is
*   entered (depending on the priority of this interrupt and other pending 
*   interrupts).
*
*******************************************************************************/
void Isr_SetPending(void) 
{
    *Isr_INTC_SET_PD = Isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: Isr_ClearPending
********************************************************************************
*
* Summary:
*   Clears a pending interrupt in the interrupt controller.
*
*   Note Some interrupt sources are clear-on-read and require the block 
*   interrupt/status register to be read/cleared with the appropriate block API 
*   (GPIO, UART, and so on). Otherwise the ISR will continue to remain in 
*   pending state even though the interrupt itself is cleared using this API.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void Isr_ClearPending(void) 
{
    *Isr_INTC_CLR_PD = Isr__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
