/**********************************************************************************************************************
File: user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
 1. Copy both user_app.c and user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserAppInitialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserAppRunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserAppFlags;                       /* Global state flags */
volatile  u8 flagnew=FALSE;
volatile u8 u8TimeCount=0;

/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u8 G_au8DebugScanfBuffer[];                     /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                     /* From debug.c  */
  

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp_StateMachine;            /* The state machine function pointer */
static u32 UserApp_u32Timeout;                      /* Timeout counter used across states */

static u8 UserApp_au8MyName[] = "A3.SuXiuLing";
static u8 MyName[]="suxiuling";
static u8 MyName1[]="SUXIULING";
volatile u8 MyNameBuffer[200];
static u8 UserApp_au8UserInputBuffer[USER_INPUT_BUFFER_SIZE]; 
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
Function: UserAppInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserAppInitialize(void)
{  //初始化名字和背景色
  LCDMessage(LINE1_START_ADDR, UserApp_au8MyName);
  LCDClearChars(LINE1_START_ADDR + 13, 5);
  LedOn(LCD_RED);
  LedOn(LCD_BLUE);
  LedOff(LCD_GREEN); 
  for(u8 i = 0; i < USER_INPUT_BUFFER_SIZE; i++)   //初始化Buffer
  {
    UserApp_au8UserInputBuffer[i] = 0;
  }

  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp_StateMachine = UserAppSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp_StateMachine = UserAppSM_FailedInit;
  }

} /* end UserAppInitialize() */


/*----------------------------------------------------------------------------------------------------------------------
Function UserAppRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserAppRunActiveState(void)
{
  UserApp_StateMachine();

} /* end UserAppRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserAppSM_Idle(void)
{   //定义静态变量
  static u16 u16BlinkCount=0;
  static u16 u16CharCount=0;
  static u8 u8CharCount=0;
  static u8 u8CharIndex=0;
  static u8 u8Message[]="\n\rCharacter count cleared!\n\r";
  static u8 u8i=0;
  static u8 u8j=0;
  static u8 u8k=0;
  bool flag=FALSE;
  u16BlinkCount++;
  //实时显示
  if(u16BlinkCount==10)
  {
    u16BlinkCount=0;
    flag=TRUE;
  }
  //标志正确，读取数据
  if(flag==TRUE)
  {
    u8CharCount = DebugScanf(UserApp_au8UserInputBuffer);
    UserApp_au8UserInputBuffer[u8CharCount] = '\0';
    if(u8CharCount!=0)
    {  //从左向右显示
       for(u8 i=0;i<u8CharCount;i++)
       {
         LCDMessage(LINE2_START_ADDR+u8CharIndex, UserApp_au8UserInputBuffer); 
         u8CharIndex++;
         u16CharCount++; 
       }      //清屏再从左开始
       if(u8CharIndex==21)
       {
         LCDClearChars(LINE2_START_ADDR,20); 
         LCDMessage(LINE2_START_ADDR, UserApp_au8UserInputBuffer); 
         u8CharIndex=1;
       }
    }
    flag=FALSE;
  }  
  // 按键操作
  if( WasButtonPressed(BUTTON0) )
  {
    ButtonAcknowledge(BUTTON0);
    LCDClearChars(LINE2_START_ADDR,20);
    u8CharIndex=0;
  } 
  if( WasButtonPressed(BUTTON1) )
  {
    ButtonAcknowledge(BUTTON1);
    DebugPrintf(" ");
    DebugPrintNumber(u16CharCount);
  }  
  if( WasButtonPressed(BUTTON2) )
  {
    ButtonAcknowledge(BUTTON2);
    u16CharCount=0;
    DebugPrintf(u8Message);
  }
  if( WasButtonPressed(BUTTON3) )
  {
    ButtonAcknowledge(BUTTON3);
    DebugPrintf(MyNameBuffer);
  }
 // 字符比较，挑出自己名字的字母
  if((UserApp_au8UserInputBuffer[u8j]==MyName[u8i])||(UserApp_au8UserInputBuffer[u8j]==MyName1[u8i]))
  {
    MyNameBuffer[u8k++]=UserApp_au8UserInputBuffer[u8j];
    u8i++;
    if(u8i==9)
    {
      u8i=0;
      u8TimeCount++;
    }
  }

} /* end UserAppSM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserAppSM_Error(void)          
{
  
} /* end UserAppSM_Error() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserAppSM_FailedInit(void)          
{
    
} /* end UserAppSM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
