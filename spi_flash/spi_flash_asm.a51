NAME	SPI_FLASH_ASM

;spi falsh驱动配置宏控制，需要和spi_flash.c中配置保持一致 
FLASH_GPIO_PORT		EQU		09H ;GPIOBIN端口偏移量为09H
FLAHS_CLK_BIT		EQU     0 ;GPIO clk	bit位
FLAHS_DI_BIT		EQU     1 ;CPIO DI bit位
FLAHS_DO_BIT		EQU     2 ;GPIO DO bit位

FLAHS_CLK_PIN		EQU     1 << FLAHS_CLK_BIT
GPIO_PORT_START 	EQU     9600H

?PR?_SpiSendByte?SPI_FLASH_ASM 	SEGMENT CODE
?PR?SpiRecvByte?SPI_FLASH_ASM 	SEGMENT CODE


PUBLIC	_SpiSendByte
PUBLIC	SpiRecvByte


	//BOOL SwSpiSendByte(BYTE Dat) 
	RSEG ?PR?_SpiSendByte?SPI_FLASH_ASM 	
_SpiSendByte:
	MOV R0, #08H

SpiSendByte_LOOP:
	
	//B1 //发送数据位，最先发MSB，最后发LSB
	MOV A, R7
	RLC A
	MOV R7, A

	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT + 01H
	MOVX A, @DPTR
	MOV ACC.FLAHS_DI_BIT, C
	//B0 = 0
	ANL A, #~FLAHS_CLK_PIN
	MOVX @DPTR, A

	//B0 = 1
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT + 01H 
	MOVX A, @DPTR
	ORL A, #FLAHS_CLK_PIN
	MOVX @DPTR, A

	DJNZ R0, SpiSendByte_LOOP

	RET

	//BYTE SwSpiRecvByte(VOID)
RSEG ?PR?SpiRecvByte?SPI_FLASH_ASM 	
SpiRecvByte:
	CLR A
	CLR C
	MOV R2, A

	//B0 = 1
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT + 01H 
	MOVX A, @DPTR
	ORL A, #FLAHS_CLK_PIN
	MOVX @DPTR, A
	
	//B0 = 0
	ANL A, #~FLAHS_CLK_PIN
	MOVX @DPTR, A	
	//B2 接收数据 bit7
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT
	MOVX A, @DPTR
	MOV C, ACC.FLAHS_DO_BIT
	CLR A
	MOV ACC.0, C
	ADD A, R2
	RL A
	MOV R2, A
	//B0 = 1
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT + 01H
	MOVX A, @DPTR
	ORL A, #FLAHS_CLK_PIN
	MOVX @DPTR, A

	// B0 = 0
	ANL A, #~FLAHS_CLK_PIN
	MOVX @DPTR, A
	//B2 接收数据
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT
	MOVX A, @DPTR
	MOV C, ACC.FLAHS_DO_BIT
	CLR A
	MOV ACC.0, C
	ADD A, R2
	RL A
	MOV R2, A
	//B0 = 1
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT + 01H
	MOVX A, @DPTR
	ORL A, #FLAHS_CLK_PIN
	MOVX @DPTR, A
	
	// B0 = 0
	ANL A, #~FLAHS_CLK_PIN
	MOVX @DPTR, A
	//B2 接收数据
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT
	MOVX A, @DPTR
	MOV C, ACC.FLAHS_DO_BIT
	CLR A
	MOV ACC.0, C
	ADD A, R2
	RL A
	MOV R2, A
	//B0 = 1
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT + 01H
	MOVX A, @DPTR
	ORL A, #FLAHS_CLK_PIN
	MOVX @DPTR, A
	
	// B0 = 0
	ANL A, #~FLAHS_CLK_PIN
	MOVX @DPTR, A
	//B2 接收数据
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT
	MOVX A, @DPTR
	MOV C, ACC.FLAHS_DO_BIT
	CLR A
	MOV ACC.0, C
	ADD A, R2
	RL A
	MOV R2, A
	//B0 = 1
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT + 01H
	MOVX A, @DPTR
	ORL A, #FLAHS_CLK_PIN
	MOVX @DPTR, A
	
	// B0 = 0
	ANL A, #~FLAHS_CLK_PIN
	MOVX @DPTR, A
	//B2 接收数据
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT
	MOVX A, @DPTR
	MOV C, ACC.FLAHS_DO_BIT
	CLR A
	MOV ACC.0, C
	ADD A, R2
	RL A
	MOV R2, A
	//B0 = 1
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT + 01H
	MOVX A, @DPTR
	ORL A, #FLAHS_CLK_PIN
	MOVX @DPTR, A
	
	// B0 = 0
	ANL A, #~FLAHS_CLK_PIN
	MOVX @DPTR, A
	//B2 接收数据
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT
	MOVX A, @DPTR
	MOV C, ACC.FLAHS_DO_BIT
	CLR A
	MOV ACC.0, C
	ADD A, R2
	RL A
	MOV R2, A
	//B0 = 1
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT + 01H
	MOVX A, @DPTR
	ORL A, #FLAHS_CLK_PIN
	MOVX @DPTR, A
	
	// B0 = 0
	ANL A, #~FLAHS_CLK_PIN
	MOVX @DPTR, A
	//B2 接收数据
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT
	MOVX A, @DPTR
	MOV C, ACC.FLAHS_DO_BIT
	CLR A
	MOV ACC.0, C
	ADD A, R2
	RL A
	MOV R2, A
	//B0 = 1
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT + 01H
	MOVX A, @DPTR
	ORL A, #FLAHS_CLK_PIN
	MOVX @DPTR, A
	
	// B0 = 0
	ANL A, #~FLAHS_CLK_PIN
	MOVX @DPTR, A
	//B2 接收数据
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT
	MOVX A, @DPTR
	MOV C, ACC.FLAHS_DO_BIT
	CLR A
	MOV ACC.0, C
	ADD A, R2
	MOV R2, A
	//B0 = 1
	MOV DPTR, #GPIO_PORT_START +  FLASH_GPIO_PORT + 01H
	MOVX A, @DPTR
	ORL A, #FLAHS_CLK_PIN
	MOVX @DPTR, A

	MOV A, R2
	MOV R7, A
	RET

	END

