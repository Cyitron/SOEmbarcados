#include <xc.h>

#define FCY 10000000UL

void task_1();
void task_2();
void config();

void __attribute__((interrupt())) _T1Interrupt(void);

typedef void (*f_ptr)(void);
f_ptr fila[2];
int next = 0;

int main(){
    
    config();
    
    fila[0] = task_1;
    fila[1] = task_2;
    
    __builtin_enable_interrupts();
    
    while(1){
        
    }
    
    return 0;
}

void task_1(){
    LATCbits.LATC14 = ~PORTCbits.RC14;
    
}

void task_2(){
    LATEbits.LATE0 = ~PORTEbits.RE0;
    
}

void config(){
    
    // config LEDs
    TRISCbits.TRISC14   = 0;
    TRISEbits.TRISE0    = 0;
    
    // config do timer
    T1CONbits.TON       = 1;        // liga o periférico do timer
    T1CONbits.TCKPS     = 0b10;     // Definição do prescaler
    T1CONbits.TCS       = 0;        // Fonte de clock (interna)
    SRbits.IPL          = 0b100;    // prioridade 4 da CPU
    IFS0bits.T1IF       = 0;        // flag timer 1 (zero)
    IEC0bits.T1IE       = 1;        // interrupção do timer 1 habilitada
    IPC0bits.T1IP       = 0b100;    // prioridade da interrupção do timer
    PR1                 = 128;
    
}

void __attribute__((interrupt())) _T1Interrupt(void){
    
    PR1                 = 128;
    IFS0bits.T1IF       = 0;
    
    fila[next]();
    next = (next + 1) % 2;
    
}