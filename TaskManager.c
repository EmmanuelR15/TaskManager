#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TITULO 100
#define MAX_DESC 250
#define MAX_NOMBRE 50
#define MAX_TAREAS 100
#define ARCHIVO "tareas.dat"

typedef enum
{
    PENDIENTE,
    EN_PROGRESO,
    COMPLETADA
} Estado;

typedef struct
{
    int id;
    char titulo[MAX_TITULO];
    char descripcion[MAX_DESC];
    char responsable[MAX_NOMBRE];
    Estado estado;
    int prioridad;
    time_t fecha;
} Tarea;

typedef struct Nodo
{
    Tarea tarea;
    struct Nodo *siguiente;
} Nodo;

// Variables globales
Nodo *lista_tareas = NULL;
int total_tareas = 0;
int proximo_id = 1;

// Prototipos de funciones
void mostrar_menu();
void agregar_tarea();
void mostrar_tareas();
void buscar_tareas();
void ordenar_tareas();
void modificar_tarea();
void completar_tarea();
void eliminar_tarea();
void guardar_datos();
void cargar_datos();
void liberar_lista();
void insertar_tarea_ordenada(Tarea t);
int leer_entero(const char *mensaje, int min, int max);
void leer_cadena(const char *mensaje, char *destino, int max);
void mostrar_tareas_resumen();
void mostrar_tarea_detalle(Tarea t);
void mostrar_tareas_por_prioridad();

int main()
{
    cargar_datos();
    int opcion;

    do
    {
        mostrar_menu();
        opcion = leer_entero("Opcion: ", 1, 9);

        switch (opcion)
        {
        case 1:
            agregar_tarea();
            break;
        case 2:
            mostrar_tareas();
            break;
        case 3:
            buscar_tareas();
            break;
        case 4:
            ordenar_tareas();
            break;
        case 5:
            modificar_tarea();
            break;
        case 6:
            completar_tarea();
            break;
        case 7:
            eliminar_tarea();
            break;
        case 8:
            guardar_datos();
            break;
        case 9:
            break;
        default:
            printf("Opcion no valida\n");
        }
    } while (opcion != 9);

    liberar_lista();
    return 0;
}

void mostrar_menu()
{
    printf("\n===== GESTOR DE TAREAS =====\n");
    printf("1. Agregar tarea\n");
    printf("2. Mostrar todas las tareas (ordenadas por prioridad)\n");
    printf("3. Buscar tareas\n");
    printf("4. Ordenar tareas (criterios avanzados)\n");
    printf("5. Modificar tarea\n");
    printf("6. Marcar como completada\n");
    printf("7. Eliminar tarea\n");
    printf("8. Guardar datos\n");
    printf("9. Salir\n");
}

void mostrar_tareas_resumen()
{
    if (lista_tareas == NULL)
    {
        printf("\n[!] No hay tareas registradas\n");
        return;
    }

    printf("\n===== TAREAS DISPONIBLES =====\n");
    Nodo *actual = lista_tareas;
    while (actual != NULL)
    {
        const char *estados[] = {"Pendiente", "En progreso", "Completada"};
        printf("ID: %-4d | Pri: %d | %-30s | %-10s\n",
               actual->tarea.id,
               actual->tarea.prioridad,
               actual->tarea.titulo,
               estados[actual->tarea.estado]);
        actual = actual->siguiente;
    }
    printf("\n");
}

void mostrar_tarea_detalle(Tarea t)
{
    char fecha_str[20];
    strftime(fecha_str, 20, "%Y-%m-%d", localtime(&t.fecha));

    const char *estados[] = {"Pendiente", "En progreso", "Completada"};
    printf("\n===== DETALLE TAREA ID: %d =====\n", t.id);
    printf("Titulo:      %s\n", t.titulo);
    printf("Descripcion: %s\n", t.descripcion);
    printf("Responsable: %s\n", t.responsable);
    printf("Estado:      %s\n", estados[t.estado]);
    printf("Prioridad:   %d\n", t.prioridad);
    printf("Fecha:       %s\n", fecha_str);
}

// Función para insertar ordenado por prioridad (1 es máxima)
void insertar_tarea_ordenada(Tarea t)
{
    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
    if (nuevo == NULL)
    {
        printf("[!] Error: Memoria insuficiente\n");
        return;
    }
    nuevo->tarea = t;
    nuevo->siguiente = NULL;

    // Caso especial: lista vacía o nueva tarea tiene mayor prioridad que la primera
    if (lista_tareas == NULL || t.prioridad < lista_tareas->tarea.prioridad)
    {
        nuevo->siguiente = lista_tareas;
        lista_tareas = nuevo;
    }
    else
    {
        // Buscar el punto de inserción
        Nodo *actual = lista_tareas;
        while (actual->siguiente != NULL &&
               actual->siguiente->tarea.prioridad <= t.prioridad)
        {
            actual = actual->siguiente;
        }
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
    }
}

// Función mejorada para leer enteros con validación de rango
int leer_entero(const char *mensaje, int min, int max)
{
    char buffer[32];
    int valor;

    while (1)
    {
        printf("%s", mensaje);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            printf("Error de entrada. Intente de nuevo.\n");
            continue;
        }

        if (sscanf(buffer, "%d", &valor) != 1)
        {
            printf("Entrada invalida. Por favor ingrese un numero.\n");
            continue;
        }

        if (valor >= min && valor <= max)
        {
            return valor;
        }

        printf("Valor fuera de rango. Debe ser entre %d y %d.\n", min, max);
    }
}

void agregar_tarea()
{
    if (total_tareas >= MAX_TAREAS)
    {
        printf("\n[!] Limite de tareas alcanzado\n");
        return;
    }

    Tarea nueva;
    nueva.id = proximo_id++;

    leer_cadena("Titulo: ", nueva.titulo, MAX_TITULO);
    leer_cadena("Descripcion: ", nueva.descripcion, MAX_DESC);
    leer_cadena("Responsable: ", nueva.responsable, MAX_NOMBRE);

    nueva.prioridad = leer_entero("Prioridad (1=Maxima, 5=Minima): ", 1, 5);

    nueva.estado = leer_entero("Estado (0:Pendiente, 1:En progreso, 2:Completada): ", 0, 2);

    nueva.fecha = time(NULL);
    insertar_tarea_ordenada(nueva);
    total_tareas++;

    printf("\n[+] Tarea agregada exitosamente\n");
    mostrar_tarea_detalle(nueva);
}

// Función para mostrar tareas ordenadas por prioridad (1 primero)
void mostrar_tareas_por_priority()
{
    if (lista_tareas == NULL)
    {
        printf("\n[!] No hay tareas registradas\n");
        return;
    }

    printf("\n===== LISTA COMPLETA DE TAREAS (Ordenadas por prioridad) =====\n");
    Nodo *actual = lista_tareas;
    while (actual != NULL)
    {
        mostrar_tarea_detalle(actual->tarea);
        actual = actual->siguiente;
    }
}

// Cambiada para usar el nuevo método de visualización
void mostrar_tareas()
{
    mostrar_tareas_por_priority();
}

void buscar_tareas()
{
    if (lista_tareas == NULL)
    {
        printf("\n[!] No hay tareas para buscar\n");
        return;
    }

    printf("\n===== BUSCAR TAREAS =====\n");
    printf("Criterios de busqueda:\n");
    printf("1. Por estado\n");
    printf("2. Por responsable\n");
    printf("3. Por titulo\n");
    int opcion = leer_entero("Seleccione opcion: ", 1, 3);
    char termino[50];
    int encontradas = 0;

    // Mostrar tareas antes de buscar
    mostrar_tareas_resumen();

    if (opcion == 1)
    {
        int estado = leer_entero("Estado (0:Pendiente, 1:En progreso, 2:Completada): ", 0, 2);
        printf("\n===== RESULTADOS =====\n");

        Nodo *actual = lista_tareas;
        while (actual != NULL)
        {
            if (actual->tarea.estado == estado)
            {
                mostrar_tarea_detalle(actual->tarea);
                encontradas++;
            }
            actual = actual->siguiente;
        }
    }
    else
    {
        const char *prompt = (opcion == 2) ? "Responsable: " : "Titulo: ";
        leer_cadena(prompt, termino, 50);
        printf("\n===== RESULTADOS =====\n");

        Nodo *actual = lista_tareas;
        while (actual != NULL)
        {
            char *campo = (opcion == 2) ? actual->tarea.responsable : actual->tarea.titulo;
            if (strstr(campo, termino) != NULL)
            {
                mostrar_tarea_detalle(actual->tarea);
                encontradas++;
            }
            actual = actual->siguiente;
        }
    }

    if (encontradas == 0)
    {
        printf("\n[!] No se encontraron tareas\n");
    }
    else
    {
        printf("\n[+] Total encontradas: %d\n", encontradas);
    }
}

void ordenar_tareas()
{
    if (lista_tareas == NULL)
    {
        printf("\n[!] No hay tareas para ordenar\n");
        return;
    }

    // Crear arreglo de tareas
    Tarea *tareas = (Tarea *)malloc(total_tareas * sizeof(Tarea));
    if (tareas == NULL)
    {
        printf("\n[!] Error: Memoria insuficiente\n");
        return;
    }

    Nodo *actual = lista_tareas;
    for (int i = 0; i < total_tareas; i++)
    {
        tareas[i] = actual->tarea;
        actual = actual->siguiente;
    }

    printf("\n===== ORDENAR TAREAS =====\n");
    printf("Criterios de ordenacion:\n");
    printf("1. Por prioridad (mayor primero)\n");
    printf("2. Por fecha (recientes primero)\n");
    int opcion = leer_entero("Seleccione opcion: ", 1, 2);

    // Ordenamiento burbuja
    for (int i = 0; i < total_tareas - 1; i++)
    {
        for (int j = 0; j < total_tareas - i - 1; j++)
        {
            int comparar = 0;

            if (opcion == 1)
            {
                comparar = tareas[j].prioridad < tareas[j + 1].prioridad;
            }
            else
            {
                comparar = tareas[j].fecha < tareas[j + 1].fecha;
            }

            if (comparar)
            {
                Tarea temp = tareas[j];
                tareas[j] = tareas[j + 1];
                tareas[j + 1] = temp;
            }
        }
    }

    // Mostrar resultados ordenados
    printf("\n===== TAREAS ORDENADAS =====\n");
    for (int i = 0; i < total_tareas; i++)
    {
        mostrar_tarea_detalle(tareas[i]);
    }

    free(tareas);
}

void modificar_tarea()
{
    if (lista_tareas == NULL)
    {
        printf("\n[!] No hay tareas para modificar\n");
        return;
    }

    // Mostrar resumen antes de modificar
    mostrar_tareas_resumen();

    int id = leer_entero("\nID de la tarea a modificar (0 para cancelar): ", 0, 1000);
    if (id == 0)
    {
        printf("\n[!] Operacion cancelada\n");
        return;
    }

    Nodo *actual = lista_tareas;

    while (actual != NULL)
    {
        if (actual->tarea.id == id)
        {
            printf("\n===== MODIFICAR TAREA ID: %d =====\n", id);

            char input[100];
            int valor;

            leer_cadena("Nuevo titulo (ENTER para mantener): ", input, MAX_TITULO);
            if (strlen(input) > 0)
                strcpy(actual->tarea.titulo, input);

            leer_cadena("Nueva descripcion (ENTER para mantener): ", input, MAX_DESC);
            if (strlen(input) > 0)
                strcpy(actual->tarea.descripcion, input);

            leer_cadena("Nuevo responsable (ENTER para mantener): ", input, MAX_NOMBRE);
            if (strlen(input) > 0)
                strcpy(actual->tarea.responsable, input);

            valor = leer_entero("Nueva prioridad (0 para mantener): ", 0, 5);
            if (valor > 0 && valor <= 5)
                actual->tarea.prioridad = valor;

            valor = leer_entero("Nuevo estado (0:Pend, 1:Progreso, 2:Comp, -1 para mantener): ", -1, 2);
            if (valor >= 0 && valor <= 2)
                actual->tarea.estado = valor;

            printf("\n[+] Tarea modificada exitosamente\n");
            mostrar_tarea_detalle(actual->tarea);
            return;
        }
        actual = actual->siguiente;
    }
    printf("\n[!] No se encontro la tarea con ID: %d\n", id);
}

void completar_tarea()
{
    if (lista_tareas == NULL)
    {
        printf("\n[!] No hay tareas para completar\n");
        return;
    }

    // Mostrar resumen antes de completar
    mostrar_tareas_resumen();

    int id = leer_entero("\nID de la tarea a completar (0 para cancelar): ", 0, 1000);
    if (id == 0)
    {
        printf("\n[!] Operacion cancelada\n");
        return;
    }

    Nodo *actual = lista_tareas;

    while (actual != NULL)
    {
        if (actual->tarea.id == id)
        {
            if (actual->tarea.estado == COMPLETADA)
            {
                printf("\n[!] La tarea ya esta completada\n");
            }
            else
            {
                actual->tarea.estado = COMPLETADA;
                printf("\n[+] Tarea marcada como completada\n");
                mostrar_tarea_detalle(actual->tarea);
            }
            return;
        }
        actual = actual->siguiente;
    }
    printf("\n[!] No se encontro la tarea con ID: %d\n", id);
}

void eliminar_tarea()
{
    if (lista_tareas == NULL)
    {
        printf("\n[!] No hay tareas para eliminar\n");
        return;
    }

    // Mostrar resumen antes de eliminar
    mostrar_tareas_resumen();

    int id = leer_entero("\nID de la tarea a eliminar (0 para cancelar): ", 0, 1000);
    if (id == 0)
    {
        printf("\n[!] Operacion cancelada\n");
        return;
    }

    Nodo *actual = lista_tareas;
    Nodo *anterior = NULL;

    while (actual != NULL)
    {
        if (actual->tarea.id == id)
        {
            printf("\n[!] ATENCION: Esta por eliminar esta tarea:\n");
            mostrar_tarea_detalle(actual->tarea);

            char confirmar;
            printf("\n¿Confirmar eliminacion? (s/n): ");
            scanf(" %c", &confirmar);
            while (getchar() != '\n')
                ; // Limpiar buffer

            if (confirmar == 's' || confirmar == 'S')
            {
                if (anterior == NULL)
                {
                    lista_tareas = actual->siguiente;
                }
                else
                {
                    anterior->siguiente = actual->siguiente;
                }

                free(actual);
                total_tareas--;
                printf("\n[+] Tarea eliminada exitosamente\n");
            }
            else
            {
                printf("\n[!] Eliminacion cancelada\n");
            }
            return;
        }

        anterior = actual;
        actual = actual->siguiente;
    }
    printf("\n[!] No se encontro la tarea con ID: %d\n", id);
}

void guardar_datos()
{
    FILE *archivo = fopen(ARCHIVO, "wb");
    if (!archivo)
    {
        printf("\n[!] Error al crear archivo de datos\n");
        return;
    }

    // Guardar tareas
    Nodo *actual = lista_tareas;
    while (actual != NULL)
    {
        fwrite(&actual->tarea, sizeof(Tarea), 1, archivo);
        actual = actual->siguiente;
    }

    fclose(archivo);
    printf("\n[+] Datos guardados exitosamente (%d tareas)\n", total_tareas);
}

void cargar_datos()
{
    FILE *archivo = fopen(ARCHIVO, "rb");
    if (!archivo)
    {
        printf("\n[!] No se encontraron datos previos\n");
        return;
    }

    Tarea t;
    while (fread(&t, sizeof(Tarea), 1, archivo))
    {
        insertar_tarea_ordenada(t);
        total_tareas++;
        if (t.id >= proximo_id)
            proximo_id = t.id + 1;
    }

    fclose(archivo);
    printf("\n[+] Datos cargados exitosamente: %d tareas\n", total_tareas);
}

void liberar_lista()
{
    Nodo *actual = lista_tareas;
    while (actual != NULL)
    {
        Nodo *siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
}

void leer_cadena(const char *mensaje, char *destino, int max)
{
    printf("%s", mensaje);
    fgets(destino, max, stdin);
    destino[strcspn(destino, "\n")] = 0; // Eliminar salto de línea
}

Tarea** buscar_tareas_por_palabra_clave(const char* palabra_clave, int* count) {
    *count = 0;
    Tarea** resultados = malloc(total_tareas * sizeof(Tarea*));
    if(!resultados) return NULL;

    Nodo* actual = lista_tareas;
    while(actual != NULL) {
        char* desc_lower = strdup(actual->tarea.descripcion);
        char* key_lower = strdup(palabra_clave);
        
        // Convertir a minúsculas
        for(char* p = desc_lower; *p; ++p) *p = tolower(*p);
        for(char* p = key_lower; *p; ++p) *p = tolower(*p);
        
        if(strstr(desc_lower, key_lower)) {
            resultados[(*count)++] = &(actual->tarea);
        }
        
        free(desc_lower);
        free(key_lower);
        actual = actual->siguiente;
    }
    
    return resultados;
}

int comparar_por_prioridad(const void* a, const void* b) {
    const Tarea* t1 = *(const Tarea**)a;
    const Tarea* t2 = *(const Tarea**)b;
    
    // Prioridad: 1 (Alta) > 5 (Baja)
    return t1->prioridad - t2->prioridad;
}

Tarea** ordenar_tareas_por_prioridad(int* count) {
    *count = total_tareas;
    Tarea** copia = malloc(*count * sizeof(Tarea*));
    if(!copia) return NULL;
    
    Nodo* actual = lista_tareas;
    for(int i = 0; i < total_tareas && actual != NULL; i++) {
        copia[i] = &(actual->tarea);
        actual = actual->siguiente;
    }
    
    qsort(copia, *count, sizeof(Tarea*), comparar_por_prioridad);
    return copia;
}

int validar_formato_fecha(const char* fecha) {
    if(strlen(fecha) != 10) return 0;
    if(fecha[4] != '-' || fecha[7] != '-') return 0;
    
    for(int i = 0; i < 10; i++) {
        if(i == 4 || i == 7) continue;
        if(!isdigit(fecha[i])) return 0;
    }
    
    // Validación básica de rangos
    int mes = (fecha[5] - '0') * 10 + (fecha[6] - '0');
    int dia = (fecha[8] - '0') * 10 + (fecha[9] - '0');
    
    return (mes >= 1 && mes <= 12) && (dia >= 1 && dia <= 31);
}
Tarea** filtrar_tareas_por_estado(Estado estado, int* count) {
    *count = 0;
    Tarea** resultados = malloc(total_tareas * sizeof(Tarea*));
    if(!resultados) return NULL;

    Nodo* actual = lista_tareas;
    while(actual != NULL) {
        if(actual->tarea.estado == estado) {
            resultados[(*count)++] = &(actual->tarea);
        }
        actual = actual->siguiente;
    }
    
    return resultados;
}
