/**
  ******************************************************************************
  * @file    main.c
  * @author  William PONSOT
  * @version V1.0.0
  * @date    15 mars 2016
  * @brief   main file of SmartVision project
  ******************************************************************************
*/


// Configration header
#include "stm32f4xx_conf.h"
#include <stm32f4xx.h>

// STM library
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_dcmi.h>
#include <stm32f4xx_dma.h>
#include <stm32f4xx_it.h>  


// Personal library
#include "dcmi_ov7675.h" 
#include "camera_api.h" 


RCC_ClocksTypeDef RCC_Clocks;
OV7675_IDTypeDef  OV7675_Camera_ID;  

//Quick hack, approximately 1ms delay
void ms_delay(int ms)
{
   while (ms-- > 0) {
      volatile int x=5971;
      while (x-- > 0)
         __asm("nop");
   }
}

int main(void)
{
  uint8_t i = 0;
  uint16_t bcounter;
  /* TypeDef */
  GPIO_InitTypeDef GPIO_InitStructure;

  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100); // Usefull to manage Delay with Systick interrupt
  
  // PA8 --> Main Clock Output (A mettre plus tard dans OV7675_HW_Init avec les autres init )
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // Enable the clock on port A
  RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1); // Set of the MCO clock and prescaler
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_MCO); // PA8 --> MCO
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  OV7675_HW_Init();
  OV7675_ReadID(&OV7675_Camera_ID); // Get information from the camera by I2C
  OV7675_Init(BMP_QQVGA);
  OV7675_QQVGAConfig(); // Send information to configure the canera for QQVGA (a lot of register are modified)

  /* Enable DMA2 stream 1 and DCMI interface then start image capture */
  DMA_Cmd(DMA2_Stream1, ENABLE); 
  DCMI_Cmd(ENABLE); 

  // UART configuration
  



  // Delay(200);

 

  // DCMI_CaptureCmd(ENABLE);
  //  while(1)
  // {
  //   bcounter = DMA_GetCurrDataCounter(DMA2_Stream1);
  // }
  
  

  return 0;  
}



// // Private typedef 
// GPIO_InitTypeDef  GPIO_InitStructure;

// // !< At this stage the microcontroller clock setting is already configured, 
// //        this is done through SystemInit() function which is called from startup
// //        file (startup_stm32f4xx.s) before to branch to application main.
// //        To reconfigure the default setting of SystemInit() function, refer to
// //         system_stm32f4xx.c file
     

//   GPIOD Periph clock enable 
//   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

//   // Configure PD12, PD13, PD14 and PD15 in output pushpull mode 
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//   GPIO_Init(GPIOD, &GPIO_InitStructure);


//   //User button
//   uint8_t button; //To save the output value of the button
//   // GPIOA Periph clock enable 
//   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//   // Configure PA0 
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//   GPIO_Init(GPIOA, &GPIO_InitStructure);

//   while (1)
//   {
//     button = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0); //Read the value of the button

//     if (button==1)
//     {
//       GPIO_SetBits(GPIOD, GPIO_Pin_12); 
//       ms_delay(50);
//       GPIO_SetBits(GPIOD, GPIO_Pin_13);
//       ms_delay(50);
//       GPIO_SetBits(GPIOD, GPIO_Pin_14);
//       ms_delay(50);
//       GPIO_SetBits(GPIOD, GPIO_Pin_15);
//       ms_delay(50);

//       GPIO_ResetBits(GPIOD, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
//     }
//     else
//     {
//       GPIO_ResetBits(GPIOD, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
//     }  
//   }