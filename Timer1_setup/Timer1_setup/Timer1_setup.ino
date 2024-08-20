#define Timer1Prescaler 1

void setup_timer1() {
  DDRB |= (1 << PB1);

  TCNT1 = 0;  // Initialize Timer 1 counter

  TCCR1A = 0;
  TCCR1B = 0;

  TCCR1A |= (1 << COM1A1) | (0 << COM1A0);  // Toggle on compare match mode

  TCCR1A |= (0 << WGM10) | (1 << WGM11);
  TCCR1B |= (1 << WGM12) | (1 << WGM13);  // Fast PWM mode

  TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);  // no prescaler
}

void setPWM_D9(uint16_t freq, uint16_t duty) {

  ICR1 = (F_CPU / (Timer1Prescaler * freq)) - 1;
  OCR1A = ICR1 / (100 / duty);
}

void setup() {
  setup_timer1();

  setPWM_D9(13000, 50);
}

void loop() {

}
