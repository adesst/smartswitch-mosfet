/*
  Smart-switch with MOSFET
*/

//see pins_energia.h for more LED definitions
#define LED               P1_0    // 2
#define LOW_BATT_LED      P2_0    // 9
#define CAP1_TOGGLE       P1_4    // 6
#define CAP2_TOGGLE       P2_4    // 12
#define PIN_DISCHARGE     P1_5    // 7
#define VOLT_SENSING      P1_7    // 15 (A7)
#define SENSING_MODE      P1_6    // 14
#define PIN_CHARGING      P2_5    // 13

/*#define BATT_THRESHOLD    540     // 1.6V  => 1.6V / (3.3/1023), 1k-1k*/
#define BATT_THRESHOLD    310     // 1V => 1V / (3.3/1023), 2.2k-1k

boolean b_is_lowbatt = false;
boolean b_is_firsttime = true;
int sensorValue = 0;

// the setup routine runs once when you press reset:
void setup() {

  /* SERIAL MODE
  Serial.begin(9600);  
  */
  
  // initialize the digital pin as an output.
  pinMode(LED, OUTPUT);
  pinMode(PIN_CHARGING,  OUTPUT);
  pinMode(CAP1_TOGGLE,   OUTPUT);
  pinMode(CAP2_TOGGLE,   OUTPUT); 
  pinMode(LOW_BATT_LED,  OUTPUT); 
  pinMode(PIN_DISCHARGE, OUTPUT);
  pinMode(SENSING_MODE,  OUTPUT);

  show_ready();
}

// the loop routine runs over and over again forever:
void loop() {

  if( b_is_lowbatt ){

    low_batt_mode();
    
    return;  // back to loop() 
  }
  
  charge_caps();

  if( b_is_firsttime ){
    
    sleepSeconds(3);
    b_is_firsttime = false;
  }
  else{
    
    sleepSeconds(7200);  // 2hrs
  }
  
  discharge_caps();

  volt_sensing();

  sleepSeconds(20); // the mosfet rest for 20s
}

void show_ready(){

  digitalWrite(SENSING_MODE, LOW);
  digitalWrite(LOW_BATT_LED, LOW);
  
  toggle_pin_with_delay(LED, 1000);
}

void low_batt_mode(){

  toggle_pin_with_delay(LOW_BATT_LED, 1000);

  discharge_caps();
  
  sleepSeconds(3);
}

void volt_sensing(){
  digitalWrite(SENSING_MODE, HIGH);
  sensorValue = analogRead(VOLT_SENSING);
  delay(50);
  digitalWrite(SENSING_MODE, LOW);
    
  /* SERIAL MODE
  Serial.print("sensor = " );                       
  Serial.println(sensorValue);  
  delay(10); // 10ms
  */

  if ( sensorValue <= BATT_THRESHOLD ){

    b_is_lowbatt = true;
  }
}

void discharge_caps(){

  toggle_pin_with_delay(PIN_DISCHARGE, 200);
}

void charge_caps(){

  digitalWrite(PIN_CHARGING, HIGH);
  
  for(int i = 0; i < 10; i++){

    toggle_pin_with_delay(CAP2_TOGGLE, 150);
    toggle_pin_with_delay(CAP1_TOGGLE, 150);
  }

  digitalWrite(PIN_CHARGING, LOW);
}

void toggle_pin_with_delay(int pin, int ms_delay){
  
  digitalWrite(pin, HIGH);
  delay(ms_delay);
  digitalWrite(pin, LOW);
}

