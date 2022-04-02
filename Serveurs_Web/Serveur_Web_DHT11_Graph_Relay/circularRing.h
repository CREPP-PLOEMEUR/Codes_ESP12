#ifndef CIRCULAR_RING_H
#define CIRCULAR_RING_H
/*
 * Code pour la gestion des tableaux circulaires
 * Auteur : Nicolas Le Guerroué
 */

 
byte temperatures[NB_DATA_TEMP] = {0};
byte humidities[NB_DATA_TEMP] = {0};
byte references[NB_DATA_TEMP] = {0};


inline String concatenateArray(byte *array, int length) { //Opération de concaténation pour l'affichage des graphiques

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
    { //Décale les élements et ajoute l'élement
      for(int i=0;i<(maxLength-1);i++) 
      {
        temperatures[i] = temperatures[i+1];
      }//End for
      temperatures[maxLength-1] = value; 
    }//End if
    else {
      temperatures[index] = value;
    }//End else
  }//End else

  return false;
  
}//End pushArrayDataTemperature


inline bool pushArrayDataHumidity(int index, int value, int maxLength) { 

  static bool firstWriting = false;

  if(!firstWriting && index==(maxLength-1)) 
  {
    humidities[index] = value;
    firstWriting = true;
  }
  else {
    
    if(index==(maxLength-1)) 
    { //Décale les élements et ajoute l'élement
      for(int i=0;i<(maxLength-1);i++) 
      {
        humidities[i] = humidities[i+1];
      }//End for
      humidities[maxLength-1] = value; 
    }//End if
    else {
      humidities[index] = value;
    }//End else
  }//End else

  return false;
  
}//End pushArrayDataHumidity


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
