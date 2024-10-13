#ifndef CIRCULAR_RING_H
#define CIRCULAR_RING_H
/*
 * Code for circular rings
 */
#define NB_DATA_TEMP 60 //Number of points in graphic

 
byte temperatures[NB_DATA_TEMP] = {0};
byte humidities[NB_DATA_TEMP] = {0};
byte references[NB_DATA_TEMP] = {0};


inline String concatenateArray(byte *array, int length) {

  String tmpString ="[";
  for (int i=0;i<length;i++) {
    tmpString += String(array[i])+",";
  }
  tmpString += "]";

  return tmpString;
}


inline bool pushArrayDataTemperature(int index, int value, int maxLength) { 

  static bool firstWriting = false;

  if(!firstWriting && index==(maxLength-1)) 
  {
    temperatures[index] = value;
    firstWriting = true;
  }
  else {
    
    if(index==(maxLength-1)) 
    { 
      for(int i=0;i<(maxLength-1);i++) 
      {
        temperatures[i] = temperatures[i+1];
      }
      temperatures[maxLength-1] = value; 
    }
    else {
      temperatures[index] = value;
    }
  }

  return false;
  
}


inline bool pushArrayDataHumidity(int index, int value, int maxLength) { 

  static bool firstWriting = false;

  if(!firstWriting && index==(maxLength-1)) 
  {
    humidities[index] = value;
    firstWriting = true;
  }
  else {
    
    if(index==(maxLength-1)) 
    { 
      for(int i=0;i<(maxLength-1);i++) 
      {
        humidities[i] = humidities[i+1];
      }
      humidities[maxLength-1] = value; 
    }
    else {
      humidities[index] = value;
    }
  }

  return false;
  
}

inline void updateRings(int *current_index, int maxIndex, float temperature, float humidity) 

{
  if(*current_index<maxIndex) {
     pushArrayDataTemperature(*current_index, int(temperature), maxIndex);
     pushArrayDataHumidity(*current_index, int(humidity), maxIndex);
     (*current_index)++;
  }
  else {
    pushArrayDataTemperature(maxIndex-1, int(temperature), maxIndex);
    pushArrayDataHumidity(maxIndex-1, int(humidity), maxIndex);
    *current_index = maxIndex-1;
  }
}

#endif
