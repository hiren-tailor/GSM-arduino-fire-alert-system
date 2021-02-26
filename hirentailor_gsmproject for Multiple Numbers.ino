#include <SoftwareSerial.h>
SoftwareSerial mysim(7,8); // connect gsm modem on this pin
String smstext;

int buz = 9;  //buzzer connected to pin 9
int led = 4;  //led connected to pin 4

const int aqsensor = A1;  //output of mq135 connected to A1 pin of Arduino
int threshold = 250;      //Threshold level for Air Quality

void setup() {
      Serial.begin(9600);
      mysim.begin(9600);
      Serial.println("System is Initializing..");
      pinMode (buz,OUTPUT);     // buzzer is connected as Output from Arduino
      pinMode (led,OUTPUT);     // led is connected as output from Arduino
      pinMode (aqsensor,INPUT); // MQ135 is connected as INPUT to arduino
      digitalWrite(buz, LOW);
      digitalWrite(led, LOW); 
      delay(5000); // wait for 5 seconds
}

void loop() {
      int ppm = analogRead(aqsensor); //read MQ135 analog outputs at A1 and store it in ppm
      Serial.println(ppm);            //print value of ppm in serial monitor           

      if (ppm > threshold)            // check is ppm is greater than threshold or not
      {
      smstext =  "\nFire Detected..!";
      sendSMS(smstext); 
      Serial.println(smstext);    
      digitalWrite(buz,HIGH);     //Turn ON Buzzer
      digitalWrite(led,HIGH);     //Turn ON LED
      delay(5000);
      mysim.println("ATD+YYxxxxxxxxxx;"); //  change ZZ with country code and xxxxxxxxxxx with phone number to dial
      updateSerial();
      Serial.println("Calling  ");
      delay(50000); // wait for 50 seconds...
      mysim.println("ATH"); //hang up
      updateSerial();
      Serial.println("Hangup Call");

    }
     else
    {
      digitalWrite(led,LOW);   //jump here if ppm is not greater than threshold and turn off LED
      digitalWrite(buz,LOW);   //Turn off Buzzer
      Serial.println("No Fire detected");
    }  
  delay (500);
}

void sendSMS(String message)
{
  mysim.print("AT+CMGF=1\r");                     // AT command to send SMS message
  updateSerial();
  delay(1000);
  
  mysim.println("AT + CMGS = \"+YYxxxxxxxxxx\"");  // recipient's mobile number, in international format
  updateSerial();
  delay(1000);
  
  mysim.println(message);                         // message to send
  updateSerial();
  delay(1000);
  
  mysim.println((char)26);                        // End AT command with a ^Z, ASCII code 26
  updateSerial();
  delay(1000); 
  
  mysim.println();
  delay(100);                                     // give module time to send SMS
 
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mysim.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mysim.available()) 
  {
    Serial.write(mysim.read());//Forward what Software Serial received to Serial Port
  }
}
