#include <18F4550.h>
#fuses xt, nowdt, noprotect, put, CPUDIV1
#use delay(internal = 8000000)
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
    // si algo llega por r2-32
    if (kbhit()) {
        char dato_recibido = getc();
        if (dato_recibido == '1') {
            output_toggle(pin_a0);
        }
    }
}

int leer_puerto() {
    int puertob = PORTB;
    puertob = puertob >> 4;
    return puertob += 48;
}

void main() {
    set_tris_b(0xff); // inicia el puerto B somo entrada. 
    set_tris_a(0x00); // inicia el puerto A somo salida.
    //habilita las interrupciones por serial
    enable_interrupts(int_rda);
    //habilita las interrupciones glovales
    enable_interrupts(global);
    while (true) {
        delay_ms(200);
        if (input(PIN_B4) || input(PIN_B5) || input(PIN_B6)) {
            putc(leer_puerto());
        }
    }
}
