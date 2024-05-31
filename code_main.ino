#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>

#define SS_PIN 10 //RX slave select
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
LiquidCrystal_I2C lcd(0x27,16,2);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

byte card_ID[4]; //card UID size 4byte
byte Name1[4]={0x2E,0xA5,0x61,0xA3};//first UID card
byte Name2[4]={0xF3,0x45,0x96,0x1A};//second UID card


//if you want the arduino to detect the cards only once
int NumbCard[2];//this array content the number of cards. in my case i have just two cards.
int j=0;        

int const RedLed=6;
int const GreenLed=5;
int const Buzzer=8;

String Name;//user name
long Number;//user number
int n ;//The number of card you want to detect (optional)  
float temp1;

void setup() {
//  lcd.begin();
  lcd.backlight();
  mlx.begin();
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();  // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  
  Serial.println("CLEARSHEET");                 // clears starting at row 1
  Serial.println("LABEL,Date,Time,Name,Roll Number,Temprature");// make four columns (Date,Time,[Name:"user name"]line 48 & 52,[Number:"user number"]line 49 & 53)

  pinMode(RedLed,OUTPUT);
  pinMode(GreenLed,OUTPUT);
  pinMode(Buzzer,OUTPUT);

   }
    
void loop() {
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("    Scan Tag    ");
  //look for new card
   if ( ! mfrc522.PICC_IsNewCardPresent()) {
  return;//got to start of loop if there is no card present
 }
 // Select one of the cards
 if ( ! mfrc522.PICC_ReadCardSerial()) {
  return;//if read card serial(0) returns 1, the uid struct contians the ID of the read card.
 }
 
 for (byte i = 0; i < mfrc522.uid.size; i++) {
     card_ID[i]=mfrc522.uid.uidByte[i];

       if(card_ID[i]==Name1[i]){
       Name="Prerana Shete";//user name
       Number=60;//user number
       temp1 = mlx.readObjectTempC();
       j=0;//first number in the NumbCard array : NumbCard[j]
      }
      else if(card_ID[i]==Name2[i]){
       Name="Shruti Pote";//user name
       Number=69;//user number
       temp1 = mlx.readObjectTempC();
       j=1;//Second number in the NumbCard array : NumbCard[j]
      }
      else{
          digitalWrite(GreenLed,LOW);
          digitalWrite(RedLed,HIGH);
          goto cont;//go directly to line 85
     }
}
      if(NumbCard[j] == 1){//to check if the card already detect
      //if you want to use LCD
      //Serial.println("Already Exist");
      }
      else{
      NumbCard[j] = 1;//put 1 in the NumbCard array : NumbCard[j]={1,1} to let the arduino know if the card was detecting 
      n++;//(optional)
      Serial.print("DATA,DATE,TIME," + Name);//send the Name to excel
      Serial.print(",");
      Serial.print(Number); //send the Number to excel
      Serial.print(",");
      Serial.println(temp1);
      digitalWrite(GreenLed,HIGH);
      digitalWrite(RedLed,LOW);
      digitalWrite(Buzzer,HIGH);
      delay(30);
      digitalWrite(Buzzer,LOW);
      Serial.println("SAVEWORKBOOKAS,Names/WorkNames");
      }
      delay(1000);
cont:
delay(2000);
digitalWrite(GreenLed,LOW);
digitalWrite(RedLed,LOW);


void temp();{
    for (byte i = 0; i < mfrc522.uid.size; i++) {
     card_ID[i]=mfrc522.uid.uidByte[i];
  if(card_ID[i]==Name1[i]){
    if(temp1>=37){
      tone(Buzzer,1000,1000);
      lcd.clear();
      lcd.print("You're High");
      delay(1000);
      break;
    }
    else{
      
     }
    
  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Anuksha Shirgave");
    lcd.setCursor(0,1);
    lcd.print("Authorized access");
    delay(3000);
    lcd.clear();
    lcd.print("Roll No = 196055");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Your Temprature");
    lcd.setCursor(0,1);
    lcd.print("is ");
    lcd.print(mlx.readObjectTempC());
    lcd.print( "C");
    delay(3000);
    lcd.clear();
    //return 0;
  }
   else if(card_ID[i]==Name2[i]){
    if(temp1>=37){
      tone(Buzzer,1000,1000);
      lcd.clear();
      lcd.print("You're High");
      delay(1000);
      break;
    }
    else{
      
     }
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Aditi");
    lcd.setCursor(0,1);
    lcd.print("Authorized access");
    delay(3000);
     lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Your Temprature");
    lcd.setCursor(0,1);
    lcd.print("is ");
    lcd.print(mlx.readObjectTempC());
    lcd.print( "C");
    delay(3000);
    //return 0;
     }
      else   {
    Serial.println(" Access Denied");
    lcd.setCursor(3,0);
    lcd.clear();
    lcd.print("Unknown");
    lcd.setCursor(0,1);
    lcd.print("Access Denied");
    delay(3000);
     lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Your Temprature");
    lcd.setCursor(0,1);
    lcd.print("is ");
    lcd.print(mlx.readObjectTempC());
    lcd.print( "C");
    delay(3000);
    //return 0;
  }
}
}
}
