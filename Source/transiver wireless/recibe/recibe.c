#include <18F4550.h>
#fuses xt, nowdt, noprotect, put, CPUDIV1
// se define el uso del reloj interno a 8Mhz
#use delay(internal = 8000000)
// se define la comunicacion serial a 9600 baudios, sin paridad y un bit de stop
#use rs232(baud = 9600, bits = 8, parity = N, xmit = PIN_C6, RCV = PIN_C7, stop=1, ERRORS)

#use fast_io(a)
#use fast_io(b)
#use fast_io(d)

int8 display[10] = {192,249,164,176,153,146,130,248,128,144};
char dato_recibido = 'l';

#int_rda
void rb_isr() {
    // si algo llega por r2-32
    if (kbhit()) {
         dato_recibido = getc();
        if (dato_recibido > 107) {
            output_toggle(pin_a0);
        }
    }
}


void main(){
    set_tris_b(0xff); // inicia el puerto B somo entrada.
    set_tris_d(0x00); // inicia el puerto B somo entrada
    set_tris_a(0x00); // inicia el puerto A somo salida.
    
   /******CONFIGURACION COMUNICACION******/
   
   delay_ms(9000);
   printf("AT+CIPMUX=1\r\n");
   
   delay_ms(5000);
   printf("AT+CIPSERVER=1,222\r\n");
   
   delay_ms(6000);
   printf("AT+CIPSTART=2,\"TCP\",\"192.168.4.2\",222\r\n");
   
   delay_ms(1000);
       //indica que se termino la etapa de conexion
    output_high(pin_a0);
    /********* fin conexion *********/

    //habilita las interrupciones por serial
    enable_interrupts(int_rda);
    
    //habilita las interrupciones glovales
    enable_interrupts(global);
    
    
  while (true) {
        delay_ms(200);
        if (input(PIN_B4)) {
               printf("AT+CIPSEND=2,1\r\n");
               delay_ms(500);
               printf("j\r\n");
        }
        if (dato_recibido > 107) {
          dato_recibido -= 107;
          output_d(display[dato_recibido]);
        }
    }
}
