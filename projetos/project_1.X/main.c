// Primeiro exemplo PIC24

#include <xc.h>

#define FCY 10000000UL

#include <libpic30.h>

typedef void (*f_ptr)(void);

void config_ports();
void task_1();
void task_2();
void ler_botao();
void config_int0();

// Tratador de interrupção
void __attribute__((interrupt())) _INT0Interrupt(void);


// Fila de tarefas
f_ptr fila[2];
int next = 0;

int main()
{
    config_ports();
    config_int0();
    
    // Instalar as tarefas
    fila[0] = task_1;
    fila[1] = task_2;
    
    __builtin_enable_interrupts();
    
    while (1) {
       
    }
    
    return 0;
}

void config_ports()
{
    TRISCbits.TRISC14 = 0; // Saída de dados
    TRISCbits.TRISC15 = 0; // Saída de dados
    TRISEbits.TRISE0  = 0;
}

void task_1()
{
    LATCbits.LATC14 = ~PORTCbits.RC14;
    __delay_ms(50);
}

void task_2()
{
    //LATCbits.LATC15 = ~PORTCbits.RC15;
    LATEbits.LATE0 = ~PORTEbits.RE0;
    __delay_ms(50);
}

void ler_botao()
{
    if (PORTFbits.RF0 == 1) {
        fila[next]();
        next = (next + 1) % 2;
    }
}

void config_int0()
{
    // Prioridade de CPU
    SRbits.IPL          = 0b100;    // Prioridade 4
    INTCON2bits.INT0EP  = 0;        // Borda de subida (sinal positivo)
    IFS0bits.INT0IF     = 0;        // Flag da interrupção zero (não atividado)
    IEC0bits.INT0IE     = 1;        // Habilita interrupção externa zero
    IPC0bits.INT0IP     = 0b100;    // Prioridade 4
    
}

void __attribute__((interrupt())) _INT0Interrupt(void)
{
    IFS0bits.INT0IF     = 0;   
    fila[next]();
    next = (next + 1) % 2;
}