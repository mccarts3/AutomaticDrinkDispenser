#define shotButton 13
#define shotLED 12
#define wineButton 11
#define wineLED 10
#define standardButton 9
#define standardLED 8
#define liqLevel A0
#define resistor 560
#define pump 2

//0 = wait, 1 = dispensing shot, 2 = dispensing wine, 3 = dispensing standard
int state;    

void setup() {
  pinMode(shotButton, INPUT);
  pinMode(shotLED, OUTPUT);
  pinMode(wineButton, INPUT);
  pinMode(wineLED, OUTPUT);
  pinMode(standardButton, INPUT);
  pinMode(standardLED, OUTPUT);
  pinMode(liqLevel, INPUT);
  pinMode(pump, OUTPUT);
  state = 0;    
  Serial.begin(9600);
}

void loop() {
  while(state == 0) {
     float waterVolt = analogRead(liqLevel);
     float waterRes = (1023 / waterVolt) - 1;
     waterRes = resistor / waterRes;
     
     Serial.print("Water resistance: ");
     Serial.print(waterRes);
     Serial.print("\t");
     digitalWrite(shotLED, LOW);
     digitalWrite(wineLED, LOW);
     digitalWrite(standardLED, LOW);
     Serial.print("Shot: ");
     Serial.print(!digitalRead(shotButton));
     Serial.print("\tWine: ");
     Serial.print(!digitalRead(wineButton));
     Serial.print("\tStandard: ");
     Serial.println(!digitalRead(standardButton));
     delay(100);
     if(!digitalRead(shotButton))
        state = 1;
     else if(!digitalRead(wineButton))
        state = 2;
     else if(!digitalRead(standardButton))
        state = 3;
  }

  if(state == 1) {
    digitalWrite(shotLED, HIGH);
    Serial.println("\npourShot() called");
    pourShot();
    state = 0;
  } else if(state == 2) {
    digitalWrite(wineLED, HIGH);
    Serial.println("\npourWine() called");
    pourWine();
    state = 0;
  } else if(state == 3) {
    digitalWrite(standardLED, HIGH);
    Serial.println("\npourStandard() called");
    pourStandard();
    state = 0;
  }
}

void pourShot() {
  float waterVolt = analogRead(liqLevel);
  float waterRes = (1023 / waterVolt) - 1;
  waterRes = resistor / waterRes;
  float finalRes;
  int count = 0;
    
  if(waterRes > 1900) {
    finalRes = waterRes-200;
  } else {
    finalRes = 1800;
  }
  
  
  while(waterRes > finalRes && count < 3) {
    waterVolt = analogRead(liqLevel);
    waterRes = (1023 / waterVolt) - 1;
    waterRes = resistor / waterRes;
    Serial.print("Water resistance: ");
    Serial.print(waterRes);
    Serial.println();

    digitalWrite(pump, HIGH);
    if(waterRes < finalRes)
      count++;
    delay(200);
  }

  digitalWrite(pump, LOW);
}

void pourWine() {
  float waterVolt = analogRead(liqLevel);
  float waterRes = (1023 / waterVolt) - 1;
  waterRes = resistor / waterRes;
  int count = 0;
  
  //50 is arbitrary amount for liqLevel sensor
  while(waterRes > 1100 && count < 3) {
    waterVolt = analogRead(liqLevel);
    waterRes = (1023 / waterVolt) - 1;
    waterRes = resistor / waterRes;
    Serial.print("Water resistance: ");
    Serial.print(waterRes);
    Serial.println();
    digitalWrite(pump, HIGH);
    if(waterRes < 1100)
      count++;
      
    delay(200);
  }
  
  digitalWrite(pump, LOW);
}

void pourStandard() {
  float waterVolt = analogRead(liqLevel);
  float waterRes = (1023 / waterVolt) - 1;
  waterRes = resistor / waterRes;
  int count = 0;
  
   //100 is arbitrary amount for liqLevel sensor
   while(waterRes > 600 && count < 3) {
    waterVolt = analogRead(liqLevel);
    waterRes = (1023 / waterVolt) - 1;
    waterRes = resistor / waterRes;
    Serial.print("Water voltage: ");
    Serial.print(waterRes);
    Serial.println();
    digitalWrite(pump, HIGH);
    if(waterRes < 600)
      count++;
    delay(200);
   }

   digitalWrite(pump, LOW);
}

