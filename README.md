# 🚀 Gestor de Tareas en C - Desarrollo Principal por Emmanuel Ruiz

Programa de línea de comandos para gestionar proyectos y tareas con persistencia en archivos binarios. **Desarrollado principalmente por Emmanuel Ruiz** como trabajo práctico para la materia de Programación, con contribuciones del equipo.

## Características principales
- ✨ Agregar tareas con título, descripción, responsable y prioridad
- 📝 Modificar tareas existentes
- ✅ Marcar tareas como completadas
- 🔍 Buscar tareas por estado, responsable o título
- 📊 Ordenar tareas por prioridad o fecha
- 💾 Guardado automático de datos
- 🗑️ Eliminar tareas con confirmación
- 🖥️ Interfaz intuitiva en español

## 👥 Integrantes del equipo (Aportes nivelados)

| Nombre    | Apellido    | Rol                          | Contribuciones clave |
|-----------|-------------|-------------------------------|------------------------|
| **Emmanuel** | Ruiz     | Desarrollo principal y coordinación técnica | Implementación base del sistema, diseño del menú, estructuras de datos, manejo de archivos binarios y lógica central del programa |
| **Kenzo**    | Kairiyama | Desarrollo y documentación     | Implementación de funciones de búsqueda y filtrado, revisión del código, redacción de documentación interna y de usuario |
| **Kevin**    | Piña      | Desarrollo y pruebas           | Implementación del ordenamiento por prioridad y fecha, validaciones de entrada, pruebas funcionales en Windows y sugerencias de mejora |

## 📋 Instrucciones de compilación y ejecución

### Requisitos previos
- Compilador GCC instalado
- Sistema operativo Windows o Linux

### Pasos para compilar y ejecutar

![image](https://github.com/user-attachments/assets/f4dc10c0-2fc7-43f5-8cfc-9be4c5f13d09)

#### En Linux
```bash
# 1. Clonar el repositorio
git clone https://github.com/EmmanuelR15/TaskManager.git

# 2. Entrar al directorio del proyecto
cd TaskManager

# 3. Compilar el programa
gcc -o gestor_tareas main.c

# 4. Ejecutar el programa
./gestor_tareas
