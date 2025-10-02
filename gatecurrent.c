#include <stdio.h>
#include <stdlib.h>

int main() {
    //defning constants
    double gate_voltage;
    double oxide_thickness;
    double channel_thickness;
    double m_gate_eff;
    double m_oxide_eff;
    double m_channel_eff;
    double Emin;
    double Emax;
    int slice_number;
    
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
    
    
    
    return 0;
}
