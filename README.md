# SerialPlot

Simple Qt-based serial plotter with an integrated serial monitor.

## Features

- Support for up to 15 different channels  
- Export plots as images  
- Automatic axis range adjustment  
- Dark mode support for comfortable viewing
- Cross-plattform compatibility 

## Screenshot

![SerialPlot in action](https://github.com/user-attachments/assets/8df2c120-78ea-4dee-8bf5-24cce4d44d6a)

## Usage
To print data into different channels, seperate your data with commas, e.g.:

```c++
float channel_1 = sin(count);
float channel_2 = cos(count);
Serial.println(String(channel_1) + "," + String(channel_2));
```
    

## License

This project is licensed under the [GNU General Public License version 3](LICENSE).

It uses Qt libraries licensed under the GNU Lesser General Public License version 3 (LGPLv3).  
For details, see [https://www.gnu.org/licenses/lgpl-3.0.html](https://www.gnu.org/licenses/lgpl-3.0.html).
