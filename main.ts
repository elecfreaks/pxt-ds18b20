//% color=#5042f4 icon="\uf2c9"
namespace DS18B20 {
    /**
     * Reads the temperature from the one-wire temperature sensor.
     * Returns a 4 digit number. value should be divided by 100 to get 
     *temperature in hundreths of a degree centigrade. 
     * block="Temperature(C)"
     */
     
    //% shim=DS18B20::Temperature
    export function Temperature(p: number): number {
        // Fake function for simulator
        return 0
    }
    
    //% weight=10 blockId="Temperature_number" 
    //% block="Read 18B20 sensor temperature to %pin "
    export function TemperatureNumber(pin: AnalogPin): number {
        // Fake function for simulator
        return Temperature(p)
    }
}
