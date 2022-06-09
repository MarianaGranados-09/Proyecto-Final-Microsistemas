#Include <18F4550.h>
#Device PASS_STRINGS = IN_RAM
#Fuses INTRC, NOPROTECT, NOWDT, CPUDIV1, PLL4
#Use Delay(Clock=8M)

#Use I2C(MASTER, SDA = PIN_B0, SCL = PIN_B1, FAST = 400000, STREAM = SSD1306_STREAM)
#use i2c(Master,sda=PIN_B0,scl=PIN_B1, STREAM = MAG)
#use i2c(Master,sda=PIN_B0,scl=PIN_B1, slow, STREAM = GIRO)
#use RS232(RCV=PIN_C7, XMIT=PIN_C6, Baud=9600, Bits=8)


#include <MPU6050.c>   // Libreria para el manejo del modulo MPU6050
#include "math.h"  

///////////////////////////////////////////////////////////////////////////////////////////////Codigo para magnetometro
// HMC5883 required Registers

#define W_DAT   0x1A     //'Used to perform a Write operation
#define R_DAT   0x1B     //'Used to perform a Read operation
#define CON_A    0xA0
#define CON_B    0x0B     //'Send continuous Measurement mode.
#define MOD_R    0x09     //'Read/Write Register, Selects the operating mode. Default = Single meVARurement

#define X_MSB    0x01     //'Read Register, Output of X MSB 8-bit value.
#define X_LSB    0x00     //'Read Register, Output of X LSB 8-bit value.
#define Z_MSB    0X05     //'Read Register, Output of Z MSB 8-bit value.
#define Z_LSB    0X04     //'Read Register, Output of Z LSB 8-bit value.
#define Y_MSB    0X03     //'Read Register, Output of Y MSB 8-bit value.
#define Y_LSB    0X02     //'Read Register, Output of Y LSB 8-bit value.

void hmc5883_write(int add, int data)
{
         i2c_start();
         i2c_write(W_DAT);
         i2c_write(add);
         i2c_write(data);
         i2c_stop();
 
}
     
int16 hmc5883_read(int add){
         int retval;
         i2c_start();
         i2c_write(W_DAT);
         i2c_write(add);
         i2c_start();
         i2c_write(R_DAT);
         retval=i2c_read(0);
         i2c_stop();
         return retval;
}
 
void hmc5883_init(){
         
         hmc5883_write(CON_B,  0x01);
         delay_ms(100);
         hmc5883_write(MOD_R,  0b10011001);
         delay_ms(100);
         hmc5883_write(CON_A,  0b10000001);
         delay_ms(100);
}

/////////////////////////////////////////////////////////////////////////// Codigo para magnetometro

//SE DEBE CAMBIAR AL LA DIRECCIÓN DE MEMORIA DE LA OLED FISICA.
#Define SSD1306_I2C_ADDRESS 0x78    //Dirección de memoria de la OLED.
#Include <SSD1306_OLED.c>           //Libreria para el manejo de la OLED.
#Include <String.h>
Float Coordenada_Y;
Float Coordenada_X;
Float Coordenada_Z;

Char Brujula[1];

int8 M_data[6]; //6 units 8 bit Measured datas
int16 Xm=0,Ym=0,Zm=0; //16 bit X,Y,Z variables
//float Ax, Ay, Az;        // Variables para almacenar los datos del acelerometro y giroscopio

Void Mensaje();
Void Plano();
Void Graficos();
Void Limites();

Void Main()
{
   //Brujula[0]='N';
   delay_ms(10);
   hmc5883_init(); //Inicializar magnetometro
   
   MPU6050_init();                        // Inicializa MPU6050
   
   SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);   //Inicializamos la OLED.
   SSD1306_ClearDisplay();    //Limpiamos la OLED.
   SSD1306_Display();
   Mensaje();  //Mandamos un mensaje inicial.
   
   while(TRUE)
   {
      //printf("LISTO!\r\n");
      Brujula[0] = 0;
      Coordenada_X = 0;            
      Coordenada_Y = 0;              
      Coordenada_Z = 0;
      
      delay_us(50);
      
      Coordenada_X = MPU6050_get_Ax();              // Acelerometro eje x
      Coordenada_Y = MPU6050_get_Ay();              // Acelerometro eje y
      Coordenada_Z = MPU6050_get_Az();              // Acelerometro eje z
      
      
      M_data[0]=hmc5883_read(0x00); //Read X (LSB)
      M_data[1]=hmc5883_read(0x01); //Read X (MSB)
      M_data[2]=hmc5883_read(0x02); //Read Y (LSB)
      M_data[3]=hmc5883_read(0x03); //Read Y (MSB)
      M_data[4]=hmc5883_read(0x04); //Read Z (LSB)
      M_data[5]=hmc5883_read(0x05); //Read Z (MSB)

      Xm=make16(M_data[1],M_data[0]);
      Ym=make16(M_data[3],M_data[2]);
      Zm=make16(M_data[5],M_data[4]);
     // char comp;
      float Heading = (atan2((signed int16)Ym,(signed int16)Xm)) * (180 / pi);
     // float declination = 4.63;
      //float grad = Heading + declination;
      if(Heading < 0)
      {
         Heading = Heading + 360;
      }
      
      if((heading >=0 && heading<=60) || (heading>350 && heading<=360))
      {
         printf("Direccion Norte\r\n");
         Brujula[0]='N';
      } 
      if(heading > 60 && heading<=160)
      {
         printf("Direccion Este\r\n");
         Brujula[0]='E';
      }
      if(heading>160 && heading<=270)
      {
         printf("Direccion Sur\r\n");
         Brujula[0]='S';
      }
      //printf("\n h=%f   ",Heading);
      if(heading>270 && heading<=350)
      {
         printf("Direccion Oeste Titan Colosal!!\r\n");
         Brujula[0]='W';
      }
      printf( "Coord x:%0.2f\r\n", Coordenada_X );   // Imprime Ax
      printf( "Coord y:%0.2f\r\n", Coordenada_Y);   // Imprime Ay
      printf("Coord z:%0.2f\r\n", Coordenada_Z);   // Imprime Az
      delay_ms(100); 
      //Brujula[1] = 0;
      Plano();    //Dibujamos el plano.
      Limites();
      Graficos(); //Hacemos los gráficos.
      delay_ms(10);
      
      //SSD1306_ClearDisplay();    //Limpiamos la OLED.
      //SSD1306_Display();
   }
 
}

Void Mensaje()
{
   SSD1306_DrawText(10,1,"LAB. MICROSYSTEMS", 1);
   SSD1306_DrawText(45,22,"FLIGHT", 1);
   SSD1306_DrawText(34,34,"SIMULATOR.", 1);
   SSD1306_DrawText(0,50,"BY: MPM", 1);
   SSD1306_Display();
   Delay_ms(3000);
   SSD1306_ClearDisplay();
   SSD1306_Display();
}

Void Plano()
{
  SSD1306_DrawRect(0,0,127,63);     //Margen general.
   SSD1306_DrawRect(70,23,15,18);   //Recuadro.
   SSD1306_DrawCircle(31,31,30);    //Círculo (Izquierda).
   SSD1306_DrawCircle(95,31,30);    //Círculo (Derecha).
   SSD1306_DrawFastVLine(62,0,63);  //Separación de la pantalla.
   SSD1306_DrawText(5,36,"-X", 1);  //Eje -X.
   SSD1306_DrawText(45,21,"X+", 1); //Eje X.
   SSD1306_DrawText(17,8,"Y+", 1);  //Eje Y.
   SSD1306_DrawText(35,48,"-Y", 1); //Eje -Y.
   SSD1306_DrawText(80,8,"Z+", 1);  //Eje Z.
   SSD1306_DrawText(100,48,"-Z", 1);//Eje -Z.
   SSD1306_Display();
}

Void Graficos()
{
   SSD1306_DrawLine(31,31,31,31-Coordenada_Y,1); //Eje Y.
   SSD1306_DrawLine(31,31,Coordenada_X+31,31,1); //Eje X.
   SSD1306_DrawLine(95,31,95,31-Coordenada_Z,1); //Eje Z.
   SSD1306_DrawChar(72,25,Brujula[0], 2);        //Orientación.
   SSD1306_Display();
}

Void Limites()
{
   //Limites para el eje Y.
   If (Coordenada_Y>30)
   {
      Coordenada_Y = 30;
   }
   If (Coordenada_Y< -30)
   {
      Coordenada_Y = -30;
   }
   
   //Limites para el eje X.
   If (Coordenada_X>30)
   {
      Coordenada_X = 30;
   }
   If (Coordenada_X< -30)
   {
      Coordenada_X = -30;
   }
   
   //Limites para el eje Z.
   If (Coordenada_Z>30)
   {
      Coordenada_Z = 30;
   }
   If (Coordenada_Z< -30)
   {
      Coordenada_Z = -30;
   }
   
}
