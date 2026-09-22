#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include <sys/stat.h>

#if defined(_WIN32)
    #include <direct.h>
#endif

typedef struct {
    int id;
    int arrival;
    int burst;
    int priority;
    int completion;
    int turnaround;
    int waiting;
    bool is_completed;
} Process;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso incorrecto.\nEjemplo de uso: %s <archivo.txt>\n", argv[0]);
        return 1;
    }

    FILE *file;
    Process processes[100]; 
    int n = 0;

    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: No se pudo abrir el archivo de entrada '%s'\n", argv[1]);
        return 1;
    }

    while (fscanf(file, "%d %d %d %d", &processes[n].id, &processes[n].arrival, &processes[n].burst, &processes[n].priority) != EOF) {
        processes[n].is_completed = false; 
        n++;
    }
    fclose(file);

    int current_time = 0;
    int completed = 0;
    float total_waiting = 0, total_turnaround = 0;

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
            current_time += processes[shortest_idx].burst;
            processes[shortest_idx].completion = current_time;
            
            processes[shortest_idx].turnaround = processes[shortest_idx].completion - processes[shortest_idx].arrival;
            processes[shortest_idx].waiting = processes[shortest_idx].turnaround - processes[shortest_idx].burst;
            processes[shortest_idx].is_completed = true;
            
            total_turnaround += processes[shortest_idx].turnaround;
            total_waiting += processes[shortest_idx].waiting;
            completed++;
        }
    }

    #if defined(_WIN32)
        _mkdir("Resultados");
    #else 
        mkdir("Resultados", 0777); 
    #endif

    time_t t = time(NULL);
    char out_filename[256];
    snprintf(out_filename, sizeof(out_filename), "Resultados/result.sjf.%ld.txt", (long)t);

    FILE *outfile = fopen(out_filename, "w");
    if (outfile == NULL) {
        printf("Error: No se pudo crear el archivo de salida '%s'\n", out_filename);
        return 1;
    }

    fprintf(outfile, "Resultados de la planificacion SJF (No Apropiativo):\n");
    fprintf(outfile, "--------------------------------------------------------------------------\n");
    fprintf(outfile, "ID\tLlegada\tRafaga\tPrioridad\tFin\tTurnaround\tEspera\n");
    fprintf(outfile, "--------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        fprintf(outfile, "%d\t%d\t%d\t%d\t\t%d\t%d\t\t%d\n", 
               processes[i].id, processes[i].arrival, processes[i].burst, 
               processes[i].priority, processes[i].completion, 
               processes[i].turnaround, processes[i].waiting);
    }
    fprintf(outfile, "--------------------------------------------------------------------------\n");
    fprintf(outfile, "Tiempo de espera promedio: %.2f\n", total_waiting / n);
    fprintf(outfile, "Tiempo de turnaround promedio: %.2f\n", total_turnaround / n);

    fclose(outfile);

    printf("Simulacion completada con exito.\nResultados guardados en: %s\n", out_filename);

    return 0;
}