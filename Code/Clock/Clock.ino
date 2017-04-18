#include <Streaming.h>
#include <TimeLib.h>
#include <DS3232RTC.h>
#include <Adafruit_NeoPixel.h>
#include <Time.h>             //http://playground.arduino.cc/Code/Time
#include <Wire.h>             //http://arduino.cc/en/Reference/Wire
#define PIN 6
#define DEBUG_ENABLED
Adafruit_NeoPixel strip = Adafruit_NeoPixel(110, PIN, NEO_GRB + NEO_KHZ800);

unsigned int const TXT_ES_IST[] {100, 99, 79, 60, 59};
unsigned int const TXT_FUENF1[] {39, 20, 19, 0};

unsigned int const TXT_ZEHN0[] {101, 98, 81, 78};
unsigned int const TXT_ZWANZIG[] {61, 58, 41, 38, 21, 18, 1};

unsigned int const TXT_DREI1[] {102, 97, 82, 77};
unsigned int const TXT_VIERTEL[] {62, 57, 42, 37, 22, 17, 2};

unsigned int const TXT_NACH[] {83, 76, 63, 56};
unsigned int const TXT_VOR[] {43, 36, 23};

unsigned int const TXT_HALB[] {104, 59, 84, 75};
unsigned int const TXT_ZWOELF[] {55, 44, 35, 24, 15};

unsigned int const TXT_ZWEI[] {105, 94, 85, 74};
unsigned int const TXT_EIN[] {85, 74, 65};
unsigned int const TXT_EINS[] {85, 74, 64, 54};
unsigned int const TXT_SIEBEN[] {54, 45, 34, 25, 14, 5};

unsigned int const TXT_DREI[] {93, 86, 73, 66};
unsigned int const TXT_FUENF[] {33, 26, 13, 6};

unsigned int const TXT_ELF[] {107, 92, 87};
unsigned int const TXT_NEUN[] {72, 67, 52, 47};
unsigned int const TXT_VIER[] {32, 27, 12, 7};

unsigned int const TXT_ACHT[] {91, 88, 79, 68};
unsigned int const TXT_ZEHN[] {51, 48, 31, 28};

unsigned int const TXT_SECHS[] {90, 98, 70, 69, 50};
unsigned int const TXT_UHR[] {29, 10, 9};

unsigned int const TXT_ERROR[] = { 100, 97, 37, 36, 23 };
unsigned int const TXT_ZEIT[] {55, 85, 74, 68};

//unsigned int const hourMapping[] = { TXT_ZWOELF, TXT_EINS, TXT_ZWEI, TXT_DREI, TXT_VIER, TXT_FUENF, TXT_SECHS, TXT_SIEBEN, TXT_ACHT, TXT_NEUN, TXT_ZEHN, TXT_ELF};

unsigned int const COLOUR[] {255, 0, 0}; // R,G,B
#define BRIGHTNESS 20
#define NUMBER_OF_PIXELS 110
unsigned int Pixel[110];

void log(const char * msg)
{
#ifdef DEBUG_ENABLED
{
	Serial << msg;
}
#endif;
}

void updatePixels()
{
	strip.setBrightness(BRIGHTNESS);

	for (unsigned int p = 0; p < NUMBER_OF_PIXELS; p++)
	{
		if (Pixel[p] > 0)
		{
			strip.setPixelColor(p, strip.Color(COLOUR[0], COLOUR[1], COLOUR[2]));
		}
		else
		{
			strip.setPixelColor(p, strip.Color(0, 0, 0));
		}
	}

	strip.show();
	delay(50);
}

void setMatrix(unsigned int const values[], unsigned int size)
{
	for (int v = 0; v < size; v++)
	{
		Pixel[values[v]] = 1;
	}
}

void resetMatrix()
{
	for (unsigned int p = 0; p < NUMBER_OF_PIXELS; p++)
	{
		Pixel[p] = 0;
	}
}

void setup()
{
	Serial.begin(115200);

	resetMatrix();
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'


	//setSyncProvider() causes the Time library to synchronize with the
	//external RTC by calling RTC.get() every five minutes by default.
	setSyncProvider(RTC.get);
	log("RTC Sync");

	if (timeStatus() != timeSet)
	{
		log("FAIL!");
		setMatrix(TXT_ERROR, sizeof(TXT_ERROR) / 2);
		setMatrix(TXT_ZEIT, sizeof(TXT_ZEIT) / 2);
		updatePixels();
	}
	log(endl);
}

void setMatrixTime(int hour, int minute, int second)
{
	resetMatrix();

	setMatrix(TXT_ES_IST, sizeof(TXT_ES_IST) / 2);

	if (minute < 5)
	{
		setMatrix(TXT_UHR, sizeof(TXT_UHR) / 2);
	}
	else if (minute < 10)
	{
		setMatrix(TXT_FUENF1, sizeof(TXT_FUENF1) / 2);
		setMatrix(TXT_NACH, sizeof(TXT_NACH) / 2);
	}
	else if (minute < 15)
	{
		setMatrix(TXT_ZEHN0, sizeof(TXT_ZEHN0) / 2);
		setMatrix(TXT_NACH, sizeof(TXT_NACH) / 2);
	}
	else if (minute < 20)
	{
		setMatrix(TXT_VIERTEL, sizeof(TXT_VIERTEL) / 2);
		setMatrix(TXT_NACH, sizeof(TXT_NACH) / 2);
	}
	else if (minute < 25)
	{
		setMatrix(TXT_ZWANZIG, sizeof(TXT_ZWANZIG) / 2);
		setMatrix(TXT_NACH, sizeof(TXT_NACH) / 2);
	}
	else if (minute < 30)
	{
		setMatrix(TXT_FUENF1, sizeof(TXT_FUENF1) / 2);
		setMatrix(TXT_VOR, sizeof(TXT_VOR) / 2);
		setMatrix(TXT_HALB, sizeof(TXT_HALB) / 2);
	}
	else if (minute < 35)
	{
		setMatrix(TXT_HALB, sizeof(TXT_HALB) / 2);
	}
	else if (minute < 40)
	{
		setMatrix(TXT_FUENF1, sizeof(TXT_FUENF1) / 2);
		setMatrix(TXT_NACH, sizeof(TXT_NACH) / 2);
		setMatrix(TXT_HALB, sizeof(TXT_HALB) / 2);
	}
	else if (minute < 45)
	{
		setMatrix(TXT_ZWANZIG, sizeof(TXT_ZWANZIG) / 2);
		setMatrix(TXT_VOR, sizeof(TXT_VOR) / 2);
	}
	else if (minute < 50)
	{
		setMatrix(TXT_VIERTEL, sizeof(TXT_VIERTEL) / 2);
		setMatrix(TXT_VOR, sizeof(TXT_VOR) / 2);
	}
	//	else if(uhrMM < 50){TEXT_DREI1;   TEXT_VIERTEL;				    }
	else if (minute < 55)
	{
		setMatrix(TXT_ZEHN0, sizeof(TXT_ZEHN0) / 2);
		setMatrix(TXT_VOR, sizeof(TXT_VOR) / 2);
	}
	else if (minute < 60)
	{
		setMatrix(TXT_FUENF1, sizeof(TXT_FUENF1) / 2);
		setMatrix(TXT_VOR, sizeof(TXT_VOR) / 2);
	}

	int hr = hour;
	if (minute >= 25)
	{
		hr++;
	}
	hr = hr % 12;

	switch (hr)
	{
	case  0:
		setMatrix(TXT_ZWOELF, sizeof(TXT_ZWOELF) / 2);
		break;
	case  1:
		if (minute < 5)
		{
			setMatrix(TXT_EIN, sizeof(TXT_EIN) / 2);
		}
		else
		{
			setMatrix(TXT_EINS, sizeof(TXT_EINS) / 2);
		}
		break;
	case  2:
		setMatrix(TXT_ZWEI, sizeof(TXT_ZWEI) / 2);
		break;
	case  3:
		setMatrix(TXT_DREI, sizeof(TXT_DREI) / 2);
		break;
	case  4:
		setMatrix(TXT_VIER, sizeof(TXT_VIER) / 2);
		break;
	case  5:
		setMatrix(TXT_FUENF, sizeof(TXT_FUENF) / 2);
		break;
	case  6:
		setMatrix(TXT_SECHS, sizeof(TXT_SECHS) / 2);
		break;
	case  7:
		setMatrix(TXT_SIEBEN, sizeof(TXT_SIEBEN) / 2);
		break;
	case  8:
		setMatrix(TXT_ACHT, sizeof(TXT_ACHT) / 2);
		break;
	case  9:
		setMatrix(TXT_NEUN, sizeof(TXT_NEUN) / 2);
		break;
	case 10:
		setMatrix(TXT_ZEHN, sizeof(TXT_ZEHN) / 2);
		break;
	case 11:
		setMatrix(TXT_ELF, sizeof(TXT_ELF) / 2);
		break;
	};

	updatePixels();
}

void checkAndSetInputtime()
{
	//check for input to set the RTC, minimum length is 12, i.e. yy,m,d,h,m,s
	if (Serial.available() >= 12)
	{
		//note that the tmElements_t Year member is an offset from 1970,
		//but the RTC wants the last two digits of the calendar year.
		//use the convenience macros from Time.h to do the conversions.
		int y = Serial.parseInt();
		tmElements_t tm;

		if (y >= 1000)
		{
			tm.Year = CalendarYrToTm(y);
		}
		else    //(y < 100)
		{
			tm.Year = y2kYearToTm(y);
		}

		tm.Month = Serial.parseInt();
		tm.Day = Serial.parseInt();
		tm.Hour = Serial.parseInt();
		tm.Minute = Serial.parseInt();
		tm.Second = Serial.parseInt();
		
		time_t t = makeTime(tm);
		RTC.set(t);        //use the time_t value to ensure correct weekday is set
		setTime(t);
		log("RTC set to: ");
		//dump any extraneous input
		while (Serial.available() > 0)
		{
			Serial.read();
		}
	}
}

void loop()
{
	checkAndSetInputtime();

	time_t t = RTC.get();
	log(hour());
	log(':');
	log(minute());
	log(':');
	log(second());
	log(endl);

	setMatrixTime(hour(), minute(), second());
	delay(1000);
}