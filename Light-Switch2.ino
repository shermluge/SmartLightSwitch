/*                         Attiny85 
 *                           ___
 *                  Reset 1-|   |-8 5v
 *     input Audio   a3-3 2-|   |-7 2 Digital in from microwave sensor
 *     input LDR     a2-4 3-|   |-6 1 Digital out for solid state relay
 *                    Gnd 4-|   |-5 0 Digital in from PIR
 *                           ---
 *                           
 *  Smart switch
 * Lic. GPLv3
 *
 * Sherman Stebbins 
 * 10/18/17
 * 
 * uses LDR for obvious light detection, microwave to see movement in room
 * and behind walls and furnature, PIR for regular motion detection, audio in case
 * people motionless but still in room talking.
 * 
 */ 
//#define DEBUG
int pirPin = 0;    //the digital pin connected to the PIR sensor's output
int ledPin = 1;
int capdet = 2; //capacitive detector
int ldr = A2;
int sound = A3;

#ifdef DEBUG
int pirPin = 3;    //the digital pin connected to the PIR sensor's output
int ledPin = 12;
int capdet = 4; //capacitive detector
int ldr = A0;
int sound = A1;
#endif

bool state = LOW;
int ldrstatus = 0;
int pirPinstatus = 0;
int capdetstatus = 0;
int soundstatus = 0;

int minlight = 750;
int maxsound = 150;
int sensorcount = 0;
int sensorcountmax = 6;
unsigned long prevmillis = 0;
unsigned long maxontime = 60000;//30 sec

/////////////////////////////
//SETUP
void setup(){
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(capdet, INPUT);
  pinMode(ldr,INPUT);
  pinMode(sound,INPUT);
//  digitalWrite(pirPin, LOW);
  
#ifdef DEBUG
  //give the sensor some time to calibrate
  Serial.begin(9600);
  Serial.println("calibrating sensor ");
  delay(10000);
  Serial.println("Done ");
#endif
}

void loop(){
  ldrstatus = analogRead(ldr);
  delay(1);
  pirPinstatus = digitalRead(pirPin);
  delay(1);
  capdetstatus = digitalRead(capdet);
  delay(1);
  soundstatus = analogRead(sound);
  delay(1);
     if(ldrstatus < minlight &&(pirPinstatus == HIGH||capdetstatus == HIGH||soundstatus > maxsound)){
        state = HIGH;
        prevmillis = millis();   
        sensorcount++;
        digitalWrite(ledPin,state);
        delay(5000);          
     }

     if(millis()- prevmillis > maxontime){
        if(sensorcount > sensorcountmax){
          sensorcount = sensorcountmax;
        }
        if(sensorcount<=0){
          state = LOW;
        }
        sensorcount--;
        prevmillis = millis();   
     } 
     digitalWrite(ledPin,state); 
     delay(1);   

#ifdef DEBUG
     if(digitalRead(pirPin) == HIGH){
      Serial.println("motion detected on pir");
     }
     if(digitalRead(capdet) == HIGH){
      Serial.println("motion detected on microwave");
     }
     if(analogRead(sound) > 100){
      Serial.println("motion detected on sound");
     }
#endif
}

