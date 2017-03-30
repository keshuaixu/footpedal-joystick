/*******************************************************************************
* File Name: Isr.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_ISR_Isr_H)
#define CY_ISR_Isr_H

#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void Isr_Start(void) ;
void Isr_StartEx(cyisraddress address) ;
void Isr_Stop(void) ;

CY_ISR_PROTO(Isr_Interrupt);

void Isr_SetVector(cyisraddress address) ;
cyisraddress Isr_GetVector(void) ;

void Isr_SetPriority(uint8 priority) ;
uint8 Isr_GetPriority(void) ;

void Isr_Enable(void) ;
uint8 Isr_GetState(void) ;
void Isr_Disable(void) ;

void Isr_SetPending(void) ;
void Isr_ClearPending(void) ;


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Isr ISR. */
#define Isr_INTC_VECTOR            ((reg16 *) Isr__INTC_VECT)

/* Address of the Isr ISR priority. */
#define Isr_INTC_PRIOR             ((reg8 *) Isr__INTC_PRIOR_REG)

/* Priority of the Isr interrupt. */
#define Isr_INTC_PRIOR_NUMBER      Isr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Isr interrupt. */
#define Isr_INTC_SET_EN            ((reg8 *) Isr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Isr interrupt. */
#define Isr_INTC_CLR_EN            ((reg8 *) Isr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Isr interrupt state to pending. */
#define Isr_INTC_SET_PD            ((reg8 *) Isr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Isr interrupt. */
#define Isr_INTC_CLR_PD            ((reg8 *) Isr__INTC_CLR_PD_REG)



#endif /* CY_ISR_Isr_H */


/* [] END OF FILE */
