#include <avr/io.h>
#include <avr/interrupt.h>

int second0 = 0;
int minute0 = 0;
int hour0 = 0;
int second1 = 0;
int minute1 = 0;
int secondold = 0;
int state = 0;
int countdown = 0;
int operationtag = 0;
int operationA = 0;
int operationB = 0;
int operationR = 0;
int digitpointer = 0;
int buttonhold = 0;
const unsigned char PEA[11]= {0b11111100,0b01100000,0b11011010,0b11110010,0b01100110
							 ,0b10110110,0b10111110,0b11100000,0b11111110,0b11110110,0b00000010};

int main(){
  DDRB = 0xFF;
  DDRC = 0x00;
  DDRD = 0xFF;
  PORTB = 0x00;
  PORTC = 0xFF;
  PORTD = 0x00;
  init_timer1();
  while(1){
    timeticking();
    if (state == 0){
      digitalwatch();
    }else if (state == 1){
      stopwatch();
    }else if (state == 2){
      calculator();
    }
    button();
  }
}

void timeticking(){
  if(second0 > 59){
    second0 = 0;
    minute0 += 1;
    if(minute0 > 59){
      hour0 +=1;
      minute0 = 0;
      if(hour0 > 23){
        hour0 = 0;
      }
    }
  }
}

void digitalwatch(){
  display7seg(minute0,hour0);
}

void stopwatch(){
  if(countdown == 1){
    if (secondold != second0){
      if(second1 == 0){
        if (minute1 == 0){
          countdown = 0;
        }else{
          minute1 -= 1;
          second1 = 59;
        }
      }else{
        second1 -= 1;
      }
      secondold = second0;
    }
  }
  display7seg(second1,minute1);
}

void calculator(){
  if (operationtag == 0){
    display7segtype2(operationA);
  }else if (operationtag == 1){
    display7segtype2(operationB);
  }else if (operationtag == 2){
    display7segtype2(operationR);
  }
}
void button(){
  unsigned char i;
  i = PINC;
  if (state == 0){
    if(buttonhold == 0){
      if(i == 0b00111110){//button right
        if(digitpointer>0){
          digitpointer -= 1;
        }
        buttonhold = 1;
      }else if(i == 0b00111101){//button left
        if(digitpointer<3){
          digitpointer += 1;
        }
        buttonhold = 1;
      }else if(i == 0b00111011){//button up       
        int digitadder=1;
        int j;
        if(digitpointer<2){
          j=0;
          while(j<digitpointer){
            digitadder *= 10;
            j++;
          }
          if((minute0+digitadder) < 60){
            minute0 += digitadder;
          }
        }else{
          j=2;
          while(j<digitpointer){
            digitadder *= 10;
            j++;
          }
          if((hour0+digitadder) < 24){
            hour0 += digitadder;
          }
        }
        buttonhold = 1;
      }else if(i == 0b00110111){//button down
        int digiterase=1;
        int j;
        if(digitpointer<2){
          j=0;
          while(j<digitpointer){
            digiterase *= 10;
            j++;
          }
          if((minute0-digiterase) >= 0){
            minute0 -= digiterase;
          }
        }else{
          j=2;
          while(j<digitpointer){
            digiterase *= 10;
            j++;
          }
          if((hour0-digiterase) >= 0){
            hour0 -= digiterase;
          }
        }
        buttonhold = 1;
      }else if(i == 0b00101111){//button mode change
        state = 1;
        second1 = 0;
        minute1 = 0;
        countdown = 0;
        buttonhold = 1;
      }
    }else{
      if(i == 0b00111111){
        buttonhold = 0;
      }
    }
  }else if (state == 1){
    if(buttonhold == 0){
      if(countdown == 0){
        if(i == 0b00111110){//button right
          if(digitpointer>0){
            digitpointer -= 1;
          }
          buttonhold = 1;
        }else if(i == 0b00111101){//button left
          if(digitpointer<3){
            digitpointer += 1;
          }
          buttonhold = 1;
        }else if(i == 0b00111011){//button up       
          int digitadder=1;
          int j;
          if(digitpointer<2){
            j=0;
            while(j<digitpointer){
              digitadder *= 10;
              j++;
            }
            if((second1+digitadder) < 60){
              second1 += digitadder;
            }
          }else{
            j=2;
            while(j<digitpointer){
              digitadder *= 10;
              j++;
            }
            if((minute1+digitadder) < 100){
              minute1 += digitadder;
            }
          }
          buttonhold = 1;
        }else if(i == 0b00110111){//button down
          int digiterase=1;
          int j;
          if(digitpointer<2){
            j=0;
            while(j<digitpointer){
              digiterase *= 10;
              j++;
            }
            if((second1-digiterase) >= 0){
              second1 -= digiterase;
            }
          }else{
            j=2;
            while(j<digitpointer){
              digiterase *= 10;
              j++;
            }
            if((minute1-digiterase) >= 0){
              minute1 -= digiterase;
            }
          }
          buttonhold = 1;
        }else if(i == 0b00101111){//button mode change
          state = 2;
          operationtag = 0;
          operationA = 0;
          operationB = 0;
          operationR = 0;
          buttonhold = 1;
        }else if(i == 0b00011111){//button enter
          secondold = second0;
          countdown = 1;
          buttonhold = 1;
        }
      }else{
        if(i == 0b00101111){//button mode change
          state = 2;
          operationtag = 0;
          operationA = 0;
          operationB = 0;
          operationR = 0;
          buttonhold = 1;
        }else if(i == 0b00011111){//button enter
          countdown = 0;
          buttonhold = 1;
        }
      }
    }else{
      if(i == 0b00111111){
        buttonhold = 0;
      }
    }
  }else if (state == 2){
    if(buttonhold == 0){
      if(i == 0b00111110){//button right
        if(digitpointer>0){
          digitpointer -= 1;
        }
        buttonhold = 1;
      }else if(i == 0b00111101){//button left
        if(digitpointer<2){
          digitpointer += 1;
        }
        buttonhold = 1;
      }else if(i == 0b00111011){//button up
        int digitadder=1;
        int j=0;
        while(j<digitpointer){
          digitadder *= 10;
          j++;
        }
        if((operationtag == 0) && ((operationA+digitadder) <1000)){
          operationA += digitadder;
        }else if ((operationtag == 1) && ((operationB+digitadder) < 1000)){
          operationB += digitadder;
        }
        buttonhold = 1;
      }else if(i == 0b00110111){//button down
        int digiterase=1;
        int j=0;
        while(j<digitpointer){
          digiterase *= 10;
          j++;
        }
        if((operationtag == 0) && ((operationA-digiterase) >= 0)){
          operationA -= digiterase;
        }else if ((operationtag == 1) && ((operationB-digiterase) >= 0)){
          operationB -= digiterase;
        }
        buttonhold = 1;
      }else if(i == 0b00101111){//button mode change
        state = 0;
        buttonhold = 1;
      }else if(i == 0b00011111){//button enter
        if(operationtag == 0){
          operationtag = 1;
        }else if (operationtag == 1){
          operationR = operationA - operationB;
          operationtag = 2;
        }else if (operationtag == 2){
          operationtag = 0;
        }
        buttonhold = 1;
      }
    }else{
      if(i == 0b00111111){
        buttonhold = 0;
      }
    }
  }
} 

void display7seg(int digit1,int digit2){
  int i=0;
  unsigned char j=0x01;
  while(i<2){
    PORTB = 0xFF;
    if (i == digitpointer){
      PORTD = PEA[digit1%10] | 0x01;
    }else{
      PORTD = PEA[digit1%10];
    }
    PORTB = ~j;
    j = j<<1;
    digit1 /=10;
    i++;
  }
  while(i<4){
    PORTB = 0xFF;
    if (i == digitpointer){
      PORTD = PEA[digit2%10] | 0x01;
    }else{
      PORTD = PEA[digit2%10];
    }
    PORTB = ~j;
    j = j<<1;
    digit2 /=10;
    i++;
  }
}

void display7segtype2(int digit){
  int i=0;
  unsigned char j=0x01;
  if(digit >=0){
    while(i<3){
      PORTB = 0xEF;
      if (i == digitpointer){
        PORTD = PEA[digit%10] | 0x01;
      }else{
        PORTD = PEA[digit%10];
      }
      PORTB = (~j)&(0xEF);
      j = j<<1;
      digit /=10;
      i++;
    }
  }else{
    digit = -digit;
    while(i<3){
      PORTB = 0xEF;
      if (i == digitpointer){
        PORTD = PEA[digit%10] | 0x01;
      }else{
        PORTD = PEA[digit%10];
      }
      PORTB = (~j)&(0xEF);
      j = j<<1;
      digit /=10;
      i++;
    }
    PORTB = 0xEF;
    if (i == digitpointer){
      PORTD = PEA[10] | 0x01;
    }else{
      PORTD = PEA[10];
    }
    PORTB = (~j)&(0xEF);
  }
}
void init_timer1(){
    // initialize Timer1
    cli();          // disable global interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B
 
    // set compare match register to desired timer count:
    OCR1A = 0xF423;
    // turn on CTC mode and set CS12 bits for 256 prescaler:
    TCCR1B |= (1 << WGM12)|(1 << CS12);
    // enable timer compare interrupt:
    TIMSK1 |= (1 << OCIE1A);
    sei();          // enable global interrupts 
}
ISR (TIMER1_COMPA_vect)
{
  // action to be done every 1 sec
  second0 +=1;
}
