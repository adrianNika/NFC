
#if 0
#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_SPI pn532spi(SPI, 10);
NfcAdapter nfc = NfcAdapter(pn532spi);
#else
#include<Arduino.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

void readNFC(void);
void writeNFC(void);

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
int flag = 0;
#endif

void readNFC(void)
{
    if (nfc.tagPresent())
    {
        Serial.print("TAG détecter, lecture en cours...\n\r");
        delay(500);
        flag = 1;
        NfcTag tag = nfc.read();
        //tag.print();
        NdefMessage message = tag.getNdefMessage(); // ton objet

        int size = message.getEncodedSize(); // taille des données
        byte encoded[size];                  // tableau temporaire
        message.encode(encoded);            // on encode dedans

        Serial.print("Données NDEF brutes : ");
        for (int i = 0; i < size; i++) 
        {
            //if (encoded[i] < 0x10) Serial.print("0"); // pour avoir 0x0A au lieu de 0xA
            Serial.print((char)encoded[i]);
            if(encoded[i] == '*')
            {
                Serial.print("\n\r");
            }
            Serial.print(" ");
        }
        Serial.println();
    }
    else
    {
        if (flag == 0)
        {
            Serial.print("Wait NFC TAG\n\r");
            flag = 2;
        }
    }

    while (flag == 1)
    {
        if (!nfc.tagPresent())
        {
            flag = 0;
        }
    }
}

void writeNFC(void)
{
    if (nfc.tagPresent()) 
    {
        Serial.print("TAG détecter, écriture en cours...\n\r");
        delay(500);
        flag = 1;
        NdefMessage message = NdefMessage();
        message.addTextRecord("\n\rINFO*\n\rArt: 110069\n\rType: GEC/2023-3/ENC/ED\n\rS/N: 1617952907\n\rV.soft: 2514\n\rDate: 31/03/2025\n\rS/N Gets: 1234567890");

        bool success = nfc.write(message);
        if (success) {
          Serial.println("Success. Try reading this tag with your phone.");        
        } else {
          Serial.println("Write failed.");
        }
    }
    else
    {
        if (flag == 0)
        {
            Serial.println("\nPlace a formatted Mifare Classic NFC tag on the reader.");
            flag = 2;
        }
    }

    while (flag == 1)
    {
        if (!nfc.tagPresent())
        {
            flag = 0;
        }
    }
}

void setup(void) 
{
    Serial.begin(9600);
    Serial.println("NDEF Reader/Writer");
    nfc.begin();
}

void loop(void) 
{
    readNFC();
    //writeNFC();
    delay(10);
}