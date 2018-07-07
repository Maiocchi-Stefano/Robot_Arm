// Arm Robot - Arduino nano
// by Maiocchi

#include <Servo.h>      // carica la libreria Servo
#include <Wire.h>       // carica la libreria del protocollo I2C
#include <veml6040.h>   // carica la libreria del sensore di colore
#include <LiquidCrystal_I2C.h>   // carica la libreria del display LCD 16x2

Servo myservo1;   // crea oggetto servo per controllare il servo 1
Servo myservo2;   // crea oggetto servo per controllare il servo 2
Servo myservo3;   // crea oggetto servo per controllare il servo 3
Servo myservo4;   // crea oggetto servo per controllare il servo 4
Servo myservo5;   // crea oggetto servo per controllare il servo 5
Servo myservo6;   // crea oggetto servo per controllare il servo 6

VEML6040 RGBWSensor;   // crea oggetto sensore di colore

LiquidCrystal_I2C lcd(0x27,16,2);   // crea oggetto display lcd indirizzo 0x27 16 caratteri 2 linee

int pinpezzo = 7;        // digital pin usato dal sensore di presenza del pezzo
boolean pezzopresente;   // cubo presente nel punto di presa e analisi pezzo (LOW)
int pinmetallo = 4;      // digital pin usato dal sensore induttivo del metallo
boolean pezzometallo;    // materiale usato per il cubo - metallo = LOW - legno = HIGH
boolean giallo;          // colore del pezzo = giallo
boolean rosso;           // colore del pezzo = rosso
boolean verde;           // colore del pezzo = verde
boolean blu;             // colore del pezzo = blu
boolean bianco;          // colore del pezzo = bianco

void setup()
{
  myservo1.attach(11);  // attacca il servo1 al pin 11 dell'oggetto servo
  myservo2.attach(10);  // attacca il servo2 al pin 10 dell'oggetto servo
  myservo3.attach(9);   // attacca il servo3 al pin 9  dell'oggetto servo
  myservo4.attach(6);   // attacca il servo4 al pin 6  dell'oggetto servo
  myservo5.attach(5);   // attacca il servo5 al pin 5  dell'oggetto servo
  myservo6.attach(3);   // attacca il servo6 al pin 3  dell'oggetto servo
  Wire.begin();         // avvia la libreria I2C
  /* init RGBW sensor with: 320ms integration time - auto mode - color sensor enable  */
  RGBWSensor.setConfiguration(VEML6040_IT_320MS + VEML6040_AF_AUTO + VEML6040_SD_ENABLE);
  lcd.init();           // inizializza il display
  lcd.backlight();      // accende la retroilluminazione
  lcd.clear();          // cancella lo schermo
  lcd.print("Scegli un pezzo");
  lcd.setCursor(0,1);
  lcd.print("posalo alla base");
  myservo1.write(92);   // angolo posizione di riposo servo1
  myservo2.write(79);   // angolo posizione di riposo servo2
  myservo3.write(94);   // angolo posizione di riposo servo3
  myservo4.write(140);  // angolo posizione di riposo servo4
  myservo5.write(90);   // angolo posizione di riposo servo5
  myservo6.write(80);   // angolo posizione di riposo servo6
}

void loop()
{
  pezzopresente = digitalRead(pinpezzo);   // leggi sensore presenza pezzo

  if (pezzopresente == LOW)      // cubo presente nel punto di presa e analisi pezzo
  {
    delay(1000);                 // attenti stabilizzazione del sensore di colore
    pezzometallo = digitalRead(pinmetallo);   // leggi sensore induttivo del metallo
       // determina colore del pezzo mediante lettura del sensore di colore
    if (RGBWSensor.getRed() > 65000 && RGBWSensor.getGreen() > 65000) giallo=1; else giallo=0;
    if (RGBWSensor.getRed() > 35000 && RGBWSensor.getGreen() < 45000) rosso=1; else rosso=0;
    if (RGBWSensor.getBlue() < 12000 && RGBWSensor.getWhite() < 40000) verde=1; else verde=0;
    if (RGBWSensor.getBlue() > 12000 && RGBWSensor.getWhite() < 55000) blu=1; else blu=0;
    if (RGBWSensor.getBlue() > 33000) {bianco=1; giallo=0;} else bianco=0;
       // scrive sul display colore e materiale del pezzo
    if (pezzometallo == LOW && giallo == 1) scrivigiallometallo();   // scrive pezzo giallo metallo
    if (pezzometallo == LOW && rosso == 1) scrivirossometallo();     // scrive pezzo rosso metallo
    if (pezzometallo == LOW && verde == 1) scriviverdemetallo();     // scrive pezzo verde metallo
    if (pezzometallo == LOW && blu == 1) scriviblumetallo();         // scrive pezzo blu metallo
    if (pezzometallo == HIGH && giallo == 1) scrivigiallolegno();    // scrive pezzo giallo legno
    if (pezzometallo == HIGH && rosso == 1) scrivirossolegno();      // scrive pezzo rosso legno
    if (pezzometallo == HIGH && verde == 1) scriviverdelegno();      // scrive pezzo verde legno
    if (pezzometallo == HIGH && blu == 1) scriviblulegno();          // scrive pezzo blu legno
    if (bianco == 1) scriviscartapezzo();                            // scrive scarta pezzo non colorato
    prendi();     // muovi avanti braccio fino a punto di presa e prendi il pezzo
    if (pezzometallo == LOW && giallo == 1) posagiallometallo();     // posa pezzo giallo metallo
    if (pezzometallo == LOW && rosso == 1) posarossometallo();       // posa pezzo rosso metallo
    if (pezzometallo == LOW && verde == 1) posaverdemetallo();       // posa pezzo verde metallo
    if (pezzometallo == LOW && blu == 1) posablumetallo();           // posa pezzo blu metallo
    if (pezzometallo == HIGH && giallo == 1) posagiallolegno();      // posa pezzo giallo legno
    if (pezzometallo == HIGH && rosso == 1) posarossolegno();        // posa pezzo rosso legno
    if (pezzometallo == HIGH && verde == 1) posaverdelegno();        // posa pezzo verde legno
    if (pezzometallo == HIGH && blu == 1) posablulegno();            // posa pezzo blu legno
    if (bianco == 1) scartapezzo();                                  // scarta pezzo non colorato
    delay(1000);
  }
}

void prendi()     // muovi avanti braccio fino a punto di presa
{
  myservo6.write(65);                 // apri pinza
  for (int i=0; i<=62; i++)
  {
    if (i<61) myservo2.write(79+i);   // abbassa braccio
    if (i<27) myservo3.write(94-i);   // chiude gomito
    if (i<27) myservo4.write(140-i);  // abbassa pinza
    delay(17);
  }
  delay(300);
  myservo6.write(120);                // chiudi pinza
  delay(300);
}

void torna()      // riporta braccio in posizione di riposo
{
  for (int i=0; i<=64; i++)
  {
    if (i<64) myservo2.write(144-i);  // alza braccio
    if (i<24) myservo3.write(70+i);   // apre gomito
    if (i<23) myservo4.write(118+i);  // alza pinza
    delay(15);
  }
  delay(2000);
  myservo6.write(75);                 // apre pinza
}

void scartapezzo()            // scarta pezzo non colorato gettandolo a lato
{
  for (int i=0; i<=15; i++)
  {
    myservo2.write(144-i);    // alza braccio
    delay(15);
  }
  for (int i=0; i<=38; i++)
  {
    myservo1.write(93-i);     // ruota braccio   
    delay(15);
  }  
  for (int i=0; i<=21; i++)
  {
    myservo2.write(129+i);    // abbassa braccio
    delay(15);
  }
  delay(300);
  myservo6.write(80);         // apri pinza
  delay(300);
  for (int i=0; i<=70; i++)
  {
    if (i<40) myservo1.write(59+i);    // ruota braccio
    if (i<72) myservo2.write(149-i);   // alza braccio
    if (i<25) myservo3.write(70+i);    // apre gomito
    if (i<23) myservo4.write(118+i);   // alza pinza
    delay(15);
  }  
  lcd.clear();                         // cancella lo schermo
  lcd.print("Scegli un pezzo");
  lcd.setCursor(0,1);                  // vai a riga successiva   
  lcd.print("posalo alla base");
}

void posagiallometallo()              // posa pezzo giallo metallo
{
  for (int i=0; i<=30; i++)
  {
    myservo2.write(144-i);   // alza braccio
    delay(20);
  }
  for (int i=0; i<=31; i++)
  {
    if (i<31) myservo1.write(93+i);    // ruota braccio   
    if (i<20) myservo3.write(70-i);    // chiude gomito
    if (i<17) myservo4.write(118-i);   // abbassa pinza
    if (i<31) myservo5.write(90+i);    // ruota pinza
    delay(15);
  }  
  for (int i=0; i<=22; i++)
  {
    myservo2.write(114+i);    // abbassa braccio
    delay(15);
  }
  delay(300);
  myservo6.write(80);         // apri pinza
  delay(300);
  for (int i=0; i<=30; i++)
  {
    myservo2.write(136-i);    // alza braccio
    delay(15);
  }
  for (int i=0; i<=43; i++)
  {
    if (i<33) myservo1.write(123-i);   // ruota braccio
    if (i<28) myservo2.write(106-i);   // alza braccio   
    if (i<44) myservo3.write(51+i);    // apre gomito
    if (i<39) myservo4.write(102+i);   // alza pinza
    if (i<31) myservo5.write(120-i);   // ruota pinza
    delay(15);
  }
  lcd.clear();                         // cancella lo schermo
  lcd.print("Scegli un pezzo");
  lcd.setCursor(0,1);                  // vai a riga successiva   
  lcd.print("posalo alla base");  
}

void posarossometallo()      // posa pezzo rosso metallo
{
  for (int i=0; i<=35; i++)
  {
    myservo2.write(144-i);   // alza braccio
    delay(20);
  }
  for (int i=0; i<=34; i++)
  {
    if (i<15) myservo1.write(93+i);    // ruota braccio   
    if (i<33) myservo3.write(70-i);    // chiude gomito
    if (i<13) myservo4.write(118-i);   // abbassa pinza
    if (i<18) myservo5.write(90+i);    // ruota pinza
    delay(15);
  }  
  for (int i=0; i<=18; i++)
  {
    myservo2.write(109+i);    // abbassa braccio
    delay(15);
  }
  delay(300);
  myservo6.write(80);         // apri pinza
  delay(300);
  for (int i=0; i<=20; i++)
  {
    myservo2.write(127-i);    // alza braccio
    delay(15);
  }
  for (int i=0; i<=58; i++)
  {
    if (i<13) myservo1.write(104-i);   // ruota braccio
    if (i<29) myservo2.write(107-i);   // alza braccio   
    if (i<58) myservo3.write(38+i);    // apre gomito
    if (i<36) myservo4.write(106+i);   // alza pinza
    if (i<18) myservo5.write(107-i);   // ruota pinza
    delay(15);
  }
  lcd.clear();                         // cancella lo schermo
  lcd.print("Scegli un pezzo");
  lcd.setCursor(0,1);                  // vai a riga successiva   
  lcd.print("posalo alla base");
}

void posaverdemetallo()      // posa pezzo verde metallo
{
  for (int i=0; i<=35; i++)
  {
    myservo2.write(144-i);   // alza braccio
    delay(20);
  }
  for (int i=0; i<=34; i++)
  {
    if (i<10) myservo1.write(92-i);    // ruota braccio   
    if (i<33) myservo3.write(70-i);    // chiude gomito
    if (i<13) myservo4.write(118-i);   // abbassa pinza
    if (i<10) myservo5.write(90-i);    // ruota pinza
    delay(15);
  }  
  for (int i=0; i<=16; i++)
  {
    myservo2.write(109+i);    // abbassa braccio
    delay(15);
  }
  delay(300);
  myservo6.write(80);         // apri pinza
  delay(300);
  for (int i=0; i<=20; i++)
  {
    myservo2.write(125-i);    // alza braccio
    delay(15);
  }
  for (int i=0; i<=58; i++)
  {
    if (i<16) myservo1.write(83+i);    // ruota braccio
    if (i<27) myservo2.write(105-i);   // alza braccio   
    if (i<58) myservo3.write(38+i);    // apre gomito
    if (i<36) myservo4.write(106+i);   // alza pinza
    if (i<15) myservo5.write(81+i);    // ruota pinza
    delay(15);
  }
  lcd.clear();                         // cancella lo schermo
  lcd.print("Scegli un pezzo");
  lcd.setCursor(0,1);                  // vai a riga successiva   
  lcd.print("posalo alla base");
}

void posablumetallo()        // posa pezzo blu metallo
{
  for (int i=0; i<=30; i++)
  {
    myservo2.write(144-i);   // alza braccio
    delay(20);
  }
  for (int i=0; i<=29; i++)
  {
    if (i<27) myservo1.write(93-i);    // ruota braccio   
    if (i<24) myservo3.write(70-i);    // chiude gomito
    if (i<17) myservo4.write(118-i);   // abbassa pinza
    if (i<29) myservo5.write(90-i);    // ruota pinza
    delay(15);
  }  
  for (int i=0; i<=18; i++)
  {
    myservo2.write(114+i);    // abbassa braccio
    delay(15);
  }
  delay(300);
  myservo6.write(80);         // apri pinza
  delay(300);
  for (int i=0; i<=30; i++)
  {
    myservo2.write(132-i);    // alza braccio
    delay(15);
  }
  for (int i=0; i<=43; i++)
  {
    if (i<33) myservo1.write(67+i);    // ruota braccio
    if (i<24) myservo2.write(102-i);   // alza braccio   
    if (i<50) myservo3.write(47+i);    // apre gomito
    if (i<39) myservo4.write(102+i);   // alza pinza
    if (i<32) myservo5.write(62+i);    // ruota pinza
    delay(15);
  }
  lcd.clear();                         // cancella lo schermo
  lcd.print("Scegli un pezzo");
  lcd.setCursor(0,1);                  // vai a riga successiva   
  lcd.print("posalo alla base");
}

void posagiallolegno()       // posa pezzo giallo legno
{
  for (int i=0; i<=10; i++)
  {
    myservo2.write(141-i);   // alza braccio
    delay(15);
  }
  for (int i=0; i<=34; i++)
  {
    myservo2.write(131-i);   // alza braccio
    myservo3.write(68-i);    // chiude gomito
    delay(15);
  }
  for (int i=0; i<=41; i++)
  {
    if (i<42) myservo1.write(92+i);    // ruota braccio   
    if (i<11) myservo3.write(34-i);    // chiude gomito
    if (i<6) myservo4.write(114-i);    // abbassa pinza
    if (i<40) myservo5.write(90+i);    // ruota pinza
    delay(15);
  }  
  for (int i=0; i<=19; i++)
  {
    myservo2.write(97+i);     // abbassa braccio
    delay(15);
  }
  delay(300);
  myservo6.write(75);         // apri pinza
  delay(300);
  for (int i=0; i<=30; i++)
  {
    myservo2.write(117-i);    // alza braccio
    delay(15);
  }
  for (int i=0; i<=77; i++)
  {
    if (i<38) myservo1.write(129-i);   // ruota braccio
    if (i<10) myservo2.write(87-i);    // alza braccio   
    if (i<71) myservo3.write(24+i);    // apre gomito
    if (i<32) myservo4.write(109+i);   // alza pinza
    if (i<40) myservo5.write(129-i);   // ruota pinza
    delay(15);
  }
  lcd.clear();                         // cancella lo schermo
  lcd.print("Scegli un pezzo");
  lcd.setCursor(0,1);                  // vai a riga successiva   
  lcd.print("posalo alla base");
}

void posarossolegno()        // posa pezzo rosso legno
{
  for (int i=0; i<=15; i++)
  {
    myservo2.write(144-i);   // alza braccio
    delay(15);
  }
  for (int i=0; i<=50; i++)
  {
    myservo2.write(129-i);   // alza braccio
    myservo3.write(70-i);    // chiude gomito
    delay(15);
  }
  for (int i=0; i<=21; i++)
  {
    if (i<21) myservo1.write(93+i);    // ruota braccio   
    if (i<11) myservo3.write(20-i);    // chiude gomito
    if (i<8) myservo4.write(118-i);    // abbassa pinza
    if (i<21) myservo5.write(90+i);    // ruota pinza
    delay(15);
  }  
  for (int i=0; i<=32; i++)
  {
    myservo2.write(79+i);     // abbassa braccio
    delay(15);
  }
  delay(300);
  myservo6.write(80);         // apri pinza
  delay(300);
  for (int i=0; i<=20; i++)
  {
    myservo2.write(111-i);    // alza braccio
    delay(15);
  }
  for (int i=0; i<=85; i++)
  {
    if (i<23) myservo1.write(113-i);   // ruota braccio
    if (i<13) myservo2.write(91-i);    // alza braccio   
    if (i<86) myservo3.write(10+i);    // apre gomito
    if (i<31) myservo4.write(111+i);   // alza pinza
    if (i<21) myservo5.write(110-i);   // ruota pinza
    delay(15);
  }
  lcd.clear();                         // cancella lo schermo
  lcd.print("Scegli un pezzo");
  lcd.setCursor(0,1);                  // vai a riga successiva   
  lcd.print("posalo alla base");
}

void posaverdelegno()        // posa pezzo verde legno
{
  for (int i=0; i<=15; i++)
  {
    myservo2.write(144-i);   // alza braccio
    delay(15);
  }
  for (int i=0; i<=50; i++)
  {
    myservo2.write(129-i);   // alza braccio
    myservo3.write(70-i);    // chiude gomito
    delay(15);
  }
  for (int i=0; i<=19; i++)
  {
    if (i<14) myservo1.write(93-i);    // ruota braccio   
    if (i<12) myservo3.write(20-i);    // chiude gomito
    if (i<8) myservo4.write(118-i);    // abbassa pinza
    if (i<13) myservo5.write(90-i);    // ruota pinza
    delay(15);
  }  
  for (int i=0; i<=32; i++)
  {
    myservo2.write(79+i);     // abbassa braccio
    delay(15);
  }
  delay(300);
  myservo6.write(80);         // apri pinza
  delay(300);
  for (int i=0; i<=20; i++)
  {
    myservo2.write(111-i);    // alza braccio
    delay(15);
  }
  for (int i=0; i<=85; i++)
  {
    if (i<19) myservo1.write(80+i);    // ruota braccio
    if (i<13) myservo2.write(91-i);    // alza braccio   
    if (i<86) myservo3.write(10+i);    // apre gomito
    if (i<31) myservo4.write(111+i);   // alza pinza
    if (i<17) myservo5.write(78+i);    // ruota pinza
    delay(15);
  }
  lcd.clear();                         // cancella lo schermo
  lcd.print("Scegli un pezzo");
  lcd.setCursor(0,1);                  // vai a riga successiva   
  lcd.print("posalo alla base");
}

void posablulegno()          // posa pezzo blu legno
{
  for (int i=0; i<=15; i++)
  {
    myservo2.write(144-i);   // alza braccio
    delay(15);
  }
  for (int i=0; i<=34; i++)
  {
    myservo2.write(129-i);   // alza braccio
    myservo3.write(70-i);    // chiude gomito
    delay(15);
  }
  for (int i=0; i<=36; i++)
  {
    if (i<36) myservo1.write(93-i);    // ruota braccio   
    if (i<15) myservo3.write(36-i);    // chiude gomito
    if (i<15) myservo4.write(118-i);   // abbassa pinza
    if (i<35) myservo5.write(90-i);    // ruota pinza
    delay(15);
  }  
  for (int i=0; i<=18; i++)
  {
    myservo2.write(95+i);     // abbassa braccio
    delay(15);
  }
  delay(300);
  myservo6.write(80);         // apri pinza
  delay(300);
  for (int i=0; i<=20; i++)
  {
    myservo2.write(113-i);    // alza braccio
    delay(15);
  }
  for (int i=0; i<=83; i++)
  {
    if (i<41) myservo1.write(59+i);    // ruota braccio
    if (i<15) myservo2.write(93-i);    // alza braccio   
    if (i<83) myservo3.write(12+i);    // apre gomito
    if (i<32) myservo4.write(110+i);   // alza pinza
    if (i<38) myservo5.write(56+i);    // ruota pinza
    delay(15);
  }
  lcd.clear();                         // cancella lo schermo
  lcd.print("Scegli un pezzo");
  lcd.setCursor(0,1);                  // vai a riga successiva   
  lcd.print("posalo alla base");
}

void scrivigiallometallo()
{
  lcd.clear();                    // cancella lo schermo
  lcd.print("   - GIALLO -");
  lcd.setCursor(0,1);             // vai a riga successiva
  lcd.print("   - METALLO -");
}

void scrivirossometallo()
{
  lcd.clear();                    // cancella lo schermo
  lcd.print("    - ROSSO -");
  lcd.setCursor(0,1);             // vai a riga successiva
  lcd.print("   - METALLO -");
}

void scriviverdemetallo()
{
  lcd.clear();                    // cancella lo schermo
  lcd.print("    - VERDE -");
  lcd.setCursor(0,1);             // vai a riga successiva
  lcd.print("   - METALLO -");
}

void scriviblumetallo()
{
  lcd.clear();                    // cancella lo schermo
  lcd.print("     - BLU -");
  lcd.setCursor(0,1);             // vai a riga successiva
  lcd.print("   - METALLO -");
}

void scrivigiallolegno()
{
  lcd.clear();                    // cancella lo schermo
  lcd.print("   - GIALLO -");
  lcd.setCursor(0,1);             // vai a riga successiva
  lcd.print("   - LEGNO -");
}

void scrivirossolegno()
{
  lcd.clear();                    // cancella lo schermo
  lcd.print("   - ROSSO -");
  lcd.setCursor(0,1);             // vai a riga successiva
  lcd.print("   - LEGNO -");
}

void scriviverdelegno()
{
  lcd.clear();                    // cancella lo schermo
  lcd.print("   - VERDE -");
  lcd.setCursor(0,1);             // vai a riga successiva
  lcd.print("   - LEGNO -");
}

void scriviblulegno()
{
  lcd.clear();                    // cancella lo schermo
  lcd.print("    - BLU -");
  lcd.setCursor(0,1);             // vai a riga successiva
  lcd.print("   - LEGNO -");
}

void scriviscartapezzo()
{
  lcd.clear();                    // cancella lo schermo
  lcd.print("   - BIANCO -");
  lcd.setCursor(0,1);             // vai a riga successiva
  lcd.print("  - SCARTARE -");
}
