#include "MicroBit.h"
#include "TimedInterruptIn.h"
#include "pxt.h"
using namespace pxt;

class microbitp : public MicroBitComponent
{
  public:
    void *pin;
    PinCapability capability;
    uint8_t pullMode;
	PinName name;

    void disconnect(){
        if (status & IO_STATUS_DIGITAL_IN)
            delete ((DigitalIn *)pin);
        if (status & IO_STATUS_DIGITAL_OUT)
            delete ((DigitalOut *)pin);
        if (status & IO_STATUS_ANALOG_IN){
            NRF_ADC->ENABLE = ADC_ENABLE_ENABLE_Disabled; // forcibly disable the ADC - BUG in mbed....
            delete ((AnalogIn *)pin);
        }
        if (status & IO_STATUS_ANALOG_OUT)
            delete ((DynamicPwm *)pin);
        if (status & IO_STATUS_TOUCH_IN)
            delete ((MicroBitButton *)pin);
        if ((status & IO_STATUS_EVENT_ON_EDGE) || (status & IO_STATUS_EVENT_PULSE_ON_EDGE))
            delete ((TimedInterruptIn *)pin);
        this->pin = NULL;
        this->status = 0;
    }

    microbitp(int id, PinName name, PinCapability capability){
        //set mandatory attributes
        this->id = id;
        this->name = name;
        this->capability = capability;
        this->pullMode = MICROBIT_DEFAULT_PULLMODE;
        this->status = 0x00;
        this->pin = NULL;
    }

    int setDigitalValue(int value){
        // Check if this pin has a digital mode...
        if(!(PIN_CAPABILITY_DIGITAL_OUT & capability))
            return MICROBIT_NOT_SUPPORTED;

        // Ensure we have a valid value.
        if (value < 0 || value > 1)
            return MICROBIT_INVALID_PARAMETER;

        // Move into a Digital input state if necessary.
        if (!(status & IO_STATUS_DIGITAL_OUT)){
            disconnect();
            pin = new DigitalOut(name);
            status |= IO_STATUS_DIGITAL_OUT;
        }

        // Write the value.
        ((DigitalOut *)pin)->write(value);

        return MICROBIT_OK;
    }

    int getDigitalValue(){
        //check if this pin has a digital mode...
        if(!(PIN_CAPABILITY_DIGITAL_IN & capability))
            return MICROBIT_NOT_SUPPORTED;

        // Move into a Digital input state if necessary.
        if (!(status & (IO_STATUS_DIGITAL_IN | IO_STATUS_EVENT_ON_EDGE | IO_STATUS_EVENT_PULSE_ON_EDGE)))
        {
//            disconnect();
//            pin = new DigitalIn(name, (PinMode)pullMode);
			((DigitalIn *)pin)->mode(PullNone);
            status |= IO_STATUS_DIGITAL_IN;
        }

        if(status & (IO_STATUS_EVENT_ON_EDGE | IO_STATUS_EVENT_PULSE_ON_EDGE))
            return ((TimedInterruptIn *)pin)->read();

        return ((DigitalIn *)pin)->read();
    }
};

namespace DS1820 {
MicroBit uBit;
microbitp  pin0(7, MICROBIT_PIN_P0, PIN_CAPABILITY_ALL);
microbitp  pin1(8, MICROBIT_PIN_P1, PIN_CAPABILITY_ALL);
microbitp  pin2(9, MICROBIT_PIN_P2, PIN_CAPABILITY_ALL);


uint8_t init() {
    pin1.setDigitalValue(0);
    for (volatile uint16_t i = 0; i < 600; i++);
    pin1.setDigitalValue(1);
    for (volatile uint8_t i = 0; i < 30; i++);
    int b = pin1.getDigitalValue();
    for (volatile uint16_t i = 0; i < 600; i++);
    return b;
}

void writeBit(int b) {
    int delay1, delay2;
    if (b == 1) {
        delay1 = 1;
        delay2 = 80;
    } else {
        delay1 = 75;
        delay2 = 6;
    }
    pin1.setDigitalValue(0);
    for (uint8_t i = 1; i < delay1; i++);
    pin1.setDigitalValue(1);
    for (uint8_t i = 1; i < delay2; i++);
}

void writeByte(int byte) {
    int i;
    for (i = 0; i < 8; i++) {
        if (byte & 1) {
            writeBit(1);
        } else {
            writeBit(0);
        }
        byte = byte >> 1;
    }
}

int readBit() {
    volatile int i;
    pin1.setDigitalValue(0);
    pin1.setDigitalValue(1);
    int b = pin1.getDigitalValue();
    for (i = 1; i < 60; i++);
    return b;
}

int convert() {
    volatile int i;
    int j;
    writeByte(0x44);
    for (j = 1; j < 1000; j++) {
        for (i = 1; i < 900; i++) {
    };
    if (readBit() == 1)
        break;
    };
    return (j);
}

int readByte() {
    int byte = 0;
    int i;
    for (i = 0; i < 8; i++) {
        byte = byte | readBit() << i;
    };
    return byte;
}

//%
uint16_t Temperature() {
    init();
    writeByte(0xCC);
    convert();
    init();
    writeByte(0xCC);
    writeByte(0xBE);
    int b1 = readByte();
    int b2 = readByte();
    uint16_t temp = (b2 << 8 | b1)*100/16;
//uBit.serial.printf("1: %d\n",b1);
//uBit.serial.printf("2: %d\n",b2);
    return temp;
}
}
/*
int main() {
  	uBit.init();
	char t[20] = "";
	while(1){
		int temp = Temperature();
		int data1 = temp/100;
		int data2 = temp%100;
		if(data2 < 10)
		sprintf(t,"temp : %d.0%d",data1,data2);
		else
		sprintf(t,"temp : %d.%d",data1,data2);
		uBit.serial.printf("%s\n",t);
		uBit.sleep(1000);
	}
}
*/
