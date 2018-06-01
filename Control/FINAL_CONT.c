#include <FINAL_CONT.h>
//#include <math.h>

#define BUFFER_SIZE 32
BYTE buffer[BUFFER_SIZE];
BYTE next_in = 0;
BYTE next_out = 0;
INT1 write,state;
unsigned int16 value[5];
unsigned int8 i,j;
unsigned int16 readValue ,count;         //value read from the sensor

//unsigned int mVperAmp = 66; // use 100 for 20A Module and 66 for 30A Module
 
float Voltage = 0;
float VRMS = 0;
float AmpsRMS = 0;
float Boost_output, Inverter_output;
int1 batt_status, load_status;
float Data;

/*
#int_rda
void serial_isr() {
   int t;

   buffer[next_in]=getc();
   t=next_in;
   next_in=(next_in+1) % BUFFER_SIZE;
   if(next_in==next_out)
     next_in=t;           // Buffer full !!
     write=1;
}



float getVPP()
{
  float result;

  
  readValue = read_adc();
  Voltage = (readValue * 5.0)/1024;
  //result = Voltage * 0.707
  result = Voltage;
  
  return result;
   }



void get_sample()
{

AmpsRMS = (value[0]*5.0)/1024;
Voltage = (value[2]*5.0)/1024;
VRMS = AmpsRMS * 1.4 ;
Boost_output = value[4] / 2.8;
Inverter_output = Voltage * 55;
Data = VRMS * Inverter_output;
}


void read()                   //fuction read input analog value
{
j=1;                                    // A0   ACS SENSOR
for(i=0; i<5; i++)                      //A1  NTC TEMP
{                                        //A2   BOOST VOLTAGE
 value[i] = Read_adc();                   //A3   NTC TEMP
 set_adc_channel(j);                     //A4   ----
delay_us(100);                          //A5   INVERTER VOLTAGE
 //Read_ADC(ADC_START_ONLY);
 j++;
 if (j==5) j=0;
while (!adc_done());
}
get_sample();
}



void write_status()
{
if (write == 1)
        { 
        delay_ms(50);
        //get_sample();
        printf("%f,%lu,%lu,%f,%f,%u,%u;\n\r", AmpsRMS,value[1],value[3],Boost_output,Inverter_output,batt_status,load_status);
 
        write = 0;
        }
}

*/
void main(){
set_tris_B(0xff);
set_tris_A(0xFF);
set_tris_c(0x80);
//set_tris_c(0x00);
state = 1;
Data = 0;
write = 0;
count = 0;
POWER = 1;
batt_status=load_status=0;

//setup_adc_ports(ALL_ANALOG);
//setup_adc(ADC_CLOCK_DIV_32);
setup_adc(ADC_CLOCK_INTERNAL); // ADC clock
setup_adc_ports(AN0); // Input combination
set_adc_channel(0);

//enable_interrupts(int_rda);
//enable_interrupts(global);

 delay_ms(10);
 
   
 BUZZER = 1;
 delay_ms(2000);
 BUZZER = 0;
  
  OUTPUT_C(0X00);

   while (TRUE) {
   
   POWER = state;

//   read();
readValue = read_adc();

delay_ms(100);
  
  // if ( (Data > 600) && (Data < 800) )
  
   if  ((readValue > 550 ) && (readValue < 662) )
   {

 
 BUZZER = 1;
 //delay_ms(5000);
 count++;
 if (count>= 50)
 {
  state = 1;
 BUZZER = 0;
 count = 0;
 }
  } 
else if (readValue > 662 )
{
POWER = 1; 
state = 1;
count = 0;
}
else
{
count= 0;
 BUZZER = 0;
}
 

if (BUTTON == 1)                      // take to interrupt
   {
   delay_ms(100);
    if (BUTTON == 1)
    {
    state=~state;
     while (BUTTON == 1) ;
    }
   }
   

  
        
   } 

}
