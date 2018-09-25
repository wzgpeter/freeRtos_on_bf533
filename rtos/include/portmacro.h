/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

/* Type definitions. */
#define portCHAR        char
#define portFLOAT       float
#define portDOUBLE      double
#define portLONG        long
#define portSHORT       short
#define portSTACK_TYPE  unsigned int
#define portBASE_TYPE   int

typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;


#if (configUSE_16_BIT_TICKS==1)
	typedef uint16_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffff
#else
	typedef uint32_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#endif
/*-----------------------------------------------------------*/

//extern unsigned int cpu_sr;
/* Interrupt control macros. */
#define portDISABLE_INTERRUPTS()	(cpu_sr = cli())	//chg by wzg
#define portENABLE_INTERRUPTS()	 	(sti(cpu_sr))	//chg by wzg
/*-----------------------------------------------------------*/

/* Critical section control macros. */
#define portNO_CRITICAL_SECTION_NESTING		( ( UBaseType_t ) 0 )

#define portENTER_CRITICAL()														\
{																					\
extern volatile uint16_t usCriticalNesting;						\
																					\
	portDISABLE_INTERRUPTS();														\
																					\
	/* Now interrupts are disabled ulCriticalNesting can be accessed */				\
	/* directly.  Increment ulCriticalNesting to keep a count of how many */		\
	/* times portENTER_CRITICAL() has been called. */								\
	usCriticalNesting++;															\
}

#define portEXIT_CRITICAL()															\
{																					\
extern volatile uint16_t usCriticalNesting;						\
																					\
	if( usCriticalNesting > portNO_CRITICAL_SECTION_NESTING )						\
	{																				\
		/* Decrement the nesting count as we are leaving a critical section. */		\
		usCriticalNesting--;														\
																					\
		/* If the nesting level has reached zero then interrupts should be */		\
		/* re-enabled. */															\
		if( usCriticalNesting == portNO_CRITICAL_SECTION_NESTING )					\
		{																			\
			portENABLE_INTERRUPTS();												\
		}																			\
	}																				\
}
/*-----------------------------------------------------------*/

/* Task utilities. */
extern void vPortYield( void );
extern void vPortStart( void );
extern void portSAVE_CONTEXT( void );
extern void portRESTORE_CONTEXT( void );
//#define portYIELD()	    asm("raise 14;")//__asm ( "trap 0" )    //do the context switching
#define portNOP()	    asm("NOP;")     //__asm ( "NOP" )       //only NOP


#define portYIELD()                     \
{                                       \
    asm("raise 14;");                   \
}


extern void vTaskSwitchContext( void );
//#define portYIELD_FROM_ISR( xHigherPriorityTaskWoken ) if( xHigherPriorityTaskWoken ) vTaskSwitchContext()
#define portYIELD_FROM_ISR( xHigherPriorityTaskWoken ) if( xHigherPriorityTaskWoken ) portYIELD()


/*-----------------------------------------------------------*/

/* Hardwware specifics. */
#define portBYTE_ALIGNMENT	4
#define portSTACK_GROWTH	( -1 )
#define portTICK_PERIOD_MS	( ( TickType_t ) 1000 / configTICK_RATE_HZ )
/*-----------------------------------------------------------*/

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )


#ifndef ULONG_MAX
#define ULONG_MAX   0xffffffff
#endif //ULONG_MAX


/*******************************************************************************
 *  MACROS
 ******************************************************************************/
#define mainCHECK_TASK_PRIORITY		( tskIDLE_PRIORITY + 4 )
#define mainUART_TASK_PRIORITY		( tskIDLE_PRIORITY + 3 )
#define mainSPORT_TASK_PRIORITY		( tskIDLE_PRIORITY + 3 )
#define mainSPI_TASK_PRIORITY		( tskIDLE_PRIORITY + 3 )
#define mainI2C_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define mainLED_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )


#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */
