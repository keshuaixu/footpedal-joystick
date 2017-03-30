/*******************************************************************************
* File Name: main.c
*
* Version: 1.0
*
* Description:
*  This is source code for the datasheet example of the Digital Filter Block 
*  (DFB) component.
*
*  Example project demonstrates matrix multiplication on scalar by means of the
*  DFB. Also it demonstrates DFB interaction with DMA.
*
********************************************************************************
* Copyright 2012, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <project.h>

#define COLUMN (3u)
#define ROW (2u)

#define DFB_ONE (0x7FFFFFu)
 
void DMA_Init(void);
void Matrix_Print(uint16 Mass[ROW][COLUMN]) CYREENTRANT;

uint8 dma_stga_in_channel;
uint8 dma_stga_in_td;
uint8 dma_stga_out_channel;
uint8 dma_stga_out_td;
uint8 dma_ack_channel, dma_ack_td;

uint16 InMass[ROW][COLUMN] = { {32u, 64u, 128u}, {256u, 512u, 1024u} };
uint16 OutMass[ROW][COLUMN] = { {0u, 0u, 0u}, {0u, 0u, 0u} };
int32 scalar;
uint8 count = 1u;
uint8 shift = 5u;


/*******************************************************************************
* Function Name: CY_ISR(ISR_SW1)
********************************************************************************
*
* Summary:
*  Interrupt handler from "Data in Holding Register A" event.
*
* Parameters:
*  None.
* 
* Return: 
*  None.
*
*******************************************************************************/
CY_ISR(ISR_SW1)
{    
    if(count < COLUMN * ROW)
    {
        DFB_SetSemaphores(DFB_SEM1);
        count++;
    }
    else
    {        
        count = 1u;
        Matrix_Print(OutMass);
        if(scalar == DFB_ONE)
        {
            CYGlobalIntDisable;
        }
    }    
}


/*******************************************************************************
* Function Name: CY_ISR(ISR_BUT)
********************************************************************************
*
* Summary:
*  Interrupt handler from button press.
*
* Parameters:
*  None.
* 
* Return: 
*  None.
*
*******************************************************************************/
CY_ISR(ISR_BUT)
{        
    scalar = (uint32)DFB_ONE >> shift--;
    DFB_LoadDataRAMA(&scalar, (uint32 *)DFB_DA_RAM_PTR, 1u);    
    /* Clears active interrupts on the port into which the button is mapped */
    CyDelay(300);
    button_ClearInterrupt();
    DFB_SetSemaphores(DFB_SEM1);
}


int main()
{	
	CYGlobalIntEnable;	
	
	LCD_Start();
    
	DFB_Start();    
	DFB_SetCoherency(DFB_STGA_KEY_MID | DFB_STGB_KEY_MID | DFB_HOLDA_KEY_MID | DFB_HOLDB_KEY_MID);	        

    Matrix_Print(InMass);
    
    DMA_Init();
    
    button_ClearInterrupt();
	Isr_StartEx(ISR_SW1);
    isr_but_StartEx(ISR_BUT);            
    
    for(;;) { }
}


/********************************************************************************
* Function Name: DMA_Init
*******************************************************************************
*
* Summary:
*  This function performs DMA_IN_A and DMA_OUT_A initialization.
*
* Parameters:
*	void
*
* Return:
*	void
*
*******************************************************************************/
void DMA_Init(void)
{	
    /* DMA_IN_A */
    dma_stga_in_td = CyDmaTdAllocate();
    #if (CY_PSOC3)
    dma_stga_in_channel = DMA_IN_A_DmaInitialize(2u, 1u, HI16(CYDEV_SRAM_BASE), HI16(CYDEV_SRAM_BASE));
    CyDmaTdSetConfiguration(dma_stga_in_td, (COLUMN * ROW * 2u), DMA_INVALID_TD, (DMA_IN_A__TD_TERMOUT_EN |
                                                                                  TD_SWAP_EN | TD_INC_SRC_ADR));
    #elif (CY_PSOC5)
    dma_stga_in_channel = DMA_IN_A_DmaInitialize(2u, 1u, HI16((uint32)&InMass[0u][0u]), HI16((uint32)DFB_STAGEA_PTR));
    CyDmaTdSetConfiguration(dma_stga_in_td, (COLUMN * ROW * 2u), DMA_INVALID_TD, (DMA_IN_A__TD_TERMOUT_EN |
                                                                                    TD_INC_SRC_ADR));
    #endif
	CyDmaTdSetAddress(dma_stga_in_td, LO16((uint32)&InMass[0u][0u]), LO16((uint32)DFB_STAGEA_PTR));
	CyDmaChSetInitialTd(dma_stga_in_channel, dma_stga_in_td);
	CyDmaChEnable(dma_stga_in_channel, 1u);
	
    /* DMA_OUT_A */
    dma_stga_out_td = CyDmaTdAllocate();
    #if (CY_PSOC3)
	dma_stga_out_channel = DMA_OUT_A_DmaInitialize(2u, 1u, HI16(CYDEV_SRAM_BASE), HI16(CYDEV_SRAM_BASE));
    CyDmaTdSetConfiguration(dma_stga_out_td, (COLUMN * ROW * 2u), DMA_INVALID_TD, (DMA_OUT_A__TD_TERMOUT_EN | 
                                                                                    TD_SWAP_EN | TD_INC_DST_ADR));    
	#elif (CY_PSOC5)
    dma_stga_out_channel = DMA_OUT_A_DmaInitialize(2u, 1u, HI16((uint32)DFB_HOLDA_PTR), HI16((uint32)&OutMass[0u][0u]));	
    CyDmaTdSetConfiguration(dma_stga_out_td, (COLUMN * ROW * 2u), DMA_INVALID_TD, (DMA_OUT_A__TD_TERMOUT_EN | 
                                                                                   TD_INC_DST_ADR));
    #endif	
    CyDmaTdSetAddress(dma_stga_out_td, LO16((uint32)DFB_HOLDA_PTR), LO16((uint32)&OutMass[0u][0u]));
	CyDmaChSetInitialTd(dma_stga_out_channel, dma_stga_out_td);
	CyDmaChEnable(dma_stga_out_channel, 1u);
}


/********************************************************************************
* Function Name: Matrix_Print
*******************************************************************************
*
* Summary:
*  This function print resulting matrix on LCD in decimal format.
*
* Parameters:
*	uint16 Mass[ROW][COLUMN]: resulting matrix.
*
* Return:
*	void
*
*******************************************************************************/
void Matrix_Print(uint16 Mass[ROW][COLUMN]) CYREENTRANT
{
    uint8 i, j, k, c;
    char8 number[5u];
    uint16 temp;
    
    for(i = 0u; i < ROW; i++)
    {
        for(j = 0u, k = 0u; j < COLUMN; j++, k += 5u)
        {
            /* Procedure of number conversion to decimal system */
            c = 3u;
            temp = Mass[i][j];
            number[0u] = 0x30u;
            number[1u] = 0x30u;
            number[2u] = 0x30u;
            number[3u] = 0x30u;
            number[4u] = '\0';
            while (temp >= 10u)
            {
                number[c--] = (temp % 10u) + 0x30u;
                temp /= 10u;
            }            
            number[c] = (temp % 10u) + 0x30u;
            
            LCD_Position(i, k);
            LCD_PrintString(number);
        }
    }
}


/* [] END OF FILE */
