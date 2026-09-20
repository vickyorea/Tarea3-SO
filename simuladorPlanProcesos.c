#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESOS 200   // por si quieren simular mas de 100

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
} Process;

/*
   Simulación del algoritmo Round Robin 
   p: arreglo de procesos 
   n: es la cantidad de procesos
   quantum: camtidad máxima de tiempo que un proceso va a poder utilizar la CPU en cada turno
*/
void simulate_rr(Process *p, int n, int quantum){
    int cola[MAX_PROCESOS];
   
    //nuestras variables de la cola circular
    int frente = 0;
    int final = 0;
    int cantidad = 0;
    
    //tiempo actual
    int tiempo = 0; 
    
    //indice deñ proceso que puede entrar a la cola
    int siguiente = 0;

    // Cantidad de procesos que ya terminaron
    int terminados = 0;

    // reinciar los datos de la simulacion
    for(int i= 0; i < n; i++){
        p[i].remaining = p[i].burst;
        p[i].start_time = -1;
        p[i].finish_time = 0;
        p[i].waiting_time = 0;
        p[i].turnaround = 0;
        p[i].response_time = -1;
    }

    printf("       ROUND ROBIN (q= %d)\n", quantum);

    //La simulacion termina cuando todos los procesos lo hagan
    while (terminados < n){
        //Agregar a la cola todos los procesos que llegaron
        while (siguiente < n && p[siguiente].arrival <= tiempo){
            cola[final] = siguiente;
            final = (final + 1) % MAX_PROCESOS;
            cantidad ++;

            siguiente++;
        }

        /*
           Si no hay procesos en la cola, entonces la CPU esta desocupada
           Vamos a avanzar el tiempo hasta la llegada del siguiente proceso
        */
        if(cantidad == 0){
            if(siguiente < n){
                tiempo = p[siguiente].arrival;
            }
            continue;
        }

        //sacar el primer proceso de la cola

        int indice = cola[frente];
        frente = (frente + 1) % MAX_PROCESOS;
        cantidad--;

        //Si es la primera vez que el proceso tiene timepo en CPU, registramos sut iempod de inicio y respuesta
        if (p[indice].start_time == -1){
            p[indice].start_time = tiempo;
            p[indice].response_time = tiempo - p[indice].arrival;
        }

        /*
          Determinamos cuanto tiempo puede ejecutarse
          Si le queda menos tiempo que el quantum, solo ejecutamos lo que le reste

        */
        int tiempo_ejecucion;

        if(p[indice].remaining < quantum){
            tiempo_ejecucion = p[indice].remaining;
        } else{
            tiempo_ejecucion = quantum;
        }

        //ejecucion del proceso

        printf("Tiempo %d - %d; P%d\n", tiempo, tiempo + tiempo_ejecucion, p[indice].id);

        tiempo += tiempo_ejecucion;
        p[indice].remaining -= tiempo_ejecucion;
        
        /*
           Como durante la ejecución pudieron haber llegado nuevos procesos
           los agregamos a la cola antes de volver a colocar el proceso actual
        */

        while (siguiente < n && p[siguiente].arrival <=tiempo){
            cola[final] = siguiente;
            final = (final + 1) % MAX_PROCESOS;
            cantidad ++;
            siguiente ++;
        }

        // si el proceso termino, guardamos resultados

        if(p[indice].remaining == 0){
            p[indice].finish_time = tiempo;
            p[indice].turnaround = p[indice].finish_time - p[indice].arrival;
            p[indice].waiting_time = p[indice].turnaround - p[indice].burst;
            terminados ++;
        }  else{
            //El proceso no termino, asi que regresa al final de la cola

            cola[final] = indice;
            final = (final+1) % MAX_PROCESOS;
            cantidad ++;
        }

    }

    //Mostramos resultados
    printf("Mostramos resultados");
    printf("Proceso   Llegada   Burst  Inicio   Fin   Espera   Retorno   Respuesta\n");

    double promedio_espera = 0;
    double promedio_retorno = 0;
    double promedio_respuesta = 0;

    for(int i = 0; i < n; i++){
        printf("P%-7d %-8d %-6d %-7d %-4d %-7d %-8d %-9d\n",
               p[i].id,
               p[i].arrival,
               p[i].burst,
               p[i].start_time,
               p[i].finish_time,
               p[i].waiting_time,
               p[i].turnaround,
               p[i].response_time);

        promedio_espera += p[i].waiting_time;
        promedio_retorno += p[i].turnaround;
        promedio_respuesta += p[i].response_time;
    }

    promedio_espera /= n;
    promedio_retorno /= n;
    promedio_respuesta /= n;

    printf("\nPromedios:\n");
    printf("Tiempo de espera promedio: %.2f\n",
           promedio_espera);

    printf("Tiempo de retorno promedio: %.2f\n",
           promedio_retorno);

    printf("Tiempo de respuesta promedio: %.2f\n",
           promedio_respuesta);

}

int main(void) {
    Process procesos[MAX_PROCESOS];
    int n = 0;

    // Lee desde stdin hasta EOF: cada línea debe tener id arrival burst priority
    while (scanf("%d %d %d %d",
                 &procesos[n].id,
                 &procesos[n].arrival,
                 &procesos[n].burst,
                 &procesos[n].priority) == 4) {

        procesos[n].remaining    = procesos[n].burst;
        procesos[n].start_time   = -1;
        procesos[n].finish_time  = 0;
        procesos[n].waiting_time = 0;
        procesos[n].turnaround   = 0;
        procesos[n].response_time= -1;
        n++;

        if (n >= MAX_PROCESOS) {
            fprintf(stderr, "Se alcanzó el límite MAX_PROCESOS=%d\n", MAX_PROCESOS);
            break;
        }
    }

    printf("Se leyeron %d procesos:\n", n);
    for (int i = 0; i < n; i++) {
        printf("P%3d: arrival=%3d burst=%2d priority=%d\n",
               procesos[i].id,
               procesos[i].arrival,
               procesos[i].burst,
               procesos[i].priority);
    }

    // Aquí ya puedes llamar a tus funciones de simulación:
    //void simulate_fcfs(Process *p, int n);
    //void simulate_rr(Process *p, int n, int quantum);
    //void simulate_sjf(Process *p, int n);
    //void simulate_srtn(Process *p, int n);
    //void simulate_priority(Process *p, int n);

    /* ejecución de Roun Robin con diferentes quantum
       Como requisito nos pidieron hacerlo con 4
    */
    simulate_rr(procesos, n, 2);
    simulate_rr(procesos, n, 4);
    simulate_rr(procesos, n, 6);


    return 0;
}
