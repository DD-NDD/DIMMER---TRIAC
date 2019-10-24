#include <main.h>
#include <stdint.h>
#include <stdbool.h>
#byte OPTION_REG = GETENV("SFR:OPTION_REG")
#bit  INTEDG = OPTION_REG.6
int1 ZC = 0;
unsigned int16 level[10] = {9000,8000,7000,6000,5000,4000,3000,2000,1000,0};
unsigned int8 i=0;
#define UP_SWITCH_PIN       PIN_B2
#define ON_OFF_SWITCH_PIN   PIN_B3
#define DOWN_SWITCH_PIN     PIN_B4
#define MODE_SWITCH_PIN      PIN_B5

#define HAND_MODE 0
#define AUTO_MODE 1
#define ON_STATE 1
#define OFF_STATE 0
bool UP_DOWN_AUTO = 0;
bool STATE = 0;
bool MODE = 0;
TICK_TYPE GetTickDifference(TICK_TYPE currTick, TICK_TYPE prevTick)
{
   return(currTick-prevTick);
}
unsigned int8 check_limit(uint8_t level_check)
{  
   if(level_check <= 0 ) level_check = 0;
   else if(level_check >=9 ) level_check = 9;
   
   return level_check;
}
void tick(void)
{
   if(input(ON_OFF_SWITCH_PIN) == 0) // nut on off duoc nhan
   {
      STATE = !STATE; // dao nguoc trang thai on off
      delay_ms(500); // delay 500ms
   }
   
   if(STATE == ON_STATE) // khi dang trong trang thai bat
   {
      output_high(LED10);
      if(input(MODE_SWITCH_PIN) == 0) // khi nut MODE duoc nhan
      {
         MODE = !MODE; // dao nguoc trang thai nhan
         delay_ms(500);
      }
      
      if(MODE == HAND_MODE) // khi che do bang tay
      {
         output_high(LED9);
         if( input(UP_SWITCH_PIN) == 0 ) // nut nhan tang
         {
            i++;  // tang level
         }
         else if( input(DOWN_SWITCH_PIN) == 0 ) // nhut nhan giam
         {
            i--; // giam level
         }
      }
      else if(MODE == AUTO_MODE) // che do auto
      {
         output_low(LED9);
         if(UP_DOWN_AUTO == 1) // tu dong tang len
         {
            i++;
            if(i >= 9) UP_DOWN_AUTO = 0; //dao chieu
         }
         else //tu dong giam xuong
         {
            i--;
            if(i <= 0) UP_DOWN_AUTO = 1; // dao chieu
         }
      }
   }
   else if(STATE == OFF_STATE) // khi trang thai tat
   {
      output_low(LED10); // tat den on off
      output_low(LED9); // tat den auto - hand
   }
}

#INT_EXT
void  EXT_isr(void) 
{
   
   ZC = 1;
   if(INTEDG) INTEDG = 0;
  else       INTEDG = 1;
}
void led_display(uint8_t level)
{
   switch(level)
   {
      case 1: {output_c(0x00);break;}
      case 2: {output_c(0x01);break;}
      case 3: {output_c(0x03);break;}
      case 4: {output_c(0x07);break;}
      case 5: {output_c(0x0f);break;}
      case 6: {output_c(0x8f);break;}
      case 7: {output_c(0xCf);break;}
      case 8: {output_c(0xEf);break;}
      case 9: {output_c(0xff);break;}
      default: break;
   }
}



void main()
{
   TICK_TYPE CurrentTick,PreviousTick; // tao tick timer
   CurrentTick = PreviousTick = get_ticks(); // lay tick timer
   enable_interrupts(INT_EXT); // bat ngat ngoai
   enable_interrupts(GLOBAL); // bat ngat toan cuc
   STATE = OFF_STATE;   // state = off
   MODE = AUTO_MODE; // mode = auto
   output_low(LED10);   // tat led on off
   output_low(LED9); // tat led auto - hand
   led_display(0);   // hien thi den
   while(TRUE)
   {
      CurrentTick = get_ticks(); // doc tick timer
      if(GetTickDifference(CurrentTick, PreviousTick) >= ((TICK_TYPE)TICKS_PER_SECOND*100)/1000) // sau 100ms thi bat vao ngat de doc nut nhan
      {
         tick(); // ham xu ly chinh
         PreviousTick = CurrentTick;
      }
      
      i = check_limit(i);
      led_display(i);
      if(ZC == 1)
      {
         delay_us(level[i]);
         output_high(TRIAC);
         delay_us(100);
         output_low(TRIAC);
         ZC = 0;
      }
   }

}
