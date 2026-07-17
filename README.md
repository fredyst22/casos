# casos

Página 1 de 8
INGENIERÍA DE SOFTWARE
1ASI0572 – DESARROLLO DE SOLUCIONES IOT
EXAMEN FINAL
202610
NRC: 17755
Profesores: León Baca, Marco Antonio
Duración: 170 minutos
Indicaciones:
1. El examen consta de 2 preguntas en base a un caso, y tendrá 170 minutos para resolver y enviar.
2. Las preguntas son en relación a un Caso y la entrega de su respuesta es a través de envío de dos
archivos: un archivo .zip, conteniendo la exportación de su solución a la pregunta desarrollo de
software y archivo de PowerPoint adjunto en la actividad del examen final, con su respuesta para
las preguntas de redacción, colocando el contenido en la hoja de respuesta según el título e
indicaciones. Utilice el documento de PowerPoint upc-pre-202610-1asi0572-examen-finalfile_v1.pptx para responder. Coloque contenido en cada hoja de respuesta según el título e
indicaciones.
3. Puede utilizar como referencia los materiales publicados en el aula virtual, lossitiosreferenciados
en el enunciado, así como sitios web de documentación de frameworks o
tecnologías referenciadas.
Página 2 de 8
Enunciado:
Caso FreshPlot
Un balcón lleno de vida, un pequeño gesto diario para reconectar con la tierra en medio del ritmo de la ciudad, una
manera sencilla de ver crecer algo propio y celebrar la vida sostenible. Y en ese punto exacto donde la humedad, la
temperatura y la luz se alinean, aparece esa sensación de bienestar con el entorno que no se puede comprar.
Impulsados por esa idea y por una vocación permanente de innovación, FreshPlot invirtió meses de pruebas con
distintos sensores, materiales y arquitecturas electrónicas, con el propósito de construir un sistema inteligente de
control para huertos e invernaderos que combinara utilidad real con un diseño cuidado. Como resultado, FreshPlot
presentó al mercado HarvestPod, un dispositivo que reúne electrónica embebida de última generación para llevar
el cultivo automatizado doméstico a otro nivel.
Como parte de la expansión de su catálogo, la compañía se encuentra desarrollando prototipos de su nueva línea
HarvestPod Advance.
Solution Features
HarvestPod Advance está construido sobre una placa ESP32 y contempla los siguientes componentes principales:
Una pantalla LCD-1602 (https://docs.wokwi.com/parts/wokwi-lcd1602).
Dos pulsadores táctiles (tactile switch) de 6mm (https://docs.wokwi.com/parts/wokwi-pushbutton-6mm).
Un sensor de temperatura y humedad DHT22 (https://docs.wokwi.com/parts/wokwi-dht22).
Dos LEDs (https://docs.wokwi.com/parts/wokwi-led) - uno verde para indicar condiciones óptimas y uno rojo para
alertas.
Un sensor de luz (photoresistor) (https://docs.wokwi.com/parts/wokwi-photoresistor-sensor).
Un módulo de reloj en tiempo real DS1307 (https://docs.wokwi.com/parts/wokwi-ds1307).
Un módulo relay (https://docs.wokwi.com/parts/wokwi-relay-module) que gobierna el sistema de riego.
Un servo motor (https://docs.wokwi.com/parts/wokwi-servo) que controla la apertura de ventilación del cultivo.
En la esquina superior izquierda de la pantalla se muestran la temperatura (°C) y la humedad (%) leídas en tiempo
real por el sensor DHT22. En la esquina inferior izquierda se presenta el rango de humedad objetivo configurado por
el usuario para sus plantas. Mediante los botones (+) y (-) el usuario puede aumentar o disminuir ese valor objetivo.
El rango permitido va de 40% a 80%, y cada pulsación modifica el valor en pasos de 5%. FreshPlot señala que
HarvestPod está pensado para huertos e invernaderos de tamaño pequeño y mediano.
Cuando la humedad se ubica más de 5% por debajo del valor objetivo, el sistema activa el relay de riego para
humedecer el sustrato, la pantalla muestra el estado RIEGO y el LED verde se apaga mientras el LED rojo se enciende.
Cuando la humedad está dentro del rango objetivo (±5%) o por encima de él, el relay de riego permanece apagado;
si está dentro del rango, la pantalla muestra OPTIMAL con el LED verde encendido, y si está por encima, muestra
VENTILANDO con el LED verde apagado.
El servo motor controla la apertura de ventilación: cuando la temperatura supera los 28°C o la humedad supera el
85%, el servo se posiciona a 90 grados (ventilación abierta). Cuando las condiciones son normales, el servo retorna
a 0 grados (ventilación cerrada).
Modos de operación
Cuando el sensor de luz (photoresistor) detecta que el nivel de luz ambiental es muy bajo (indicando que es de
noche, con valores inferiores a 200 lux en la lectura del sensor), entonces el dispositivo entra en modo Night Mode,
dejando de regar, cerrando la ventilación (servo a 0°), apagando el display, apagando ambos LEDs y dejando de
verificar reglas de humedad. Cuando el sensor detecta que hay suficiente luz ambiental (valores superiores a 200 
Página 3 de 8
lux), debe retornar al modo Day Mode verificando las reglas de humedad y temperatura, activando el display,
actualizando el estado de los LEDs y controlando el riego y ventilación según se requiera.
Actualización de estado
El dispositivo debe enviar a consola cada 5 segundos información de estado de operación, mostrando una estructura
en formato JSON conteniendo:
• deviceMacAddress (MAC Address del Dispositivo)
• operationMode (NIGHT_MODE / DAY_MODE)
• currentHumidity (Valor en %)
• currentTemperature (Valor en °C)
• lightLevel (Valor analógico 0-1023)
• irrigationState (RIEGO, OPTIMAL, VENTILANDO)
• createdAt (Fecha y hora actual)
Technical constraints:
• El prototipo debe elaborarse con Wokwi y la programación debe ser con Arduino sketch (C++) aplicando
Object-Oriented Programming y basado en ModestIoT para el Embedded Application, creando clases y
objetos para representar el estado y comportamiento de los diversos tipos de sensors y actuators en el
device (cuando ModestIoT no incluya tipos que ofrezcan dichas características).
• El contenido de sketch.ino debería enfocarse en la generación de instancia de una clase que sea
descendiente de Device, e invocar al comportamiento correspondiente en los métodos setup() y loop().
• Aplique convenciones de nomenclatura en inglés, con principios, patrones y buenas prácticas de diseño
de software orientado a objetos.
• Declare constantes para evitar los magic string y magic number anti-patterns.
• Documente con comentarios en inglés sus archivos de código fuente con las convenciones de Doxygen
(ver Doxygen: Documenting the code en referencias).
• El MAC Address debe ser personalizado, utilizando un generador aleatorio de MAC Address donde el MAC
Address Prefix sean los 4 últimos dígitos de su código de estudiante (ver utilidad en línea en referencias
adicionales). El simulador de ESP32 cuenta con el atributo macAddress para asignar un valor
personalizado (ver ESP32 Simulator | Changing the MAC address en referencias adicionales).
• Aplique convenciones de nomenclatura en inglés, con principios, patrones y buenas prácticas de diseño
de software orientado a objetos, aplicando el approach de ModestIoT incluyendo descendientes de
Sensor, Actuator según se requiera, con Commands, Events, Command Handlers, Query Handlers.
• Documente con comentarios en inglés cada uno de sus archivos de código fuente. Además incluya title
blocks (ver lo referente a Explanation of Code en el Arduino Style Guide for Writing Content en la sección
de Referencias).
• Al inicio de la ejecución debe mostrarse en consola la información de la empresa, así como sus nombres y
apellidos como developer miembro del equipo de la empresa del caso. Considere en su sketch el código
necesario para describir y demostrar los modos de operación y actualización de estado requeridos en el
IoT Device.
• Exporte el proyecto de Wokwi y empaquete el mismo como un archivo .zip. (único formato válido) con el
nombre eb<NRC>u<código-estudiante>.zip (por ejemplo, eb17755u201621873.zip).
• Suba dos archivos: El archivo upc-pre-202610-1asi0572-examen-final-file_v1.pptx con sus respuestas
(renombrado como eb<NRC>u<código-estudiante>-file.pptx, por ejemplo eb17755u201621873-file.pptx,
sin empaquetar) y el archivo .zip con el proyecto de solución de Wokwi, en la Actividad indicada para el
Examen final.
En Wokwi, el archivo libraries.txt debe contener:
Página 4 de 8
# Wokwi Library List
# See https://docs.wokwi.com/guides/libraries
ArduinoJson
PubSubClient
ESP32Servo
LiquidCrystal I2C
DHT sensor library
Adafruit Unified Sensor
EspSoftwareSerial
HX711 Arduino Library
ModestIoT@wokwi:fb939e3848beed149e380d7392ca7967aa440c62
En base al caso:
Pregunta 1 (5 p.).
Bajo el marco de los 12 pasos del IoT System Design Steps (ver Anexo A), elabore el Definition of application service
layer requirements (requisitos relacionados con la capa de servicio de la aplicación). Aquí considere a) Definición de
la interfaz de usuario para cada servicio prestado (dependiendo del servicio la aplicación para dicho servicio podría
requerir una interfaz gráfica para web, móvil, o tablet, o desktop, o consola, o más de un tipo).
Pregunta 2 (15 p.)
Cree un nuevo proyecto en Wokwi y desarrolle un prototipo que satisfaga los requisitos planteados y technical
constraints para el Embedded Application del IoT Device solicitado en el caso.
Referencias
Wokwi Parts:
https://docs.wokwi.com/guides/esp32
https://docs.wokwi.com/guides/esp32-wifi
https://docs.wokwi.com/parts/wokwi-dht22
https://docs.wokwi.com/parts/wokwi-lcd1602
https://docs.wokwi.com/parts/wokwi-pushbutton-6mm
https://docs.wokwi.com/parts/wokwi-led
https://docs.wokwi.com/parts/wokwi-photoresistor-sensor
https://docs.wokwi.com/parts/wokwi-relay-module
https://docs.wokwi.com/parts/wokwi-ds1307
https://docs.wokwi.com/parts/wokwi-servo
Wokwi projects de referencia:
https://wokwi.com/projects/322410731508073042
https://wokwi.com/projects/344891772964438612
https://wokwi.com/projects/305193627138654786
https://wokwi.com/projects/323706614646309460
https://wokwi.com/projects/377396146936355841
Wokwi Libraries (buscar por nombre en Wokwi Library Manager):
ArduinoJson
OneWire
Time
WiFi
Página 5 de 8
Referencias adicionales:
MAC Address Generator
https://dnschecker.org/mac-address-generator.php
ESP32 Simulator | Changing the MAC address
https://docs.wokwi.com/guides/esp32#changing-the-mac-address
ESP32: Guide for DS1307 Real Time Clock (RTC) Module (Arduino IDE)
https://randomnerdtutorials.com/esp32-ds1307-real-time-clock-rtc-arduino/
Arduino Style Guide for Writing Content
https://bit.ly/arduinoWriting
Página 6 de 8
Rúbrica de calificación
Criterio de
Calificación
Sobresaliente (S) Esperado (E) Necesita Mejorar (M) Insuficiente (I)
C01. Definition
of application
service layer
requirements
Define de forma clara y
completa los requisitos
relacionados con la capa de
servicio de la aplicación, en
términos de definición de la
interfaz de usuario para cada
servicio prestado, estando
en todos los casos alineados
con las necesidades de la
solución de IoT.
Define de forma clara y
completa la mayoría de los
requisitos relacionados con la
capa de servicio de la
aplicación, en términos de
definición de la interfaz de
usuario para cada servicio
prestado, estando en todos
los casos alineados con las
necesidades de la solución de
IoT.
Define algunos de los
requisitos relacionados con la
capa de servicio de la
aplicación, en términos de
definición de la interfaz de
usuario para cada servicio
prestado, o en varios casos
estos no están alineados con
las necesidades de la
solución de IoT.
No brinda
respuesta o lo
especificado no
tiene relación
con el caso.
5.0 puntos 3.5 puntos 1.5 punto 0 puntos
C02. IoT Device
Design
El diseño de prototipo de
dispositivo cumple de forma
completa con la
configuración solicitada.
El diseño del prototipo de
dispositivo cumple con la
mayoría de los requisitos
sobre configuración.
El diseño del prototipo de
dispositivo cumple con
algunos de los requisitos
sobre configuración.
No diseña
prototipo de
dispositivo o
este no cumple
con la mayoría
de requisitos
sobre
configuración.
5.0 puntos 3.5 punto 1.5 puntos 0 puntos
C03. Software
Features
Se evidencia la funcionalidad
de las operaciones
solicitadas, proporcionando
en cada caso los valores
esperados y respondiendo
adecuadamente ante las
excepciones.
Se evidencia parcialmente la
funcionalidad de las
operaciones solicitadas,
proporcionando en algunos
casos los valores esperados y
respondiendo de forma
parcialmente adecuada ante
las excepciones.
La ejecución no cumple con
las funcionalidades
solicitadas.
No implementa
comportamiento
para el
dispositivo.
5.0 puntos 3.5 punto 1.5 puntos 0 puntos
C04. Quality
Attributes
Utiliza el lenguaje de
programación Arduino
Sketch C++. En todos los
casos Incluye información de
propósito y autoría como
comentarios. La codificación
tiene un estilo claro,
indentando los bloques de
código según los estándares
de programación
correspondientes al
lenguaje, aplicando una
lógica consistente en los
métodos, condicionales sin
escenarios no
contemplados, uso
adecuado de reutilización de
código para evitar
redundancia. Aplica patrones
de arquitectura y patrones
de diseño. Cumple de forma
completa con los technical
constraints.
Utiliza el lenguaje de
programación Arduino Sketch
C++. En la mayoría de los
casos Incluye información de
propósito y autoría como
comentarios. La codificación
es funcional, aplica en la
mayoría de casos los
estándares de indentación de
bloques de código, ó existen
algunas ineficiencias en la
codificación: redundancia ó
inconsistencias en la lógica
de programación. Aplica
parcialmente patrones de
arquitectura y patrones de
diseño. Cumple con la
mayoría de technical
constraints.
Utiliza el lenguaje de
programación Arduino Sketch
C++. En algunos casos
Incluye información de
propósito y autoría como
comentarios. La codificación
es funcional, pero solo aplica
algunos de los estándares de
indentación de bloques de
código, ó existen muchas
ineficiencias en la
codificación: redundancia ó
inconsistencias en la lógica
de programación. Aplica
algunos patrones de
arquitectura y patrones de
diseño. Cumple con solo
algunos de los technical
constraints.
No utiliza el
lenguaje de
programación
Arduino Sketch
C++, ó la
codificación es
funcional pero
no se evidencia
aplicación de
estándares ó
criterios de
eficiencia en la
codificación, con
ausencia de
comentarios, ó
no aplica
patrones de
arquitectura ni
patrones de
diseño, o la
codificación no
es funcional.
3.0 puntos 2.0 punto 1.0 puntos 0 puntos
C05. Naming
standards
El desarrollador aplica en
todos los nombres de
objetos de programación y
base de datos como
paquetes, componentes,
interfaces, clases, objetos,
variables, constantes,
métodos, la nomenclatura en
inglés y la nomenclatura
estándar para identificadores
de clases, objetos, miembros
de programación, así como
los recursos.
El desarrollador aplica en la
mayoría de casos la
nomenclatura en inglés y la
nomenclatura estándar para
identificadores de clases,
objetos, miembros de
programación, así como los
recursos.
El desarrollador aplica en
muy pocos casos la
nomenclatura en inglés y la
nomenclatura estándar para
identificadores de clases,
objetos, miembros de
programación, así como los
recursos.
El desarrollador
no aplica
nomenclatura en
inglés para los
objetos de
programación ó
recursos.
2.0 puntos 1.0 punto 0.5 puntos 0 puntos
Total 20 puntos 13.5 puntos 7.0 puntos 0 puntos
Lima, 14 de Julio del 2026
Página 7 de 8
Anexo A. IoT System Design Steps
Como se trató en clase, los ingenieros Eulalia Balestrieri, Luca De Vito, Francesco Lamonaca,
Francesco Picariello, Sergio Rapuano y Ioan Tudosa de la Universidad de Sannio en Italia,
proponen un conjunto de design guidelines para IoT System Design, sintetizados en 12 steps.
Página 8 de 8
Anexo B. Edge-To-Cloud Architecture Layers
Este diagrama que ilustra la relación entre las capas Edge Layer, Fog Layer y Cloud layer.
