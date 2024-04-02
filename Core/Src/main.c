/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "main.h"

void SystemClock_Config(void);
// LED INITIALIZATIONS
void setRED(char value);
void toggleRED(void);
void setBLUE(char value);
void toggleBLUE(void);
void setGREEN(char value);
void toggleGREEN(void);
void setORANGE(char value);
void toggleORANGE(void);


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
	
	// Enable GPIOB and GPIOC in the RCC.
	RCC->AHBENR  |= (RCC_AHBENR_GPIOCEN); // Enables the GPIOC clock in the RCC.
	RCC->APB2ENR |= (RCC_APB2ENR_ADC1EN);
	
	// Configures GPIOC Pins 6 and 7 (RED LED and BLUE LED)
	GPIOC->MODER   |=  (1 << 12) | (1 << 14);
	GPIOC->OTYPER  &= ~((1 << 6) | (1 << 7));
	GPIOC->OSPEEDR &= ~((1 << 12) | (1 << 14));
	GPIOC->PUPDR   &= ~((1 << 12) | (1 << 13) | (1 << 14) | (1 << 15));
	
	// Configures GPIOC Pins 8 and 9 (ORANGE LED and GREEN LED)
	GPIOC->MODER   |=  (1 << 16) | (1 << 18);
	GPIOC->OTYPER  &= ~((1 << 8) | (1 << 9));
	GPIOC->OSPEEDR &= ~((1 << 16) | (1 << 18));
	GPIOC->PUPDR   &= ~((1 << 16) | (1 << 17) | (1 << 18) | (1 << 19));
	
	// Configures PC0 for analog mode.
	GPIOC->MODER |=   (1 << 0) | (1 << 1);
	GPIOC->PUPDR &= ~((1 << 0) | (1 << 1));
	
	
	ADC1->CFGR1 |=  (1 << 4) | (1 << 13);             // 8-bit resolution, continuous conversion mode, hardware triggers disabled.
	ADC1->CFGR1 &= ~((1 << 3)| (1 << 10)| (1 << 11)); // 8-bit resolution, continuous conversion mode, hardware triggers disabled.
	ADC1->CHSELR |= (1 << 10); // Sets channel 1 for conversion.

	// CALIBRATION SOFTWARE PROCEDURE
	/* (1) Ensure that ADEN = 0 */
	/* (2) Clear ADEN by setting ADDIS*/
	/* (3) Clear DMAEN */
	/* (4) Launch the calibration by setting ADCAL */
	/* (5) Wait until ADCAL=0 */
	if ((ADC1->CR & ADC_CR_ADEN) != 0) /* (1) */
	{
		ADC1->CR |= ADC_CR_ADDIS; /* (2) */
	}
	while ((ADC1->CR & ADC_CR_ADEN) != 0)
	{
		/* For robust implementation, add here time-out management */
	}
	ADC1->CFGR1 &= ~ADC_CFGR1_DMAEN; /* (3) */
	ADC1->CR |= ADC_CR_ADCAL; /* (4) */
	while ((ADC1->CR & ADC_CR_ADCAL) != 0) /* (5) */
	{
		/* For robust implementation, add here time-out management */
	}
	
	/*
	ADC1->CR |= (1 << 1); // Set ADIS to 1 to disable ADEN.
	while ((ADC1->CR & ADC_CR_ADEN) != 0){} // Wait for ADEN = 0;
	
	ADC1->CFGR1 &= (1 << 0); // Ensure DMAEN = 0
	ADC1->CR |= (1 << 31); // Set ADCAL = 1
	
	//while(ADC1->CR & 0x80000000){} // WAIT FOR ADCAL = 0
		
	while ((ADC1->CR & ADC_CR_ADCAL) != 0)
	{
	// WAIT FOR ADCAL = 0
	}
	*/
	
	
	// ADC ENABLE SEQUENCE
	/* (1) Ensure that ADRDY = 0 */
	/* (2) Clear ADRDY */
	/* (3) Enable the ADC */
	/* (4) Wait until ADC ready */
	if ((ADC1->ISR & ADC_ISR_ADRDY) != 0) /* (1) */
	{
		ADC1->ISR |= (1 << 0); /* (2) */
	}
	ADC1->CR |= ADC_CR_ADEN; /* (3) */
	while ((ADC1->ISR & ADC_ISR_ADRDY) == 0) /* (4) */
	{
		/* For robust implementation, add here time-out management */
	}
	
	ADC1->CR |= (1 << 2);
	
	/*
	ADC1->ISR |= (1 << 0); // ENSURE ADRDY = 0
	ADC1->CR |= (1 << 0); // ENABLE ADC
	
	while(!(ADC1->ISR & 0x1)){}
		
	if(ADC1->ISR & 0x1){
		ADC1->CR |= (1 << 2);
	}
	*/
	
  while (1)
  {
		
	if(ADC1->DR>=0 && ADC1->DR<64){
		setRED('1');
		setBLUE('0');
		setBLUE('0');
		setORANGE('0');
		setGREEN('0');
	}
	else if(ADC1->DR>=64 && ADC1->DR<128){
		setRED('1');
		setBLUE('1');
		setORANGE('0');
		setGREEN('0');
	}
	else if(ADC1->DR>=128 && ADC1->DR<192){
		setRED('1');
		setBLUE('1');
		setORANGE('1');
		setGREEN('0');
	}
	else if(ADC1->DR>=192 && ADC1->DR<256){
		setRED('1');
		setBLUE('1');
		setORANGE('1');
		setGREEN('1');
	}
	
  }
}

void toggleRED(void){
	// Toggle Pin PC6 (RED).
	if(GPIOC->IDR & 0x40){
		GPIOC->BSRR |= (1 << 22); // Resets State of PC6.
	}
	else{
		GPIOC->BSRR |= (1 << 6); // Sets State of PC6.
	}
}

/**
	* @brief Sets RED LED.
	* @retval None
	*/
void setRED(char value){
	if(value == '1'){
		GPIOC->BSRR |= (1 << 6); // Sets State of PC6.
	}
	else if(value == '0'){
		GPIOC->BSRR |= (1 << 22); // Resets State of PC6.
	}
}

/**
	* @brief Toggles BLUE LED.
	* @retval None
	*/
void toggleBLUE(void){
	// Toggle Pin PC7 (BLUE).
	if(GPIOC->IDR & 0x80){
		GPIOC->BSRR |= (1 << 23); // Resets State of PC7.
	}
	else{
		GPIOC->BSRR |= (1 << 7); // Sets State of PC7.
	}
}

/**
	* @brief Sets BLUE LED.
	* @retval None
	*/
void setBLUE(char value){
	if(value == '1'){
		GPIOC->BSRR |= (1 << 7); // Sets State of PC7.
	}
	else if(value == '0'){
		GPIOC->BSRR |= (1 << 23); // Resets State of PC7.
	}
}

/**
	* @brief Toggles ORANGE LED.
	* @retval None
	*/
void toggleORANGE(void){
	// Toggle Pin PC8 (ORANGE).
	if(GPIOC->IDR & 0x100){
		GPIOC->BSRR |= (1 << 24); // Resets State of PC8.
	}
	else{
		GPIOC->BSRR |= (1 << 8); // Sets State of PC8.
	}
}

/**
	* @brief Sets ORANGE LED.
	* @retval None
	*/
void setORANGE(char value){
	if(value == '1'){
		GPIOC->BSRR |= (1 << 8); // Sets State of PC8.
	}
	else if(value == '0'){
		GPIOC->BSRR |= (1 << 24); // Resets State of PC8.
	}
}

/**
	* @brief Toggles GREEN LED.
	* @retval None
	*/
void toggleGREEN(void){
	// Toggle Pin PC9 (GREEN).
	if(GPIOC->IDR & 0x200){
		GPIOC->BSRR |= (1 << 25); // Resets State of PC9.
	}
	else{
		GPIOC->BSRR |= (1 << 9); // Sets State of PC9.
	}
}

/**
	* @brief Sets GREEN LED.
	* @retval None
	*/
void setGREEN(char value){
	if(value == '1'){
		GPIOC->BSRR |= (1 << 9); // Sets State of PC9.
	}
	else if(value == '0'){
		GPIOC->BSRR |= (1 << 25); // Resets State of PC9.
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
