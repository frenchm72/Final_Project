void beep(void){
    BEEP_PORT->OUT |= BEEP_PIN;
    delay_ms(SMALL_PAUSE);
    BEEP_PORT->OUT &=~ BEEP_PIN;
    delay_ms(SMALL_PAUSE);
    BEEP_PORT->OUT |= BEEP_PIN;
    delay_ms(SMALL_PAUSE);
    BEEP_PORT->OUT &=~ BEEP_PIN;
    delay_ms(BIG_PAUSE);
}
