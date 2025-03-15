// PIC24 SO embarcados

//definições do projeto
#define FCY 10000000UL
#define OSCIOFNC_ON          0xFFDF
#define POSCMOD_NONE         0xFFFF

#include <xc.h>
#include <libpic30.h>

typedef void (*f_ptr)(void);

void config_ports();
void task_1();
void task_2();
void press_button();

f_ptr fila[2];
int next = 0;

int main(){
    
    config_ports();
    
    //instalar tarefas
    fila[0] = task_1;
    fila[1] = task_2;
    
    while(1){
        press_button();
        
    }
    
    return 0;
}

void config_ports(){
    
    TRISCbits.TRISC14 = 0; // saída de dados
    TRISCbits.TRISC15 = 0; // saída de dados
    //OSCIOFCN = 0b111111111111111110111111;
    
}

void task_1(){
    
    LATCbits.LATC14 = ~PORTCbits.RC14;
    __delay_ms(50);
    
}

void task_2(){
    
    LATCbits.LATC15 = ~PORTCbits.RC15;
    __delay_ms(50);
    
}

void press_button(){
    
    if(PORTFbits.RF0 == 1){
        fila[next]();
        next = (next + 1) % 2;
    }
}