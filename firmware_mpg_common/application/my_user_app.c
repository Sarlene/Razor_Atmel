/**********************************************************************************************************************
File: my_user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this my_user_app as a template:
 1. Copy both my_user_app.c and my_user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "my_user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "MyUserApp" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "MY_USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a my_user_app.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void MyUserAppInitialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void MyUserAppRunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32MyUserAppFlags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


extern volatile bool flagnew;
extern volatile u8 u8TimeCount;
extern volatile u8 MyNameBuffer[200];  

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "MyUserApp_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type MyUserApp_StateMachine;            /* The state machine function pointer */
static u32 MyUserApp_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: MyUserAppInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void MyUserAppInitialize(void)
{
  PWMAudioSetFrequency(BUZZER1, 200);

  /* If good initialization, set state to Idle */
  if( 1 )
  {
    MyUserApp_StateMachine = MyUserAppSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    MyUserApp_StateMachine = MyUserAppSM_FailedInit;
  }

} /* end MyUserAppInitialize() */


/*----------------------------------------------------------------------------------------------------------------------
Function MyUserAppRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void MyUserAppRunActiveState(void)
{
  MyUserApp_StateMachine();

} /* end MyUserAppRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void MyUserAppSM_Idle(void)
{
  
} /* end MyUserAppSM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void MyUserAppSM_Error(void)          
{
  static u16 u16Count=0;
  static u16 u16Count1 = 0;
  if(flagnew)
  {
    u16Count++;
    PWMAudioOn(BUZZER1);
    LedBlink(RED, LED_1HZ);
    // print 5s
    if(u16Count==100) 
    {
      u16Count1++;
      u16Count=0;
      if(u16Count1==50)
      {
        PWMAudioOff(BUZZER1);
        LedOff(RED);
        
        //DebugPrintf(u8String2);
        DebugPrintNumber(u8TimeCount);
        
        u16Count1=0;
      }
    }
    flagnew=FALSE;
  }

} /* end MyUserAppSM_Error() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void MyUserAppSM_FailedInit(void)          
{
    
} /* end MyUserAppSM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
