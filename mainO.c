#Include <18F4550.h>
#Device PASS_STRINGS = IN_RAM
#Fuses HS, NOPROTECT, NOWDT, CPUDIV1, PLL1
#Use I2C(MASTER, SDA = PIN_B0, SCL = PIN_B1, FAST = 400000, STREAM = SSD1306_STREAM)
#Use Delay(Clock=8M)

//SE DEBE CAMBIAR AL LA DIRECCIÓN DE MEMORIA DE LA OLED FISICA.
#Define SSD1306_I2C_ADDRESS 0x78    //Dirección de memoria de la OLED.
#Include <SSD1306_OLED.c>           //Libreria para el manejo de la OLED.
Int16 Coordenada_Y = 20;
Int16 Coordenada_X = 20;
Int16 Coordenada_Z = 20;


Void Mensaje();
Void Limites();
Void Main()
{
   SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);   //Inicializamos la OLED.
   SSD1306_ClearDisplay();    //Limpiamos la OLED.
   SSD1306_Display();
   Mensaje();   //Mandamos un mensaje inicial.
   SSD1306_DrawRect(0,0,127,63); //Ponemos un margen.
   SSD1306_DrawCircle(63,31,30); //Dibujamos un círculo.
   SSD1306_Display();
   SSD1306_DrawLine(63,31,63,31-Coordenada_Y,1); //Eje Y.
   SSD1306_DrawLine(63,31,33+Coordenada_X,31,1); //Eje X
   SSD1306_Display();
   
   /*While(True);
   {
      SSD1306_DrawLine(63,31,63,10,1);
      SSD1306_DrawLine(63,31,10,31,1);
      SSD1306_Display();
      Delay_ms(1000);
      SSD1306_DrawLine(63,31,63,10,0);
      SSD1306_DrawLine(63,31,10,31,0);
      SSD1306_Display();
      Delay_ms(1000);
      
   }
   */
}

Void Mensaje()
{
   SSD1306_DrawText(18,1,"LAB. MICROSTEMAS", 1);
   SSD1306_DrawText(30,25,"SIMULADOR DE", 1);
   SSD1306_DrawText(45,40,"VUELO.", 1);
   SSD1306_DrawText(0,50,"By: MPM", 1);
   SSD1306_Display();
   Delay_ms(2000);
   SSD1306_ClearDisplay();
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
