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


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp_StateMachine;            /* The state machine function pointer */
static u32 UserApp_u32Timeout;                      /* Timeout counter used across states */

static bool flag=TRUE;
static u8 i=0;
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
{
  LedOn(LCD_BLUE);
  LedOff(LCD_GREEN);
  LedOn(LCD_RED);
  u8 au8WelcomeMessage[]="Welcome to Delicious";
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR,au8WelcomeMessage);

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
{
  static u8 LCDColourIndex=0;
  static u16 Total=0;
  static u8 au8DisplayCost2[5];
  unsigned char NUM[6]={0,0,0,0,0,0};
  u8 au8Message1[] = "Yes            ";
  u8 au8Message2[] = "No             ";
  u8 au8Message3[]=" $ ";
  static u16 u16BlinkCount = 0;        /*time counter 100ms*/
  static u16 u16Counter = 1;           /*which light to bright*/
  static u8 Led[]={WHITE,PURPLE,BLUE,CYAN,GREEN,YELLOW};
  static u8 Price[]={30,48,22,20,12,15};
  static u8 au8DisplayCost[3]; 
  static u8 LEDColourIndex=5;
  static u8 Button2Counter=0;  
  if(flag)
  {
    u16BlinkCount++;                    /*time goes*/
    if(u16BlinkCount == 100)             /*time to change the light*/
    {
      u16BlinkCount = 0;               /*clear the time counter*/
      /* Update the counter and roll at 16 */
      if(u16Counter == 256)                /*for light to come back*/
      {
        u16Counter = 1;
      }
      /*bright the lights in turn*/
      if(u16Counter & 0x01)
      {
        LedOn(RED);
      }
      else
      {
        LedOff(RED);
      }
      if(u16Counter & 0x02)
      {
        LedOn(ORANGE);
      }
      else
      {
        LedOff(ORANGE);
      }
      if(u16Counter & 0x04)
      {
        LedOn(YELLOW);
       }
      else
      {
        LedOff(YELLOW);
      }
      if(u16Counter & 0x08)
      {
        LedOn(GREEN);
      }
      else
      {
        LedOff(GREEN);
      }
      if(u16Counter & 0x10)
      {
        LedOn( CYAN);
      }
      else
      {
        LedOff( CYAN);
      }
      if(u16Counter & 0x20)
      {
        LedOn(BLUE);
      }
      else
      {
        LedOff(BLUE);
      }
      if(u16Counter & 0x40)
      {
        LedOn(PURPLE);
      }
      else
      {
        LedOff(PURPLE);
      }
      if(u16Counter & 0x80)
      {
        LedOn(WHITE);
      }
      else
      {
        LedOff(WHITE);
      }
      u16Counter*=2;
    }
  }
    /*if press button3,then start to order*/
  if(WasButtonPressed(BUTTON3))
  {
    flag=FALSE;
    ButtonAcknowledge(BUTTON3);
    LedOff(WHITE);
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOn(ORANGE);
    LedOff(RED);
    LedOn(LCD_RED);
    LedOn(LCD_GREEN);
    LedOn(LCD_BLUE);  
    LEDColourIndex+=1;                 /*display the kinds of food*/
    LedOn(Led[LEDColourIndex%6]);
    LCDMessage(LINE2_START_ADDR+17, au8Message3);
    LCDMessage(LINE2_START_ADDR, au8Message2);
    Button2Counter=0;
    au8DisplayCost[0]=Price[LEDColourIndex%6]/10+48;              /*display price*/
    au8DisplayCost[1]=Price[LEDColourIndex%6]%10+48;
    LCDMessage(LINE2_START_ADDR+15, au8DisplayCost);
  }
  if(WasButtonPressed(BUTTON2))    /*if press button2,then display No*/
  {
    ButtonAcknowledge(BUTTON2);
    if(Button2Counter++%2==0)
    {
      NUM[LEDColourIndex%6]++;
      LCDMessage(LINE2_START_ADDR, au8Message1);   /*if press button2 again,then display Yes*/
    }
    else
    {
      LCDMessage(LINE2_START_ADDR, au8Message2);
    }
  }    /* price calculation */
  for(u8 j=0;j<6;j++)
  {
    Total += Price[j]*NUM[j];
  }
  if(WasButtonPressed(BUTTON1))       /*if press button1,then move left to change */
  {
    ButtonAcknowledge(BUTTON1);
    LedOff(WHITE);
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOn(ORANGE);
    LedOff(RED);
    LedOn(LCD_RED);
    LedOn(LCD_GREEN);
    LedOn(LCD_BLUE);
    LEDColourIndex-=1;
    LedOn(Led[LEDColourIndex%6]);
    LCDMessage(LINE2_START_ADDR, au8Message2);
    Button2Counter=0;
    au8DisplayCost[0]=Price[LEDColourIndex%6]/10+48;
    au8DisplayCost[1]=Price[LEDColourIndex%6]%10+48;
    LCDMessage(LINE2_START_ADDR+15, au8DisplayCost);

  } 
  /*if press button0,then start to inc LCDColourIndex*/
  if( WasButtonPressed(BUTTON0) )
  {
    /* Be sure to acknowledge the button press */
    ButtonAcknowledge(BUTTON0);
    LCDColourIndex++;
    LCDMessage(LINE2_START_ADDR, au8Message2);
    Button2Counter=0;
    switch(LCDColourIndex%7)                       /*start to change LCD*/         
    {                                             
      case 0: /* white */
        LedOn(LCD_RED);
        LedOn(LCD_GREEN);
        LedOn(LCD_BLUE);
        break;

      case 1: /* purple */
        LedOn(LCD_RED);
        LedOff(LCD_GREEN);
        LedOn(LCD_BLUE);
        break;
        
      case 2: /* blue */
        LedOff(LCD_RED);
        LedOff(LCD_GREEN);
        LedOn(LCD_BLUE);
        break;
        
      case 3: /* cyan */
        LedOff(LCD_RED);
        LedOn(LCD_GREEN);
        LedOn(LCD_BLUE);
        break;
        
      case 4: /* green */
        LedOff(LCD_RED);
        LedOn(LCD_GREEN);
        LedOff(LCD_BLUE);
        break;
        
      case 5: /* yellow */
        LedOn(LCD_RED);
        LedOn(LCD_GREEN);
        LedOff(LCD_BLUE);
        break;
        
      case 6: /* red */
        LedOn(LCD_RED);
        LedOff(LCD_GREEN);
        LedOff(LCD_BLUE);
        break;

    } /* end switch */
  }
  if(IsButtonHeld(BUTTON3,1000))    /*if press button3 1s,then music on*/
  {
    au8DisplayCost2[0]=Total/1000+48;              
    au8DisplayCost2[1]=Total/100%10+48;
    au8DisplayCost2[2]=Total/10%10+48;              /*display price*/
    au8DisplayCost2[3]=Total%10+48;
    LCDClearChars(LINE2_START_ADDR,20);
    LCDMessage(LINE2_START_ADDR+10, au8DisplayCost2);
    UserApp_StateMachine = UserAppSM_Buzzer;
  }

} /* end UserAppSM_Idle() */
     

static void UserAppSM_Buzzer(void)          /* music */
{
  static bool flag1=FALSE;
  static u8 u8TimeStamp=500;
  static u8 u8MusicNote=0;
  char MusicScore[]="022333055550022333005555055666031222220022333055550022333005555055666031222220006677AAAA77AAA00677AAAA77AAA00367AAAA7ACCCCBBAA777700336677AAAA77AAAA6677AA777765321111001666556633331133222200336677AAAA77AAAAA6677AA77776532111100166554433332211222205544332333111110";
  u8TimeStamp--;
  if(u8TimeStamp==0 && MusicScore[u8MusicNote]!='\0')
  {
    switch(MusicScore[u8MusicNote])
    {
      case '0':
        PWMAudioOff(BUZZER1);
        LedOn(WHITE);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(RED);
        break;
      case '1':
        PWMAudioSetFrequency(BUZZER1, 523);
        PWMAudioOn(BUZZER1);
        LedOn(PURPLE);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(WHITE);
        LedOff(BLUE);
        LedOff(RED);
        break;
      case '2':
        PWMAudioSetFrequency(BUZZER1, 587);
        PWMAudioOn(BUZZER1);
        LedOn(BLUE);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(PURPLE);
        LedOff(WHITE);
        LedOff(RED);
        break;
      case '3':
        PWMAudioSetFrequency(BUZZER1, 659);
        PWMAudioOn(BUZZER1);
        LedOn(CYAN);
        LedOff(WHITE);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(RED);
        break;
      case '4':
        PWMAudioSetFrequency(BUZZER1, 698);
        PWMAudioOn(BUZZER1);
        LedOn(GREEN);
        LedOff(CYAN);
        LedOff(WHITE);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(RED);
        break;
      case '5':
        PWMAudioSetFrequency(BUZZER1, 783);
        PWMAudioOn(BUZZER1);
        LedOn(YELLOW);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(WHITE);
        LedOff(ORANGE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(RED);
        break;
      case '6':
        PWMAudioSetFrequency(BUZZER1, 880);
        PWMAudioOn(BUZZER1);
        LedOn(ORANGE);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(WHITE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(RED);
        break;
      case '7':
        PWMAudioSetFrequency(BUZZER1, 987);
        PWMAudioOn(BUZZER1);
        LedOn(RED);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(WHITE);
        break;
      case 'A':
        PWMAudioSetFrequency(BUZZER1, 1046);
        PWMAudioOn(BUZZER1);
        LedBlink(RED,LED_1HZ);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(WHITE);
        break;
      case 'B':
        PWMAudioSetFrequency(BUZZER1, 1174);
        PWMAudioOn(BUZZER1);
        LedBlink(RED,LED_2HZ);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(WHITE);
        break;
      case 'C':
        PWMAudioSetFrequency(BUZZER1, 1318);
        PWMAudioOn(BUZZER1);
        LedBlink(RED,LED_4HZ);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(WHITE);
        break;
    }
    u8MusicNote++;
  }
  if(WasButtonPressed(BUTTON0))       /* hurry up */
  {
    flag1=TRUE;
    ButtonAcknowledge(BUTTON0);
    LedOn(LCD_RED);
    LedOff(LCD_GREEN);
    LedOff(LCD_BLUE);
  }
  if(IsButtonHeld(BUTTON2, 1000))             /*if press button3 1s,then music off*/
  {
    flag=TRUE;
    LedOff(WHITE);
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(ORANGE);
    LedOff(RED);
    LedOn(LCD_RED);
    LedOn(LCD_GREEN);
    LedOn(LCD_BLUE);
    PWMAudioOff(BUZZER1);
    UserApp_StateMachine = UserAppInitialize;
  }
} /* end UserAppSM_Buzzer() */
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
