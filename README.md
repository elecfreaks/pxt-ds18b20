# DS18B20 Temperature Sensor for Microbit 

## Table of Contents

* [Summary](#summary)
* [Blocks](#blocks)
* [Example](#example)
* [License](#license)

## Summary
 
DS18B20 module, support single bus control, read temperature by pin1.

## Blocks
readTemperature<br>
![image](https://github.com/mengbishu/pxt-ds18b20/blob/master/image/temp.png)<br>

## Example
Example for JavaScript
```
basic.forever(() => {
    serial.writeLine("temp : "+DS18B20.Temperature())
})
```

## License

GUI
## Supported targets

* for PXT/microbit
(The metadata above is needed for package search.)


