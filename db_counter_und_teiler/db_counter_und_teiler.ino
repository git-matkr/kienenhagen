               int  SensorPortInput   = 4;     //Pin D4
               int  SensorLevel       = LOW;
               bool SensorFlank       = false;
               int  DividerPortoutput = 13;    //Pin D13
               int  DividerLevel      = LOW;
      unsigned int  divider           = 10;
      unsigned int  FlankCounter      = 0;     // Zählt LOW/HIGH Flanken vom Sensoreingang
               bool NewStartCounter   = false;
      unsigned int  Measurement       = 0;     //Zeit an der eine Messperiode beginnt
      unsigned int  SetupWarte        = 1000;  // in ms
      unsigned int  InputTime         = 0;
      unsigned int  WorkTime          = 0;
const unsigned int  min               = 60000;  // ms für eine Minute
const          int  baud              = 19200; 

void setup() //Hier beginnt das Setup.
{
  pinMode(DividerPortoutput,      OUTPUT);
  pinMode( SensorPortInput,       INPUT);
  digitalWrite(DividerPortoutput, DividerLevel);
 





  NewStartCounter = true;
  InputTime       = min/1;
   
  Serial.begin(baud);
  Serial.println("START");
  Serial.print("Flankenzaehler : ");
  Serial.println(FlankCounter);
  Serial.print("MessZeit       : ");
  Serial.println(InputTime);
  Serial.print("ArbeitzeitZeit : ");
  Serial.println(WorkTime);
  Serial.print("Teiler         : ");
  Serial.print(divider);
 
  delay(SetupWarte);
}

void loop(){
  // Sensorereignisse zählen pro MessZeit
  // Teiler an den Ausgang gleich weiter leiten
  if ( NewStartCounter ) {
    NewStartCounter = false;
    Measurement = millis();
  }
 
  SensorLevel = digitalRead(SensorPortInput);
  if ( SensorLevel == LOW and SensorFlank == false ) { // Flankenerkennung LOW HIGH
    SensorFlank = true;
    FlankCounter++;
  } else if ( SensorLevel == HIGH and SensorFlank == true ) {
    SensorFlank = false;
  }
 
  // Ausgang setzen
  if ( FlankCounter % divider) {   // Wenn modulo true ist
    if ( DividerLevel == LOW ){    // Dann Ausgangs Pegel invertieren
      DividerLevel = HIGH;
    } else {
      DividerLevel = LOW;
    }
    digitalWrite(DividerPortoutput, DividerLevel);
  }
    
  // Ausgabe über USB an die Console  Wenn Zeit abgelaufen
  if ( (millis() - Measurement) > InputTime ) {
    NewStartCounter = true;
    Serial.println("Messwert");
    Serial.print("Flankenzaehler : ");
    Serial.println(FlankCounter);
    Serial.print("MessZeit       : ");
    Serial.println(InputTime);
    Serial.print("ArbeitzeitZeit : ");
    Serial.println(WorkTime);
    Serial.print("Teiler         : ");
    Serial.print(divider);
    FlankCounter = 0;
    
  }
    
}
