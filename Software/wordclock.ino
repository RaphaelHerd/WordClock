//Arduino pro mini
#include "LedControl.h"


//Globale Variable
byte  stoerung;      //Unplausibler Empfang
byte  zeitEmpfangen; //Schaltet auf Zeitanzeige statt Sekundenanzeige um

byte  uhrHH;  //Die interne Uhr (wird von DCF-Uhr gestellt)
byte  uhrMM;
byte  uhrSS;
byte  dcfSekunde; //0..59 - wird bei ausbleibendem Signal (59. Sekunde) auf 0 gesetzt

byte  pixel[16];
//  unsigned long n;

/*
 We need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn
 pin 11 is connected to the CLK
 pin 10 is connected to LOAD
 We have two cascaded MAX7219.
 */
LedControl led = LedControl(12, 11, 10, 2);


//I/O Makros DCF77-Empfänger
#define   DI_DCF77 9
//#define   DO_DCF_DISABLE 8     //over 4K7 to PON. PON over 5K6 to GND
#define   DO_DCF_POWERSUPPLY 7 //over 470R to VDD. VDD over white LED (2.8V) to GND. 
// Texte im Display
#define a 0
#define b 1
#define c 2
#define d 3
#define e 4
#define f 5
#define g 6
#define h 7
#define A 8
#define B 9 
#define C 10
#define D 11
#define E 12
#define F_ 13
#define G 14
#define H 15

/*
a..h: Digits controller 1
A..H: Digits controller 2
0..7: Segments
a0 1  2  3  4  5  6  a7 D0 E0 F0
E  S  _  i  s  t  _  f  ü  n  f

b0                   b7 D1 E1 F1
z  e  h  n  z  w  a  n  z  i  g

c0                   c7 D2 F1 F2
d  r  e  i  v  i  e  r  t  e  l

d0                   d7 D3
_  _  n  a  c  h  v  o  r  _  _

e0                   e7 D4
h  a  l  b  _  z  w  ö  l  f  _

f0    f2 f3 f4 f5 f6 f7
z  w  e  i  n  s  i  e  b  e  n

g0                   g7 D6
_  d  r  e  i  _  _  f  ü  n  f

h0                   h7 D7 E7 F7
e  l  f  n  e  u  n  v  i  e  r

A0                   A7 G0 G1 G2
_  a  c  h  t  z  e  h  n  r  s

B0                   B7 H0 H1 H2
_  s  e  c  h  s  _  _  u  h  r

C3 C4 C5 C6



spiTransfer(addr, row+1,[device*8 + row]);
*/
//Aufbau: 1. Zeile Display = 1.Zeile Max + halbe 2. Zeile

#define		TEXT_ES_IST     {set(a,0); set(a,1); set(a,3); set(a,4); set(a,5);} 
#define		TEXT_FUENF1		{set(a,7); set(D,0); set(E,0); set(F_,0); } 

#define		TEXT_ZEHN0		{set(b,0); set(b,1); set(b,2); set(b,3); } 
#define		TEXT_ZWANZIG	{set(b,4); set(b,5); set(b,6); set(b,7); set(D,1); set(E,1); set(F_,1); } 

#define		TEXT_DREI1		{set(c,0); set(c,1); set(c,2); set(c,3); }
#define		TEXT_VIERTEL	{set(c,4); set(c,5); set(c,6); set(c,7); set(D,2); set(E,2); set(F_,2);}

#define		TEXT_NACH		{set(d,2); set(d,3); set(d,4); set(d,5); }
#define		TEXT_VOR	    {set(d,6); set(d,7); set(D,3);  }

#define		TEXT_HALB		{set(e,0); set(e,1); set(e,2); set(e,3); }
#define		TEXT_ZWOELF		{set(e,5); set(e,6); set(e,7); set(D,4); set(E,4); }

#define		TEXT_ZWEI		{set(f,0); set(f,1); set(f,2); set(f,3); }
#define		TEXT_EIN		{set(f,2); set(f,3); set(f,4); }
#define		TEXT_EINS		{set(f,2); set(f,3); set(f,4); set(f,5); }
#define		TEXT_SIEBEN		{set(f,5); set(f,6); set(f,7); set(D,5); set(E,5); set(F_,5);}

#define		TEXT_DREI		{set(g,1); set(g,2); set(g,3); set(g,4); }
#define		TEXT_FUENF		{set(g,7); set(D,6); set(E,6); set(F_,6); } 

#define		TEXT_ELF		{set(h,0); set(h,1); set(h,2);}
#define		TEXT_NEUN 		{set(h,3); set(h,4); set(h,5); set(h,6); }
#define		TEXT_VIER		{set(h,7); set(D,7); set(E,7); set(F_,7); } 

#define		TEXT_ACHT		{set(A,1); set(A,2); set(A,3); set(A,4); }
#define		TEXT_ZEHN 		{set(A,5); set(A,6); set(A,7); set(G,0); } 

#define		TEXT_SECHS		{set(B,1); set(B,2); set(B,3); set(B,4); set(B,5); } 
#define		TEXT_UHR  		{set(H,0); set(H,1); set(H,2); } 


//-----------------------------------------------------------------------------
inline void set(byte row, byte col){
	//-----------------------------------------------------------------------------
	pixel[row] |= (1 << col);
}

//-----------------------------------------------------------------------------
void updateDisplay(void){
	//-----------------------------------------------------------------------------
	byte i;
	for (i = 0; i < 16; i++){
		led.setRow(i / 8, i % 8, pixel[i]); //(int addr, int row, byte value); 
	}
}

//-----------------------------------------------------------------------------
void cls(void){
	//-----------------------------------------------------------------------------
	for (byte i = 0; i < 16; i++) pixel[i] = 0;
}

//-----------------------------------------------------------------------------
void ledtest(void){
	//-----------------------------------------------------------------------------
	for (byte y = 0; y < 16; y++){
		for (byte x = 0; x < 8; x++){
			cls();
			pixel[y] |= (1 << x);
			updateDisplay();
			delay(40);
			if (digitalRead(DI_DCF77)) digitalWrite(13, HIGH);
			else  digitalWrite(13, LOW);
		}
	}
	delay(200);
	if (digitalRead(DI_DCF77)) digitalWrite(13, HIGH);
	else  digitalWrite(13, LOW);
	cls();
	for (byte y = 0; y < 16; y++){
		for (byte x = 0; x < 8; x++){
			pixel[y] |= (1 << x);
			updateDisplay();
			delay(30);
			if (digitalRead(DI_DCF77)) digitalWrite(13, HIGH);
			else  digitalWrite(13, LOW);
		}
	}
	delay(3500);
}



//------------------------------------------------------------------------------
void anzeigen(void){  // Zeit ausgeben 
	//------------------------------------------------------------------------------
	byte i;
	byte hr;
	byte zehner;
	byte einer;

	cls(); //Bildschirm löschen


	if (zeitEmpfangen){
		TEXT_ES_IST;
		if (uhrMM < 5){ TEXT_UHR; }
		else if (uhrMM < 10){ TEXT_FUENF1;  TEXT_NACH; }
		else if (uhrMM < 15){ TEXT_ZEHN0;   TEXT_NACH; }
		else if (uhrMM < 20){ TEXT_VIERTEL; TEXT_NACH; }
		else if (uhrMM < 25){ TEXT_ZWANZIG; TEXT_NACH; }
		else if (uhrMM < 30){ TEXT_FUENF1;  TEXT_VOR;    TEXT_HALB; }
		else if (uhrMM < 35){ TEXT_HALB; }
		else if (uhrMM < 40){ TEXT_FUENF1; 	TEXT_NACH; 	 TEXT_HALB; }
		else if (uhrMM < 45){ TEXT_ZWANZIG; TEXT_VOR; }
		else if (uhrMM < 50){ TEXT_VIERTEL; TEXT_VOR }
		//	else if(uhrMM < 50){TEXT_DREI1;   TEXT_VIERTEL;				    }
		else if (uhrMM < 55){ TEXT_ZEHN0;	TEXT_VOR; }
		else if (uhrMM < 60){ TEXT_FUENF1; 	TEXT_VOR; }

		hr = uhrHH;
		if (uhrMM >= 25) hr++;

		switch (hr % 12){
		case  0: TEXT_ZWOELF; break;
		case  1: if (uhrMM < 5) {
			TEXT_EIN;
		}
				 else{
					 TEXT_EINS;
				 } break;
		case  2: TEXT_ZWEI; break;
		case  3: TEXT_DREI; break;
		case  4: TEXT_VIER; break;
		case  5: TEXT_FUENF; break;
		case  6: TEXT_SECHS; break;
		case  7: TEXT_SIEBEN; break;
		case  8: TEXT_ACHT; break;
		case  9: TEXT_NEUN; break;
		case 10: TEXT_ZEHN; break;
		case 11: TEXT_ELF; break;
		}
		;
		//Minuten mit 4 Pixeln am untere Rand 
		switch (uhrMM % 5){
		case 4: set(C, 6);  //absichtlich kein BREAK!
		case 3: set(C, 5);
		case 2: set(C, 4);
		case 1: set(C, 3);
		}


	}
	else{ //Noch keine DCF-Zeit vorhanden. Zeige Sekunden.
		zehner = (dcfSekunde / 10);
		einer = (dcfSekunde % 10);
		/*
			for (i=0; i<5; i++){
			spalte[i] = ziffern[z][i];
			spalte[i+6] = ziffern[e][i];
			}
			*/
	}
	updateDisplay();
}



//-----------------------------------------------------------------------------
void scope() { //Scope in oberen beiden Zeilen
	//-----------------------------------------------------------------------------
	static byte y[11];
	byte i;
	byte mask;
	for (i = 0; i < 10; i++) y[i] = y[i + 1];
	if (digitalRead(DI_DCF77)){ //Signalabsenkung liefert High für 100ms oder 200ms
		y[10] = 1;
	}
	else{
		y[10] = 0;
	}
	cls();
	mask = 1;
	for (i = 0; i < 8; i++){
		if (y[i]){
			pixel[0] |= mask;
		}
		else{
			pixel[1] |= mask;
		}
		mask = mask + mask;
	}
	if (y[8]){
		pixel[11] = 1;
	}
	else{
		pixel[11] = 2;
	}
	if (y[9]){
		pixel[12] = 1;
	}
	else{
		pixel[12] = 2;
	}
	if (y[10]){
		pixel[13] = 1;
	}
	else{
		pixel[13] = 2;
	}

	updateDisplay();
}
//------------------------------------------------------------------------------
void displayEnable(byte en){
	static byte  oldEn = 222;
	if (oldEn != en){
		oldEn = en;
		led.shutdown(0, (en == 0));
		led.shutdown(1, (en == 0));
	}

}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void decodeDCF77(void){  //Alle 5ms aufrufen
	//------------------------------------------------------------------------------
	static int iTic = 0;
	static int iTiefpass = 0;
	static bool bSteigend = 0;
	static byte nZeitumstellung = 0;
	static byte hs = 0;
	static byte hh = 0;
	static byte mm = 0;
	static byte ss = 0;
	static byte tag = 0;
	static byte wtag = 0;
	static byte monat = 0;
	static byte jahr = 0;
	static byte keinEmpfang = 0;
	static byte neuesDatum = 0;
	static bool bToggle;
	static byte sommerzeit;
	static byte winterzeit;
	static byte startphase = 1;
	static byte bChecksum = 0;
	static byte zeitUebernehmen;
	static byte bBit[60];      //die empfangenen Bits
	static byte erfolgreichdecodiert;
	static byte vorHH; //Die empfangene Zeit in der letzten Minute
	static byte vorMM;
	static byte vorJJ;
	static byte ms50;
	static long tries = 0;
	byte i;

	iTic += 5;  //5ms sind rum
	if (iTic > 3000){ //Kein Signal
		iTic = 3000;
	}
	//Eingang abfragen und filtern
	if (digitalRead(DI_DCF77)){ //Signalabsenkung liefert High für 100ms oder 200ms
		iTiefpass += 5;
		digitalWrite(13, HIGH);
		if (iTiefpass > 200) iTiefpass = 200;  //auf 200ms begrenzen
	}
	else{
		digitalWrite(13, LOW);
		iTiefpass -= 5;
		if (iTiefpass < 0){
			iTiefpass = 0;
			bSteigend = 1;
		}
	}


	//Minutenanfang erkennen
	if (iTic > 1500){ //Minutensignal: mehr als 1s keine Absenkung
		dcfSekunde = 59;
	}
	else
		//Signallänge bestimmen und im Bitfeld eintragen
		if (160 == iTic){ //160ms  Tiefpass ist entweder +100-60=40 oder +100+60=160
			//pulsdauer=iTiefpass;
			if (iTiefpass > 90){
				bBit[dcfSekunde] = 1;
			}
			else{
				bBit[dcfSekunde] = 0;
			}

			if ((iTiefpass < 5) || ((iTiefpass > 60) && (iTiefpass < 100))){ //Unplausible Signaldauer
				if (stoerung < 4) stoerung++;
			}
			else{
				if (stoerung > 0) stoerung--;
			}
		}
	//Sekundenanfang erkennen
	if ((60 == iTiefpass) && bSteigend){ //60ms seit Pulsbeginn
		bSteigend = 0;
		iTic = 60;
		dcfSekunde++;
		if (dcfSekunde > 59){  //Jetzt ist Minutenanfang
			dcfSekunde = 0;
			if (0 == startphase){ //beim 1. Minutenanfang nach Einschalten ist die Zeit noch nicht übertragen
				zeitUebernehmen = 1;
			}

			//Auswertung zum Minutenanfang
			if (zeitUebernehmen){
				if (bBit[16]){
					nZeitumstellung++;
				}
				else{
					if (nZeitumstellung) nZeitumstellung--;
				}
				if (bBit[17]){
					if (sommerzeit < 60) sommerzeit++;
				}
				else{
					if (sommerzeit) sommerzeit--;
				}
				if (bBit[18]){
					if (winterzeit < 60) winterzeit++;
				}
				else{
					if (winterzeit) winterzeit--;
				}
				bChecksum = bBit[21] + bBit[22] + bBit[23] + bBit[24] + bBit[25] + bBit[26] + bBit[27] + bBit[28]; //Minute 
				if (0 == (bChecksum % 2)){ //P1
					bChecksum = bBit[29] + bBit[30] + bBit[31] + bBit[32] + bBit[33] + bBit[34] + bBit[35];        //Stunde  
					if (0 == (bChecksum % 2)){ //P2
						if (bBit[20]){ //Startbit; immer gesetzt
							bChecksum = 0;
							for (i = 36; i < 59; i++){ //Checksumme über Datum 
								bChecksum += bBit[i];
							}
							if (0 == (bChecksum % 2)){ //P3
								neuesDatum = 0;
								keinEmpfang = 0;
								tag = bBit[36] + bBit[37] * 2 + bBit[38] * 4 + bBit[39] * 8 + bBit[40] * 10 + bBit[41] * 20;
								wtag = bBit[42] + bBit[43] * 2 + bBit[44] * 4;
								monat = bBit[45] + bBit[46] * 2 + bBit[47] * 4 + bBit[48] * 8;
								jahr = bBit[50] + bBit[51] * 2 + bBit[52] * 4 + bBit[53] * 8 + bBit[54] * 10 + bBit[55] * 20 + bBit[56] * 40 + bBit[57] * 80;
								if (zeitUebernehmen){
									//Übernehme Zeit
									zeitUebernehmen = 0;
									hh = bBit[29] + bBit[30] * 2 + bBit[31] * 4 + bBit[32] * 8 + bBit[33] * 10 + bBit[34] * 20;
									mm = bBit[21] + bBit[22] * 2 + bBit[23] * 4 + bBit[24] * 8 + bBit[25] * 10 + bBit[26] * 20 + bBit[27] * 40;
									ss = 0;
									//Ist diese Zeit genau eine Minute später als vor einer Minute? (zusätzliche Plausibilitätscheck)
									vorMM++;
									if (vorMM > 59){
										vorMM = 0;
										vorHH++;
										if (vorHH > 23) vorHH = 0;
									}
									if ((vorHH == hh)
										&& (vorMM == mm)
										&& (vorJJ == jahr) //Beim Jahreswechsel muss die Uhr ja nicht um Mitternacht verdreht werden
										&& (jahr > 14)){       //Wir haben 2015 - und mehr als bis zum Jahr 2099 soll die Uhr gar nicht laufen
										erfolgreichdecodiert = 1;
									}
									vorHH = hh;
									vorMM = mm;
									vorJJ = jahr;
								}
							}//P3
						}//Bit20
					}//P2
				}//P1
			}//zeitUebernehmen
			if (startphase) startphase = 0;
			for (i = 0; i < 60; i++){
				bBit[i] = 0;
			}
		}//(dcfSekunde>59)

		//Interne Uhr feintunen
		//Hundertstel Sekunden hs sollte auf 6 stehen , wenn die Uhr richtig läuft (60ms seit steigender Flanke)
		//Nur machen, wenn Empfang OK
		if ((keinEmpfang < 2) && (0 == stoerung)){
			if ((hs > 6) && (hs < 56)) {
				hs--; //Uhr etwas zurückdrehen
			}
			else{
				if (hs != 6){
					hs++; //Uhr etwas vor drehen. Überlauf wird weiter unten automatisch behandelt.
				}
			}
		}
	}//Sekundenanfang (60ms)

	//Intere Uhr weiterdrehen
	bToggle = !bToggle;
	if (bToggle){ //10ms sind rum
		hs++; //Hundertstel Sekunden
		if (hs > 99){ //1s ist rum
			hs -= 100; //Nicht auf 0 setzen, um Überlauf bei Uhr vordrehen (Feintuning) zu berücksichtigen
			uhrSS++;
			if (uhrSS > 59){
				uhrSS = 0;
				uhrMM++;
				if (keinEmpfang < 100){ //Wird bei Empfang mit korreten Prüfsummen auf 0 gesetzt	
					keinEmpfang++;
				}
				//       if (!zeitEmpfangen && (keinEmpfang > 5)){
				//         displayEnable(0);
				//       }
				if (zeitEmpfangen && (keinEmpfang > 15) && (uhrHH > 2) && (uhrHH < 6)) {
					displayEnable(0);
				}
				if (uhrMM > 59){
					uhrMM = 0;
					uhrHH++;
					if (uhrHH > 23){
						uhrHH = 0;
						neuesDatum = 1; //Datum wird ja nicht angezeigt. Aber wenn ich das mal einbauen will, habe ich hiermit den Tagesbeginn.
					}
					//Zeitumstellung?
					if ((nZeitumstellung > 45) && (uhrHH >= 2) && (uhrHH <= 3)){ //"nZeitumstellung" wurde in der Stunde vor der Umstellung jede Minute hochgezählt
						if (sommerzeit > winterzeit){ //MESZ->MEZ
							uhrHH--;
						}
						else{ //MEZ->MESZ
							uhrHH++;
						}
					}
				}//uhrMM>59
				anzeigen();
			}//uhrSS>59
			if (erfolgreichdecodiert){ //Zeit übernehmen (wenn vorhanden)
				erfolgreichdecodiert = 0;
				zeitEmpfangen = 1;
				uhrHH = hh;
				uhrMM = mm;
				uhrSS = 0;
				displayEnable(1);
			}

		}//hs>99
		if (!zeitEmpfangen){
			ms50++;
			if (ms50 > 4){ //50 ms sind rum
				ms50 = 0;
				//scope();
			}
		}
	}//Toggle
}//decodeDCF77


//-----------------------------------------------------------------------------
void setup() {
	//-----------------------------------------------------------------------------
	/* The MAX72XX is in power-saving mode on startup, we have to do a wakeup call */
	displayEnable(1);

	/* Set the brightness to a low values */
	led.setIntensity(0, 2);
	led.setIntensity(1, 2);

	/* and clear the display */
	led.clearDisplay(0);
	led.clearDisplay(1);
	pinMode(DI_DCF77, INPUT_PULLUP);
	pinMode(DO_DCF_POWERSUPPLY, OUTPUT);
	pinMode(13, OUTPUT);
	//pinMode(DO_DCF_DISABLE, OUTPUT);


	digitalWrite(DO_DCF_POWERSUPPLY, HIGH);
	//digitalWrite(DO_DCF_DISABLE, HIGH);      
	//delay(1000);

	ledtest();

	//digitalWrite(DO_DCF_DISABLE, LOW);     //Fallende Flanke auf PON startet den Empfänger  
	/* Set the brightness to a high values */
	led.setIntensity(0, 15);
	led.setIntensity(1, 15);
	//Display off
	displayEnable(0);
}




//-----------------------------------------------------------------------------
void loop() {
	//-----------------------------------------------------------------------------
	static uint32_t previousMillis = 0;
	static uint32_t currentMillis = 0;

	currentMillis = millis();
	if (currentMillis - previousMillis >= 5){ //use unsigned subtraction to correctly handle rollover of millis
		previousMillis = currentMillis;
		decodeDCF77();
	}
}
