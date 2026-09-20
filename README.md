#simulador de Planificación de CPU
##Algoritmos implementados:
- **Round Robin**
- .
Para compilar el proyecto se utiliza GCC: gcc -Wall -Wextra -std=c11 simuladorPlanProcesos.c -o simulador
Para ejecutarlo: ./simulador < datos/Procesos.txt
Resultados: Los resultados de la simulación se guardo en la carpeta resultados
Métricas:
El simulador calcula las siguientes  métricas:
Tiempo de retorno = Tiempo de finalización - Tiempo de llegada
Tiempo de espera = Tiempo de retorno - Ráfaga de CPU
Tiempo de respuesta = Tiempo de inicio - Tiempo de llegada
