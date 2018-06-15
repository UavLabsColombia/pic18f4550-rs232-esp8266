#include <18F4550.h>
#fuses xt, nowdt, noprotect, put, CPUDIV1
// se define el uso del reloj interno a 8Mhz
#use delay(internal = 8000000)
// se define la comunicacion serial a 9600 baudios, sin paridad y un bit de stop
#use rs232(baud = 9600, bits = 8, parity = N, xmit = PIN_C6, RCV = PIN_C7, stop=1, ERRORS)

/*
  Este programa recibe 3 pulsos en binario y envia su correspondiente en
  deciman por r2-32
*/

//definicion de los puertos
#use fast_io(a)
#use fast_io(b)

#byte PORTB = 0XF81

#int_rda
void rb_isr() {
    // si algo llega por rs232
    if (kbhit()) {
        char dato_recibido = getc();
        if (dato_recibido == 'j') {
            output_toggle(pin_a0);
        }
    }
}

int leer_puerto() {
    int puertob = PORTB;
    puertob = puertob >> 4;
    return puertob += 107;
}

void main() {
    set_tris_b(0xff); // inicia el puerto B somo entrada.
    set_tris_a(0x00); // inicia el puerto A somo salida.

    /********* conexion *********/
    delay_ms(9000);
    printf("AT+CIPMUX=1\r\n");

    delay_ms(3000);
    printf("AT+CIPSERVER=1,222\r\n");
    
    delay_ms(5000);
    printf("AT+CIPSTART=2,\"TCP\",\"192.168.4.1\",222\r\n");
    
    //indica que se termino la etapa de conexion
    output_high(pin_a0);
    
    /********* fin conexion *********/

    //habilita las interrupciones por serial
    enable_interrupts(int_rda);
    
    //habilita las interrupciones glovales
    enable_interrupts(global);

    while (true) {
        delay_ms(200);
        if (input(PIN_B4) || input(PIN_B5) || input(PIN_B6)) {
            int caracter = leer_puerto();
            if(caracter != 107){
               printf("AT+CIPSEND=2,1\r\n");
               delay_ms(500);
               printf("%c\r\n",caracter);
            }
        }
    }
}
