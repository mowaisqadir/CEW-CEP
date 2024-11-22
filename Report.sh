#!/bin/bash
JSON_FILE="Data.json"

if [[ ! -f "$JSON_FILE" ]]; then
  echo "Error: JSON file not found!"
  exit 1
fi

current_day=$(date +%u)

format_data() {
  jq -r '
    .[] |
    "Timestamp: \(.timestamp)\n" +
    "Location:\n  City: \(.location_details.city)\n  Area: \(.location_details.area)\n  Nation: \(.location_details.nation)\n" +
    "Weather:\n  Temperature (°C): \(.weather_conditions.temperature)\n  Humidity: \(.weather_conditions.humidity)%\n" +
    "---------------------------------------------\n"
  ' "$1"
}

if [[ "$current_day" -eq 1 ]]; then
  last_monday=$(date -d 'last Monday' +%Y-%m-%d)
  last_week_data=$(jq --arg last_monday "$last_monday" '
    [.[] | select(.timestamp >= $last_monday)]' "$JSON_FILE")

  if [[ "$last_week_data" == "[]" ]]; then
    echo "No data found for the past week. Displaying all available data:"
    format_data "$JSON_FILE"
  else
    echo "$last_week_data" | jq '.' > temp.json
    format_data "temp.json"
    rm -f temp.json
  fi
else
  echo "No report generated as it's not Monday."
  exit 0
fi
