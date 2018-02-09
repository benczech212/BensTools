//Screen Stuff
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include <BensTools_ScreenTools.h>
#include <BensTools_PrintTools.h>
#include <BensTools_Convert.h>

// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


//NeoPixel Stuff

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define LED_1_PIN 5
#define LED_2_PIN 6

#define LED_1_COUNT 64
#define LED_2_COUNT 8

#define LED_1_BRIGHTNESS 32
#define LED_2_BRIGHTNESS 32

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_1_COUNT, LED_1_PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel stick = Adafruit_NeoPixel(LED_2_COUNT, LED_2_PIN, NEO_GRBW + NEO_KHZ800);



byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


void setup() {
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif

  strip.setBrightness(LED_1_BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  stick.setBrightness(LED_2_BRIGHTNESS);
  stick.begin();
  stick.show(); // Initialize all pixels to 'off'
    randomSeed(analogRead(0));

  //Screen Stuff START //
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(20);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  //Screen Stuff END //
}



//  | \\\\////\\\\////\\\\////\\\\////
//  | ////\\\\////\\\\////\\\\////\\\\
//  | \\\\////\\\\////|      START       |
//  | ////\\\\////\\\\| Global Variables |
//  | \\\\////\\\\////\\\\////\\\\////
//  | ////\\\\////\\\\////\\\\////\\\\


//Constants
uint8_t C_Pixel_Start = 0;
uint8_t C_Pixel_Total = 64;

uint8_t C_PixelsPerY = 8;
uint8_t C_PixelsPerX= 8;

bool  C_PrintStats_Master = false;
bool  C_PrintStats_Debug = false;
//bool  C_PrintStats_Screen = true;
bool  C_DebugMonitor_Serial_Enabled = false;
bool  C_DebugMonitor_Screen_Enabled = false;
bool  C_DebugMonitor_Stick_Enabled = false;
bool  C_Effect_Timer_Enabled = true;

uint32_t      C_Delay_Pixel = 50;

uint16_t C_Screen_RowHeight = 10;
uint16_t C_Screen_ColWidth = 128;




//Variables
//G_Index
uint16_t G_Index = 0;
uint16_t C_Index_Min = 0;
uint16_t C_Index_Max  = 1024;


//Pixel
uint16_t G_Pixel_UpdateEveryX;
uint16_t G_Pixel_Step_Row, G_Pixel_Step_Col;
uint16_t G_Pixel_Offset_Min = 1;
uint16_t G_Pixel_Max = C_Pixel_Start + C_Pixel_Total;
        
uint16_t G_Pixel_Offset_Max = G_Pixel_Max + 1;
uint16_t G_Pixel_Offset = G_Pixel_Offset_Min;
uint16_t G_Pixel_Actual, G_Pixel_Actual_Col, G_Pixel_Actual_Row;

//Effect
double   G_Effect_LengthModifier;
char     G_EffectRunning_Name[16];
uint16_t G_Effect_Start_Offset;
uint16_t G_UpdateEveryX;
uint8_t G_Effect_Symmetry[4];


//ColorBrushes
uint32_t      G_Color32_Input,        G_Color32_Output,       G_Color32_Target,     G_Color32_Mask,       G_Color32_NewRandom;
uint8_t       G_Color8_Input[4],      G_Color8_Output[4],     G_Color8_Target[4],   G_Color8_Mask[4],     G_Color8_NewRandom[4];
uint8_t       G_ColorChannel_Enabled[4];
uint32_t      Color32_Test[8];
uint32_t      G_Color32_Actual;
uint8_t       G_Color8_PrintBuffer[4];

//EffectTimer
uint32_t G_EffectTimer_Master, G_EffectTimer_Effect, G_EffectTimer_Current, G_EffectTimer_Last;




//Delay
uint32_t G_Delay_Pixel;
uint32_t G_Delay_Pixel_Actual;

//Misc
char    PrintBuffer[256];
char    G_Text_Buffer32[32];
int G_k;  //Increments every time offset goes over max

//End Global

void loop() {
  Print_Screen_Disabled();
   for(int TestingLoop1 = 0; TestingLoop1 < 1024; TestingLoop1++){


                uint16_t Offset_Test1 = LimitValue(TestingLoop1, 0 , 64, false);
                uint16_t Offset_Test2 = LimitValue(TestingLoop1, 0 , 24, false);
                G_ColorChannel_Enabled[4] = Make_Random_ColorChannels4();
                uint16_t Pixel_Step_Setting = random(16)- 8;
                uint16_t Row_Step_Setting = random(16)- 8;
                
                //Pixel_Step_Setting -= G_Index;//((random(4) + 2) * 2) - 1;
                //Row_Step_Setting +=  G_Index; //((random(4) + 2) * 2) - 1;

                //if(random(4) == 0){Pixel_Step_Setting = 0 - Pixel_Step_Setting;}
                //if(random(4) == 0){Row_Step_Setting = 0 - Row_Step_Setting;}
                
                Pixel_Step_Setting = Pixel_Step_Setting % 64;
                Row_Step_Setting = Pixel_Step_Setting % 64;

                
                if(Pixel_Step_Setting + 1 % 8 == 0)                                       {Pixel_Step_Setting+= G_Index;}
                if(Row_Step_Setting + 1 % 8 == 0)                                         {Row_Step_Setting-= G_Index;}
                if(Pixel_Step_Setting == 0)                                               {Pixel_Step_Setting+= random(4) + 3;}
                if(Row_Step_Setting == 0)                                                 {Row_Step_Setting-= random(4) + 3;}         
                if(Pixel_Step_Setting == Row_Step_Setting)                                {Pixel_Step_Setting+= random(4) + 3; }
                Make_Random_Color32_WChannels( 256, G_ColorChannel_Enabled);
                
                Color32_Test[0] = Make_Random_Color32_Basic(true);
           
                Color32_Test[4] = Make_Random_Color32_Limit(255, 255, 255, 255);
                Color32_Test[3] = Make_Random_Color32_Limit(128, 128, 128, 128);
                Color32_Test[2] = Make_Random_Color32_Limit(64, 64, 64, 64);
                Color32_Test[1] = Make_Random_Color32_Limit(32, 32, 32, 32);

                
                uint32_t Color32_Test_Red = strip.Color(255,0,0,0);
                uint32_t Color32_Test_Green = strip.Color(0,255,0,0);
                uint32_t Color32_Test_Blue = strip.Color(0,0,255,0);
                uint32_t Color32_Test_White = strip.Color(0,0,0,255);
                //Color32_Test[3] = 
                
                G_Effect_Symmetry[0] = random(3);
                G_Effect_Symmetry[1] = random(3);
                G_Effect_Symmetry[2] = random(3);
                G_Effect_Symmetry[3] = random(3);

                int Selector = random(4);
                G_Effect_Symmetry[Selector] =+ 1;
                Selector = (Selector + 2) % 4;
                G_Effect_Symmetry[Selector] =+ 1;




//Pixel Test//==========================================================================================
/*
                int Pixel_Test = 0;
                int Pixel_Test_Max = 16;
                int TransformType = 3;
                
                      for(Pixel_Test; Pixel_Test < Pixel_Test_Max; Pixel_Test++){
                          //Print_Padded_ValueLabeled("G_Pixel_Actual: ", G_Pixel_Actual, 2, true);
                          
                          G_Pixel_Actual = Pixel_Test;
                          Test_DrawDot(G_Pixel_Actual, Color32_Test_Red,true);
                          Update_G_Pixel_Actual_Transform(G_Pixel_Actual, TransformType, 0);
                          Serial.println();
                          
                          G_Pixel_Actual = Pixel_Test;
                          Update_G_Pixel_Actual_Transform(G_Pixel_Actual, TransformType, 1);
                          Serial.println();
                          Test_DrawDot(G_Pixel_Actual, Color32_Test_Green,true);
          
                          G_Pixel_Actual = Pixel_Test;
                          Update_G_Pixel_Actual_Transform(G_Pixel_Actual, TransformType, 2);
                          Serial.println();
                          Test_DrawDot(G_Pixel_Actual, Color32_Test_Blue,true);
          
                          G_Pixel_Actual = Pixel_Test;
                          Update_G_Pixel_Actual_Transform(G_Pixel_Actual, TransformType, 3);
                          Serial.println();
                          Test_DrawDot(G_Pixel_Actual, Color32_Test_White,true);
          
                          //Serial.println();
                          delay(1000);
                          strip.clear();
                        }
                  */      
//Pixel Test//==========================================================================================

//ColorWipeX Test          //==========================================================================================

    
                

                Convert_uint32_TO_uint8(Color32_Test[1]);
                uint16_t Step = 1;

           
               
                      //ColorStep(G_Color32_Output, 3);
                      colorWipeX(
                        Color32_Test[1],       //Color, 
                        C_Delay_Pixel,               //Delay 
                        Pixel_Step_Setting,                   //Pixel_Step
                        Row_Step_Setting,                  //Row_Step
                        0,                 //Effect_Starting_Offset
                        0.875,//* random(8) + 1,                //Effect_LengthModifier
                        G_Effect_Symmetry,       //Symmetry
                        1                  //UpdateEveryX
                        ); 
                        strip.show();
                      
                      Step += 1;
                      if((G_Index % (Step^2)) == 0){
                        for(int n = 0; n < (Step^2); n++){
                          colorWipeX(
                            Color32_Test[Step],       //Color, 
                            C_Delay_Pixel/(Step^2),               //Delay 
                            Pixel_Step_Setting,                   //Pixel_Step
                            Row_Step_Setting,                  //Row_Step
                            0,                 //Effect_Starting_Offset
                            0.875/(Step^2),                //Effect_LengthModifier
                            G_Effect_Symmetry,       //Symmetry
                            1                  //UpdateEveryX
                            ); 
                            strip.show();
                        }
                      }
                      
                      Step += 1;
                      if((G_Index % (Step^2)) == 0){
                        for(int n = 0; n < (Step^2); n++){
                        colorWipeX(
                          Color32_Test[Step],       //Color, 
                          C_Delay_Pixel/(Step^2),               //Delay 
                          Pixel_Step_Setting,                   //Pixel_Step
                          Row_Step_Setting,                  //Row_Step
                          0,                 //Effect_Starting_Offset
                          0.875/(Step^2),                //Effect_LengthModifier
                          G_Effect_Symmetry,       //Symmetry
                          1                  //UpdateEveryX
                          ); 
                          strip.show();
                        }
                      }
                      
                      Step += 1;
                      if((G_Index % (Step^2)) == 0){
                        for(int n = 0; n < (Step^2); n++){
                        colorWipeX(
                          Color32_Test[Step],       //Color, 
                          C_Delay_Pixel/(Step^2),               //Delay 
                          Pixel_Step_Setting,                   //Pixel_Step
                          Row_Step_Setting,                  //Row_Step
                          0,                 //Effect_Starting_Offset
                          0.875/(Step^2),                //Effect_LengthModifier
                          G_Effect_Symmetry,       //Symmetry
                          1                  //UpdateEveryX
                          ); 
                          strip.show();
                        }
                      }

//ColorWipeX Test          //==========================================================================================
   }

}





//=====-----=====-----     Updates
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Update_G_Index(){
  if(     G_Index > C_Index_Max ){  G_Index = C_Index_Min;  } 
  else                      {   G_Index++;         }
  if(C_PrintStats_Master == true){Print_DebugMonitor_MainLoop();}
  if(C_Effect_Timer_Enabled == true){Update_Effect_Timer();}
}



void Update_Effect_Timer(){
  G_EffectTimer_Last = G_EffectTimer_Current;
  G_EffectTimer_Current = millis();
  G_EffectTimer_Master += G_EffectTimer_Current;
  //G_EffectTimer_Effect = ;
}

void Update_SetPixelColor_Of_G_Pixel_Actual(bool ShowUpdate){
 strip.setPixelColor(G_Pixel_Actual, G_Color32_Actual);
 if(ShowUpdate == true){if(G_Pixel_Offset % G_UpdateEveryX == 0){strip.show();}}
}



void Update_G_Pixel_Actual_From_XY(uint16_t L_InputX, uint16_t L_InputY){
  G_Pixel_Actual_Col = L_InputX;
  G_Pixel_Actual_Row = L_InputY;
  G_Pixel_Actual = L_InputX + (L_InputY * C_PixelsPerY);
}




void Update_G_Pixel_Actual_StepForward(){
    G_Pixel_Actual = ( (G_Pixel_Offset * G_Pixel_Step_Col) + (G_Pixel_Offset * G_Pixel_Step_Row) +  G_Effect_Start_Offset);
    if(G_Pixel_Actual > G_Pixel_Offset_Max){G_k+=1;}
    G_Pixel_Actual = LimitValue(G_Pixel_Actual, G_Pixel_Offset_Min, G_Pixel_Offset_Max, false);
    G_Pixel_Actual_Row = G_Pixel_Actual % C_PixelsPerY;
    G_Pixel_Actual_Col = G_Pixel_Actual / C_PixelsPerX;
}





void Update_G_Color32_Actual(uint32_t InputColor32){
  G_Color32_Actual = InputColor32;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Get calculations
void Update_G_Pixel_Actual_Transform(uint16_t L_Pixel_Input, uint16_t Transform_Type, byte Transform_Step){
  Transform_Step = Transform_Step % 4;
  uint16_t    L_Calc[2], L_Pixel_Output;
  uint16_t    L_Act[2];
  
  uint16_t Transform_Count_Total = 3;
  Transform_Step % Transform_Count_Total;
  L_Act[0] = Get_Pixel_X_From_Pixel_Actual(L_Pixel_Input);
  L_Act[1] = Get_Pixel_Y_From_Pixel_Actual(L_Pixel_Input);
  uint16_t Transform_Count = 0;
  //Col = X
  //Row = Y
  
  //Rotation+90 //==============================================
  if(Transform_Type == 0){
    if(Transform_Step == 0){      L_Calc[0] = ( 0                 +   L_Act[0] );   L_Calc[1] = ( 0                 +    L_Act[1] );      G_Pixel_Actual = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);    }     
    if(Transform_Step == 1){      L_Calc[0] = ( (C_PixelsPerX-1)  -   L_Act[1] );   L_Calc[1] = ( 0                 +    L_Act[0] );      G_Pixel_Actual = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);    }
    if(Transform_Step == 2){      L_Calc[0] = ( (C_PixelsPerX-1)  -   L_Act[0] );   L_Calc[1] = ( (C_PixelsPerY-1)  -    L_Act[1] );      G_Pixel_Actual = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);    }
    if(Transform_Step == 3){      L_Calc[0] = ( 0                 +   L_Act[1] );   L_Calc[1] = ( (C_PixelsPerY-1)  -    L_Act[0] );      G_Pixel_Actual = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);    }
  }

  
  //MirrorX_MirrorY //==============================================
  if(Transform_Type == 1){
    if(Transform_Step == 0){      L_Calc[0] = ( 0                 +    L_Act[0] );   L_Calc[1] = ( 0                 +   L_Act[1] );      G_Pixel_Actual = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);    }     
    if(Transform_Step == 1){      L_Calc[0] = ( (C_PixelsPerX-1)  -    L_Act[0] );   L_Calc[1] = ( 0                 +   L_Act[1] );      G_Pixel_Actual = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);    }
    if(Transform_Step == 2){      L_Calc[0] = ( (C_PixelsPerX-1)  -    L_Act[0] );   L_Calc[1] = ( (C_PixelsPerY-1)  -   L_Act[1] );      G_Pixel_Actual = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);    }
    if(Transform_Step == 3){      L_Calc[0] = ( 0                 +    L_Act[0] );   L_Calc[1] = ( (C_PixelsPerY-1)  -   L_Act[1] );      G_Pixel_Actual = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);    }
  }
  

    //MirrorX_MirrorY_FlipY ?? //==============================================
  if(Transform_Type == 2){
    if(Transform_Step == 0){      L_Calc[0] = ( 0                 +    L_Act[0] );   L_Calc[1] = ( 0                 +   L_Act[1] );      G_Pixel_Actual = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);    }     
    if(Transform_Step == 1){      L_Calc[0] = ( (C_PixelsPerX-1)  -    L_Act[0] );   L_Calc[1] = ( 0                 +   L_Act[1] );      G_Pixel_Actual = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);    }
    if(Transform_Step == 2){      L_Calc[0] = ( (C_PixelsPerX-1)  -    L_Act[1] );   L_Calc[1] = ( (C_PixelsPerY-1)  -   L_Act[0] );      G_Pixel_Actual = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);    }
    if(Transform_Step == 3){      L_Calc[0] = ( 0                 +    L_Act[1] );   L_Calc[1] = ( (C_PixelsPerY-1)  -   L_Act[0] );      G_Pixel_Actual = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);    }
  }


    //Tile //==============================================
  if(Transform_Type == 3){
    if(Transform_Step == 0){      L_Calc[0] = ( 0                 +    L_Act[0] ) % C_PixelsPerX;   L_Calc[1] = ( 0                 +   L_Act[1] ) % C_PixelsPerY;      G_Pixel_Actual = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);    }     
    if(Transform_Step == 1){      L_Calc[0] = ( (C_PixelsPerX/2)  +    L_Act[0] ) % C_PixelsPerX;   L_Calc[1] = ( 0                 +   L_Act[1] ) % C_PixelsPerY;      G_Pixel_Actual = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);    }
    if(Transform_Step == 2){      L_Calc[0] = ( (C_PixelsPerX/2)  +    L_Act[0] ) % C_PixelsPerX;   L_Calc[1] = ( (C_PixelsPerX/2)  +   L_Act[1] ) % C_PixelsPerY;      G_Pixel_Actual = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);    }
    if(Transform_Step == 3){      L_Calc[0] = ( 0                 +    L_Act[0] ) % C_PixelsPerX;   L_Calc[1] = ( (C_PixelsPerX/2)  +   L_Act[1] ) % C_PixelsPerY;      G_Pixel_Actual = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);    }
  }
  
}

uint16_t Get_Pixel_X_From_Pixel_Actual(uint16_t L_Pixel_Actual){
  return (L_Pixel_Actual % C_PixelsPerX);
}

uint16_t Get_Pixel_Y_From_Pixel_Actual(uint16_t L_Pixel_Actual){
  return (L_Pixel_Actual / C_PixelsPerX);
}

uint16_t Get_Pixel_Actual_From_XY(uint16_t L_InputX, uint16_t L_InputY){
  return (L_InputX + (L_InputY * C_PixelsPerY));
}










//=====-----=====-----     Effects
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void colorWipeX(uint32_t L_Input_ColorValue32, uint32_t L_Delay_Pixel, uint16_t L_Pixel_Step_Col, uint16_t L_Pixel_Step_Row, uint16_t L_Effect_Start_Offset, double L_LengthModifier, uint8_t Symmetry[4], uint16_t L_UpdateEveryX) {  
 
  for(
    G_Pixel_Offset = G_Pixel_Offset_Min; 
    G_Pixel_Offset < G_Pixel_Offset_Max * L_LengthModifier;
    G_Pixel_Offset+=1
    ) {
      
        Buffer_EffectRunning("ColorWipeX");
        
        G_Color32_Target        =   L_Input_ColorValue32;
        G_Delay_Pixel           =   L_Delay_Pixel;
        G_Pixel_Step_Col        =   L_Pixel_Step_Col % C_PixelsPerX;          
        G_Pixel_Step_Row        =   L_Pixel_Step_Row % C_PixelsPerY;
        G_Effect_Start_Offset   =   L_Effect_Start_Offset;
        G_Effect_LengthModifier =   L_LengthModifier;
        G_UpdateEveryX          =   L_UpdateEveryX;
        G_Delay_Pixel           =   L_Delay_Pixel;
      
      
      Update_G_Index();
      Update_G_Pixel_Actual_StepForward();
      int OffsetTestStep, OffsetTestType;
      OffsetTestStep = 0;//(G_Index>>1) % 4;
      OffsetTestType = (G_Index>>7) % 4;
      
      int ActTest = G_Pixel_Actual;

       G_Pixel_Actual = ActTest;
      Update_G_Pixel_Actual_Transform(G_Pixel_Actual, OffsetTestType, 0);
      Update_G_Color32_Actual(BlendAtPixel(L_Input_ColorValue32, G_Pixel_Actual));
      Update_SetPixelColor_Of_G_Pixel_Actual(true);  

      G_Pixel_Actual = ActTest;
      Update_G_Pixel_Actual_Transform(G_Pixel_Actual, OffsetTestType, 1);
      Update_G_Color32_Actual(BlendAtPixel(L_Input_ColorValue32, G_Pixel_Actual));
      Update_SetPixelColor_Of_G_Pixel_Actual(true);  
    
      G_Pixel_Actual = ActTest;
      Update_G_Pixel_Actual_Transform(G_Pixel_Actual, OffsetTestType, 2);
      Update_G_Color32_Actual(BlendAtPixel(L_Input_ColorValue32, G_Pixel_Actual));
      Update_SetPixelColor_Of_G_Pixel_Actual(true);  

      G_Pixel_Actual = ActTest;
      Update_G_Pixel_Actual_Transform(G_Pixel_Actual, OffsetTestType, 3);
      Update_G_Color32_Actual(BlendAtPixel(L_Input_ColorValue32, G_Pixel_Actual));
      Update_SetPixelColor_Of_G_Pixel_Actual(true);  

      
      /*
          Update_G_Pixel_Actual_Transform(G_Pixel_Actual, 0, random(2));
          Update_SetPixelColor_Of_G_Pixel_Actual(true);
          Update_G_Pixel_Actual_Transform(G_Pixel_Actual, 0, 2);
          Update_SetPixelColor_Of_G_Pixel_Actual(true);    
          if(random(3) != 0){
              Update_G_Pixel_Actual_Transform(G_Pixel_Actual, 0, 3);
              Update_SetPixelColor_Of_G_Pixel_Actual(true);  
          }
          if(random(3) == 0){
              Update_G_Pixel_Actual_Transform(G_Pixel_Actual, 0, 2);
              Update_SetPixelColor_Of_G_Pixel_Actual(true);  
          }
            */
      /*
      Update_G_Pixel_Actual_Transform(G_Pixel_Actual, 0, 0);
      Update_SetPixelColor_Of_G_Pixel_Actual(true);
      Update_G_Pixel_Actual_Transform(G_Pixel_Actual, 0, 1);
      Update_SetPixelColor_Of_G_Pixel_Actual(true);
      Update_G_Pixel_Actual_Transform(G_Pixel_Actual, 0, 2);
      Update_SetPixelColor_Of_G_Pixel_Actual(true);
      Update_G_Pixel_Actual_Transform(G_Pixel_Actual, 0, 3);
      Update_SetPixelColor_Of_G_Pixel_Actual(true);
      */

      
      G_Delay_Pixel_Actual = map(G_Index, C_Index_Min, C_Index_Max , 0, C_Delay_Pixel);
      
      delayMicroseconds(G_Delay_Pixel_Actual);
  }
  strip.show();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FadeAllPixels(){
  for(G_Pixel_Offset = G_Pixel_Offset_Min; G_Pixel_Offset < G_Pixel_Offset_Max; G_Pixel_Offset++){
    Convert_uint32_TO_uint8(strip.getPixelColor(G_Pixel_Offset));
    for(int i = 0; i < 3; i++){
      G_Color8_Output[i] -= ((G_Index % 4) + 1) * 2;
      G_Color8_Output[i] = LimitValue(G_Color8_Output[i], 0, 255, false);
    }

    G_Color32_Output = strip.Color(G_Color8_Output[0], G_Color8_Output[1], G_Color8_Output[2], G_Color8_Output[3]);
    strip.setPixelColor(G_Pixel_Offset, G_Color32_Output);
    
  }
  strip.show();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t BlendAtPixel(uint32_t InputColor32, uint16_t Pixel){
  uint32_t  Color1_32,
            Color2_32, 
            ColorAvg_32;
            
  uint8_t   Color1_8[4],
            Color2_8[4],
            ColorAvg_8[4];
  byte i;
          
    Convert_uint32_TO_uint8(strip.getPixelColor(Pixel));
    for(i = 0; i < 4; i++){    Color1_8[i] = G_Color8_Output[i];   }
    
    Convert_uint32_TO_uint8(InputColor32);
    for(i = 0; i < 4; i++){    Color2_8[i] = G_Color8_Output[i];   }

   
      
    for(i = 0; i < 4; i++){    ColorAvg_8[i] = (Color1_8[i] + Color2_8[i])/2;   }
     
      
    ColorAvg_32 = strip.Color(ColorAvg_8[0], ColorAvg_8[1], ColorAvg_8[2], ColorAvg_8[3]/2);
    int MinimumThreshold = 8;
    if(
      (abs(Color1_8[0] - Color2_8[0]) < MinimumThreshold) &
      (abs(Color1_8[1] - Color2_8[1]) < MinimumThreshold) &
      (abs(Color1_8[2] - Color2_8[2]) < MinimumThreshold) &
      (abs(Color1_8[3] - Color2_8[3]) < MinimumThreshold)
      ){
        ColorAvg_32 = ColorAvg_32 << 5;
        ColorAvg_32 = ColorAvg_32 << 9;
        ColorAvg_32 = ColorAvg_32 << 13;
        ColorAvg_32 = ColorAvg_32 << 17;
      }
      
    return ColorAvg_32;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawShape_Test(){
  int DrawLength = 64;
  uint16_t ShapeTest1[DrawLength];
  //{0, 1, 2, 3, 4, 5, 6, 7, 15, 23, 31, 39, 47, 55, 63};
  G_Index+=1;
  int StartSetting = G_Index;
  int Step_Current = StartSetting % 4;
  int StartPreset[4] = {0, 7, 63, 56};
  int Step[4] = {1, 8, -1, -8};
  
  int i = 0;
  int l = 8;
  int w = 8; 
  int Pixel, G_Pixel_Actual, Offset;
  
  int Direction = -1;
  
//  Color8_R[4] = strip.Color(255, 0, 0, 0);
  uint32_t ColorActual[4] = {
    strip.Color(255, 0, 0, 0),
    strip.Color(0, 255, 0, 0),
    strip.Color(0, 0, 255, 0),
    strip.Color(0, 0, 0, 255),
  };
  
  uint32_t ColorActual2[4] = {
    strip.Color(random(255), random(255), random(255), random(255)),
    strip.Color(random(255), random(255), random(255), random(255)),
    strip.Color(random(255), random(255), random(255), random(255)),
    strip.Color(random(255), random(255), random(255), random(255))
  };
  
  
  Offset = StartPreset[Step_Current];
  
  
  //First Sweep (Step_R)
  for(int j = 0; j < 16; j++){
      for(i = 0; i < l; i ++){    Pixel = (i * Step[Step_Current % 4]) + Offset;    BlendAtPixel(Pixel, ColorActual2[Step_Current % 4]);    strip.show();  }
      l -= 1;
      Offset = Pixel;
      Step_Current += Direction + random(3);
      
  }

  /*
  //Second Sweep (Step_D)
  
  for(i = 0; i < l; i ++){    Pixel = (i * Step[Step_Current % 4]) + Offset;    strip.setPixelColor(Pixel, Color32_Test[0]);    strip.show();  }
  
//Third Sweep (Step_L)
  l -=1;
  Offset = Pixel;
  Step_Current += 1;
  for(i = 0; i < l; i ++){    Pixel = (i * Step[Step_Current % 4]) + Offset;    strip.setPixelColor(Pixel, Color32_Test[0]);    strip.show();  }
*/

delay(10);
  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Test_DrawDot_XY(uint16_t L_InputX, uint16_t L_InputY, uint32_t L_InputColor32, bool L_ShowUpdate){
  Update_G_Pixel_Actual_From_XY(L_InputX, L_InputY);
  Update_G_Color32_Actual(L_InputColor32);
  Update_SetPixelColor_Of_G_Pixel_Actual(L_ShowUpdate);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Test_DrawDot(uint16_t L_Pixel_Actual, uint32_t L_InputColor32, bool L_ShowUpdate){
  G_Pixel_Actual = L_Pixel_Actual;
  Update_G_Color32_Actual(L_InputColor32);
  Update_SetPixelColor_Of_G_Pixel_Actual(L_ShowUpdate);
}

      /*
      //Smooth the color a bit
      Color32_Test[0] << ((G_Pixel_Offset << 0) % 25);
      Color32_Test[0] << ((G_Pixel_Offset << 0) % 17);
      Color32_Test[0] << ((G_Pixel_Offset << 0) % 9);
      Color32_Test[0] << ((G_Pixel_Offset << 0) % 5);
      
*/











int isSierpinskiCarpetPixelFilled(int x, int y)
{
    while (x > 0 || y > 0) // when either of these reaches zero the pixel is determined to be on the edge 
                               // at that square level and must be filled
    {
        if (x % 3 == 1 && y % 3 == 1) //checks if the pixel is in the center for the current square level
            return 0;
        x /= 3; //x and y are decremented to check the next larger square level
        y /= 3;
    }
    return 1; // if all possible square levels are checked and the pixel is not determined 
                   // to be open it must be filled
}









void ColorStep(uint32_t Color32_Input, byte Step){
  Convert_uint32_TO_uint8(Color32_Input);
  for(int i = 0; i < 4; i++){
    G_Color8_Output[i] = G_Color8_Output[(1 + Step) % 4];
  }
  G_Color32_Output = strip.Color(G_Color8_Output[0], G_Color8_Output[1], G_Color8_Output[2], G_Color8_Output[3]);
}








uint8_t Convert_uint32_TO_r(uint32_t Input){        G_Color8_Output[0] = Input >> 16;        return G_Color8_Output[0]; }
uint8_t Convert_uint32_TO_g(uint32_t Input){        G_Color8_Output[1] = Input >> 8;         return G_Color8_Output[1]; }
uint8_t Convert_uint32_TO_b(uint32_t Input){        G_Color8_Output[2] = Input >> 0;         return G_Color8_Output[2]; }
uint8_t Convert_uint32_TO_w(uint32_t Input){        G_Color8_Output[3] = Input >> 24;        return G_Color8_Output[3]; }




void Convert_uint32_TO_uint8(uint32_t Input){
  G_Color8_Output[0] = Input >> 16;
  G_Color8_Output[1] = Input >> 8;
  G_Color8_Output[2] = Input >> 0;
  G_Color8_Output[3] = Input >> 24;  
}





//=====-----=====-----     Tools


int Make_Random_ColorChannels4(){
  byte Chances = 3;
  G_ColorChannel_Enabled[0] = random(Chances);
  G_ColorChannel_Enabled[1] = random(Chances);
  G_ColorChannel_Enabled[2] = random(Chances);
  G_ColorChannel_Enabled[3] = random(Chances);
  
  //if(C_PrintStats_Debug == true){    for(int n = 0; n < 4; n++){        Serial.print(G_ColorChannel_Enabled[n]); }  }
  //return G_ColorChannel_Enabled[4];
}




void Make_Random_Color32_WChannels(uint8_t Input, uint8_t L_ColorChannel_Enabled[4]){
  uint8_t Multiplier = 256 / Input;
  
    G_Color8_NewRandom[0] =  ((random(Input) * Multiplier) - 1) * L_ColorChannel_Enabled[0];
    G_Color8_NewRandom[1] =  ((random(Input) * Multiplier) - 1) * L_ColorChannel_Enabled[1]; 
    G_Color8_NewRandom[2] =  ((random(Input) * Multiplier) - 1) * L_ColorChannel_Enabled[2]; 
    G_Color8_NewRandom[3] = (((random(Input) * Multiplier) - 1) * L_ColorChannel_Enabled[3]) / 4;
    ;
    G_Color32_NewRandom = strip.Color(G_Color8_NewRandom[0], G_Color8_NewRandom[1], G_Color8_NewRandom[2], G_Color8_NewRandom[3]);
    
}





uint32_t Make_Random_Color32_Basic(bool IncludeWhite){
uint8_t WhiteValue;
if(IncludeWhite == true) {WhiteValue = random(255);}  else{WhiteValue = 0;}
  return strip.Color(    random(255),     random(255),     random(255),     WhiteValue);
}


uint32_t Make_Random_Color32_Limit(uint8_t L_LimitR, uint8_t L_LimitG, uint8_t L_LimitB, uint8_t L_LimitW){
  return strip.Color(    random(L_LimitR),     random(L_LimitG),     random(L_LimitB),     random(L_LimitW));
}







int nDigits(int Input) {
  return floor(log10(abs(Input))) + 1;
}





int LimitValue(int Value_Input, int Value_Min, int Value_Max, bool LimitValue_Type_ZigZag){
  int Value_Offset_Min;
  int Value_Offset_Max = Value_Max - Value_Min;
  int Value_Offset_Input = Value_Input - Value_Min;

  
  //Zig-Zag Type Limit - Has some issues
 /*
   if(LimitValue_Type_ZigZag == true){
    
      if(Value_Input > Value_Max){
        if(Value_Input > Value_Range * 2){
          while(Value_Input > Value_Range * 2){
            Value_Input -= (Value_Range * 2);}
          }
          Value_Input = Value_Range - abs(Value_Input -  Value_Max) ;
        }
      if(Value_Input < Value_Min){       Value_Input = Value_Min   + abs(Value_Input +  Value_Min)   ;     }
    }
  */
    
  //Loop Type Limit
  //else{
    
    if(Value_Offset_Input > Value_Offset_Max){      Value_Offset_Input = Value_Offset_Input % (Value_Offset_Max + 1);    }    
    if(Value_Offset_Input < Value_Offset_Min){      Value_Offset_Input = Value_Offset_Input % (Value_Offset_Max + 1);      Value_Offset_Input = Value_Offset_Max - (0 - Value_Offset_Input);      }
    Value_Input = Value_Offset_Input + Value_Min;
    return Value_Input;
  }
  

  

  




void Print_DebugMonitor_MainLoop(){
  //Defaults
  

  //Serial
  if(C_DebugMonitor_Serial_Enabled == true){
  Print_DebugMonitor_Serial();
  Serial.println();
  }

  //Stick
  if(C_DebugMonitor_Stick_Enabled == true)  {
    Print_DebugMonitor_Stick();
    stick.show();
  }
  //Screen
  if(C_DebugMonitor_Screen_Enabled == true) {
    display.clearDisplay();
    Print_DebugMonitor_Screen_Graph();
    Print_DebugMonitor_Screen_ColorChannels();
    Print_DebugMonitor_Screen();  
    display.display();
    }
    else{    Print_Screen_Disabled(); }
    
    
}





void Print_DebugMonitor_Serial(){
  
  Print_Padded_XofY(          "Index",      G_Index,        C_Index_Max, 4, true);
  Serial.print(G_EffectRunning_Name);
  
  Print_Padded_ValueLabeled(  "Px Actual",  G_Pixel_Actual,                2, true);
  Print_Padded_XofY(  "Px Offset",  G_Pixel_Offset, (G_Pixel_Offset_Max * G_Effect_LengthModifier),                nDigits(G_Pixel_Offset_Max * G_Effect_LengthModifier), true);
  Print_Padded_ValueLabeled(  "StepRow",    G_Pixel_Step_Row,              2, true);
  Print_Padded_ValueLabeled(  "StepCol",    G_Pixel_Step_Col,              2, true);
  
  Convert_uint32_TO_uint8(G_Color32_Actual);
  Print_Padded_RGBW8("ColorAct", G_Color8_Output[0], G_Color8_Output[1], G_Color8_Output[2], G_Color8_Output[3]);
  
}



//-----===== Print Data to Screen Functions =====-----//
void Print_DebugMonitor_Screen_DataBar(  uint16_t DataBar_Input, uint16_t DataBar_Input_Min, uint16_t DataBar_Input_Max, uint16_t DataBar_RowNumber, uint16_t DataBar_DrawMargins[2], uint16_t DataBar_DrawSize[2]){

  
  int DataBar_Size[2] = {C_Screen_ColWidth - DataBar_DrawMargins[0] - DataBar_DrawMargins[1], DataBar_DrawSize[1]};
  int DataBar_Location[2] = {DataBar_DrawMargins[0] ,(DataBar_RowNumber * C_Screen_RowHeight)};

  int DataBar_Cursor_Row;
  int DataBar_Cursor_Col;
  
  DataBar_Cursor_Row = DataBar_RowNumber * C_Screen_RowHeight;

  int DataBar_Input_Mapped = map(DataBar_Input, DataBar_Input_Min, DataBar_Input_Max, 0, DataBar_Size[0]);
  
  for(int Width = 0; Width < DataBar_DrawSize[0]; Width++){
    int DataBar_Location_Actual[2] = {    DataBar_Location[0] + DataBar_Input_Mapped + Width,    DataBar_Location[1]};    
      display.drawFastVLine(DataBar_Location_Actual[0],   DataBar_Location_Actual[1],    DataBar_DrawSize[1], WHITE);
  }
}



void Print_DebugMonitor_Screen(){
  //UserDefined
  char    PrintBuffer[256]; 
  int     Cursor_Row = 0;
  int     Cursor_Col = 0;
  int     Cursor_Row_Step = 10;
  int     DataBar_Width = 128;
  int     Screen_Row = 0;
  
  uint16_t DrawMargins[2];
  uint16_t DrawSize[2];

  
  
  Screen_Row = 0;
  Cursor_Row  = Cursor_Row_Step * Screen_Row;
  display.setCursor(Cursor_Col,Cursor_Row);
  sprintf(PrintBuffer, "G_Index %0*d of %0*d", 4, G_Index, 4, C_Index_Max  );
  display.print(PrintBuffer);
  //DataBar
  DrawMargins[0] = 0;     //Left
  DrawMargins[1] = 0;     //Right
  DrawSize[0]    = 1;     //Width
  DrawSize[1]    = 8;     //Height
  Print_DebugMonitor_Screen_DataBar(G_Index, C_Index_Min, C_Index_Max , Screen_Row, DrawMargins, DrawSize);
  //DataBar


  
  Screen_Row = 1;
  Cursor_Row  = Cursor_Row_Step * Screen_Row;
  display.setCursor(Cursor_Col,Cursor_Row);
  sprintf(PrintBuffer, "Col %1d (%+04d / step)", G_Pixel_Actual_Col, G_Pixel_Step_Col) ;
  display.print(PrintBuffer);
  //DataBar
  DrawMargins[0] = 32;     //Left
  DrawMargins[1] = 0;     //Right
  DrawSize[0]    = 8;     //Width
  DrawSize[1]    = 8;     //Height
  Print_DebugMonitor_Screen_DataBar(G_Pixel_Actual_Col, 0, 7, Screen_Row, DrawMargins, DrawSize);
  


  Screen_Row = 2;
  Cursor_Row  = Cursor_Row_Step * Screen_Row;
  display.setCursor(Cursor_Col,Cursor_Row);
  sprintf(PrintBuffer, "Row %1d (%+04d / step)", G_Pixel_Actual_Row, G_Pixel_Step_Row) ;
  display.print(PrintBuffer);
  //DataBar
  DrawMargins[0] = 32;     //Left
  DrawMargins[1] = 0;     //Right
  DrawSize[0]    = 8;     //Width
  DrawSize[1]    = 8;     //Height
  Print_DebugMonitor_Screen_DataBar(G_Pixel_Actual_Row, 0, 7, Screen_Row, DrawMargins, DrawSize);


  
  Screen_Row = 3;
  Cursor_Row  = Cursor_Row_Step * Screen_Row;
  Cursor_Col = 80;
  display.setCursor(Cursor_Col,Cursor_Row);
  int TimePerCycle = G_EffectTimer_Current - G_EffectTimer_Last;
  sprintf(PrintBuffer, "%d ms", TimePerCycle) ;
  display.print(PrintBuffer);
  //DataBar
  DrawMargins[0] = 74;     //Left
  DrawMargins[1] = 0;     //Right
  DrawSize[0]    = 1;     //Width
  DrawSize[1]    = 4;     //Height
  Print_DebugMonitor_Screen_DataBar(TimePerCycle, 0, 200, Screen_Row, DrawMargins, DrawSize);


  
  
}




void Print_DebugMonitor_Screen_Graph(){

    //User Defined
//Graph

  char    PrintBuffer[256]; 
  int     Cursor_Row = 0;
  int     Cursor_Col = 0;
  int     Cursor_Row_Step = 10;
  int     G_Pixel_Actual_Row = G_Pixel_Actual % C_PixelsPerY;
  int     G_Pixel_Actual_Col = G_Pixel_Actual / C_PixelsPerX;
  
  uint16_t Graph_Location[2] = {0, 29};
  uint16_t Graph_Size[2] = {34, 34};
  uint16_t Graph_Draw_Input[2] =  {G_Pixel_Actual_Row, G_Pixel_Actual_Col};
  uint16_t Graph_Draw_InputMin[2] =  {0, 0};
  uint16_t Graph_Draw_InputMax[2] =  {8, 8};
  uint16_t Graph_Border_Thickness[4]= {
    1,      //Top
    1,      //Left
    1,      //Bottom
    1       //Right
  };


  //Calculated Variables
  
  uint16_t ThicknessOffset;
  uint16_t Graph_SizeActual[2];
  uint16_t Graph_LocationActual[2];
  
  uint16_t  Graph_Grid_Size[2],
            Graph_Grid_Minor1[2],
            Graph_Grid_Minor2[2],
            Graph_Grid_Minor3[2],
            Graph_Grid_Minor4[2];
  
  
  uint16_t  Border_ThicknessY = Graph_Border_Thickness[0] + Graph_Border_Thickness[2];
  uint16_t  Border_ThicknessX = Graph_Border_Thickness[1] + Graph_Border_Thickness[3];

  uint16_t  Graph_Draw_Offset[2];
  uint16_t  Graph_Draw_Actual[2];
  uint16_t  Graph_InputMapped[2];

           //Offset Graph Size due to Border
           Graph_SizeActual[0] = Graph_Size[0] - Border_ThicknessX;
           Graph_SizeActual[1] = Graph_Size[1] - Border_ThicknessY;

           //Offset Graph Location due to Border
           Graph_LocationActual[0] = Graph_Location[0] + Graph_Border_Thickness[1];
           Graph_LocationActual[1] = Graph_Location[1] + Graph_Border_Thickness[0];
           
           //Calculate Grid Size (1/4 Total Size)
           Graph_Grid_Size[0] = (Graph_SizeActual[0] / 4);
           Graph_Grid_Size[1] = (Graph_SizeActual[1] / 4);
           
           //Calculate Minor 1 (1/4 Total Size)
           Graph_Grid_Minor1[0] = (Graph_Grid_Size[0] * 1) + Graph_LocationActual[0] - 1;
           Graph_Grid_Minor1[1] = (Graph_Grid_Size[1] * 1) + Graph_LocationActual[1] - 1;
           
           //Calculate Minor 2 (2/4 Total Size)
           Graph_Grid_Minor2[0] = (Graph_Grid_Size[0] * 2) + Graph_LocationActual[0];
           Graph_Grid_Minor2[1] = (Graph_Grid_Size[1] * 2) + Graph_LocationActual[1];
           
           //Calculate Minor 3 (3/4 Total Size)
           Graph_Grid_Minor3[0] = (Graph_Grid_Size[0] * 3) + Graph_LocationActual[0] + 1;
           Graph_Grid_Minor3[1] = (Graph_Grid_Size[1] * 3) + Graph_LocationActual[1] + 1;

           //Map Input to graph range
           Graph_InputMapped[0] = map(Graph_Draw_Input[0], Graph_Draw_InputMin[0], Graph_Draw_InputMax[0], Graph_Border_Thickness[0], Graph_SizeActual[0]);
           Graph_InputMapped[1] = map(Graph_Draw_Input[1], Graph_Draw_InputMin[1], Graph_Draw_InputMax[1], Graph_Border_Thickness[1], Graph_SizeActual[1]);
            
            //Offset for thick major gridline
           if(Graph_InputMapped[0] >= Graph_Grid_Minor2[0]){Graph_InputMapped[0] += 3;}
           if(Graph_InputMapped[1] >= Graph_Grid_Minor2[1]){Graph_InputMapped[1] += 3;}


   //====Border=====//
  
  for(ThicknessOffset = 0; ThicknessOffset < Graph_Border_Thickness[0]; ThicknessOffset++){    display.drawFastHLine(Graph_Location[0],                                   Graph_Location[1] + ThicknessOffset,                     Graph_Size[0],    WHITE );  };     //Top
  for(ThicknessOffset = 0; ThicknessOffset < Graph_Border_Thickness[1]; ThicknessOffset++){    display.drawFastVLine(Graph_Location[0] + ThicknessOffset,                 Graph_Location[1],                                       Graph_Size[1],    WHITE );  };     //Left
  for(ThicknessOffset = 0; ThicknessOffset < Graph_Border_Thickness[2]; ThicknessOffset++){    display.drawFastHLine(Graph_Location[0],                                   Graph_Location[1] - ThicknessOffset  + Graph_Size[1],    Graph_Size[0],    WHITE );  };     //Bottom
  for(ThicknessOffset = 0; ThicknessOffset < Graph_Border_Thickness[3]; ThicknessOffset++){    display.drawFastVLine(Graph_Location[0] - ThicknessOffset + Graph_Size[0], Graph_Location[1],                                       Graph_Size[1],    WHITE );  };     //Right

 

  //=====Major Gridlines
  for(ThicknessOffset = 0; ThicknessOffset < 3; ThicknessOffset++){  display.drawFastVLine(Graph_Grid_Minor2[0] + ThicknessOffset - 1,    Graph_LocationActual[1],                        Graph_SizeActual[1], WHITE); }
  for(ThicknessOffset = 0; ThicknessOffset < 3; ThicknessOffset++){  display.drawFastHLine(Graph_LocationActual[0],                       Graph_Grid_Minor2[1]+ ThicknessOffset - 1,      Graph_SizeActual[0], WHITE); }


  //=====Minor Gridlines
  display.drawFastVLine(Graph_Grid_Minor1[0],     Graph_LocationActual[1],         Graph_SizeActual[1] , WHITE);
  display.drawFastVLine(Graph_Grid_Minor2[0],     Graph_LocationActual[1],         Graph_SizeActual[1] , BLACK);
  display.drawFastVLine(Graph_Grid_Minor3[0],     Graph_LocationActual[1],         Graph_SizeActual[1] , WHITE);
  
  display.drawFastHLine(Graph_LocationActual[0],        Graph_Grid_Minor1[1],      Graph_SizeActual[0] , WHITE);
  display.drawFastHLine(Graph_LocationActual[0],        Graph_Grid_Minor2[1],      Graph_SizeActual[0] , BLACK);
  display.drawFastHLine(Graph_LocationActual[0],        Graph_Grid_Minor3[1],      Graph_SizeActual[0] , WHITE);


    
//=====Draw Centerpoint
display.drawPixel(Graph_Grid_Minor2[0],     Graph_Grid_Minor2[1],    WHITE);


//=====Main Graph of Input
  for(Graph_Draw_Offset[0] = Graph_Draw_InputMin[0]; Graph_Draw_Offset[0] < Graph_Draw_InputMax[0] / 2; Graph_Draw_Offset[0]++){
    for(Graph_Draw_Offset[1] = Graph_Draw_InputMin[1]; Graph_Draw_Offset[1] < Graph_Draw_InputMax[1] / 2; Graph_Draw_Offset[1]++){
      
      Graph_Draw_Actual[0] = Graph_Location[0] + Graph_Draw_Offset[0] + Graph_InputMapped[0];
      Graph_Draw_Actual[1] = Graph_Location[1] + Graph_Draw_Offset[1] + Graph_InputMapped[1];
      display.drawPixel(Graph_Draw_Actual[0], Graph_Draw_Actual[1], WHITE    );
      

      }
  }
  //DrawVector
  display.drawLine(Graph_Draw_Actual[0], Graph_Draw_Actual[1], Graph_Draw_Actual[0] + (G_Pixel_Step_Col * 2), Graph_Draw_Actual[1] + (G_Pixel_Step_Row * 2), WHITE);
}


void Print_DebugMonitor_Screen_ColorChannels(){
  display.setTextSize(2);
  int Location[2] = {40, 30};
  int Size[2] = {28, 32};
  if(G_ColorChannel_Enabled[0] == 1){  display.setTextColor(BLACK, WHITE); display.setCursor(Location[0],Location[1]);  display.println("R");  }
  else{                                display.setTextColor(WHITE, BLACK); display.setCursor(Location[0],Location[1]);  display.println("R");  }
  
  if(G_ColorChannel_Enabled[1] == 1){  display.setTextColor(BLACK, WHITE); display.setCursor(Location[0],Location[1]+16);  display.println("G");  }
  else{                                display.setTextColor(WHITE, BLACK); display.setCursor(Location[0],Location[1]+16);  display.println("G");  }
  
  if(G_ColorChannel_Enabled[2] == 1){  display.setTextColor(BLACK, WHITE); display.setCursor(Location[0]+16,Location[1]);  display.println("B");  }
  else{                                display.setTextColor(WHITE, BLACK); display.setCursor(Location[0]+16,Location[1]);  display.println("B");  }
  
  if(G_ColorChannel_Enabled[3] == 1){  display.setTextColor(BLACK, WHITE); display.setCursor(Location[0]+16,Location[1]+16);  display.println("W");  }
  else{                                display.setTextColor(WHITE, BLACK); display.setCursor(Location[0]+16,Location[1]+16);  display.println("W");  }

  display.drawFastHLine(Location[0]-1,                  Location[1] - 1,                    Size[0], WHITE);
  //display.drawFastHLine(Location[0]-1,                  Location[1] + (Size[1] / 2) - 1,    Size[0], WHITE);
  display.drawFastHLine(Location[0]-1,                  Location[1] + Size[1],          Size[0], WHITE);
  
  display.drawFastVLine(Location[0]-1,                  Location[1] - 1,                Size[1], WHITE);
  //display.drawFastVLine(Location[0] + (Size[0] / 2) - 1,  Location[1] - 1,                    Size[1], WHITE);
  display.drawFastVLine(Location[0] + Size[0],        Location[1] - 1,                    Size[1], WHITE);
  
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
}




void Print_DebugMonitor_Stick(){
  
  uint16_t ColorChannelsOutput_Pixel[8];
  uint32_t ColorChannelsOutput_Pixel_Color[8];
  byte Low = 1;
  byte High = 32;
  uint16_t Pixel;

  Pixel = 0;
  if(G_ColorChannel_Enabled[0] == 1){ColorChannelsOutput_Pixel_Color[Pixel] = strip.Color(High, 0, 0, 0);}
  else                            {ColorChannelsOutput_Pixel_Color[Pixel] = strip.Color(Low, 0, 0, 0);}
  stick.setPixelColor(Pixel, ColorChannelsOutput_Pixel_Color[Pixel]);


  Pixel = 1;
  if(G_ColorChannel_Enabled[1] == 1){ColorChannelsOutput_Pixel_Color[Pixel] = strip.Color(0, High, 0, 0);}
  else                            {ColorChannelsOutput_Pixel_Color[Pixel] = strip.Color(0, Low, 0, 0);}
  stick.setPixelColor(Pixel, ColorChannelsOutput_Pixel_Color[Pixel]);


  Pixel = 2;
  if(G_ColorChannel_Enabled[2] == 1){ColorChannelsOutput_Pixel_Color[Pixel] = strip.Color(0, 0, High, 0);}
  else                                {ColorChannelsOutput_Pixel_Color[Pixel] = strip.Color(0, 0, Low, 0);}
  stick.setPixelColor(Pixel, ColorChannelsOutput_Pixel_Color[Pixel]);


  Pixel = 3;
  if(G_ColorChannel_Enabled[3] == 1){ColorChannelsOutput_Pixel_Color[Pixel] = strip.Color(0, 0, 0, High/ 4);}
  else                                {ColorChannelsOutput_Pixel_Color[Pixel] = strip.Color(0, 0, 0, Low);}
  stick.setPixelColor(Pixel, ColorChannelsOutput_Pixel_Color[Pixel]);

  Pixel = 4;
  ColorChannelsOutput_Pixel_Color[Pixel] = Color32_Test[0];
  stick.setPixelColor(Pixel, ColorChannelsOutput_Pixel_Color[Pixel]);



}




void Print_Screen_Disabled(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Output Monitor Disabled");
  display.display();
}





//////////////////////////////Print Stats to Serial
void Buffer_Text(char TextInput[32]){
  sprintf(G_Text_Buffer32, "%s ", TextInput);
}

void Buffer_EffectRunning(char TextInput[32]){
  sprintf(G_EffectRunning_Name, "%s ", TextInput);
}


void Print_Padded_Value(uint32_t Value, byte Output_Length, bool PadWith0) {
  char    PrintBuffer[256]; //= PrintBuffer_Clear(PrintBuffer);
  
  if (PadWith0 == true) {    sprintf(PrintBuffer, "%0*d", Output_Length, Value);  }
  else                  {    sprintf(PrintBuffer, "%*d",  Output_Length, Value);  };
  Serial.print(PrintBuffer);
}

void Print_Padded_ValueLabeled(char Title[16], uint32_t Value, byte Output_Length, bool PadWith0) {
  char    PrintBuffer[256]; //= PrintBuffer_Clear(PrintBuffer);
  
  if (PadWith0 == true) {    sprintf(PrintBuffer, "(%s %0*d) ", Title, Output_Length, Value);  }
  else                  {    sprintf(PrintBuffer, "(%s %*d) ",  Title, Output_Length, Value);  }
  Serial.print(PrintBuffer);
}


void Print_Padded_XofY(char Title[16], uint32_t ValueX, uint32_t ValueY, byte Output_Length, bool PrintPercent) {
  char    PrintBuffer[256];
  double Percent = map(ValueX, 0, ValueY, 0, 100) / 100;
  if(PrintPercent == true){    sprintf(PrintBuffer, "(%s %0*d of %0*d %G) ", Title, Output_Length, ValueX, Output_Length, ValueY, Percent); }
  //else{                       sprintf(PrintBuffer, "(%s %0*d of %0*d) ", Title, Output_Length, ValueX, Output_Length, ValueY); }
  
  
  Serial.print(PrintBuffer);
}


void Print_Padded_RGBW8(char Title[16], uint8_t R, uint8_t G, uint8_t B, uint8_t W) {
  char    PrintBuffer[256];
  sprintf(PrintBuffer, "(%s r %3d g %3d b %3d w %3d ) ", Title, R, G, B, W);  
  Serial.print(PrintBuffer);
}

void Print_Padded_RGBW32(char Title[16], uint32_t ColorInput32) {
  char    PrintBuffer[256];
  G_Color8_Output[0] = G_Color8_PrintBuffer[0];
  G_Color8_Output[1] = G_Color8_PrintBuffer[1];
  G_Color8_Output[2] = G_Color8_PrintBuffer[2];
  G_Color8_Output[3] = G_Color8_PrintBuffer[3];

  sprintf(PrintBuffer, "( %s r %d g %d b %d w %d ) ", Title, G_Color8_PrintBuffer[0] , G_Color8_PrintBuffer[1] , G_Color8_PrintBuffer[2] , G_Color8_PrintBuffer[3] );  
  Serial.print(PrintBuffer);
}

void Print_Padded_Text(char Text[16], byte OutputLength) {
  char    PrintBuffer[256];
  
  sprintf(PrintBuffer, "%s", Text );  
  Serial.print(PrintBuffer);
}
