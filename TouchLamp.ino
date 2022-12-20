#include <avr/io.h>
#include <avr/interrupt.h>

//define attiny pins
#define LED_PIN PB1
#define INTERRUPT_PIN PCINT0      // This is PB0 per the schematic
#define INT_PIN PB0               // Interrupt pin PB0 (same as PCINT1) - Pin 5
#define PCINT_VECTOR PCINT0_vect  // Naming thing for clarit

//timing setup
const int delayTime = 1;
volatile uint8_t* Port[] = {&OCR0A, &OCR0B, &OCR1B};

//variables
int bright[] = {100, 75, 50, 25, 0}; //brightness in %
int level;

void setup()
{
  pinMode(LED_PIN, OUTPUT); 
  
  cli();               // Disable interrupts during setup
  // Interrupts
  PCMSK |= (1 << INTERRUPT_PIN);    // Enable interrupt handler (ISR)
  GIMSK |= (1 << PCIE);             // Enable PCINT interrupt in the general interrupt mask
  pinMode(INT_PIN, INPUT_PULLUP);   // Set our interrupt pin as input with a pullup to keep it stable
  //Timing
  TCCR0A = 3<<COM0A0 | 3<<COM0B0 | 3<<WGM00; // Configure counter/timer0 for fast PWM on PB0 and PB1
  TCCR0B = 0<<WGM02 | 3<<CS00;    // Optional; already set
  
  level = 0;
  sei();              //enable interrupts after setup
}

void loop()
{
  *Port[LED_PIN] = map(bright[level],0,100,0,255);
}

ISR(PCINT_VECTOR)
{
  if( digitalRead(INT_PIN) == HIGH )
  {
    if(level < 4)
    {
      level +=1;
    }
    else
    {
      level = 0;
    }
  }
}
