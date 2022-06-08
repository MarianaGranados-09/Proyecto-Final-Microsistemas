#Include <18F4550.h>
#Fuses INTRC,NOPROTECT, NOWDT, CPUDIV1, PLL1
#Use Delay(Clock=8M)
#use i2c(Master,sda=PIN_B0,scl=PIN_B1)
#use RS232(RCV=PIN_C7, XMIT=PIN_C6, Baud=9600, Bits=8)

#include "math.h"   

// HMC5883 required Registers

#define W_DATA   0x1A     //'Used to perform a Write operation
#define R_DATA   0x1B     //'Used to perform a Read operation
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
         i2c_write(W_DATA);
         i2c_write(add);
         i2c_write(data);
         i2c_stop();
 
}
     
int16 hmc5883_read(int add){
         int retval;
         i2c_start();
         i2c_write(W_DATA);
         i2c_write(add);
         i2c_start();
         i2c_write(R_DATA);
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

////////////////////////////////////////////////////////////////////////////

int8 M_data[6]; //6 units 8 bit Measured datas
int16 Xm=0,Ym=0,Zm=0; //16 bit X,Y,Z variables 

              
void main()
{
   delay_ms(10);
   hmc5883_init();
   //printf("LISTO!\r\n");
    
   while(TRUE)
   {
      printf("LISTO!\r\n");
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
      float declination = 6.63;
      float grad = Heading + declination;
      if(grad < 0)
         grad = grad + 360;
      
      printf("X=%ld  ",Xm);

      printf("Y=%ld  ",Ym);

      printf("Z=%ld  ",Zm);

      printf("grados: %f   ",grad);

   
      //printf("\n h=%f   ",Heading);
      delay_ms(250); 
      
   }

}
