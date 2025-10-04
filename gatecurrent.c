#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define maxpoints 10000 //datapoitns from tansmition_data.txt
#define k_B 8.617333e-5 //in eV/kelvin
#define q_magnitude 1.602e-19 //in coulombs
#define h 6.62607015e-34 //planks constants

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
        printf("ERROR: can't open input_parameters.txt :(\n");
        return 1;
    }
        
    int params_read = fscanf(input_file, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%d", &gate_voltage, &oxide_thickness, &channel_thickness, &m_gate_eff, &m_oxide_eff, &m_channel_eff, &Emin, &Emax, &slice_number);
    
       fclose(input_file);
    
    if (params_read != 9) {
        printf("Error: Expected 9 parameters but read %d\n", params_read);
        return 1;
    }
    
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FILE *datafile;
    double *energy;
    double *transmition;
      int sample_points_count = 0;

    energy = (double *)malloc(maxpoints * sizeof(double));
    transmition = (double *)malloc(maxpoints * sizeof(double)); //allocation of memory for arrays
  
    datafile = fopen("transmition_data.txt", "r");
   
  if (datafile == NULL) {
      printf("ERROR: can't open file transmition_data.txt :(\n");
      free(energy);
      free(transmition); //freeing memory is imp. as lot of space will be wasted!
      return 1;
  }
  
   while (sample_points_count < maxpoints && 
          fscanf(datafile, "%lf %lf", &energy[sample_points_count], &transmition[sample_points_count]) == 2) {
          sample_points_count++;
    }

    fclose(datafile);


/*
printf(" \n %e \n ",integrand( energy[75], transmition[75], gate_voltage, 300 ) );
*/

/*
  printf("number of sample points is %d , 76th sample point data is %f %f",sample_points_count , energy[75], transmition[75]);
*/
  
double del = energy[1] - energy[0];
int N = sample_points_count - 1;

double integral_value = 0.0;

  //if we have odd number of points (which implied and is implied that) N is even (in our case its 4999 but it may vary depending on tansmition_data.txt and input_parameters.txt)
if (N % 2 == 0) {
    for (int i = 0; i <= N - 2; i += 2) {
        if (energy[i] < Emin || energy[i + 2] > Emax) continue;

        double fi = integrand(energy[i], transmition[i], gate_voltage, 300);
        double fi_1 = integrand(energy[i + 1], transmition[i + 1], gate_voltage, 300);
        double fi_2 = integrand(energy[i + 2], transmition[i + 2], gate_voltage, 300);

        integral_value += (del / 3.0) * (fi + 4 * fi_1 + fi_2);

       //here we used the summation of area under each segment

    }
}
  //if we have odd number of points or N is odd then one segment will be left at last, for which we use trapezoidal
else {
    int last_index = N - 1;

    //simpsons part
    for (int i = 0; i <= N - 3; i += 2) {
        if (energy[i] < Emin || energy[i + 2] > Emax) continue;

        double fi = integrand(energy[i], transmition[i], gate_voltage, 300);
        double fi_1 = integrand(energy[i + 1], transmition[i + 1], gate_voltage, 300);
        double fi_2 = integrand(energy[i + 2], transmition[i + 2], gate_voltage, 300);

        integral_value += (del / 3.0) * (fi + 4 * fi_1 + fi_2);
    }

    //trapezoidal part
    double last1 = integrand(energy[N - 1], transmition[N - 1], gate_voltage, 300);
    double last2 = integrand(energy[N], transmition[N], gate_voltage, 300);
    integral_value += 0.5 * del * (last1 + last2);
}

printf("\nintegral result %.6e\n", integral_value);

  double gatecurrentvalue = (integral_value)*(2)*(sqre(q_magnitude)/h);


FILE *gtcrntoutput;

gtcrntoutput = fopen("output.txt", "w");

if (gtcrntoutput == NULL) {
    printf("can't open output,txt :(\n");
    return 1;
}
  
fprintf(gtcrntoutput, "\nthe value of integral is %e\n", integral_value);
fprintf(gtcrntoutput, "\nthe value of gate current is %e\n", gatecurrentvalue);
  fprintf(gtcrntoutput, "\n:)\n");

fclose(gtcrntoutput);

    return 0;
}
