
#!/bin/bash
weather_data=$(curl "http://api.weatherapi.com/v1/current.json?key=d8170b5ec9f84e669bf123724240311&q=karachi")
escaped_data=$(echo "$weather_data" | sed 's/"/\"/g')
echo $escaped_data >> RawData.json
echo >> RawData.json
echo $escaped_data > TemporaryFile.txt
