/*

  ******************************************************************************
  * @file 			( ???? ):   TM1637.h
  * @brief 		( ???????? ):  	?????????? 7-?????????? 4 ????????? ???????? ????? TM1637
  ******************************************************************************
  * @attention 	( ???????? ):	
  ******************************************************************************
  
*/

#ifndef _TM1637_H
#define _TM1637_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------*/

// ??????????? ?????
// ???? ???????? ?? ?????????? ????? ????????? ? ?? ? ???????????? ????????????
#include "stm32f10x.h"                  // Device header


/* Functions prototypes ----------------------------------------------*/


/*
	******************************************************************************
	* @brief	 ( ???????? ):  ??????? ???????? ? ?????????????
	* @param	( ????????? ):	???-?? ???????????
	* @return  ( ?????????? ):	

	******************************************************************************
*/
void Delay_us (uint32_t i);
//--------------------------------------------------------------------------------

/*
	******************************************************************************
	* @brief	 ( ???????? ):  ??????? ??? ?????? ???????? ?????? ??? ?????? (????????? )
	* @param	( ????????? ):	
	* @return  ( ?????????? ):	

	******************************************************************************
*/
void TM1637_Generate_START (void);
//--------------------------------------------------------------------------------

/*
	******************************************************************************
	* @brief	 ( ???????? ):  ??????? ??? ????? ???????? ?????? ??? ?????? ( ????????? ???????? )
	* @param	( ????????? ):	
	* @return  ( ?????????? ):	

	******************************************************************************
*/
void TM1637_Generate_STOP (void);
//--------------------------------------------------------------------------------

/*
	******************************************************************************
	* @brief	 ( ???????? ):  ??????? ???????? ?????? ??? ??????
	* @param	( ????????? ):	1 ???? ?????? ??? ??????
	* @return  ( ?????????? ):	
					//???????? ?? ???????? ? ????????!!!?? ????????
	******************************************************************************
*/
void TM1637_WriteData (	uint8_t oneByte);
//-----------------------------------------------------------------------------------

/*
	******************************************************************************
	* @brief	 ( ???????? ):  ?????? ?????  ?? ??? ???????  ? ??????? digitToSegment
	* @param	( ????????? ):	1 ???? ????? ??? ???????????
	* @return  ( ?????????? ):	1 ???? ?? ??????? digitToSegment ??????? ????????? ???????? ????? 
						( ??? ????? ?? ??????? )??? ???????????
		
	******************************************************************************
*/
int8_t TM1637_coding(uint8_t DispData);
//--------------------------------------------------------------------------------

/*
	******************************************************************************
	* @brief	 ( ???????? ):  ?????? ?????? ????? ?? 1 ?? 4  ?? ??? ???????  ? ??????? digitToSegment
	* @param	( ????????? ):	?????? ???? ??? ???????????
	* @return  ( ?????????? ):	?????? ???? ?? ??????? digitToSegment ??????? ????????? ????????? ?????
						( ??? ????? ?? ??????? )??? ???????????
		
	******************************************************************************
*/
void TM1637_coding_all(uint8_t DispData[]);
//-------------------------------------------------------------------------------

/*
	******************************************************************************
	* @brief	 ( ???????? ):  ????????? ????? ?? "????????" ????? 251 ???????? 2 5 1 ??? ??????? ????????
	* @param	( ????????? ):	1 ???. ???? ?????, 2 ???. ?????? ??? ???????? ??? ? ????????? ?????????? ???????
	* @return  ( ?????????? ):			
	******************************************************************************
*/
void separate_Digit_to_digits(int16_t Digit,uint8_t SegArray[]);
//------------------------------------------------------------------------------------------

/*
	******************************************************************************
	* @brief	 ( ???????? ):  ?????????? ???? ?????? ?? ???????
	* @param	( ????????? ):	1 ???. ????? ??????? ??????? ( ?? 0 ?? 3 )
								2 ???. ????? ??????? ????? ??????????
	* @return  ( ?????????? ):			
	******************************************************************************
*/
void TM1637_display(uint8_t Seg_N, int8_t DispData);
//----------------------------------------------------------------------------------------------

/*
	******************************************************************************
	* @brief	 ( ???????? ):  ??????????? ????? ?? -999 ?? 9999
	* @param	( ????????? ):	????? ?? 1 ?? 4 ???????? ?? -999 ?? 9999
	* @return  ( ?????????? ):			
	******************************************************************************
*/
void TM1637_display_all(uint16_t Digit);
//------------------------------------------------------------------------------------

/*
	******************************************************************************
	* @brief	 ( ???????? ):  ????????? ? ?????????? ??????
	* @param	( ????????? ):	1 ???     0 ????
	* @return  ( ?????????? ):			
	******************************************************************************
*/
void TM1637_displayOnOff(uint8_t OnOff);
//-----------------------------------------------------------------------------------------

/*
	******************************************************************************
	* @brief	 ( ???????? ):  ???????? ???????
	* @param	( ????????? ):	
	* @return  ( ?????????? ):			
	******************************************************************************
*/
void TM1637_clearDisplay(void);
//---------------------------------------------------------------------------------

/*
	******************************************************************************
	* @brief	 ( ???????? ):  ??????? ???????
	* @param	( ????????? ):	????? ??????? ?? 0 ?? 7 ( 0-???    7-???)
	* @return  ( ?????????? ):			
	******************************************************************************
*/
void TM1637_brightness(uint8_t brightness);
//---------------------------------------------------------------------------------

/*
	******************************************************************************
	* @brief	 ( ???????? ):  ??????????? ? ???? ????? 4 ???????
	* @param	( ????????? ):	1 ???. ????   2 ???. ??????
	* @return  ( ?????????? ):			
	******************************************************************************
*/
void TM1637_displayTime(uint8_t hours,uint8_t minutes);	
//------------------------------------------------------------------------------------

/*
	??????
	
	TM1637_brightness(7);	// ?????? ??????? ???????
	TM1637_clearDisplay();	// ??????? ???????
	TM1637_display(0,2);	// ????????????? ? ?????? ??????? ????? 2
	TM1637_display_all(257);// ????????????? ?? ???? ??????? ????? 257
	TM1637_displayTime(17,20);
	
*/

#ifdef __cplusplus
}
#endif

#endif	/*	_TM1637_H */

/************************ (C) COPYRIGHT GKP *****END OF FILE****/