#include <Arduino.h>
#include "HX711.h"

HX711 First_loadcell(23, 18);
HX711 Second_loadcell(19, 21);

float calibration_factor = 215.f; // this calibration factor is adjusted according to my 1 kg load cell
float units;
float units1;
float sum_unit;
float units_old;
int time_measuring = 0;
float val_min = 0;
float val_max = 0;
float val_min1 = 0;
float val_max1 = 0;
float resoluation = 0.25;
float tare_2;

// float load = 0;
// int32_t load_val;

bool is_process_done = true;

void reset_minmax()
{
  val_min = units;
  val_max = units;

  val_min1 = units1;
  val_max1 = units1;
}

void tare_tare_tare()
{
  First_loadcell.tare();
  Second_loadcell.tare();

  reset_minmax();

  tare_2 = 0;
}

void setup()
{
  Serial.begin(115200);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  First_loadcell.set_scale();
  Second_loadcell.set_scale();

  long zero_factor = First_loadcell.read_average();   // Get a baseline reading
  long zero_factor1 = Second_loadcell.read_average(); // Get a baseline reading

  Serial.print("Zero factor: "); // This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
  Serial.print("Zero factor1: ");
  Serial.println(zero_factor1);

  time_measuring = millis();
}

void loop()
{

  First_loadcell.set_scale(calibration_factor); // Adjust to this calibration factor
  Second_loadcell.set_scale(calibration_factor);

  if (((millis() - time_measuring) < 360000))

  {

    Serial.print("Reading: ");
    units = First_loadcell.get_units(10);
    units1 = Second_loadcell.get_units(10);

    sum_unit = units1 + units;

    Serial.print("units: ");
    Serial.print(units);
    Serial.print(" grams | ");

    Serial.print("units1: ");
    Serial.print(units1);
    Serial.print(" grams | ");

    Serial.print("sum_unit: ");
    Serial.print(sum_unit);
    Serial.print(" grams | ");

    if (units > val_max)
    {
      val_max = units;
    }
    else if (units < val_min)
    {
      val_min = units;
    }
    if (units1 > val_max1)
    {
      val_max1 = units1;
    }
    else if (units1 < val_min1)
    {
      val_min1 = units1;
    }

    Serial.print(" grams | min: ");
    Serial.print(val_min);
    Serial.print(" grams | max: ");
    Serial.print(val_max);
    Serial.print(" grams | t_2: ");

    Serial.print(" grams | min1: ");
    Serial.print(val_min1);
    Serial.print(" grams | max: ");
    Serial.print(val_max1);
    Serial.print(" grams | t_2: ");

    Serial.print("  | ");

    Serial.print(" calibration_factor: ");
    Serial.print(calibration_factor);
    Serial.println();
  }
  else if (is_process_done)
  {
    Serial.println("bitti");
    is_process_done = false;
  }

  while (Serial.available() > 0)

  {
    char temp = Serial.read();
    if (temp == '+' || temp == 'a')
      calibration_factor += 1;
    else if (temp == '-' || temp == 'z')
      calibration_factor -= 1;
    else if (temp == '0')
    {
      First_loadcell.tare();
      Second_loadcell.tare();
    }

    else if (temp == '3')
    {
      reset_minmax();
    }

    is_process_done = true;

    time_measuring = millis();
  }
}