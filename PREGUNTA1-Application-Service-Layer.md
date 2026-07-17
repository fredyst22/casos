# Pregunta 1 - Definition of Application Service Layer Requirements
## FreshPlot / HarvestPod Advance
**Estudiante:** Carlos Fredy Fernandez Camayo  
**Codigo:** u202320083  
**NRC:** 17755  

### Objetivo
Definir los requisitos de la capa de servicio de aplicacion (Application Service Layer) del sistema IoT HarvestPod Advance, incluyendo la interfaz de usuario requerida por cada servicio.

---

### 1) Servicio de Monitoreo Ambiental en Tiempo Real
**Descripcion del servicio:** Consultar temperatura, humedad y nivel de luz del cultivo en tiempo casi real.

**Requisitos de interfaz de usuario:**
- **Mobile App (Android/iOS):** dashboard con tarjetas de temperatura (°C), humedad (%) y luz (lux/nivel 0-1023), actualizacion automatica cada 5 segundos.
- **Web Dashboard:** panel responsive con graficos de tendencia (ultimas 24 horas) y valor puntual actual.
- **Consola del dispositivo (Serial/LCD):** respaldo local; LCD muestra temperatura/humedad y estado; Serial publica JSON de telemetria.

**Usuarios:** horticultor domestico, soporte tecnico FreshPlot.

---

### 2) Servicio de Configuracion de Humedad Objetivo
**Descripcion del servicio:** Permitir ajustar el set-point de humedad del cultivo en el rango 40%-80% con pasos de 5%.

**Requisitos de interfaz de usuario:**
- **Interfaz embebida (dispositivo):** botones tactiles (+) y (-) + LCD (valor objetivo en esquina inferior izquierda).
- **Mobile App:** control deslizante o stepper con validacion de limites 40-80 y paso 5.
- **Web/Tablet:** formulario de configuracion con confirmacion visual del nuevo target.

**Regla UX:** toda UI debe impedir valores fuera de rango y mostrar el valor confirmado tras el cambio.

---

### 3) Servicio de Control de Riego Automatizado
**Descripcion del servicio:** Activar/desactivar el relay de riego segun la humedad relativa al target (±5%).

**Estados de negocio a visualizar:**
- `RIEGO`: humedad < target - 5
- `OPTIMAL`: humedad dentro de target ± 5
- `VENTILANDO`: humedad > target + 5

**Requisitos de interfaz de usuario:**
- **LCD del dispositivo:** muestra el estado textual (RIEGO / OPTIMAL / VENTILANDO).
- **LEDs locales:** verde = OPTIMAL; rojo = alerta de riego.
- **Mobile/Web:** indicador de estado del irrigador + historial de ciclos de riego.
- **Notificaciones push (mobile):** alerta cuando el sistema entra en RIEGO prolongado.

---

### 4) Servicio de Control de Ventilacion
**Descripcion del servicio:** Abrir/cerrar ventilacion mediante servo (0° cerrado, 90° abierto) cuando temperatura > 28°C o humedad > 85%.

**Requisitos de interfaz de usuario:**
- **Mobile/Web:** widget de angulo de ventilacion y motivo de apertura (alta temperatura / alta humedad).
- **Interfaz embebida:** estado VENTILANDO en LCD cuando aplica por humedad alta; servo como actuador fisico.
- **Desktop de operaciones (soporte):** vista de diagnostico de actuadores para mantenimiento.

---

### 5) Servicio de Gestion de Modos Dia/Noche
**Descripcion del servicio:** Cambiar entre `DAY_MODE` y `NIGHT_MODE` segun el photoresistor (< 200 = noche).

**Comportamiento de negocio:**
- **NIGHT_MODE:** detiene riego, cierra ventilacion, apaga LCD y LEDs, no evalua reglas de humedad.
- **DAY_MODE:** reactiva reglas, display, LEDs, riego y ventilacion.

**Requisitos de interfaz de usuario:**
- **Mobile App:** badge de modo actual (Dia/Noche) y timeline de transiciones.
- **Web Dashboard:** switch informativo (solo lectura o override administrativo).
- **Consola Serial:** campo `operationMode` en el JSON periodico.

---

### 6) Servicio de Telemetria y Auditoria de Estado
**Descripcion del servicio:** Publicar cada 5 segundos el estado operativo del dispositivo.

**Payload minimo (JSON):**
- `deviceMacAddress`
- `operationMode`
- `currentHumidity`
- `currentTemperature`
- `lightLevel`
- `irrigationState`
- `createdAt`

**Requisitos de interfaz de usuario:**
- **Consola Serial (dispositivo):** impresion periodica del JSON (requisito del prototipo).
- **Cloud Web Console:** explorador de mensajes, filtros por MAC y modo.
- **Mobile App:** pantalla de “ultimo estado recibido” con timestamp.
- **API/Integraciones:** endpoint o broker para que sistemas externos consuman la telemetria.

---

### 7) Servicio de Identidad y Trazabilidad del Dispositivo
**Descripcion del servicio:** Identificar univocamente cada HarvestPod Advance mediante MAC personalizada.

**Requisitos de interfaz de usuario:**
- **Pantalla de registro (Web/Mobile):** asociar MAC al perfil del usuario/huerto.
- **Panel administrativo FreshPlot:** inventario de dispositivos por MAC, propietario y firmware.
- **Boot banner en consola:** empresa, producto y developer para soporte en campo.

---

### Resumen de interfaces por canal
| Servicio | LCD/Botones | LEDs | Serial Console | Mobile | Web/Tablet | Desktop Ops |
|---|---|---|---|---|---|---|
| Monitoreo ambiental | Si | - | Si | Si | Si | Si |
| Config humedad objetivo | Si | - | - | Si | Si | - |
| Control de riego | Si | Si | Si | Si | Si | Si |
| Ventilacion | Si | - | Si | Si | Si | Si |
| Modo dia/noche | Si | Si | Si | Si | Si | - |
| Telemetria | - | - | Si | Si | Si | Si |
| Identidad dispositivo | - | - | Si | Si | Si | Si |

### Criterios de aceptacion transversales
1. Toda UI de configuracion respeta rango 40-80 y paso 5.
2. Los estados visibles coinciden con las reglas embebidas del dispositivo.
3. En NIGHT_MODE, las UI remotas reflejan suspension de riego/ventilacion.
4. La telemetria se actualiza al menos cada 5 segundos y es auditable por `createdAt` + MAC.
