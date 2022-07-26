#include "bsp_hj12864d.h"
#include "bsp_TiMbase.h" 
#include "bsp_i2c_ee.h"
#include "bsp_spi_flash.h"
#include "bsp_usart_dma.h"
#include "bsp_TiMbase.h"
#include "bsp_GeneralTim_PWM.h" 
#include "bsp_key.h"
#include "bsp_sd3078.h"
#include "bsp_led.h"
#include "bsp_iwdg.h"   
#include "bsp_setdata_eeprom.h"
#include "bsp_UpDownData.h"
int main(void)
{
	Key_IO_Init();
	BASIC_TIM_Init();
	BL_PWM_Init();
	ee_CheckOk();
	SavingDataReadOout();
	SD3078Init();
	LED_IO_Init();
	SPI_FLASH_Init();
	HJ12864D_Init();
	USARTx_DMA_Init();
	IWDG_Init();
	BL_CTRL(uBackLightData);
	while(1)
	{
		HeartBeat(200);
		KeyCtrl();
		UIDisplay();
		IWDG_Feed(500);
		RTCDataGet(1000);
		DataREV();
		SendSetUpData();
		LEDCtrl();
	}
}
