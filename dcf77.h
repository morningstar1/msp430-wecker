/*
 * dcf77.h
 *
 *  Created on: 17.10.2019
 *      Author: sebastian
 */
#pragma once
/*
struct dcf77{
    unsigned int intern : 15;       //0..14   Interne PTB-Informationen / Seit Oktober 2006 Wetterinformationen
    unsigned int rufbit : 1;        //15 R    Rufbit für Alarmierung der PTB-Mitarbeiter
    unsigned int wechsel : 1;       //16 A1   Ankündigung des Wechsels MEZ <-> MESZ
    unsigned int MES : 1;           //17 Z1   \__ Z1/Z2: 10 = MESZ, 01 = MEZ
    unsigned int MESZ : 1;          //18 Z2   /
    unsigned int Schaltsekunde : 1; //19 A2   Ankündigung einer Schaltsekunde
    unsigned int minute_1 : 1;      //20 S    Startbit f. Zeitinformationen (immer 1)
    unsigned int minute_bcd_low : 4; //21  Minute × 1
                                     //22  Minute × 2
                                     //23  Minute × 4
                                     //24  Minute × 8
    unsigned int minute_bcd_high : 3;//25  Minute × 10
                                     //26  Minute × 20
                                     //27  Minute × 40
    unsigned int minute_parity : 1; //28 P1   Minutenparität (gerade Parität Bits 21..27)
    unsigned int hour_bcd_low : 4;  //29  Stunde × 1
                                    //30  Stunde × 2
                                    //31  Stunde × 4
                                    //32  Stunde × 8
    unsigned int hour_bcd_high : 2; //33  Stunde × 10
                                    //34  Stunde × 20
    unsigned int hour_parity : 1;   //35 P2   Stundenparität (gerade Parität Bits 29..34)
    unsigned int day_bcd_low : 4;   //36  Tag × 1
                                    //37  Tag × 2
                                    //38  Tag × 4
                                    //39  Tag × 8
    unsigned int day_bcd_high : 2;  //40  Tag × 10
                                    //41  Tag × 20
    unsigned int weekday_bcd : 3;   //42  Wochentag × 1
                                    //43  Wochentag × 2
                                    //44  Wochentag × 4
    unsigned int month_bcd_low : 4; //45  Monat × 1
                                    //46  Monat × 2
                                    //47  Monat × 4
                                    //48  Monat × 8
    unsigned int month_bcd_high : 1;//49  Monat × 10
    unsigned int year_bcd_low : 4;  //50  Jahr × 1
                                    //51  Jahr × 2
                                    //52  Jahr × 4
                                    //53  Jahr × 8
    unsigned int year_bcd_high : 4; //54  Jahr × 10
                                    //55  Jahr × 20
                                    //56  Jahr × 40
                                    //57  Jahr × 80
    unsigned int date_parity : 1;   //58 P3   Datumsparität (gerade Parität Bits 36..57)
    unsigned int alignment : 5;
};*/

void decodeDCF();
void showDCFCounter();

extern uint8_t minute_done;

void initCapturePort();
void enableCapture();
void disableCapture();
void toggleCapture();

void showTime();
void showDate();


