#include <Arduino.h>
#include <NimBLEDevice.h>

// put function declarations here:
int myFunction(int, int);

std::string verify = "fuck.open";
std::string tmp = "";

class ServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
        Serial.println("Connected");
    }

    void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
        Serial.println("Disconnected");
        pServer->startAdvertising();
    }
};

class CharCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic* pChar, NimBLEConnInfo& connInfo) override {
        std::string value = pChar->getValue();
        tmp = value;
        Serial.printf("Received: %s\n", value.c_str());
    }
};


void autoOpendoor();
void roll();
void unroll();

int pin = 10;
int pinde =6; 

void setup() {
  // put your setup code here, to run once:
  NimBLEDevice::init("Door");
  NimBLEServer *pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  NimBLEService *pService = pServer->createService("ABCD");
  NimBLECharacteristic *pCharacteristic = pService->createCharacteristic("1234",NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
  pCharacteristic->setCallbacks(new CharCallbacks());

  if(pServer->start()){
    Serial.printf("fail to start server");
  }
  pCharacteristic->setValue("Hello BLE");
  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID("ABCD");
  pAdvertising->setName("Door");
  pAdvertising->setMinInterval(1600); 
  pAdvertising->setMaxInterval(1600); 
  pAdvertising->start();

  pinMode(pin,OUTPUT);
  pinMode(pinde,OUTPUT);
  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);
}
bool isOpening = false;
unsigned long lastTime = 0;
unsigned long rollTime = 20000;
void loop() {
  // put your main code here, to run repeatedly:
  if(tmp ==verify){
      tmp = "";
      lastTime = millis();
  }

  autoOpendoor();
  delay(1);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

void roll(){
    digitalWrite(pin,HIGH);
}

void unroll(){
  digitalWrite(pin,LOW);
  digitalWrite(pinde,LOW);
}

void deroll(){
  digitalWrite(pinde,HIGH);
}

void autoOpendoor(){
    if(millis() - lastTime < rollTime){
      roll();
    }else{
      unroll();
    }
    if(millis() - lastTime > rollTime && millis() -lastTime < rollTime *2){
      deroll();
    }else{
      unroll();
    }
}