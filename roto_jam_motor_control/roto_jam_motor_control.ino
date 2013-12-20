const int analogInPin = 6;  // Analog input pin that the potentiometer is attached to

const int motor1Pin = 2;    //PA2
const int motor2Pin = 3;    //PA3
const int ledPin1 = 0;    //PA0
const int ledPin2 = 1;    //PA1

const int switchPin = 7;    // PA7
boolean switchPressed=false;

unsigned int recording[15];

unsigned int recCount=0;
unsigned long lastMillis=0;
unsigned int sampleTime=1000;

float motorSpeed=.5;

void setup()  
{
  digitalWrite(analogInPin, LOW);

  pinMode(motor1Pin, OUTPUT); 
  pinMode(motor2Pin, OUTPUT); 
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  digitalWrite(switchPin, HIGH);

  for (int i=0; i < (sizeof(recording)/sizeof(int))-1; i++){
    recording[i]=-1;
  }
}

void loop()
{
  if(digitalRead(switchPin)==LOW) {   // button hold down
    record(); 
  }
  else{
    if(switchPressed){
      switchPressed=false; 
      recCount=0;
    }
    play();
  }
}


void record(){

  if(millis()-lastMillis>sampleTime){
    if(recCount>(sizeof(recording)/sizeof(int))-1){
      recCount=0;  
    }

    recording[recCount]=analogRead(analogInPin);
    recCount++;
    lastMillis=millis();

    //flash lights
    digitalWrite(ledPin1, recCount%2);  
    digitalWrite(ledPin2, (recCount+1)%2);  

  }


}


void play(){

  if(recCount>(sizeof(recording)/sizeof(int))-1){
    recCount=0;  
  }
  int pos=recording[recCount];

  if(pos>0){
    spinMotor(pos,true,motorSpeed);
  }

  else{
    runMotor(0,true,0);
    recCount++; 
  }

  if(millis()-lastMillis>sampleTime){
    recCount++;
    lastMillis=millis();


  }
}

void spinMotor(int targetPosition, boolean lightsOn, float sp){
  int currentPosition = analogRead(analogInPin);

  if(currentPosition<targetPosition){
    runMotor(1,lightsOn,sp);
  }

  else if(currentPosition>targetPosition){
    runMotor(-1,lightsOn,sp);
  }



  /*
  int currentPosition = analogRead(analogInPin);
   
   if (currentPosition<targetPosition) {
   int a=1023-targetPosition+currentPosition;
   int b=targetPosition-currentPosition;
   
   if(a<=b+100){
   //clockwise
   runMotor(1,lightsOn,sp);
   }
   else{
   //counterclockwise
   runMotor(-1,lightsOn,sp);
   }
   }
   else if (currentPosition>targetPosition){
   int a=1023-currentPosition+targetPosition;
   int b=currentPosition-targetPosition;
   if(a<=b){
   //counterclockwise
   runMotor(-1,lightsOn,sp); 
   }
   else{
   //clockwise
   runMotor(1,lightsOn,sp);
   }
   } 
   */
}


void runMotor(int c, boolean lightsOn, float sp){
  if(c==1){
    //clockwise
    digitalWrite(motor1Pin, HIGH);   // set leg 1 of the H-bridge low
    digitalWrite(motor2Pin, LOW);  // set leg 2 of the H-bridge high
    if(lightsOn){
      digitalWrite(ledPin1, HIGH);  
      digitalWrite(ledPin2, LOW);  
    }

  }
  else if(c==-1){
    //counterclockwise
    digitalWrite(motor1Pin, LOW);   // set leg 1 of the H-bridge lowa
    digitalWrite(motor2Pin, HIGH);  // set leg 2 of the H-bridge high
    if(lightsOn){
      digitalWrite(ledPin1, LOW);  
      digitalWrite(ledPin2, HIGH);  
    }
  }
  //rest
  else{
    digitalWrite(motor1Pin, LOW);   // set leg 1 of the H-bridge low
    digitalWrite(motor2Pin, LOW);  // set leg 2 of the H-bridge high
    if(lightsOn){
      digitalWrite(ledPin1, LOW);  
      digitalWrite(ledPin2, LOW);
    }
  }

  if(c!=0){
    //fake speed control
    delayMicroseconds(sp*10000.0);
    digitalWrite(motor1Pin, LOW); 
    digitalWrite(motor2Pin, LOW);
    if(lightsOn)digitalWrite(ledPin1, LOW);  
    if(lightsOn)digitalWrite(ledPin2, LOW); 
    delayMicroseconds(11000.0-sp*10000.0);
  }
}










































