//Encoder Stuff
#include <Encoder.h>
Encoder knobLeft(8, 9);
Encoder knobRight(10, 12);


//Screen Stuff
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>


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

#define LED_1_PIN 11
#define LED_2_PIN 12
#define LED_3_PIN 10

#define LED_1_COUNT 64
#define LED_2_COUNT 64
#define LED_3_COUNT 15

#define LED_1_BRIGHTNESS 32
#define LED_2_BRIGHTNESS 32
#define LED_3_BRIGHTNESS 32

Adafruit_NeoPixel pannelA = Adafruit_NeoPixel(LED_1_COUNT, LED_1_PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel pannelB = Adafruit_NeoPixel(LED_2_COUNT, LED_2_PIN, NEO_GRBW + NEO_KHZ800);

Adafruit_NeoPixel stick = Adafruit_NeoPixel(LED_3_COUNT, LED_3_PIN, NEO_GRBW + NEO_KHZ800);



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
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};


void setup() {
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  pannelA.setBrightness(LED_1_BRIGHTNESS);
  pannelA.begin();
  pannelA.show(); 

  pannelB.setBrightness(LED_2_BRIGHTNESS);
  pannelB.begin();
  pannelB.show();

  stick.setBrightness(LED_2_BRIGHTNESS);
  stick.begin();
  stick.show(); 
  randomSeed(analogRead(1));

  //Screen Stuff START //
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(20);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //Screen Stuff END //
}



//  | \\\\////\\\\////\\\\////\\\\////\\\\////\\\\////
//  | ////\\\\////\\\\////\\\\////\\\\////\\\\////\\\\
//  | \\\\////\\\\////|      START       |////\\\\////
//  | ////\\\\////\\\\| Global Variables |\\\\////\\\\
//  | \\\\////\\\\////\\\\////\\\\////\\\\////\\\\////
//  | ////\\\\////\\\\////\\\\////\\\\////\\\\////\\\\

//Constants

long G_UserInput_Knob_PositionLeft  = -999;
long G_UserInput_Knob_PositionRight = -999;


const uint16_t 
C_Pixel_Start = 0,
C_Pixel_Total = 64,

C_PixelsPerY = 8,
C_PixelsPerX = 8;

uint16_t C_PixelsPerDimension[2] = {C_PixelsPerX, C_PixelsPerY};

bool  C_PrintStats_Master = true;
bool  C_PrintStats_Debug = true;
//bool  C_PrintStats_Screen = true;
bool  C_DebugMonitor_Serial_Enabled = true;
bool  C_DebugMonitor_Screen_Enabled = false;
bool  C_DebugMonitor_Stick_Enabled = false;
bool  C_Effect_Timer_Enabled = true;

bool  C_Debug_ClearOnStep = false;



float C_Delay_Step[3], G_Delay_StepActual[3];

float C_Delay_Effect[3] = {
                5000000,
                0,
                0.5
};
                


const uint16_t C_Screen_RowHeight = 10;
const uint16_t C_Screen_ColWidth = 128;




//Variables
//G_Index
uint16_t G_Index = 0;
const uint16_t C_Index_Min = 0;
const uint16_t C_Index_Max  = 4096;

uint16_t G_Index_Step = 0;
uint16_t C_Index_Step_Max = 128;
uint16_t C_Index_Step_Min = 0;

//Pixel
uint16_t G_Pixel_UpdateEveryX;
uint16_t G_Pixel_Step_Row, G_Pixel_Step_Col;
uint16_t G_Pixel_Max = C_Pixel_Start + C_Pixel_Total;

uint16_t G_Pixel_Offset_Min = 1;
uint16_t G_Pixel_Offset_Max = G_Pixel_Max + 1;

uint16_t G_Pixel_Offset = G_Pixel_Offset_Min;
uint16_t G_Pixel_Actual, G_Pixel_Actual_Col, G_Pixel_Actual_Row;
uint16_t G_Pixel_ActualXY[2];

//Effect
double   G_Effect_LengthModifier;
char     G_EffectRunning_Name[16];
uint16_t G_Effect_Start_Offset;
uint16_t G_Effect_UpdateEveryXSteps;
uint16_t G_Effect_Symmetry[4] = {random(2),random(2),random(2),random(2)};
uint16_t G_Effect_SymmetryTest[4] = {1, 1, 1, 1};
uint16_t G_Effect_TransformType = 0;


uint16_t G_Effect_RunCount = 0;
uint16_t G_Effect_Step[2];
uint16_t G_Effect_StartingOffset[2];

uint8_t G_Pannels_Enabled[2] = {1, 1};


//ColorBrushes
uint32_t      G_Color32_Input,        G_Color32_Output,       G_Color32_Target,     G_Color32_Mask,       G_Color32_NewRandom;
uint16_t       G_Color8_Input[4],      G_Color8_Output[4],     G_Color8_Target[4],   G_Color8_Mask[4],     G_Color8_NewRandom[4];
uint16_t       G_ColorChannel_Enabled[4];
float        G_Color_Mask[4], G_Color_Resolution[4],  G_Color_Multiplier[4];
uint32_t      Color32_Test[8];
uint32_t      G_Color32_Master[32];
uint32_t      G_Color32_Actual;

uint16_t       G_Color8_PrintBuffer[4];
uint32_t      Color32_Test_RGBW[4] = {
  pannelA.Color(255, 0, 0, 0),
  pannelA.Color(0, 255, 0, 0),
  pannelA.Color(0, 0, 255, 0),
  pannelA.Color(0, 0, 0, 255)
};


//
float         G_Brightness_FaderMaster = 1.0;
float         G_Brightness_Actual = 1.0;
//Color32_Sequences
uint32_t      G_Color32_Sequence1[64];


//EffectTimer
int  G_EffectTimer_Master[4], G_EffectTimer_Effect[4], G_EffectTimer_Current[4], G_EffectTimer_Last[4], G_EffectTimer_StepRunning[4], G_EffectTimer_StepRemaining[4], G_EffectTimer_StepStart[4];





//Delay
uint32_t G_Delay_Pixel;
uint32_t G_Delay_Pixel_Actual;

//Misc
char    PrintBuffer[256];
char    G_Text_Buffer32[32];
int G_k;  //Increments every time offset goes over max

//


//  | \\\\////\\\\////\\\\////\\\\////\\\\////\\\\////
//  | ////\\\\////\\\\////\\\\////\\\\////\\\\////\\\\
//  | \\\\////\\\\////|       END        |////\\\\////
//  | ////\\\\////\\\\| Global Variables |\\\\////\\\\
//  | \\\\////\\\\////\\\\////\\\\////\\\\////\\\\////
//  | ////\\\\////\\\\////\\\\////\\\\////\\\\////\\\\


//=================================================================================================================================================================//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//=================================================================================================================================================================//



void loop() {
  Print_Screen_Disabled();

  for (int TestingLoop1 = 0; TestingLoop1 < 1024; TestingLoop1++) {

    G_Effect_Step[0] = G_Effect_Step[0] % C_Pixel_Total;
    G_Effect_Step[1] = G_Effect_Step[1] % C_Pixel_Total;
    
    
    C_Delay_Step[0] = 2500;
    C_Delay_Step[1] = C_Delay_Step[0] / 1000;
    C_Delay_Step[2] = C_Delay_Step[1] / 1000;
   
    G_Effect_Step[0] = (G_Index>>6) % 64;;
    G_Effect_Step[1] = (G_Index>>4) % 64;

    G_Effect_StartingOffset[0] = 0;
    G_Effect_StartingOffset[1] = 0;

    
    bool Effect_Masterloop = true;
    if (Effect_Masterloop == true) {Effect_MasterLoop();    }
    Update_ColorSequence1();
  }
  //End Testing Loop
}
//End Master Loop




//=================================================================================================================================================================//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//=================================================================================================================================================================//

void Effect_MasterLoop(){
  Update_Step_Start();
  /////////////////////////
  
  Update_G_Index();
  
  Effect_colorWipeX(        G_Color32_Sequence1,  G_Effect_Step,     G_Effect_StartingOffset,  (0.125 * random(8) + 1) , 0);
 // Effect_colorWipeX(        0,  G_Effect_Step,     G_Effect_StartingOffset,       (0.875 * ((G_Index>>7) % 4)), 0   );
  
  Effect_DrawShape_FromPixel();
  
  //Effect_StepBG_TO_ColorTarget();
  
  Effect_DotTest4(G_Index, 0, G_Effect_SymmetryTest, 1);
  Update_SmoothBrightnessChange(0.3);
  /////////////////////////
  Update_Step_End();
  stick.clear();
}


//User Input




//=====-----=====-----     Effects
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Effect_colorWipeX(uint32_t L_Input_Color32_Sequence[32], uint16_t L_Step[2], uint16_t L_Effect_StartOffset[2], double L_LengthModifier, uint16_t L_BlendType){
  uint16_t L_Offset_Max = C_Pixel_Total * L_LengthModifier;
  uint16_t L_Offset = G_Index;
  L_Offset = L_Offset % L_Offset_Max;
  Update_Effect_Timer();
  //Print_ValName("L_Offset", L_Offset, 2, false);
  
  G_Pixel_Actual += Get_Effect_StepForward(L_Offset, L_Step, L_Effect_StartOffset) + L_Step[0];
  G_Pixel_Actual = G_Pixel_Actual % 64;
  uint16_t L_Sequence = (G_Index>>4) % 8;  
    
   Update_G_Pixel_Actual_Transform(G_Pixel_Actual, G_Effect_TransformType, G_Effect_Symmetry, L_Input_Color32_Sequence[L_Sequence], L_BlendType);
   
  //Effect_StepBG_TO_ColorTarget();
  
  if(G_Index % 8 == 0){if(C_Debug_ClearOnStep == true){Update_ClearPannels(G_Pannels_Enabled);}}
  
  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Effect_FadeAllPixels() {
  uint16_t L_Pixel_Offset, L_Pixel_Actual;
  for (L_Pixel_Offset = G_Pixel_Offset_Min; L_Pixel_Offset < G_Pixel_Offset_Max; L_Pixel_Offset++) {
    L_Pixel_Actual = L_Pixel_Offset - G_Pixel_Offset_Min;
    pannelA.setPixelColor(L_Pixel_Actual, (Make_BlendColor_PixANDColor32(L_Pixel_Offset, pannelA.Color(0, 0, 0, 0) ) ) );
  }
  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t Make_BlendColor_PixANDColor32(uint16_t Pixel, uint32_t InputColor32) {
  uint32_t  Color1_32,
            Color2_32,
            ColorAvg_32;

  uint16_t   Color1_8[4],
             Color2_8[4],
             ColorAvg_8[4];
  byte i;

  Set_Color32_TO_Color8_Output(pannelA.getPixelColor(Pixel));
  for (i = 0; i < 4; i++) {
    Color1_8[i] = G_Color8_Output[i];
  }

  Set_Color32_TO_Color8_Output(InputColor32);
  for (i = 0; i < 4; i++) {
    Color2_8[i] = G_Color8_Output[i];
  }



  for (i = 0; i < 4; i++) {
    ColorAvg_8[i] = (Color1_8[i] + Color2_8[i]) / 2;
  }


  ColorAvg_32 = pannelA.Color(ColorAvg_8[0], ColorAvg_8[1], ColorAvg_8[2], ColorAvg_8[3] / 2);
  int MinimumThreshold = 8;
  if (
    (abs(Color1_8[0] - Color2_8[0]) < MinimumThreshold) &
    (abs(Color1_8[1] - Color2_8[1]) < MinimumThreshold) &
    (abs(Color1_8[2] - Color2_8[2]) < MinimumThreshold) &
    (abs(Color1_8[3] - Color2_8[3]) < MinimumThreshold)
  ) {
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

void Effect_DrawShape_FromPixel() {
  
  int L_ShapeLength = C_PixelsPerX * 2;
  uint16_t L_Offset_Output;
  uint16_t L_Offset_Input = G_Index % L_ShapeLength;
  if(L_Offset_Input < C_PixelsPerX)           {L_Offset_Output = L_Offset_Input;}
  else if(L_Offset_Input < C_PixelsPerX * 2)  {L_Offset_Output = (L_Offset_Input + (C_PixelsPerX * (C_PixelsPerY-2)));}
  uint32_t L_Color32 = Get_Random_Color32();
  uint16_t L_BlendType = 0;
  
  //Borders
Update_G_Pixel_Actual_Transform( L_Offset_Output, 0, G_Effect_Symmetry, L_Color32, L_BlendType);
//pannelA.setPixelColor(L_Offset_Output,L_Color32);
//pannelA.show();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Effect_DrawDot_XY(uint16_t L_InputX, uint16_t L_InputY, uint32_t L_InputColor32, bool L_ShowUpdate) {
  Set_G_Pixel_Actual_From_XY(L_InputX, L_InputY);
  Set_G_Color32_Actual(L_InputColor32);
//  Set_SetPixelColor_Of_G_Pixel_Actual(L_ShowUpdate);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//START Pixel Test//==========================================================================================
void Effect_DotTest4(uint16_t L_Offset, uint16_t L_TransformType, uint16_t L_TransformSymmetry[4], uint16_t L_BlendType) {
  Update_Effect_Start("Effect_DotTest4"); //Buffer Effect Name
  L_Offset = L_Offset % C_Pixel_Total;  //Limit Offset Input to Total Pixels
  
  //Transform & Show Pixel Offset Input
  Update_G_Pixel_Actual_Transform( G_Index % 64, L_TransformType, L_TransformSymmetry, G_Color32_Sequence1[G_Index % 64], L_BlendType);
  
}
//Pixel Test//==========================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Effect_DrawDot1(uint16_t L_Pixel_Actual, uint32_t L_InputColor32, uint16_t L_BlendType) {
  L_Pixel_Actual = L_Pixel_Actual % C_Pixel_Total;
  G_Pixel_Actual = L_Pixel_Actual;
  Set_G_Color32_Actual(L_InputColor32);
  (L_Pixel_Actual, L_InputColor32, L_BlendType);
  
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Effect_StepBG_TO_ColorTarget (){
  uint16_t L_ColorModifier[4] = {-4, -5, -3, -2};
  uint32_t L_Input_Color32_Target = Get_Color32_Modify(Get_Random_Color32_Limit(32, 32, 32, 8), L_ColorModifier);
  uint16_t L_ColorFade_Ammount = 1;
  uint16_t L_LoopCount = 1;
  uint16_t L_Loop;
  int L_Pixel;
  uint32_t L_Color32_Get;
  uint8_t L_Color8_Get[4];
  uint8_t L_Color8_Target[4];
  uint8_t L_Color8_Output[4];
  uint32_t L_Color32_Output;
  if((G_Index % 512) == 0){L_Input_Color32_Target = Get_Random_Color32();}
  
  for(L_Loop = 0; L_Loop < L_LoopCount; L_Loop++){
    for(L_Pixel = 0; L_Pixel < C_Pixel_Total; L_Pixel++){
      
      L_Color32_Get = pannelA.getPixelColor(L_Pixel);
      for(int n = 0; n < 4; n++){
        
        L_Color8_Get[n] = Get_uint8_FROM_uint32(L_Color32_Get, n);
        L_Color8_Target[n] = Get_uint8_FROM_uint32(L_Input_Color32_Target, n);
        if(L_Color8_Target[n] != L_Color8_Get[n]){
          if        (abs(L_Color8_Target[n] -  L_Color8_Get[n]) < L_ColorFade_Ammount)    {L_Color8_Output[n] = L_Color8_Target[n];                 }
          else if   (L_Color8_Target[n]       >     L_Color8_Get[n])                      {L_Color8_Output[n] = L_Color8_Get[n] + L_ColorFade_Ammount;     }
          else if   (L_Color8_Target[n]       <     L_Color8_Get[n])                      {L_Color8_Output[n] = L_Color8_Get[n] - L_ColorFade_Ammount;     }
         // L_Color8_Output[n] = (L_Color8_Output[n] + (L_Pixel>>3)) % C_Pixel_Total;
        }
      }
      L_Color32_Output = pannelA.Color(L_Color8_Output[0], L_Color8_Output[1], L_Color8_Output[2], L_Color8_Output[3]);
      
      Update_G_Pixel_Actual_Transform(L_Pixel, G_Effect_TransformType, G_Effect_Symmetry, L_Color32_Output, 0);
      //if(G_Index % 16 == 0){pannelA.show();}    
    }
    //Update_ShowPannels();
  }
  //pannelA.show();
  
}






/*
  //Smooth the color a bit
  Color32_Test[0] << ((G_Pixel_Offset << 0) % 25);
  Color32_Test[0] << ((G_Pixel_Offset << 0) % 17);
  Color32_Test[0] << ((G_Pixel_Offset << 0) % 9);
  Color32_Test[0] << ((G_Pixel_Offset << 0) % 5);

*/











//=================================================================================================================================================================//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//=================================================================================================================================================================//



// | //=============================================================================================================================// | //
// | // >><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<// | // 
// | // [] START _Updates []                                                                                                        // | // 
// | // >><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<// | // 
// | //=============================================================================================================================// | //
void Update_G_Index() {
  
  if (     G_Index > C_Index_Max )    {    G_Index = C_Index_Min;           }   //Check if above max
  else                                {    G_Index++;                       }   //Increment normally


  //Update Every Time
  //G_Effect_TransformType = map(G_Index, C_Index_Min, C_Index_Max, 0, 8);
  

  //Update If Enabled
  if (C_PrintStats_Master == true)    {    Print_DebugMonitor_MainLoop();   }   //PrintStats if enabled

    if((G_Index == 0) | (G_Index % 768 == 0)) {}
    if((G_Index == 0) | (G_Index % 256 == 0)) {
      uint16_t Selector = random(4);
                               G_Effect_Symmetry[Selector] = 1;
      if(random(2) == 1){      G_Effect_Symmetry[(Selector + 2) % 4] = 1;
        if(random(3) == 1){      G_Effect_Symmetry[(Selector + 1) % 4] = 1;}
        if(random(4) == 1){      G_Effect_Symmetry[(Selector + 3) % 4] = 1;}
      }
    }

  //Update Every X
  //if(G_Index % 8 == 0){Effect_StepBG_TO_ColorTarget();}
  if(  (G_Index == 0)       | (G_Index % 256 == 0)  )   {  Set_Random_Sequence_Colors32_Growing(64);} //Set_Random_Sequence_Colors32(12);}
  if(  (G_Index == 0)       | (G_Index % 96 == 0)  )    {  G_Brightness_FaderMaster = Get_float_rand(0.05, 1);}
  if(  (G_Index == 0)       | (G_Index % 16 == 0)   )   {  Set_G_Delay_StepActual(map(G_Index % (C_Index_Max / 4), C_Index_Min, (C_Index_Max/4), C_Delay_Step[0], C_Delay_Step[0]/1000), 0); }
  if(  (G_Index == 0)       | (G_Index % 768 == 0)  )   {
    for (int n = 0; n < 4; n ++){
      if(random(3) == 0){
        G_Effect_Symmetry[n] = G_Effect_Symmetry[n] = 1;
      } 
      else{G_Effect_Symmetry[n] = 0;}
    }
  }
}



void Update_G_Index_Step(){
  
  if (     G_Index_Step > 4098 )    {    G_Index_Step = C_Index_Step_Min;           }   //Check if above max
  else                                     {    G_Index_Step++;                       }   //Increment normally
  //Print_ValName("G_Index_Step", G_Index_Step, 2, true);
}
void Update_Effect_Timer() {
  
  G_EffectTimer_Last[1] = G_EffectTimer_StepStart[1];
  G_EffectTimer_Last[0] = G_EffectTimer_StepStart[0];
  G_EffectTimer_Last[2] = G_EffectTimer_StepStart[2];
  
  G_EffectTimer_Current[1] = millis();
  G_EffectTimer_Current[0] = G_EffectTimer_Current[1] * 1000;
  G_EffectTimer_Current[2] = G_EffectTimer_Current[1] / 1000;
  
  G_EffectTimer_Master[1] += G_EffectTimer_Current[1];
  G_EffectTimer_Master[0] += G_EffectTimer_Current[0];
  G_EffectTimer_Master[2] += G_EffectTimer_Current[2];

  G_EffectTimer_StepRunning[1] = G_EffectTimer_Current[1] - G_EffectTimer_Last[1];  
  G_EffectTimer_StepRunning[0] = G_EffectTimer_Current[0] - G_EffectTimer_Last[0];  
  G_EffectTimer_StepRunning[2] = G_EffectTimer_Current[2] - G_EffectTimer_Last[2];  

  G_EffectTimer_StepRemaining[0]  = G_Delay_StepActual[0] - G_EffectTimer_StepRunning[0];
  G_EffectTimer_StepRemaining[1]  = G_Delay_StepActual[1] - G_EffectTimer_StepRunning[1];
  G_EffectTimer_StepRemaining[2]  = G_Delay_StepActual[2] - G_EffectTimer_StepRunning[2];
  Update_Metronome();
  //Update_Metronome();
    //Stats
    bool L_PrintStats = false;
    if(L_PrintStats == true){
      Print_ValName("Start", G_EffectTimer_Last[1], 8, true);
      Print_ValName("Current", G_EffectTimer_Current[1], 8, true);
      Print_ValName("RunningFor", G_EffectTimer_StepRunning[1],  8, true);
      Print_ValName("RunningTarget", G_Delay_StepActual[1],  8, true);
      Print_ValName("Remaining", G_EffectTimer_StepRemaining[1],  8, true);
      Serial.println();
    }
  
  
  //G_EffectTimer_Effect = ;
}

void Update_Effect_Start(char L_EffectName[64]){
  Buffer_EffectRunning(L_EffectName);
  Update_Effect_Timer();
}

void Update_Effect_End(){
  Update_Effect_Timer();
  G_Effect_RunCount+=1;
 Print_ValName("Run Count", G_Effect_RunCount, 2, false);
  Serial.println("EndEffect");
}

void Update_Step_Start(){
  
  Update_Effect_Timer();
  Update_Metronome();
  G_EffectTimer_StepStart[0] = G_EffectTimer_Current[0];
  G_EffectTimer_StepStart[1] = G_EffectTimer_Current[1];
  G_EffectTimer_StepStart[2] = G_EffectTimer_Current[2];
  
}
void Update_Step_End(){
  if(G_Index % G_Effect_UpdateEveryXSteps == 0){Update_ShowPannels(G_Pannels_Enabled);}
  Update_DelayLoop();
  Update_StepHasEnded();
  Update_Metronome();
}

void Update_StepHasEnded(){
  Serial.println("EndStep");
  
}

void Update_Metronome(){
  uint16_t L_PixelActual_Step, L_PixelActual_Index;
  uint32_t L_Color32;
  uint16_t L_Start = 0;
  uint16_t L_Half = stick.numPixels() / 2;
  uint16_t L_End = stick.numPixels();
  uint16_t L_Devider;
  
  L_Devider = 64;
  //L_PixelActual_Step = map(G_Index_Step % L_Devider, 0, L_Devider, L_End, L_Start);
  if(G_Index_Step % L_Devider == 0){
    for(int n = 0; n < L_End; n++){
      //Print_ValName("L_PixelActual", L_PixelActual_Step, 2, false);
      L_Color32 = stick.Color(32, 0, 0, 0);
      stick.setPixelColor(n, L_Color32);
      }
    stick.show();
    stick.clear();
  }
  
  L_Devider = 256;
  L_PixelActual_Step = map(G_Index_Step % L_Devider, 0, L_Devider, L_End, L_Start);
  L_Color32 = stick.Color(0, 255, 0, 0);
  stick.setPixelColor(L_PixelActual_Step, L_Color32);

  L_PixelActual_Index = map(G_Index, C_Index_Min, C_Index_Max, L_End, 0);
  stick.setPixelColor(L_PixelActual_Index, stick.Color(0,255,0,0));
 // stick.setPixelColor(L_PixelActual-1, stick.Color(0,128,0,0));

  
    
  

  stick.show();  
  //stick.setPixelColor(0  , stick.Color(32,32,32,32));
  stick.setPixelColor(L_Half  , stick.Color(32,32,32,32));
  //stick.setPixelColor(L_End   , stick.Color(32,32,32,32));
  stick.show();
  //if(L_PixelActual_Step == L_Start){stick.setPixelColor(L_Start , stick.Color(255,255,255,255));}
  if(L_PixelActual_Step == L_Half){stick.setPixelColor(L_Half , stick.Color(255,255,255,255));}
  //if(L_PixelActual_Step >= L_End-1){stick.setPixelColor(L_End , stick.Color(255,255,255,255));}
  stick.show();
  //delayMicroseconds(10);
  
}


void Update_DelayLoop(){
  Update_Effect_Timer();
  bool L_PrintStatsLocal  = 0;
  if(L_PrintStatsLocal == 1){
    Print_ValName("Current", G_EffectTimer_Current[1], 4, true);
    Print_ValName("Start", G_EffectTimer_Last[1], 4, true);
    Print_ValName("Remaining", G_EffectTimer_StepRunning[1], 4, true);
  }
  if( G_EffectTimer_StepRemaining[1] > 0){
    while(G_EffectTimer_StepRemaining[1] > 0){      
        
    
      Update_Metronome();
      Serial.println(G_EffectTimer_StepRemaining[1]);
      delayMicroseconds(10);       
      Update_Effect_Timer(); 
      }
  }
}

void Update_ClearPannels(uint8_t L_PannelToggle[2]){
  if(L_PannelToggle[0] == 1){pannelA.clear();}
  if(L_PannelToggle[1] == 1){pannelB.clear();}
}

void Update_ShowPannels(uint8_t L_PannelToggle[2]){
  if(L_PannelToggle[0] == 1){pannelA.show();}
  if(L_PannelToggle[1] == 1){pannelB.show();}
}


void Update_SmoothBrightnessChange(float L_Speed){

  if(abs(G_Brightness_Actual - G_Brightness_FaderMaster) < L_Speed)   {G_Brightness_Actual =  G_Brightness_FaderMaster;}      //Less than 1 step away, make equal to
  if(    G_Brightness_Actual < G_Brightness_FaderMaster)              {G_Brightness_Actual += L_Speed;}      //FaderMaster is larger, ++
  if(    G_Brightness_Actual > G_Brightness_FaderMaster)              {G_Brightness_Actual -= L_Speed;}      //FaderMaster is smaller, --
}



void Update_ColorSequence1(){
  uint8_t L_Color8_Out[4];
  int n, i;


  //Establish MasterColor for Sequence
   for(n = 0; n < 4; n++){
       if(random(3) == 0){G_ColorChannel_Enabled[n] = 1;}
       else              {G_ColorChannel_Enabled[n] = 0;}   
       G_Color_Mask[n]          = Get_float_rand(0.1,1) * G_ColorChannel_Enabled[n];
       //Serial.print("Mask"); Serial.print(G_Color_Mask[n]);
       G_Color_Resolution[n]    = Get_float_rand(0.1,1);
       //Serial.print("Res"); Serial.print(G_Color_Resolution[n]);
       G_Color_Multiplier[n] = 256 / (G_Color_Resolution[n] * 256);
       //Serial.print("Mult"); Serial.print(G_Color_Multiplier[n]);
   }

for (i = 0; i < C_Pixel_Total; i++){
  //Once for Each Channels (RGBW)
  for(n = 0; n < 4; n++){
   
       if(random(3) == 0){G_ColorChannel_Enabled[n] = 1;}
       else              {G_ColorChannel_Enabled[n] = 0;}   
       G_Color_Mask[n]          = Get_float_rand(0.1,1) * G_ColorChannel_Enabled[n];
       //Serial.print("Mask"); Serial.print(G_Color_Mask[n]);
       G_Color_Resolution[n]    = Get_float_rand(0.1,1);
       //Serial.print("Res"); Serial.print(G_Color_Resolution[n]);
       G_Color_Multiplier[n] = 256 / (G_Color_Resolution[n] * 256);
       //Serial.print("Mult"); Serial.print(G_Color_Multiplier[n]);
    
    

   //Map to Sequence
    L_Color8_Out[n] = map(i, 0, C_Pixel_Total,0,G_Color_Mask[n]);   
    
    //Scale to resolution
    L_Color8_Out[n] = (i / G_Color_Multiplier[n]) * G_Color_Multiplier[n];
  }
 G_Color32_Sequence1[i] = Get_Color32_FROM_4Color8(L_Color8_Out);
 Print_Padded_RGBW32("", G_Color32_Sequence1[i]);
}
}
  

// | //=============================================================================================================================// | //
// | // >><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<// | // 
// | // []  END  _Updates []                                                    // | // 
// | // >><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<// | // 
// | //=============================================================================================================================// | //



// | //=================================================================================================================================// | //
// | // \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ // | // 
// | // [] START _Set []                                                            // | // 
// | // / = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ =// | // 
// | //=================================================================================================================================// | //


void Set_G_Delay_StepActual(float L_Input, uint16_t L_InputUnit){
  L_InputUnit = L_InputUnit % 3;
  if(L_InputUnit == 0){}
  else{L_Input = L_Input / (1000 * L_InputUnit);}
  
  G_Delay_StepActual[0] = L_Input;
  G_Delay_StepActual[1] = G_Delay_StepActual[0]/ 1000;
  G_Delay_StepActual[2] = G_Delay_StepActual[1]/ 1000;
}




void Set_SetPixelColor_From_Input(uint16_t L_Pixel_Input, uint32_t L_Color32_Input, uint8_t L_BlendType) {
  uint32_t L_Color32_Output;
  
      //Replace
      if(L_BlendType == 0)      {L_Color32_Output = L_Color32_Input;}

      //Blend Avg
      else if(L_BlendType == 1) {L_Color32_Output = Make_BlendColor_PixANDColor32(L_Pixel_Input, L_Color32_Input);}

      L_Color32_Output = Get_Brightness_FROM_Color32(L_Color32_Output, G_Brightness_Actual);
      Serial.print(" ( ");
      Serial.print("Px "); Print_Padded_Value(L_Pixel_Input, 2, true);
      Print_Padded_RGBW32("", L_Color32_Output);
      Serial.print(" ) ");
      pannelA.setPixelColor(L_Pixel_Input, L_Color32_Output);
      pannelB.setPixelColor(L_Pixel_Input, L_Color32_Output);
      Update_ShowPannels(G_Pannels_Enabled);
      Update_G_Index_Step();

  
}


void Set_G_Color32_Actual(uint32_t InputColor32) {
  G_Color32_Actual = InputColor32;
}

void Set_PixelActual(uint16_t L_Input) {
  G_Pixel_Actual = L_Input % C_Pixel_Total;
  G_Pixel_ActualXY[0] = Get_Pixel_XY_From_Pixel_Actual(L_Input, 0);
  G_Pixel_ActualXY[1] = Get_Pixel_XY_From_Pixel_Actual(L_Input, 1);
}

void Set_G_Pixel_Actual_From_XY(uint16_t L_InputX, uint16_t L_InputY) {
  G_Pixel_ActualXY[0] = L_InputX;
  G_Pixel_ActualXY[1] = L_InputY;
  G_Pixel_Actual = L_InputX + (L_InputY * C_PixelsPerX);
}

void Set_Color32_TO_Color8_Output(uint32_t Input) {
  G_Color8_Output[0] = Input >> 16;
  G_Color8_Output[1] = Input >> 8;
  G_Color8_Output[2] = Input >> 0;
  G_Color8_Output[3] = Input >> 24;
}

void    Set_Random_Sequence_Colors32(uint16_t L_ColorsCount) {
  uint16_t L_Color[4];
  for (uint16_t L_Offset = 0; L_Offset < L_ColorsCount; L_Offset++) {
    G_Color32_Sequence1[L_Offset] = pannelA.Color(
                                   L_Color[0] = (L_Color[0] + (random(255) + 32)) % 256,
                                   L_Color[1] = (L_Color[1] + (random(255) + 32)) % 256,
                                   L_Color[2] = (L_Color[2] + (random(255) + 32)) % 256,
                                   L_Color[3] = (L_Color[3] + (random(255) + 32)) % 256
                                 );
  }
}

void    Set_Random_Sequence_Colors32_Growing(uint16_t L_ColorsCount) {
  uint16_t L_Color[4];
  uint16_t L_Offset, L_Step;


  for (L_Offset = 0; L_Offset < L_ColorsCount; L_Offset++) {
    L_Step = ((255 / L_ColorsCount) * L_Offset) / 2;
    G_Color32_Sequence1[L_Offset] = pannelA.Color(
                                   L_Color[0] = (random(L_Step) + L_Step) % 256,
                                   L_Color[1] = (random(L_Step) + L_Step) % 256,
                                   L_Color[2] = (random(L_Step) + L_Step) % 256,
                                   L_Color[3] = (random(L_Step) + L_Step) % 256
                                 );
    Serial.print(L_Offset);
    //Print_Padded_RGBW8_Split("", L_Color[0], L_Color[1], L_Color[2], L_Color[3]);
    Serial.println();

  }
}




// | //=================================================================================================================================// | //
// | // \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ = / \ // | // 
// | // []  END _Set []                                                             // | // 
// | // / = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ = \/ =// | // 
// | //=================================================================================================================================// | //







// | //=============================================================================================================================// | //
// | // ? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿?// | //
// | // [] START _Get []                                                          // | // 
// | // ? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿?// | //
// | //=============================================================================================================================// | //


float Get_float_rand( float L_min, float L_max )
{
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return L_min + scale * ( L_max - L_min );      /* [min, max] */
}

uint32_t Get_Brightness_FROM_Color32(uint32_t L_Color32_In, float L_Brightness){
  uint8_t L_Color8_In[4], L_Color8_Out[4];
   
  for(int n = 0; n < 4; n++){
    L_Color8_In[n] = Get_uint8_FROM_uint32(L_Color32_In, n);
    L_Color8_Out[n] = L_Color8_In[n] * L_Brightness;
  }
  return Get_Color32_FROM_4Color8(L_Color8_Out);
}

uint32_t Get_Masked_Color32_FROM_Color32(uint32_t L_Color32_Input){
  uint8_t L_Color8_Input[4], L_Color8_Output[4];
    double  L_ColorMask[4] = {
    0.25,//(((G_Index>>1) % 10) / 10),
    0.25,//(((G_Index>>2) % 10) / 10),
    0.25,//(((G_Index>>3) % 10) / 10),
    0.25//(((G_Index>>4) % 10) / 10)
    };
    
  for(int n = 0; n < 4; n++){
    L_Color8_Input[n] = Get_uint8_FROM_uint32(L_Color32_Input, n);
    L_Color8_Output[n] = L_Color8_Input[n] * (L_ColorMask[n] * G_ColorChannel_Enabled[n]);
  }
  return pannelA.Color(L_Color8_Output[0], L_Color8_Output[1], L_Color8_Output[2], L_Color8_Output[3]);
  
}

uint8_t Get_Random_4BoolArray(bool L_AllowAllFalse){
    uint8_t L_OutOf = 14;
    uint8_t BoolStacked, n, L_Output4[4];
    if (L_AllowAllFalse == true){L_OutOf += 1;}
    BoolStacked = random(L_OutOf) + 1;
    for(n = 0; n < 4; n++){      L_Output4[n] = ((BoolStacked >> n) % 2);      }
    return L_Output4[4];
}


uint32_t Get_Random_Colors32Sequence_Growing(uint16_t L_ColorsCount, uint16_t L_Offset_Output) {
  uint16_t L_Color8[4];
  uint16_t L_Offset, L_Step;
  uint16_t L_Color_Min = 0;
  uint16_t L_Color_Max = 255;
  uint16_t L_Color_Range =  L_Color_Max - L_Color_Min;
  uint16_t L_Sequence_Offset;
  uint32_t L_Output32[64];
  //Serial.println("Generated New Sequence");
  
  L_Step = (L_Color_Range / L_ColorsCount);
  for(L_Sequence_Offset = 0; L_Sequence_Offset < L_ColorsCount; L_Sequence_Offset++){
    L_Color8[0] = LimitValue((L_Step * (L_Sequence_Offset + 1)), 0, 255, 2); 
    //L_Color8[1] = LimitValue((L_Step * (L_Sequence_Offset + 1)), 0, 255, 2); 
    //L_Color8[2] = LimitValue((L_Step * (L_Sequence_Offset + 1)), 0, 255, 2); 
    //L_Color8[3] = LimitValue((L_Step * (L_Sequence_Offset + 1)), 0, 255, 2); 
    
    L_Output32[L_Sequence_Offset] = pannelA.Color(L_Color8[0], L_Color8[1], L_Color8[2], L_Color8[3]);
    
    //Print_Padded_Value(L_Sequence_Offset, 2, false);
    //Print_Padded_RGBW8_Split("", L_Color8[0], L_Color8[1], L_Color8[2], L_Color8[3]);
    //Serial.println();
  }
  
  return L_Output32[L_Offset_Output];

  

}


uint8_t Get_Random_ColorChannel_Enabled(uint8_t L_Chances){
  uint16_t  L_Output[4];
  uint16_t  L_Channel;
  for (L_Channel = 0; L_Channel < 4; L_Channel++) {
    if(random(L_Chances) == 1){L_Output[L_Channel] = 1;}
    else{L_Output[L_Channel] = 0;}
  }
  return L_Output[4];
}


uint32_t Get_ConvertTime(uint32_t L_Input, uint8_t L_InputUnit, uint8_t L_OutputUnit){
  //Unit Type 0 = Microsecond   
  //Unit Type 1 = Milisecond
  //Unit Type 2 = Second
  uint32_t L_TypeStep;
  uint32_t L_Input_InMicro;
  uint32_t L_OutputInMicro;
  uint32_t L_Output;
  L_Input_InMicro   =   (L_Input          *   (L_InputUnit * L_TypeStep));
  if(L_OutputUnit == 0){L_Output = L_Output;}
  else{L_Output  =   (L_Input_InMicro  /   (L_OutputUnit * L_TypeStep));}
  return L_Output;
  
}


uint32_t Get_Color32_Modify(uint32_t L_Color32_Input, uint16_t L_InputModifier[4]){
  uint8_t L_Color8_Input[4];
  uint8_t L_Color8_Output[4];
  uint16_t L_Color32_Output;
  for(int n = 0; n < 4; n++){
    L_Color8_Input[n] = Get_uint8_FROM_uint32(L_Color32_Input, n);
    L_Color8_Output[n] = LimitValue((L_Color8_Input[n]  + L_InputModifier[n]), 0, 255, 0);
  }
  L_Color32_Output = pannelA.Color(L_Color8_Output[0],L_Color8_Output[1], L_Color8_Output[2],L_Color8_Output[3]);
  return L_Color32_Output;
}




//Current
uint16_t Get_Effect_StepForward (uint16_t L_Pixel_Input, uint16_t L_Step[2], uint16_t L_StartingOffset[2]){
  uint16_t L_StepActual = ((L_Step[0] + L_StartingOffset[0]) + (L_Step[1] * C_PixelsPerX) + (L_StartingOffset[1] * C_PixelsPerX)) % C_Pixel_Total;
  uint16_t L_Pixel_Output = (L_Pixel_Input + L_StepActual) % C_Pixel_Total;
  //Print_ValName("StepForward Input", L_Pixel_Input, 2, true);
  //Print_ValName("StepForward StepAct", L_StepActual, 2, true);
  //Print_ValName("StepForward Output", L_Pixel_Output, 2, true);
  return L_Pixel_Output;
}


  //OLD
uint16_t Get_PixelActual_StepForward(uint16_t L_PixelActual_Input, uint16_t L_Step[2], uint16_t L_Offset[2], uint16_t L_LimitType) {
  L_PixelActual_Input = (G_Pixel_Offset + L_PixelActual_Input) % C_Pixel_Total;
  uint16_t L_PixelAct_InputXY[2];
    L_PixelAct_InputXY[0] = Get_Pixel_XY_From_Pixel_Actual(L_PixelActual_Input, 0);
    L_PixelAct_InputXY[1] = Get_Pixel_XY_From_Pixel_Actual(L_PixelActual_Input, 1);
  uint16_t L_StepActual = (
      ( L_Offset[0] + (L_Step[0])  +  
        (L_Offset[1] + (L_Step[1] * C_PixelsPerX) )
      )
    );
  
  uint16_t L_PixelActual_Output = (L_StepActual + L_PixelActual_Input) % G_Pixel_Max;
  
  uint16_t L_PixelAct_OutputXY[2];
  L_PixelAct_OutputXY[0] = Get_Pixel_XY_From_Pixel_Actual(L_PixelActual_Input, 0);
  L_PixelAct_OutputXY[1] = Get_Pixel_XY_From_Pixel_Actual(L_PixelActual_Input, 1);
  
  Print_ValName("L_PixelActual_Input", L_PixelActual_Input, 2, true);
  Print_ValName("L_PixelAct_InputX", L_PixelAct_InputXY[0], 2, true);
  Print_ValName("L_PixelAct_InputY", L_PixelAct_InputXY[1], 2, true);
  Print_ValName("L_StepX", L_Step[0], 2, true);
  Print_ValName("L_StepY", L_Step[1], 2, true);
  Print_ValName("L_StepActual", L_StepActual, 2, true);
  Print_ValName("L_OffsetX", L_Offset[0], 2, true);
  Print_ValName("L_OffsetY", L_Offset[1], 2, true);
  Print_ValName("L_LimitType", L_LimitType, 2, true);
  Print_ValName("L_PixelAct_OutputX", L_PixelAct_OutputXY[0], 2, true);
  Print_ValName("L_PixelAct_OutputY", L_PixelAct_OutputXY[1], 2, true);
  Print_ValName("L_PixelActual_Output", L_PixelActual_Output, 2, true);
  
  L_PixelActual_Output = L_PixelActual_Output % 64;

  
  return L_PixelActual_Output;
}

void Update_G_Pixel_Actual_Transform(uint16_t L_Transform_Offset, uint16_t L_Transform_Type, uint16_t L_Transform_Symmetry[4], uint32_t L_Color32, uint16_t L_BlendType) {

  uint16_t    L_InputXY[2], L_Calc[2], L_Pixel_Output, L_Type_Length;
  uint16_t    L_ShowEachPixel = 1;
  uint32_t    L_Color32_Actual;
//  L_Transform_Symmetry = L_Transform_Symmetry % 4;    //Limit Symmetry Input to 0-3
  //L_Transform_Type = L_Transform_Type % 5;      //Limit Transform Type Input to 0-3

  


  //Print_ValName("TransformInputX", L_InputXY[0], 2, true);
  //Print_ValName("TransformInputY", L_InputXY[1], 2, true);
  //Col = X
  //Row = Y

  //Rotation+90 //==============================================
  if (G_Effect_TransformType == 0) {
    //Limit Transform Offset to Type Length
    L_Type_Length = 64;
    L_Transform_Offset = L_Transform_Offset % L_Type_Length;
    L_InputXY[0] = Get_Pixel_XY_From_Pixel_Actual(L_Transform_Offset, 0);
    L_InputXY[1] = Get_Pixel_XY_From_Pixel_Actual(L_Transform_Offset, 1);
    
      if (L_Transform_Symmetry[0] == 1) {
        L_Calc[0] = ( 0                 +   L_InputXY[0] );
        L_Calc[1] = ( 0                 +    L_InputXY[1] );
        
        L_Color32_Actual = Get_ShiftColor_FROM_Color32(L_Color32, 0, 0);
        Set_SetPixelColor_From_Input(Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]), L_Color32_Actual, L_BlendType); 
        if(L_ShowEachPixel == 1){          Update_ShowPannels(G_Pannels_Enabled);          Update_G_Index_Step();        }
      }
      if (L_Transform_Symmetry[1] == 1) {
        L_Calc[0] = ( (C_PixelsPerX - 1)  -   L_InputXY[1] );
        L_Calc[1] = ( 0                  +    L_InputXY[0] );
        
        L_Color32_Actual = Get_ShiftColor_FROM_Color32(L_Color32, 1, 0);
        Set_SetPixelColor_From_Input(Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]), L_Color32_Actual, L_BlendType);
        if(L_ShowEachPixel == 1){          Update_ShowPannels(G_Pannels_Enabled);          Update_G_Index_Step();        }
      }
      if (L_Transform_Symmetry[2] == 1) {
        L_Calc[0] = ( (C_PixelsPerX - 1)  -   L_InputXY[0] );
        L_Calc[1] = ( (C_PixelsPerY - 1)  -   L_InputXY[1] );
        
        L_Color32_Actual = Get_ShiftColor_FROM_Color32(L_Color32, 2, 0);
        Set_SetPixelColor_From_Input(Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]), L_Color32_Actual, L_BlendType);
        if(L_ShowEachPixel == 1){          Update_ShowPannels(G_Pannels_Enabled);          Update_G_Index_Step();        }
      }
      if (L_Transform_Symmetry[3] == 1) {
        L_Calc[0] = ( 0                 +   L_InputXY[1] );
        L_Calc[1] = ( (C_PixelsPerY - 1)-   L_InputXY[0] );
        
        L_Color32_Actual = Get_ShiftColor_FROM_Color32(L_Color32, 3, 0);
        Set_SetPixelColor_From_Input(Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]), L_Color32_Actual, L_BlendType);
        if(L_ShowEachPixel == 1){          Update_ShowPannels(G_Pannels_Enabled);          Update_G_Index_Step();        }
      }
  }
/*

  //MirrorX_MirrorY //==============================================
  if (G_Effect_TransformType == 1) {
 //Limit Transform Offset to Type Length
    L_Type_Length = 64;
    L_Transform_Offset = L_Transform_Offset % L_Type_Length;
    L_InputXY[0] = Get_Pixel_XY_From_Pixel_Actual(L_Transform_Offset, 0);
    L_InputXY[1] = Get_Pixel_XY_From_Pixel_Actual(L_Transform_Offset, 1);
    
      if (L_Transform_Symmetry == 0) {
        L_Calc[0] = ( 0                 +    L_InputXY[0] );
        L_Calc[1] = ( 0                 +   L_InputXY[1] );
        L_Pixel_Output = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);
      }
      if (L_Transform_Symmetry == 1) {
        L_Calc[0] = ( (C_PixelsPerX - 1)  -    L_InputXY[0] );
        L_Calc[1] = ( 0                 +   L_InputXY[1] );
        L_Pixel_Output = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);
      }
      if (L_Transform_Symmetry == 2) {
        L_Calc[0] = ( (C_PixelsPerX - 1)  -    L_InputXY[0] );
        L_Calc[1] = ( (C_PixelsPerY - 1)  -   L_InputXY[1] );
        L_Pixel_Output = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);
      }
      if (L_Transform_Symmetry == 3) {
        L_Calc[0] = ( 0                 +    L_InputXY[0] );
        L_Calc[1] = ( (C_PixelsPerY - 1)  -   L_InputXY[1] );
        L_Pixel_Output = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);
      }
    }


  //MirrorX_MirrorY_FlipY ?? //==============================================
  if (G_Effect_TransformType == 2) {
//Limit Transform Offset to Type Length
    L_Type_Length = 64;
    L_Transform_Offset = L_Transform_Offset % L_Type_Length;
    L_InputXY[0] = Get_Pixel_XY_From_Pixel_Actual(L_Transform_Offset, 0);
    L_InputXY[1] = Get_Pixel_XY_From_Pixel_Actual(L_Transform_Offset, 1);
    
      if (L_Transform_Symmetry == 0) {
        L_Calc[0] = ( 0                 +    L_InputXY[0] );
        L_Calc[1] = ( 0                 +   L_InputXY[1] );
        L_Pixel_Output = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);
      }
      if (L_Transform_Symmetry == 1) {
        L_Calc[0] = ( (C_PixelsPerX - 1)  -    L_InputXY[0] );
        L_Calc[1] = ( 0                 +   L_InputXY[1] );
        L_Pixel_Output = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);
      }
      if (L_Transform_Symmetry == 2) {
        L_Calc[0] = ( (C_PixelsPerX - 1)  -    L_InputXY[1] );
        L_Calc[1] = ( (C_PixelsPerY - 1)  -   L_InputXY[0] );
        L_Pixel_Output = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);
      }
      if (L_Transform_Symmetry == 3) {
        L_Calc[0] = ( 0                 +    L_InputXY[1] );
        L_Calc[1] = ( (C_PixelsPerY - 1)  -   L_InputXY[0] );
        L_Pixel_Output = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);
      }
    }


  //Tile //==============================================
  if (G_Effect_TransformType == 3) {
        //Limit Transform Offset to Type Length
    L_Type_Length = 64;
    L_Transform_Offset = L_Transform_Offset % L_Type_Length;
    L_InputXY[0] = Get_Pixel_XY_From_Pixel_Actual(L_Transform_Offset, 0);
    L_InputXY[1] = Get_Pixel_XY_From_Pixel_Actual(L_Transform_Offset, 1);
      
      if (L_Transform_Symmetry == 0) {
        L_Calc[0] = ( 0                 +    L_InputXY[0] ) % C_PixelsPerX;
        L_Calc[1] = ( 0                 +   L_InputXY[1] ) % C_PixelsPerY;
        L_Pixel_Output = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);
      }
      if (L_Transform_Symmetry == 1) {
        L_Calc[0] = ( (C_PixelsPerX / 2)  +    L_InputXY[0] ) % C_PixelsPerX;
        L_Calc[1] = ( 0                 +   L_InputXY[1] ) % C_PixelsPerY;
        L_Pixel_Output = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);
      }
      if (L_Transform_Symmetry == 2) {
        L_Calc[0] = ( (C_PixelsPerX / 2)  +    L_InputXY[0] ) % C_PixelsPerX;
        L_Calc[1] = ( (C_PixelsPerX / 2)  +   L_InputXY[1] ) % C_PixelsPerY;
        L_Pixel_Output = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);
      }
      if (L_Transform_Symmetry == 3) {
        L_Calc[0] = ( 0                 +    L_InputXY[0] ) % C_PixelsPerX;
        L_Calc[1] = ( (C_PixelsPerX / 2)  +   L_InputXY[1] ) % C_PixelsPerY;
        L_Pixel_Output = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);
      }
    }
  
//Top and Bottom Borders //==============================================
  if (G_Effect_TransformType == 4) {
        //Limit Transform Offset to Type Length
    L_Type_Length = C_PixelsPerX*2;
    L_Transform_Offset = L_Transform_Offset % L_Type_Length;
    
      
      if (L_Transform_Symmetry == 0) {
        if(L_Transform_Offset > (C_PixelsPerX*1)){       L_Pixel_Output = Get_Pixel_Actual_From_XY(L_Transform_Offset, 0);                   }
        if(L_Transform_Offset > (C_PixelsPerX*2)){       L_Pixel_Output = Get_Pixel_Actual_From_XY(L_Transform_Offset, C_PixelsPerY);        }
      }
      
      if (L_Transform_Symmetry == 1) {
        L_Calc[0] = ( (C_PixelsPerX / 2)  +    L_InputXY[0] ) % C_PixelsPerX;
        L_Calc[1] = ( 0                 +   L_InputXY[1] ) % C_PixelsPerY;
        L_Pixel_Output = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);
      }
      if (L_Transform_Symmetry == 2) {
        L_Calc[0] = ( (C_PixelsPerX / 2)  +    L_InputXY[0] ) % C_PixelsPerX;
        L_Calc[1] = ( (C_PixelsPerX / 2)  +   L_InputXY[1] ) % C_PixelsPerY;
        L_Pixel_Output = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);
      }
      if (L_Transform_Symmetry == 3) {
        L_Calc[0] = ( 0                 +    L_InputXY[0] ) % C_PixelsPerX;
        L_Calc[1] = ( (C_PixelsPerX / 2)  +   L_InputXY[1] ) % C_PixelsPerY;
        L_Pixel_Output = Get_Pixel_Actual_From_XY(L_Calc[0], L_Calc[1]);
      }
      */
   
}



uint32_t Get_Masked_Color32_FROM_Color32(){}



uint32_t Get_ShiftColor_FROM_Color32(uint32_t L_Color32_Input, uint16_t L_ShiftOffset, uint16_t L_ShiftType){
  uint8_t L_Color8_Input[4], L_Color8_Output[4];
  uint32_t L_Color32_Output;
  uint8_t L_Shift0,L_Shift1,L_Shift2,L_Shift3;
  
  //Takes the uint32_t (L_Color32_Input) and converts it to 4 uint8_t values in an array (Color8_Input[4])
  for(int n = 0; n < 4; n++){  L_Color8_Input[n] = Get_uint8_FROM_uint32(L_Color32_Input, n); } 

  //Calculate Shift Offset
  L_Shift0 = (0 + L_ShiftOffset) % 4;
  L_Shift1 = (1 + L_ShiftOffset) % 4;
  L_Shift2 = (2 + L_ShiftOffset) % 4;
  L_Shift3 = (3 + L_ShiftOffset) % 4;

  //Set Output8 to Shifted Channels
  L_Color8_Output[0] = L_Color8_Input[L_Shift0];
  L_Color8_Output[1] = L_Color8_Input[L_Shift1];
  L_Color8_Output[2] = L_Color8_Input[L_Shift2];
  L_Color8_Output[3] = L_Color8_Input[L_Shift3];

  //Convert Output8 to Output32
  L_Color32_Output = Get_Color32_FROM_4Color8(L_Color8_Output);

  //Return Final Result;
  return L_Color32_Output;
}




uint32_t Get_Color32_FROM_4Color8(uint8_t L_Color8_Input[4]){
  return ((uint32_t)L_Color8_Input[3] << 24) | ((uint32_t)L_Color8_Input[0] << 16) | ((uint32_t)L_Color8_Input[1] <<  8) | L_Color8_Input[2];
}


uint16_t Get_Pixel_X_From_Pixel_Actual(uint16_t L_Pixel_Actual) {
  return (L_Pixel_Actual % C_PixelsPerX);
  }

uint16_t Get_Pixel_Y_From_Pixel_Actual(uint16_t L_Pixel_Actual) {
  return (L_Pixel_Actual / C_PixelsPerX);
  }

uint16_t Get_Pixel_XY_From_Pixel_Actual(uint16_t L_Pixel_Actual, uint16_t Dimension) {
  if (Dimension == 0) {    return (L_Pixel_Actual % C_PixelsPerY);  }
  if (Dimension == 1) {    return (L_Pixel_Actual / C_PixelsPerX);  }
  ;
}

uint16_t Get_Pixel_Actual_From_XY(uint16_t L_InputX, uint16_t L_InputY) {
  return (L_InputX + (L_InputY * C_PixelsPerY));
}

uint32_t Get_ColorStep_Color32(uint32_t L_Color32_Input, uint16_t L_Step[4]) {
  uint16_t L_Input_Color8[4];
  uint16_t L_Output_Color8[4];
  uint16_t L_Channel;
  //Convert Input to uint8 array                                                          //Set Output to step setting
  for (L_Channel = 0; L_Channel < 4; L_Channel++) {
    L_Input_Color8[L_Channel] = Get_uint8_FROM_uint32(L_Color32_Input, L_Channel);
    L_Output_Color8[L_Channel] = L_Input_Color8[L_Step[L_Channel]];
  }
  return pannelA.Color(L_Output_Color8[0], L_Output_Color8[1], L_Output_Color8[2], L_Output_Color8[3]);
}

uint16_t Get_r_FROM_uint32(uint32_t Input) {
  G_Color8_Output[0] = Input >> 16;
  return G_Color8_Output[0];
}
uint16_t Get_g_FROM_uint32(uint32_t Input) {
  G_Color8_Output[1] = Input >> 8;
  return G_Color8_Output[1];
}
uint16_t Get_b_FROM_uint32(uint32_t Input) {
  G_Color8_Output[2] = Input >> 0;
  return G_Color8_Output[2];
}
uint16_t Get_w_FROM_uint32(uint32_t Input) {
  G_Color8_Output[3] = Input >> 24;
  return G_Color8_Output[3];
}

uint16_t Get_uint8_FROM_uint32(uint32_t Input, uint16_t Channel)  {
//Get Indivigual Color Channel in array form from uint32
  uint8_t Output;
  if (Channel == 0) {
    Output = Input >> 16;
  }
  if (Channel == 1) {
    Output = Input >> 8;
  }
  if (Channel == 2) {
    Output = Input >> 0;
  }
  if (Channel == 3) {
    Output = Input >> 24;
  }
  return Output;
}



uint32_t Get_Random_Color32_WChannels(uint16_t L_ColorDepth, uint16_t L_Channel_Mask[4]) {
  L_ColorDepth = LimitValue(L_ColorDepth, 0, 255, 0);
  uint16_t L_Multiplier = 256 / L_ColorDepth;
  uint16_t L_Color[4];
  for (int n = 0; n < 4; n++) {
    L_Channel_Mask[n] = LimitValue(L_Channel_Mask[n], 0, 255, 0);
    L_Color[n] = LimitValue(map( (random(L_ColorDepth) * L_Multiplier ) - 1, 0, 255, 0, L_Channel_Mask[n]), 0, 255, 2);
  }
  return  pannelA.Color(L_Color[0], L_Color[1], L_Color[2], L_Color[3]);
}

uint32_t Get_Random_Color32() {
  return pannelA.Color(    random(255),     random(255),     random(255),     random(255));
}

uint32_t Get_Random_Color32_Limit(uint16_t L_LimitR, uint16_t L_LimitG, uint16_t L_LimitB, uint16_t L_LimitW) {
  return pannelA.Color(    random(L_LimitR),     random(L_LimitG),     random(L_LimitB),     random(L_LimitW));
}

int Get_nDigits(int Input) {
  return floor(log10(abs(Input))) + 1;
}


int LimitValue(int L_ValueInput, int L_ValueMin, int L_ValueMax, uint16_t L_Type) {

  int L_Type_Count = 3;                                           //Set to the total number of types to limit the L_Type input to the proper range
  if (L_Type >  L_Type_Count) {
    L_Type = L_Type % L_Type_Count; //Limits the L_Type Input to how many types there actually are
  }

  int L_ValueMin_Offset   =   0;
  int L_ValueMax_Offset   =   L_ValueMax    - L_ValueMin;
  int L_ValueInput_Offset =   L_ValueInput  - L_ValueMin;
  int L_ValueOutput, L_ValueOutput_Offset;

  /////////////////////////////////////
  //+-------------------------------+//
  //|Types                          |//
  //|-------------------------------|//
  //|0 = Loop (Default)             |//
  //|1 = ZigZag [IN PROGRESS]       |//
  //|2 = TopAndBottom               |//
  //|                               |//
  //|                               |//
  //+-------------------------------+//
  /////////////////////////////////////


  //Type = TopAndBottom
  if (L_Type = 1) {
    if (       L_ValueInput_Offset > L_ValueMax_Offset)    {
      L_ValueOutput  =   L_ValueMax;
    }
    else if (  L_ValueInput_Offset < L_ValueMin_Offset)    {
      L_ValueOutput  =   L_ValueMin;
    }
    else                                                  {
      L_ValueOutput  =   L_ValueInput;
    }
  }

  //Type = ZigZag
  else if (L_Type = 1) {
    Serial.println("LimitType 1 (ZigZag) is not completed");
  }

  //Type = Loop (Default)
  else {
    if (L_ValueInput_Offset > L_ValueMax_Offset) {
      L_ValueOutput_Offset = L_ValueInput_Offset % (L_ValueMax_Offset + 1);
    }
    if (L_ValueInput_Offset < L_ValueMin_Offset) {
      L_ValueOutput_Offset = L_ValueInput_Offset % (L_ValueMax_Offset + 1);
      L_ValueInput_Offset = L_ValueMax_Offset - (0 - L_ValueInput_Offset);
    }
    L_ValueOutput = L_ValueOutput_Offset + L_ValueMin;
  }
  return L_ValueOutput;
}


int Get_DelayRemaining(){
  Update_Effect_Timer();
  int L_OutputDelay;
  int L_RunningFor =  (G_EffectTimer_Current[0] - G_EffectTimer_Last[0]);
  
  if(L_RunningFor > C_Delay_Step[0])    { L_OutputDelay = 0;}
  else                                { L_OutputDelay = C_Delay_Step[0] - L_RunningFor;  }
  
  bool L_PrintStats_Local = true;
  if(L_PrintStats_Local == true){
    //if(C_PrintStats_Debug == true){
      Print_ValName("Running:",L_RunningFor, 8, true);
      Print_ValName("DelayTarget", C_Delay_Step[0], 8, true);
      Print_ValName("DelayAdd", L_OutputDelay, 8, true);
      Serial.println();
    //}
  }
  return L_OutputDelay;
}

// | //=============================================================================================================================// | //
// | // ? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿?// | //
// | // []  END  _Get []                                                          // | // 
// | // ? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿? ¿?// | //
// | //=============================================================================================================================// | //


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//Misc



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



//Print Stuff

void Print_DebugMonitor_MainLoop() {
  //Defaults


  //Serial
  if (C_DebugMonitor_Serial_Enabled == true) {
    Print_DebugMonitor_Serial();
    
  }

  //Stick
  if (C_DebugMonitor_Stick_Enabled == true)  {
    Print_DebugMonitor_Stick();
    stick.show();
  }
/*
  
  //Screen
  if (C_DebugMonitor_Screen_Enabled == true) {
    display.clearDisplay();
    Print_DebugMonitor_Screen_Graph();
    Print_DebugMonitor_Screen_ColorChannels();
    Print_DebugMonitor_Screen();
    display.display();
  }
  else {
    Print_Screen_Disabled();
  }
  */


}





void Print_DebugMonitor_Serial() {
  Serial.print(G_EffectRunning_Name);
  Print_Padded_XofY(          "Index",      G_Index,        C_Index_Max, 4, true);
  
  //Print_ValName(            "Px Actual",  G_Pixel_Actual,                2, true);
  //Print_Padded_XofY(                    "Px Offset",  G_Pixel_Offset, (G_Pixel_Offset_Max * G_Effect_LengthModifier),                2, true);
  //Print_ValName(            "StepX",    G_Effect_Step[0],              2, true);
  //Print_ValName(            "StepY",    G_Effect_Step[1],              2, true);
  Print_ValName(            "TransformType",    G_Effect_TransformType,            1, false);
  Serial.print("( TransformSymmetry ");
  Serial.print(G_Effect_Symmetry[0]);
  Serial.print(G_Effect_Symmetry[1]);
  Serial.print(G_Effect_Symmetry[2]);
  Serial.print(G_Effect_Symmetry[3]);
  Serial.print(" ) ");

  Serial.print("(Brightness ");
  Serial.print(G_Brightness_Actual);
  Serial.print(" Target ");
  Serial.print(G_Brightness_FaderMaster);
  Serial.print(") ");
  //Print_Padded_RGBW8_Split("Mask", G_Color_Mask[0], G_Color_Mask[1], G_Color_Mask[2], G_Color_Mask[3]);
  //Print_Padded_RGBW8_Split("Channel", G_ColorChannel_Enabled[0], G_ColorChannel_Enabled[1], G_ColorChannel_Enabled[2], G_ColorChannel_Enabled[3]);
  //Print_Padded_RGBW8_Split("Resolution", G_Color_Resolution[0], G_Color_Resolution[1], G_Color_Resolution[2], G_Color_Resolution[3]);
  //Serial.println();
}



//-----===== Print Data to Screen Functions =====-----//
void Print_DebugMonitor_Screen_DataBar(  uint16_t DataBar_Input, uint16_t DataBar_Input_Min, uint16_t DataBar_Input_Max, uint16_t DataBar_RowNumber, uint16_t DataBar_DrawMargins[2], uint16_t DataBar_DrawSize[2]) {


  int DataBar_Size[2] = {C_Screen_ColWidth - DataBar_DrawMargins[0] - DataBar_DrawMargins[1], DataBar_DrawSize[1]};
  int DataBar_Location[2] = {DataBar_DrawMargins[0] , (DataBar_RowNumber * C_Screen_RowHeight)};

  int DataBar_Cursor_Row;
  int DataBar_Cursor_Col;

  DataBar_Cursor_Row = DataBar_RowNumber * C_Screen_RowHeight;

  int DataBar_Input_Mapped = map(DataBar_Input, DataBar_Input_Min, DataBar_Input_Max, 0, DataBar_Size[0]);

  for (int Width = 0; Width < DataBar_DrawSize[0]; Width++) {
    int DataBar_Location_Actual[2] = {    DataBar_Location[0] + DataBar_Input_Mapped + Width,    DataBar_Location[1]};
    display.drawFastVLine(DataBar_Location_Actual[0],   DataBar_Location_Actual[1],    DataBar_DrawSize[1], WHITE);
  }
}



void Print_DebugMonitor_Screen() {
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
  display.setCursor(Cursor_Col, Cursor_Row);
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
  display.setCursor(Cursor_Col, Cursor_Row);
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
  display.setCursor(Cursor_Col, Cursor_Row);
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
  display.setCursor(Cursor_Col, Cursor_Row);
  int TimePerCycle = G_EffectTimer_Current[0] - G_EffectTimer_Last[0];
  sprintf(PrintBuffer, "%d ms", TimePerCycle) ;
  display.print(PrintBuffer);
  //DataBar
  DrawMargins[0] = 74;     //Left
  DrawMargins[1] = 0;     //Right
  DrawSize[0]    = 1;     //Width
  DrawSize[1]    = 4;     //Height
  Print_DebugMonitor_Screen_DataBar(TimePerCycle, 0, 200, Screen_Row, DrawMargins, DrawSize);




}




void Print_DebugMonitor_Screen_Graph() {

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
  uint16_t Graph_Border_Thickness[4] = {
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
  if (Graph_InputMapped[0] >= Graph_Grid_Minor2[0]) {
    Graph_InputMapped[0] += 3;
  }
  if (Graph_InputMapped[1] >= Graph_Grid_Minor2[1]) {
    Graph_InputMapped[1] += 3;
  }


  //====Border=====//

  for (ThicknessOffset = 0; ThicknessOffset < Graph_Border_Thickness[0]; ThicknessOffset++) {
    display.drawFastHLine(Graph_Location[0],                                   Graph_Location[1] + ThicknessOffset,                     Graph_Size[0],    WHITE );
  };     //Top
  for (ThicknessOffset = 0; ThicknessOffset < Graph_Border_Thickness[1]; ThicknessOffset++) {
    display.drawFastVLine(Graph_Location[0] + ThicknessOffset,                 Graph_Location[1],                                       Graph_Size[1],    WHITE );
  };     //Left
  for (ThicknessOffset = 0; ThicknessOffset < Graph_Border_Thickness[2]; ThicknessOffset++) {
    display.drawFastHLine(Graph_Location[0],                                   Graph_Location[1] - ThicknessOffset  + Graph_Size[1],    Graph_Size[0],    WHITE );
  };     //Bottom
  for (ThicknessOffset = 0; ThicknessOffset < Graph_Border_Thickness[3]; ThicknessOffset++) {
    display.drawFastVLine(Graph_Location[0] - ThicknessOffset + Graph_Size[0], Graph_Location[1],                                       Graph_Size[1],    WHITE );
  };     //Right



  //=====Major Gridlines
  for (ThicknessOffset = 0; ThicknessOffset < 3; ThicknessOffset++) {
    display.drawFastVLine(Graph_Grid_Minor2[0] + ThicknessOffset - 1,    Graph_LocationActual[1],                        Graph_SizeActual[1], WHITE);
  }
  for (ThicknessOffset = 0; ThicknessOffset < 3; ThicknessOffset++) {
    display.drawFastHLine(Graph_LocationActual[0],                       Graph_Grid_Minor2[1] + ThicknessOffset - 1,      Graph_SizeActual[0], WHITE);
  }


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
  for (Graph_Draw_Offset[0] = Graph_Draw_InputMin[0]; Graph_Draw_Offset[0] < Graph_Draw_InputMax[0] / 2; Graph_Draw_Offset[0]++) {
    for (Graph_Draw_Offset[1] = Graph_Draw_InputMin[1]; Graph_Draw_Offset[1] < Graph_Draw_InputMax[1] / 2; Graph_Draw_Offset[1]++) {

      Graph_Draw_Actual[0] = Graph_Location[0] + Graph_Draw_Offset[0] + Graph_InputMapped[0];
      Graph_Draw_Actual[1] = Graph_Location[1] + Graph_Draw_Offset[1] + Graph_InputMapped[1];
      display.drawPixel(Graph_Draw_Actual[0], Graph_Draw_Actual[1], WHITE    );


    }
  }
  //DrawVector
  display.drawLine(Graph_Draw_Actual[0], Graph_Draw_Actual[1], Graph_Draw_Actual[0] + (G_Pixel_Step_Col * 2), Graph_Draw_Actual[1] + (G_Pixel_Step_Row * 2), WHITE);
}


void Print_DebugMonitor_Screen_ColorChannels() {
  display.setTextSize(2);
  int Location[2] = {40, 30};
  int Size[2] = {28, 32};
  if (G_ColorChannel_Enabled[0] == 1) {
    display.setTextColor(BLACK, WHITE);
    display.setCursor(Location[0], Location[1]);
    display.println("R");
  }
  else {
    display.setTextColor(WHITE, BLACK);
    display.setCursor(Location[0], Location[1]);
    display.println("R");
  }

  if (G_ColorChannel_Enabled[1] == 1) {
    display.setTextColor(BLACK, WHITE);
    display.setCursor(Location[0], Location[1] + 16);
    display.println("G");
  }
  else {
    display.setTextColor(WHITE, BLACK);
    display.setCursor(Location[0], Location[1] + 16);
    display.println("G");
  }

  if (G_ColorChannel_Enabled[2] == 1) {
    display.setTextColor(BLACK, WHITE);
    display.setCursor(Location[0] + 16, Location[1]);
    display.println("B");
  }
  else {
    display.setTextColor(WHITE, BLACK);
    display.setCursor(Location[0] + 16, Location[1]);
    display.println("B");
  }

  if (G_ColorChannel_Enabled[3] == 1) {
    display.setTextColor(BLACK, WHITE);
    display.setCursor(Location[0] + 16, Location[1] + 16);
    display.println("W");
  }
  else {
    display.setTextColor(WHITE, BLACK);
    display.setCursor(Location[0] + 16, Location[1] + 16);
    display.println("W");
  }

  display.drawFastHLine(Location[0] - 1,                  Location[1] - 1,                    Size[0], WHITE);
  //display.drawFastHLine(Location[0]-1,                  Location[1] + (Size[1] / 2) - 1,    Size[0], WHITE);
  display.drawFastHLine(Location[0] - 1,                  Location[1] + Size[1],          Size[0], WHITE);

  display.drawFastVLine(Location[0] - 1,                  Location[1] - 1,                Size[1], WHITE);
  //display.drawFastVLine(Location[0] + (Size[0] / 2) - 1,  Location[1] - 1,                    Size[1], WHITE);
  display.drawFastVLine(Location[0] + Size[0],        Location[1] - 1,                    Size[1], WHITE);

  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
}




void Print_DebugMonitor_Stick() {

  uint16_t ColorChannelsOutput_Pixel[8];
  uint32_t ColorChannelsOutput_Pixel_Color[8];
  byte Low = 1;
  byte High = 32;
  uint16_t Pixel;

  Pixel = 0;
  if (G_ColorChannel_Enabled[0] == 1) {
    ColorChannelsOutput_Pixel_Color[Pixel] = pannelA.Color(High, 0, 0, 0);
  }
  else                            {
    ColorChannelsOutput_Pixel_Color[Pixel] = pannelA.Color(Low, 0, 0, 0);
  }
  stick.setPixelColor(Pixel, ColorChannelsOutput_Pixel_Color[Pixel]);


  Pixel = 1;
  if (G_ColorChannel_Enabled[1] == 1) {
    ColorChannelsOutput_Pixel_Color[Pixel] = pannelA.Color(0, High, 0, 0);
  }
  else                            {
    ColorChannelsOutput_Pixel_Color[Pixel] = pannelA.Color(0, Low, 0, 0);
  }
  stick.setPixelColor(Pixel, ColorChannelsOutput_Pixel_Color[Pixel]);


  Pixel = 2;
  if (G_ColorChannel_Enabled[2] == 1) {
    ColorChannelsOutput_Pixel_Color[Pixel] = pannelA.Color(0, 0, High, 0);
  }
  else                                {
    ColorChannelsOutput_Pixel_Color[Pixel] = pannelA.Color(0, 0, Low, 0);
  }
  stick.setPixelColor(Pixel, ColorChannelsOutput_Pixel_Color[Pixel]);


  Pixel = 3;
  if (G_ColorChannel_Enabled[3] == 1) {
    ColorChannelsOutput_Pixel_Color[Pixel] = pannelA.Color(0, 0, 0, High / 4);
  }
  else                                {
    ColorChannelsOutput_Pixel_Color[Pixel] = pannelA.Color(0, 0, 0, Low);
  }
  stick.setPixelColor(Pixel, ColorChannelsOutput_Pixel_Color[Pixel]);

  Pixel = 4;
  ColorChannelsOutput_Pixel_Color[Pixel] = Color32_Test[0];
  stick.setPixelColor(Pixel, ColorChannelsOutput_Pixel_Color[Pixel]);



}




void Print_Screen_Disabled() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Output Monitor Disabled");
  display.display();
}





//////////////////////////////Print Stats to Serial
void Buffer_Text(char TextInput[32]) {
  sprintf(G_Text_Buffer32, "%s ", TextInput);
}

void Buffer_EffectRunning(char TextInput[32]) {
  sprintf(G_EffectRunning_Name, "%s ", TextInput);
}


void Print_Padded_Value(uint32_t Value, byte Output_Length, bool PadWith0) {
  char    PrintBuffer[256]; //= PrintBuffer_Clear(PrintBuffer);
  if (PadWith0 == true) {    sprintf(PrintBuffer, "%0*d", Output_Length, Value);  }
  else                  {    sprintf(PrintBuffer, "%*d",  Output_Length, Value);  };
  Serial.print(PrintBuffer);
}

void Print_ValName(char Title[16], uint32_t Value, byte Output_Length, bool PadWith0) {
  char    PrintBuffer[256]; //= PrintBuffer_Clear(PrintBuffer);
  if (PadWith0 == true) {    sprintf(PrintBuffer, "(%s %0*d) ", Title, Output_Length, Value);  }
  else                  {    sprintf(PrintBuffer, "(%s %*d) ",  Title, Output_Length, Value);  }
  Serial.print(PrintBuffer);
}

void Print_Padded_DecimalL(char Title[16], float Value, byte Output_Length, bool PadWith0) {
  char    PrintBuffer[256]; //= PrintBuffer_Clear(PrintBuffer);
  if (PadWith0 == true) {    sprintf(PrintBuffer, "(%s %4.2e) ", Title, Value);  }
  
  Serial.print(PrintBuffer);
}


void Print_Padded_XofY(char Title[16], uint32_t ValueX, uint32_t ValueY, byte Output_Length, bool PrintPercent) {
  char    PrintBuffer[256];
  double Percent = map(ValueX, 0, ValueY, 0, 100) / 100;
  if (PrintPercent == true) {
    sprintf(PrintBuffer, "(%s %0*d of %0*d %G) ", Title, Output_Length, ValueX, Output_Length, ValueY, Percent);
  }
  //else{                       sprintf(PrintBuffer, "(%s %0*d of %0*d) ", Title, Output_Length, ValueX, Output_Length, ValueY); }


  Serial.print(PrintBuffer);
}

void Print_Padded_4Array(char Title[16], uint16_t Input[4]) {
  char    PrintBuffer[256];
  sprintf(PrintBuffer, "(%s 0 %4.2e 1 %4.2e 2 %4.2e 3 %4.2e ) ", Title, Input[0], Input[1], Input[2], Input[3]);
  Serial.print(PrintBuffer);
}

void Print_Padded_RGBW8(char Title[16], uint8_t Color8_Input[4]) {
  char    PrintBuffer[256];
  sprintf(PrintBuffer, "(%s r %3d g %3d b %3d w %3d ) ", Title, Color8_Input[0], Color8_Input[1], Color8_Input[2], Color8_Input[3]);
  Serial.print(PrintBuffer);
}

void Print_Padded_RGBW8_Split(char Title[16], uint16_t R, uint16_t G, uint16_t B, uint16_t W) {
  char    PrintBuffer[256];
  sprintf(PrintBuffer, "(%s r %3d g %3d b %3d w %3d ) ", Title, R, G, B, W);
  Serial.print(PrintBuffer);
}

void Print_Padded_RGBW32(char Title[16], uint32_t ColorInput32) {
  char    PrintBuffer[256];
  for(int n = 0; n < 4; n++){    G_Color8_PrintBuffer[n] = Get_uint8_FROM_uint32(ColorInput32, n);   }
  sprintf(PrintBuffer, " (%s %3d %3d %3d %3d) ", Title, G_Color8_PrintBuffer[0] , G_Color8_PrintBuffer[1] , G_Color8_PrintBuffer[2] , G_Color8_PrintBuffer[3] );
  Serial.print(PrintBuffer);
}

void Print_Padded_Text(char Text[16], byte OutputLength) {
  char    PrintBuffer[256];

  sprintf(PrintBuffer, "%s", Text );
  Serial.print(PrintBuffer);
}



void Print_Test(bool NewLine) {
  if(NewLine == true){Serial.println("| Test |");}
  else               {Serial.print("| Test |");}
  
}

