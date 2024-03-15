/***********************************************************************
 * Source File:
 *    PHYSICS
 * Author:
 *    Br. Helfrich, Ashlee Hart, Emilay Raventos, chatGPT
 * Summary:
 *    Laws of motion, effects of gravity, wind resistence, etc.
 ************************************************************************/

#include "physics.h"  // for the prototypes
#include <map>          // for linear interpolation
#include <math.h>       // for PI, sin, and cos

using namespace std;

 /*********************************************************
 * LINEAR INTERPOLATION
 * From a list of domains and ranges, linear interpolate
 *********************************************************/
double linearInterpolation(const Mapping mapping[], int numMapping, double domain)
{
   // If the domain is less than the lowest value in the table, return the lowest range
   if (domain <= mapping[0].domain)
      return mapping[0].range;

   // If the domain is greater than the highest value in the table, return the highest range
   if (domain >= mapping[numMapping - 1].domain)
      return mapping[numMapping - 1].range;

   // Find the two nearest mappings for the given domain
   int index = 0;
   while (index < numMapping && domain > mapping[index].domain)
      index++;

   // Perform linear interpolation between the two nearest mappings
   double domainDiff = domain - mapping[index - 1].domain;
   double rangeDiff = mapping[index].range - mapping[index - 1].range;
   double ratio = domainDiff / (mapping[index].domain - mapping[index - 1].domain);
   double interpolatedRange = mapping[index - 1].range + (rangeDiff * ratio);

   return interpolatedRange;
}

/*********************************
* LOOKUP VALUE
********************************/
double linearInterpolation(const std::map<double, double>& table, const double& value)
{
   if (value < table.begin()->first)
   {
      return table.begin()->second;
   }
   else if (value > table.rbegin()->first)
   {
      return table.rbegin()->second;
   }

   for (auto i = table.begin(); i != table.end(); i++)
   {
      if (i->first == value)
      {
         return i->second;
      }
      else if (i->first > value)
      {
         // ---> allows you to pass an array as an argument supposedly. 
         return linearInterpolation(i->first, i->second, i--->first, i->second, value);
      }
   }
   return 0.0;
} 

/*********************************************************
 * GRAVITY FROM ALTITUDE
 * Determine gravity coefficient based on the altitude
 *********************************************************/
double gravityFromAltitude(double altitude)
{
   std::map<double, double> const gravityChart
   {
         {    0.0, 9.807}, { 1000.0, 9.804}, { 2000.0, 9.801}, { 3000.0, 9.797},
         { 4000.0, 9.794}, { 5000.0, 9.791}, { 6000.0, 9.788}, { 7000.0, 9.785},
         { 8000.0, 9.782}, { 9000.0, 9.779}, {10000.0, 9.776}, {15000.0, 9.761},
         {20000.0, 9.745}, {25000.0, 9.730}
   };

   // If we don't want to use our lookupValue method then we need to convert this chart to an array of maps
   //// Convert the std::map to an array of Mapping structures
   //Mapping gravityMapping[14];
   //int index = 0;
   //for (const auto& entry : gravityChart)
   //{
   //   gravityMapping[index].domain = entry.first;
   //   gravityMapping[index].range = entry.second;
   //   ++index;
   //}

   //// Perform linear interpolation
   //return linearInterpolation(gravityMapping, sizeof(gravityMapping) / sizeof(gravityMapping[0]), altitude);

   // using our old lookupValue function
   return linearInterpolation(gravityChart, altitude);
}

/*********************************************************
 * DENSITY FROM ALTITUDE
 * Determine the density of air based on the altitude
 *********************************************************/
double densityFromAltitude(double altitude)
{
   std::map<double, double> const densityChart
   {
         {    0.0, 1.2250000}, { 1000.0, 1.1120000}, { 2000.0, 1.0070000}, { 3000.0, 0.9093000},
         { 4000.0, 0.8194000}, { 5000.0, 0.7364000}, { 6000.0, 0.6601000}, { 7000.0, 0.5900000},
         { 8000.0, 0.5258000}, { 9000.0, 0.4671000}, {10000.0, 0.4135000}, {15000.0, 0.1948000},
         {20000.0, 0.0889100}, {25000.0, 0.0400800}, {30000.0, 0.0184100}, {40000.0, 0.0039960},
         {50000.0, 0.0010270}, {60000.0, 0.0003097}, {70000.0, 0.0000828}, {80000.0, 0.0000185}
   };

   //// Convert the std::map to an array of Mapping structures
   //Mapping densityMapping[20];
   //int index = 0;
   //for (const auto& entry : densityChart)
   //{
   //   densityMapping[index].domain = entry.first;
   //   densityMapping[index].range = entry.second;
   //   ++index;
   //}

   //// Perform linear interpolation
   //return linearInterpolation(densityMapping, sizeof(densityMapping) / sizeof(densityMapping[0]), altitude);

   return linearInterpolation(densityChart, altitude); 
}

/*********************************************************
 * SPEED OF SOUND FROM ALTITUDE
 * determine the speed of sound for a given altitude.
 ********************************************************/
double speedSoundFromAltitude(double altitude)
{
   std::map<double, double> const soundChart
   {
         {    0.0, 340.0}, { 1000.0, 336.0}, { 2000.0, 332.0}, { 3000.0, 328.0},
         { 4000.0, 324.0}, { 5000.0, 320.0}, { 6000.0, 316.0}, { 7000.0, 312.0},
         { 8000.0, 308.0}, { 9000.0, 303.0}, {10000.0, 299.0}, {15000.0, 295.0},
         {20000.0, 295.0}, {25000.0, 295.0}, {30000.0, 305.0}, {40000.0, 324.0}
   };
   return linearInterpolation(soundChart, altitude); 
}


/*********************************************************
 * DRAG FROM MACH
 * Determine the drag coefficient for a M795 shell given speed in Mach
 *********************************************************/
double dragFromMach(double speedMach)
{
   std::map<double, double> const dragChart
   {
      {0.300, 0.1629}, {0.500, 0.1659}, {0.700, 0.2031}, {0.890, 0.2597},
      {0.920, 0.3010}, {0.960, 0.3287}, {0.980, 0.4002}, {1.000, 0.4258},
      {1.020, 0.4335}, {1.060, 0.4483}, {1.240, 0.4064}, {1.530, 0.3663},
      {1.990, 0.2897}, {2.870, 0.2297}, {2.890, 0.2306}, {5.000, 0.2656}
   };
   return linearInterpolation(dragChart, speedMach);
}

