#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include <sys/stat.h>

#if defined(_WIN32)
    #include <direct.h>
#endif

#define MAX_PROCESOS 200

// Estructura de procesos unificada tomada de Round Robin
typedef struct {
    int id;   			// ID del proceso
    int arrival;                // Tiempo de llegada (t0, t1... tn)
    int burst;                  // Ráfaga de CPU original (tiempo cpu)
    int priority;               // Tiempo restante (para RR, SRTN)
    int remaining;		// Para algoritmos con prioridades
    int start_time;		// Primer instante en que se ejecuta
    int finish_time;		// Momento en que termina
    int waiting_time;		// Tiempo total de espera
    int turnaround;		// finish_time - arrival
    int response_time;          // primer instante en que el proceso recibe CPU − arrival
    bool is_completed;          // Variable especifica para SJF
} Process;

int main(int argc, char *argv[]) {
    FILE *file;

    if (argc == 1) {
        file = stdin;
    } else if (argc == 2) {
        file = fopen(argv[1], "r");
        if (file == NULL) {
            printf("Error: No se pudo abrir el archivo de entrada '%s'\n", argv[1]);
            return 1;
        }
    } else {
        printf("Uso incorrecto.\n");
        printf("Desde archivo: %s <archivo.txt>\n", argv[0]);
        printf("Desde stdin:   %s\n", argv[0]);
        return 1;
    }

    Process processes[MAX_PROCESOS]; 
    int n = 0;

    // Lectura e inicialización de la estructura
    while (fscanf(file, "%d %d %d %d", &processes[n].id, &processes[n].arrival, &processes[n].burst, &processes[n].priority) != EOF) {
        processes[n].remaining = processes[n].burst;
        processes[n].start_time = -1;
        processes[n].finish_time = 0;
        processes[n].waiting_time = 0;
        processes[n].turnaround = 0;
        processes[n].response_time = -1;
        processes[n].is_completed = false; 
        n++;
        
        if (n >= MAX_PROCESOS) {
            fprintf(stderr, "Se alcanzó el límite MAX_PROCESOS=%d\n", MAX_PROCESOS);
            break;
        }
    }
    
    if (file != stdin) {
        fclose(file);
    }

    int current_time = 0;
    int completed = 0;
    float total_waiting = 0, total_turnaround = 0, total_response = 0;

    while (completed != n) {
        int min_burst = INT_MAX;
        int shortest_idx = -1;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= current_time && !processes[i].is_completed) {
                if (processes[i].burst < min_burst) {
                    min_burst = processes[i].burst;
                    shortest_idx = i;
                }
                else if (processes[i].burst == min_burst) {
                    if (processes[i].arrival < processes[shortest_idx].arrival) {
                        min_burst = processes[i].burst;
                        shortest_idx = i;
                    }
                }
            }
        }

        if (shortest_idx == -1) {
            current_time++;
        } else {
            // Asignación de la CPU
            if(processes[shortest_idx].start_time == -1){
                processes[shortest_idx].start_time = current_time;
                processes[shortest_idx].response_time = current_time - processes[shortest_idx].arrival;
            }
            
            current_time += processes[shortest_idx].burst;
            processes[shortest_idx].finish_time = current_time;
            
            // Calculo de métricas
            processes[shortest_idx].turnaround = processes[shortest_idx].finish_time - processes[shortest_idx].arrival;
            processes[shortest_idx].waiting_time = processes[shortest_idx].turnaround - processes[shortest_idx].burst;
            processes[shortest_idx].is_completed = true;
            
            total_turnaround += processes[shortest_idx].turnaround;
            total_waiting += processes[shortest_idx].waiting_time;
            total_response += processes[shortest_idx].response_time;
            completed++;
        }
    }

    #if defined(_WIN32)
        _mkdir("resultados");                    
        _mkdir("resultados/sjf_simulador");      
    #else 
        mkdir("resultados", 0777);               
        mkdir("resultados/sjf_simulador", 0777);
    #endif

    time_t t = time(NULL);
    char out_filename[256];
    snprintf(out_filename, sizeof(out_filename), "resultados/sjf_simulador/result.sjf.%ld.txt", (long)t);

    FILE *outfile = fopen(out_filename, "w");
    if (outfile == NULL) {
        printf("Error: No se pudo crear el archivo de salida '%s'\n", out_filename);
        return 1;
    }

    fprintf(outfile, "Resultados de la planificacion SJF (No Apropiativo):\n");
    fprintf(outfile, "-----------------------------------------------------------------------------------\n");
    fprintf(outfile, "Proceso   Llegada   Burst  Inicio   Fin   Espera   Retorno   Respuesta\n");
    fprintf(outfile, "-----------------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        fprintf(outfile, "P%-7d %-8d %-6d %-7d %-4d %-7d %-8d %-9d\n", 
               processes[i].id, processes[i].arrival, processes[i].burst, 
               processes[i].start_time, processes[i].finish_time, 
               processes[i].waiting_time, processes[i].turnaround, processes[i].response_time);
    }
    fprintf(outfile, "-----------------------------------------------------------------------------------\n");
    fprintf(outfile, "Tiempo de espera promedio: %.2f\n", total_waiting / n);
    fprintf(outfile, "Tiempo de retorno promedio: %.2f\n", total_turnaround / n);
    fprintf(outfile, "Tiempo de respuesta promedio: %.2f\n", total_response / n);

    fclose(outfile);

    printf("Simulacion completada con exito.\nResultados guardados en: %s\n", out_filename);

    return 0;
}