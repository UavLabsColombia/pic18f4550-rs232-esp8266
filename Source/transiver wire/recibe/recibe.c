#include <18F4550.h>
#fuses xt, nowdt, noprotect, put, CPUDIV1
#use delay(internal = 8000000)
#use rs232(baud = 9600, bits = 8, parity = N, xmit = PIN_C6, RCV = PIN_C7, stop=1, ERRORS)
// Indica el uso de los puertos 

#use fast_io(b)
#use fast_io(d)

//numeros del display
int8 display[10] = {192,249,164,176,153,146,130,248,128,144};

#int_rda
void rb_isr(){ 
    //si llega algo al puerto
   if(kbhit()){
      int dato_recibido = getc();
      dato_recibido -= 48;
      output_d(display[dato_recibido]);
   }
}

#int_rb
void int_rb_pulsadores(){
   if (input(pin_b4)){    
       putc('1');
       delay_ms(100);
   }
}

void main(){
    set_tris_b(0xFF); // Inicia el puerto B como Entrada. 
    set_tris_d(0x00); // indica el puerto D como Salida

    //habilita las interrupciones por serial
    enable_interrupts(int_rda);
    //habilita las interrupciones puerto b
    enable_interrupts(int_rb);
    //habilita las interrupciones glovales
    enable_interrupts(global);
    //intrerrupcion por flanco de subida
    ext_int_edge(L_TO_H);
}
