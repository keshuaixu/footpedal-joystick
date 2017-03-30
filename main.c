/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"


static uint8 Joystick_Data[3] = {0,0,0};

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    uint8 i = 1;

	CYGlobalIntEnable;           					/* Enable Global interrupts */
	USBFS_1_Start(0, USBFS_1_5V_OPERATION);	/* Start USBFS operation/device 0 and with 5V operation */
	while(!USBFS_1_bGetConfiguration());			/* Wait for Device to enumerate */
    USBFS_1_LoadInEP(1, Joystick_Data, 3); /* Loads an inital value into EP1 and sends it out to the PC */

    for(;;)
    {
        while(!USBFS_1_bGetEPAckState(1)); 	/* Wait for ACK before loading data */
        Joystick_Data[0] = ~(Status_Reg_1_Read()) & 0xFF;
        Joystick_Data[1] = ~(Status_Reg_2_Read()) & 0xFF;
        Joystick_Data[2] = ~(Status_Reg_3_Read()) & 0xFF;
		USBFS_1_LoadInEP(1, Joystick_Data, 3); /* Load latest mouse data into EP1 and send to PC */
    }
}

/* [] END OF FILE */
