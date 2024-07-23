
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9

byte readCard[4];
String MasterTag = "D359A6F7";
String tagID = "";
int quantity = 10;
int total_quantity = 0;

//new
int tag = 0;
MFRC522 mfrc522(SS_PIN, RST_PIN);
void setup() 
{
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println("--------------------------");
  Serial.println(" Inventory Management ");
  Serial.println("Scan Your Card>>");
}

void loop() 
{
  while (getID()) {

    if (tagID == MasterTag && tag == 0){
      Serial.println("Product: Tomatoes ");
      Serial.println("Quantity in: 10kg ");
      Serial.println("Status:IN");
      Serial.println("Total quantity:");
      total_quantity = total_quantity+10;
      Serial.println(total_quantity);
      
      tag = 1;
      Serial.println("--------------------------");
    
    }
    else{
      Serial.println("Product: Tomatoes ");
      Serial.println("Quantity out: 10kg ");
      Serial.println("Status:OUT");
      Serial.println("Total quantity:");
      total_quantity = total_quantity-10;
      Serial.println(total_quantity);
      tag = 0;
      Serial.println("--------------------------");
    }

    delay(10);
    Serial.println(" Inventory Management ");
    Serial.println("Scan Your Card>>");
  }
  //----------------------------------------------------------------------
}



boolean getID() 
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
  return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) {
  tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}