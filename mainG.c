#include <18F4550.h>
#fuses HS,NOWDT,NOPROTECT,NOPUT,NOLVP,NOBROWNOUT
#use delay(clock=8M)
#use i2c(Master,sda=PIN_B0,scl=PIN_B1, slow)
#use rs232(RCV=PIN_C7, XMIT=PIN_C6, Baud=9600, Bits=8)
//#use standard_io(D)

#include <MPU6050.c>                      // Libreria para el manejo del modulo MPU6050
float Ax, Ay, Az, Gx, Gy, Gz, t;          // Variables para almacenar los datos del acelerometro y giroscopio

void main()
{
   //lcd_init();                            // Inicializa la pantalla LCD
   MPU6050_init();                        // Inicializa MPU6050
   
   while(true)
   {
      Ax = MPU6050_get_Ax();              // Acelerometro eje x
      Ay = MPU6050_get_Ay();              // Acelerometro eje y
      Az = MPU6050_get_Az();              // Acelerometro eje z
      
      Gx = MPU6050_get_Gx();              // Giroscopio eje x
      Gy = MPU6050_get_Gy();              // Giroscopio eje y
      Gz = MPU6050_get_Gz();              // Giroscopio eje z
      
      
      //lcd_clear();
      //lcd_gotoxy(1,1);
      printf( "Ax:%0.2f\r\n", Ax);   // Imprime Ax
      //lcd_gotoxy(1,2);
      printf( "Ay:%0.2f\r\n", Ay);   // Imprime Ay
      //lcd_gotoxy(1,3);
      printf("Az:%0.2f\r\n", Az);   // Imprime Az
      //lcd_gotoxy(1,4);
      printf("T:%f\r\n", t);        // Imprime temperatura
      
      //lcd_gotoxy(11,1);
      printf("Gx:%0.2f\r\n", Gx);   // Imprime Gx
      //lcd_gotoxy(11,2);
      printf("Gy:%0.2f\r\n", Gy);   // Imprime Gy
      //lcd_gotoxy(11,3);
      printf("Gz:%0.2f\r\n", Gz);   // Imprime Gz
      delay_ms(200);
   }
}
