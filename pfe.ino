#include <avr/io.h>
#include <avr/interrupt.h>

// Look up tables with 200 entries each, normalised to have max value of 1600 which is the period of the PWM loaded into register ICR1.
int lookUp1[] = {50, 100, 151, 201, 250, 300, 349, 398, 446, 494, 542, 589, 635, 681, 726, 771, 814, 857, 899, 940, 981, 1020, 1058, 1095, 1131, 1166, 1200, 1233, 1264, 1294, 1323, 1351, 1377, 1402, 1426, 1448, 1468, 1488, 1505, 1522, 1536, 1550, 1561, 1572, 1580, 1587, 1593, 1597, 1599, 1600, 1599, 1597, 1593, 1587, 1580, 1572, 1561, 1550, 1536, 1522, 1505, 1488, 1468, 1448, 1426, 1402, 1377, 1351, 1323, 1294, 1264, 1233, 1200, 1166, 1131, 1095, 1058, 1020, 981, 940, 899, 857, 814, 771, 726, 681, 635, 589, 542, 494, 446, 398, 349, 300, 250, 201, 151, 100, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int lookUp2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 100, 151, 201, 250, 300, 349, 398, 446, 494, 542, 589, 635, 681, 726, 771, 814, 857, 899, 940, 981, 1020, 1058, 1095, 1131, 1166, 1200, 1233, 1264, 1294, 1323, 1351, 1377, 1402, 1426, 1448, 1468, 1488, 1505, 1522, 1536, 1550, 1561, 1572, 1580, 1587, 1593, 1597, 1599, 1600, 1599, 1597, 1593, 1587, 1580, 1572, 1561, 1550, 1536, 1522, 1505, 1488, 1468, 1448, 1426, 1402, 1377, 1351, 1323, 1294, 1264, 1233, 1200, 1166, 1131, 1095, 1058, 1020, 981, 940, 899, 857, 814, 771, 726, 681, 635, 589, 542, 494, 446, 398, 349, 300, 250, 201, 151, 100, 50, 0};

void setup()
{

    Serial.begin(9600);
}

void loop()
{
    int sensorValue = analogRead(A0);
    static int sensorValue2;
    if (sensorValue > sensorValue2 * 1.01 || sensorValue < sensorValue2 * 0.99)
    {
        sensorValue2 = sensorValue;
        setFreq(map(sensorValue, 0, 1023, 5, 300));

        Serial.print(phaseinc >> 23);
        Serial.print(".");
        Serial.print(phaseinc & 0x007FFFFF);
        Serial.print("\n");
    }
}

ISR(TIMER3_OVF_vect)
{
    static int num;
    // change duty-cycle every period.
    OCR3A = lookUp1[num];
    OCR3B = lookUp2[num];

    if (++num >= 200)
    {            // Pre-increment num then check it's below 200.
        num = 0; // Reset num.
    }
}
ISR(TIMER4_OVF_vect)
{
    static int num;
    // change duty-cycle every period.
    OCR4A = lookUp1[num];
    OCR4B = lookUp2[num];

    if (++num >= 200)
    {            // Pre-increment num then check it's below 200.
        num = 0; // Reset num.
    }
}
ISR(TIMER1_OVF_vect)
{
    static int num;
    // change duty-cycle every period.
    OCR1A = lookUp1[num];
    OCR1B = lookUp2[num];

    if (++num >= 200)
    {            // Pre-increment num then check it's below 200.
        num = 0; // Reset num.
    }
}

void registerInit(void)
{
    // Register initilisation, see datasheet for more detail.
    TCCR3A = 0b10100010;
    TCCR3B = 0b00011001;
    TIMSK3 = 0b00000001;
    ICR3 = 1600;
    TCCR4A = 0b10100010;
    TCCR4B = 0b00011001;
    TIMSK4 = 0b00000001;
    ICR4 = 1600;
    TCCR1A = 0b10100010;
    TCCR1B = 0b00011001;
    TIMSK1 = 0b00000001;
    ICR1 = 1600;
    sei();

    pinMode(5, OUTPUT);
    pinMode(2, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
}