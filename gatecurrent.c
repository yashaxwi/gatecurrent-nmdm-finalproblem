#include <stdio.h>
#include <stdlib.h>
#include <math.h>



#define maxpoints 10000 //datapoitns from tansmition_data.txt
#define k_B 8.617333e-5 //in eV/Kelvin
#define q_magnitude 1.602e-19 //in Coulombs




double sqre(double x){
  return x * x;//this function will return x^2...
}



double fermidirac(double E, double E_f, double T ){
 
  double power = ( E - E_f ) / ( k_B * T );

  if(power > 100){
    return 0; //fermi function value will become 0
  }

  if(power < -100){
    return 1; //fermi function value will become 1
  }

  return 1 / ( 1 + exp(power) );
}


double integrand( double Ei, double T_Ei, double gate_voltage, double T ){

double diff =  fermidirac( Ei , (-1)*gate_voltage , T ) - fermidirac( Ei , 0 , T );


  return ( T_Ei * diff );
  

}




int main() {



    //defning constants
    double gate_voltage; //V
    double oxide_thickness; 
    double channel_thickness;
    double m_gate_eff;
    double m_oxide_eff;
    double m_channel_eff;
    double Emin; //eV
    double Emax; //eV
    int slice_number; //number
    
    //recording the data
    FILE *input_file = fopen("input_parameters.txt", "r");
    
    if (input_file == NULL) {
        printf("Error: Could not open input_parameters.txt\n");
        return 1;
    }
    
    
    int params_read = fscanf(input_file, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%d", &gate_voltage, &oxide_thickness, &channel_thickness, &m_gate_eff, &m_oxide_eff, &m_channel_eff, &Emin, &Emax, &slice_number);
    
       fclose(input_file);
    
    
    if (params_read != 9) {
        printf("Error: Expected 9 parameters but read %d\n", params_read);
        return 1;
    }
    
    
    
    printf("Parameters loaded successfully from input_parameters.txt\n");

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FILE *datafile;
    double *energy;
    double *transmition;
      int sample_points_count = 0;

    energy = (double *)malloc(maxpoints * sizeof(double));
    transmition = (double *)malloc(maxpoints * sizeof(double)); //allocation of memory for arrays
  
    datafile = fopen("transmition_data.txt", "r");
   
  if (datafile == NULL) {
      printf("Error: Could not open file 'transmition_data.txt'\n");
      free(energy);
      free(transmition); //freeing memory is imp. as lot of space will be wasted!
      return 1;
  }
  
   while (sample_points_count < maxpoints && 
          fscanf(datafile, "%lf %lf", &energy[sample_points_count], &transmition[sample_points_count]) == 2) {
          sample_points_count++;
    }

    fclose(datafile);

printf(" \n %e \n ",integrand( energy[75], transmition[75], gate_voltage, 300 ) );


/*
  printf("number of sample points is %d , 76th sample point data is %f %f",sample_points_count , energy[75], transmition[75]);
*/
  








    
    return 0;
}

