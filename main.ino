#define klima 22 //22.Porta klimayı bağladım.
#define oturmaled 24 //24.porta oturma odası ledlerini bağladım.
#define mutfakled 26 //26.porta mutfaktaki ledleri bağladım.
#define TV 28 //28.porta LCD Ekranı bağladım.
#define LED 30 //
#define LED1 32
#include <dht11.h> //DHT11 sıcaklık ve ısı sensörünün kütüphanesini ekledim.
#define DHT11PIN 36 //36.porta sıcaklık ve ısı sensörünü bağladım.
dht11 DHT11;
int trigPin=2; //Ultrasonik sensörün trigPin'ini 2.porta bağladım.
int echoPin=3; //Ultrasonik sensörün echoPin'ini 3.porta bağladım.
int servo=38;  //38.porta garaj kapısında kullandığım servo motoru bağladım.
char state = 0; //bluetooth'dan gelen değeri atmak için değişken oluşturdum.
#include <Servo.h> //servo motor kütüphanesini dahil ettim.
Servo servomotorum;
int poz=180; //Garaj kapısı için servomotorun pozisyonu için değişken tanımlayıp 180 değerini verdim.
int mesafe;//HC-SR04 Ultrasonik sensör için mesafeyi atmak için değişken tanımladım.
#include <OLED_I2C.h> //OLED Ekranın kütüphanesini dahil ettim.
OLED  myOLED(SDA, SCL, 8); //OLED ekran OLED_I2C Modülüyle geldiği için bunu tanıttım.
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];
void setup() {
  servomotorum.attach(6);
  pinMode(servo,OUTPUT);
  digitalWrite(servo,LOW);

  servomotorum.write(poz);
  delay(1000);
  digitalWrite(servo,HIGH);
  pinMode(klima, OUTPUT);
  myOLED.begin();
  myOLED.setFont(SmallFont);
  pinMode(oturmaled,OUTPUT);
  pinMode(mutfakled,OUTPUT);
  pinMode(TV,OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(LED1,OUTPUT);
  digitalWrite(klima, HIGH);
  digitalWrite(oturmaled, LOW);
  digitalWrite(mutfakled, LOW);
  digitalWrite(TV,LOW);
  digitalWrite(LED,LOW);
  digitalWrite(LED1,LOW);

  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(34,OUTPUT);
}


void loop() {
  int sure;
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  sure = pulseIn(echoPin, HIGH);
  mesafe = (sure/2) / 29.1;


    Serial.println(poz);
    if(Serial.available() > 0){
    state = Serial.read();
    }
    if (state == '0') {
    digitalWrite(oturmaled , HIGH);
    state = 0;
    }
    else if (state == '1') {
    digitalWrite(oturmaled , LOW);
    state = 0;
    }
    else if (state == '2') {
    digitalWrite(klima, LOW);
    state = 0;
    }
    else if (state == '3') {
    digitalWrite(klima, HIGH);
    state = 0;
    }
    else if (state == '4') {
    digitalWrite(mutfakled , HIGH);
    state = 0;
    }
    else if (state == '5') {
    digitalWrite(mutfakled , LOW);
    state = 0;
    }
    else if (state == '6') {
    digitalWrite(TV , HIGH);
    state = 0;
    }
    else if (state == '7') {
    digitalWrite(TV , LOW);
    state = 0;
    }
    if(state == 'A') {
    digitalWrite(servo,LOW);
    digitalWrite(LED1,HIGH);
    if(poz>=170&&poz<=190){
    for(poz=180; poz>=20; poz--)
  {
    servomotorum.write(poz);
    delay(2);
  }
    poz=20;

    digitalWrite(servo,HIGH);
    state=0;
    }
    }
    else if(state == 'B') {

    if(poz>=10&&poz<=30){
      digitalWrite(servo,LOW);
    for(poz=20; poz<=180; poz++)
  {
    servomotorum.write(poz);
    delay(2);
  }
    servomotorum.write(180);
    poz=180;

    digitalWrite(servo,HIGH);
     myOLED.clrScr();
     myOLED.update();
    digitalWrite(LED1,LOW);
    state =0;
    }
    }
    else if(state == 'C') {
    digitalWrite(LED,HIGH);
    state =0;
    }
    else if(state == 'D') {
    digitalWrite(LED,LOW);
    state =0;
    }

      if(poz>=10&&poz<=40)
      {
        mesafee();
      }
      else
      {
        sicaklik();
      }


}
void sicaklik(){
  int chk = DHT11.read(DHT11PIN);
    myOLED.clrScr();
    myOLED.setFont(SmallFont);
    myOLED.print("Sicaklik :", 0, 26);
    myOLED.print(" C", 84, 26);
    myOLED.setFont(MediumNumbers);
    myOLED.printNumI((float)DHT11.temperature, 60, 16);
    myOLED.setFont(SmallFont);
    myOLED.print("Nem      :", 0, 46);
    myOLED.print(" %", 84, 46);
    myOLED.setFont(MediumNumbers);
    myOLED.printNumI((float)DHT11.humidity, 60, 36);
    myOLED.update();

}
void mesafee(){
    myOLED.clrScr();
    myOLED.setFont(SmallFont);
    myOLED.print("DUVARA OLAN UZAKLIK", CENTER, 16);
    myOLED.print("cm    ", RIGHT, 55);
    myOLED.setFont(BigNumbers);
    myOLED.printNumI(mesafe, CENTER, 40);
    myOLED.update();
    if(mesafe>=4&&mesafe<=5){
    digitalWrite(34,HIGH);
    delay(30);
    }
    else if(mesafe>=3&&mesafe<=4){
    digitalWrite(34,HIGH);
    delay(100);
    }
    else if(mesafe<3){
    digitalWrite(34,HIGH);
    delay(250);
    }
    digitalWrite(34,LOW);
}
