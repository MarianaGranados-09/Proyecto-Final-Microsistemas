#Include <18F4550.h>
#Device PASS_STRINGS = IN_RAM
#Fuses INTRC, NOPROTECT, NOWDT, CPUDIV1, PLL4
#Use I2C(MASTER, SDA = PIN_B0, SCL = PIN_B1, FAST = 400000, STREAM = SSD1306_STREAM)
#Use Delay(Clock=8M)

//SE DEBE CAMBIAR AL LA DIRECCIÓN DE MEMORIA DE LA OLED FISICA.
#Define SSD1306_I2C_ADDRESS 0x78    //Dirección de memoria de la OLED.
#Include <SSD1306_OLED.c>           //Libreria para el manejo de la OLED.
#Include <String.h>
Float Coordenada_Y = -10.2;
Float Coordenada_X = -18.5;
Float Coordenada_Z = 10.6;
Char Brujula[1]='N';

Void Mensaje();
Void Plano();
Void Graficos();
Void Limites();

Void Main()
{
   Brujula[0]='N';
   SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);   //Inicializamos la OLED.
   SSD1306_ClearDisplay();    //Limpiamos la OLED.
   SSD1306_Display();
   Mensaje();  //Mandamos un mensaje inicial.
   Plano();    //Dibujamos el plano.
   Limites();
   Graficos(); //Hacemos los gráficos. 
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
   SSD1306_DrawRect(0,0,127,63); //Ponemos un margen.
   SSD1306_DrawRect(68,26,15,10); //Ponemos un margen.
   SSD1306_DrawCircle(31,31,30); //Dibujamos un círculo.
   SSD1306_DrawCircle(95,31,30); //Dibujamos un círculo.
   SSD1306_DrawFastVLine(62,0,63); //Separación de la pantalla.
   SSD1306_DrawText(5,36,"-X", 1);
   SSD1306_DrawText(45,21,"X+", 1);
   SSD1306_DrawText(17,8,"Y+", 1);
   SSD1306_DrawText(35,48,"-Y", 1);
   SSD1306_DrawText(80,8,"Z+", 1);
   SSD1306_DrawText(100,48,"-Z", 1);
   SSD1306_Display();
}

Void Graficos()
{
   SSD1306_DrawLine(31,31,31,31-Coordenada_Y,1); //Eje Y.
   SSD1306_DrawLine(31,31,Coordenada_X+31,31,1); //Eje X.
   SSD1306_DrawLine(95,31,95,31-Coordenada_Z,1); //Eje Z.
   SSD1306_DrawText(70,27,Brujula, 1);
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
