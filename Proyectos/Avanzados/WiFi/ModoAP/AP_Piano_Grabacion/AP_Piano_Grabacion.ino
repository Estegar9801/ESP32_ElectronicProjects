/* 
  Proyecto: Piano electrónico - Modo AP ESP32
  Descripción: Estableciendo el ESP32 como punto de acceso, se disponibiliza una interfaz la cuál estará disponible para los usuarios que se conecten al
  dispositivo con las credenciales correctas. Dentro de esta interfaz, podrán interactuar con un piano electrónico, el cuál permitirá el control del buzzer
  conectado a la placa, emitiendo tonos con las diferentes notas y crear canciones.
  Autor: David Esteban Garnica - Ingeniería Electrónica
  Fecha de creación: 15/09/2024
  Última modificación: 26/04/2025
*/
// ------------------------------------------------------------------------------
// Carga de librerías
// ------------------------------------------------------------------------------
#include <WiFi.h>
#include <NetworkClient.h>
#include <WiFiAP.h>

// ------------------------------------------------------------------------------
// Definición de pines
// ------------------------------------------------------------------------------
#define pinBuzzer 13

// ------------------------------------------------------------------------------
// Definición de las credenciales de la red
// ------------------------------------------------------------------------------
const char *ssid = "PianoWiFi";
const char *password = "asdfg123";
// ------------------------------------------------------------------------------
// Creacion de la red
// ------------------------------------------------------------------------------
NetworkServer server(80);
String ultimaLinea = "";
String lineaActual = "";
// ------------------------------------------------------------------------------
// Definición de variables para controlar el buzzer
// ------------------------------------------------------------------------------
uint16_t frecuenciaPWMBuzzer = 2000;
uint16_t resolucionPWMBuzzer = 16;
uint16_t duracionPulsoBuzzer = 100;

// Almacena la frecuencia de cada tono correspondiente a cada nota del piano
uint16_t frecuenciaNotas [17] = { 261, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523, 539, 556, 573,592 };

// Graba la tecla oprimida en la grabación.
uint16_t cancionGrabada [250];
bool enGrabacion = false;
String Grabando = "";
uint16_t numeroNotasGrabadas = 0;
uint8_t rebote = 0;

void setup() {
  ledcAttach(pinBuzzer, frecuenciaPWMBuzzer, resolucionPWMBuzzer);

  Serial.begin(115200); // Inicio del puerto serial
  Serial.print("Configurando Punto de acceso ...");

  if(!WiFi.softAP(ssid, password)){
    log_e("Falla en la creación del punto de acceso.");
    while(1);
  }

  
  IPAddress direccionIP = WiFi.softAPIP();  // Se define la dirección IP del punto de acceso.
  delay(1000);
  Serial.print("Mi dirección IP es: "); 
  Serial.println(direccionIP);  // Se imprime en el puerto serial la dirección IP a la cuál se debe acceder desde el navegador.
  server.begin(); 

  Serial.println("Servidor iniciado.");
}

void loop() {
  NetworkClient client = server.accept();  // Detecta si hay un nuevo cliente entrando

  if(client){
    Serial.println("Nuevo cliente conectado");  // Imprime mensaje en el monitor serie
    String currentLine  = ""; // Se crea un String para mantener los datos entrantes del cliente
    while(client.connected()){ // Mientras el cliente está conectado
      if(client.available()){  // Si hay nuevos datos que leer del cliente
        char c = client.read();  // Lee un byte
        Serial.write(c);  // Escribe la lectura en el monitor serie

        if (c == '\n') {

          // Si la línea actual está en blanco, tiene dos caracteres de nueva línea seguidos
          // Ese es el final de la solicitud HTTP del cliente, así que se se envía una respuesta:
          if(currentLine.length() == 0){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type: text/html");
            client.println();

            // El contenido de la respuesta HTTP continua tras la definición del encabezado.
            // En cada una de las siguientes líneas se envía como respuesta a la solicitud la estructura de la página HTML5, los estilos como CSS3
            // Y algunos comandos de Javascript para mejorar la experiencia a la hora de interactura con el piano.
            client.print("<html lang=\"es-en\">");
            client.print("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            client.print("<meta charset=\"UTF-8\">");
            client.print("<title>Modo AP ESP32 | Piano</title>");
            client.print("<style type = \"text/css\">");
            client.print("body{ background: #2D2D2D; color: #FFFFFF; font-family: 'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif; }");
            client.print(".container{ max-width: 1440px; padding: 0px 20px; text-align: center; }");
            client.print(".header { box-shadow: 0px 4px 4px 0px rgba(0,0,0,0.75); margin-bottom: 10px; padding: 5px; }");
            client.print(".header-title{ font-size: 35px; }");
            client.print(".principal{ text-align: center; }");
            client.print(".principal-info__title{ font-size: 1.5rem; text-align: center; }");
            client.print(".principal-info__text{ font-size: 1.2rem; text-align: left; line-height: 1.3; }");
            client.print(".principal-piano{ display: flex; width: fit-content; padding: 20px; flex-direction: column; align-items: center; margin: auto; box-shadow: 0px 4px 10px 0px rgba(0,0,0,0.75); background: rgb(20, 20, 20); border-radius: 10px; margin-bottom: 30px; }");
            client.print(".piano__panel{ padding: 20px; width: 100%; margin-bottom: 30px; color: #FFFFFF; display: flex; justify-content: space-between; box-shadow: 0px 4px 2px 0px rgba(0,0,0,1); }");
            client.print(".tecla-grabacion{ border-radius: 50%; width: 50px; height: 50px; background: red; box-shadow: 0px 4px 2px 0px rgba(0,0,0,1); }");
            client.print("button { border: none; }");
            client.print(".tecla-reproducir{ border-radius: 20px 10px; padding: 0px 10px; background: #314ef7; color: #FFF; }");
            client.print(".tecla-reproducir:hover { cursor: pointer }");
            client.print(".tecla-borrar { border-radius: 20px 10px; padding: 0px 10px; background: #ff5050; color: #FFF; }");
            client.print(".tecla-borrar:hover { cursor: pointer }");
            client.print(".piano__teclado{ display: flex; justify-content: center; gap: 1px; }");
            client.print(".tecla { width: 5rem; cursor: pointer; position: relative; height: 20rem; border-radius: 0px 0px 10px 10px; box-shadow: 0px 4px 2px 0px rgba(0,0,0,1); }");
            client.print(".tecla--blanca { background: #FFFFFF; }");
            client.print(".tecla--blanca:hover{ background: #F0F0F0; }");
            client.print(".tecla--negra{ width: 2rem; height: 13rem; border-top-left-radius: 0; border-top-right-radius: 0; background-color: #000000; z-index: 2; margin: 0px -16px; }");
            client.print("@media ( max-width: 720px){ .principal-piano{ padding: 5px; } .piano__panel{ padding: 5px; } .tecla{ width: 2.1rem; } }");
            client.print("</style>");
            client.print("</head>");
            client.print("<body>");
            client.print("<header class=\"container header\"> ");
            client.print("<h1 class=\"header-title\"> Modo AP ESP32 </h1>");
            client.print("</header>");
            client.print("<main class=\"container principal\">");
            client.print("<div class=\"principal-info\">    ");
            client.print("<h2 class=\"principal-info__title\">¡Felicidades por conectarte!</h2>");
            client.print("<p class=\"principal-info__text\">Esta vez traemos este <b>piano electrónico</b>, con el cuál podrás interactuar y controlar el buzzer que está conectado al ESP32.</p>");
            client.print("</div>");
            client.print("<div class=\"principal-piano\">");
            client.print("<div class=\"piano__panel\">");
            client.print("<a class=\"tecla-grabacion\" href=\"/grabar\"></a>");
            client.print("<h3>" + Grabando + "</h3>");
            client.print("<button class=\"tecla-reproducir\" onclick=\"pulsarBoton('reproducir')\">Reproducir</button>");
            client.print("<button class=\"tecla-borrar\" onclick=\"pulsarBoton('borrar')\">Borrar</button>");
            client.print("</div>");
            client.print("<div class=\"piano__teclado\">");
            client.print("<button type=\"submit\" onclick=\"pulsarBoton('NoA')\" class=\"tecla tecla--blanca\"></button>");
            client.print("<button type=\"submit\" onclick=\"pulsarBoton('NoB')\" class=\"tecla tecla--negra\"></button>");
            client.print("<button type=\"submit\" onclick=\"pulsarBoton('NoC')\" class=\"tecla tecla--blanca\"></button>");
            client.print("<button type=\"submit\" onclick=\"pulsarBoton('NoD')\" class=\"tecla tecla--negra\"></button>");
            client.print("<button type=\"submit\" onclick=\"pulsarBoton('NoE')\" class=\"tecla tecla--blanca\"></button>");            
            client.print("<button type=\"submit\" onclick=\"pulsarBoton('NoF')\" class=\"tecla tecla--blanca\"></button>");
            client.print("<button type=\"submit\" onclick=\"pulsarBoton('NoG')\" class=\"tecla tecla--negra\"></button>");
            client.print("<button type=\"submit\" onclick=\"pulsarBoton('NoH')\" class=\"tecla tecla--blanca\"></button>");
            client.print("<button type=\"submit\" onclick=\"pulsarBoton('NoI')\" class=\"tecla tecla--negra\"></button>");
            client.print("<button type=\"submit\" onclick=\"pulsarBoton('NoJ')\" class=\"tecla tecla--blanca\"></button>");
            client.print("<button type=\"submit\" onclick=\"pulsarBoton('NoK')\" class=\"tecla tecla--negra\"></button>");
            client.print("<button type=\"submit\" onclick=\"pulsarBoton('NoL')\" class=\"tecla tecla--blanca\"></button>");            
            client.print("<button type=\"submit\" onclick=\"pulsarBoton('NoM')\" class=\"tecla tecla--blanca\"></button>");
            client.print("<button type=\"submit\" onclick=\"pulsarBoton('NoN')\" class=\"tecla tecla--negra\"></button>");
            client.print("<button type=\"submit\" onclick=\"pulsarBoton('NoO')\" class=\"tecla tecla--blanca\"></button>");
            client.print("<button type=\"submit\" onclick=\"pulsarBoton('NoP')\" class=\"tecla tecla--negra\"></button>");
            client.print("<button type=\"submit\" onclick=\"pulsarBoton('NoQ')\" class=\"tecla tecla--blanca\"></button>");            
            client.print("</div></div></main>");
            client.print("<script>");
            client.print("function pulsarBoton(boton){ ");
            client.print("enlace = \"/\" + boton;"); 
            client.print("actual = location.pathname; if (enlace == actual) { enlace += \"2\"; }"); 
            client.print("window.history.pushState(null, null, enlace);");            
            client.print("}");
          
            client.print("</script>");
            client.print("</body>");            
            client.print("</html>");

            // La respuesta HTTP termina con otra línea en blanco
            client.println();

            // Rompe el ciclo 
            break;
          }else { // Si se tiene una nueva línea, se limpia la línea actual            
            currentLine = "";
            lineaActual = "";
          }          
        }else if(c != '\r'){  // Si se tiene más de un carácter de retorno
          currentLine += c; // Lo añade al final de la línea actual
          lineaActual = currentLine;          
        }
        // Se identifica en el encabezado de la solicitud, si alguna de las teclas del piano fue oprimida, se identifica cuál y se activa el buzzer
        // enviando la posición de la nota en el arreglo de frecuencias.
        
        rebote++;

        if (rebote % 2 == 0){
          if(currentLine.endsWith("/NoA")){
            sonarBuzzer(0);
            if (enGrabacion){
              grabarNota(0);
            }
          }
          if(currentLine.endsWith("/NoB")){          
            sonarBuzzer(1);
            if (enGrabacion){ 
              grabarNota(1);
            }
          }
          if(currentLine.endsWith("/NoC")){          
            sonarBuzzer(2);
            if (enGrabacion){
              grabarNota(2);
            }
          }
          if(currentLine.endsWith("/NoD")){          
            sonarBuzzer(3);
            if (enGrabacion){
              grabarNota(3);
            }
          }
          if(currentLine.endsWith("/NoE")){          
            sonarBuzzer(4);
            if (enGrabacion){
              grabarNota(4);
            }
          }
          if(currentLine.endsWith("/NoF")){          
            sonarBuzzer(5);
            if (enGrabacion){
              grabarNota(5);
            }
          }
          if(currentLine.endsWith("/NoG")){          
            sonarBuzzer(6);
            if (enGrabacion){
              grabarNota(6);
            }
          }
          if(currentLine.endsWith("/NoH")){          
            sonarBuzzer(7);
            if (enGrabacion){
              grabarNota(7);
            }
          }
          if(currentLine.endsWith("/NoI")){          
            sonarBuzzer(8);
            if (enGrabacion){
              grabarNota(8);
            }
          }
          if(currentLine.endsWith("/NoJ")){          
            sonarBuzzer(9);
            if (enGrabacion){
              grabarNota(9);
            }
          }
          if(currentLine.endsWith("/NoK")){          
            sonarBuzzer(10);
            if (enGrabacion){
              grabarNota(10);
            }
          }
          if(currentLine.endsWith("/NoL")){          
            sonarBuzzer(11);
            if (enGrabacion){
              grabarNota(11);
            }
          }
          if(currentLine.endsWith("/NoM")){          
            sonarBuzzer(12);
            if (enGrabacion){
              grabarNota(12);
            }
          }
          if(currentLine.endsWith("/NoN")){          
            sonarBuzzer(13);
            if (enGrabacion){
              grabarNota(13);
            }
          }
          if(currentLine.endsWith("/NoO")){          
            sonarBuzzer(14);
            if (enGrabacion){
              grabarNota(14);
            }
          }
          if(currentLine.endsWith("/NoP")){          
            sonarBuzzer(15);
            if (enGrabacion){
              grabarNota(15);
            }
          }
          if(currentLine.endsWith("/NoQ")){          
            sonarBuzzer(16);
            if (enGrabacion){
              grabarNota(16);
            }
          }                  
          // Si se oprimió el botón de reproducir, se reproduce la canción.
          if(currentLine.endsWith("/reproducir")){
            Serial.println("Voy a reproducir la canción.");
            reproducirCancion();
          }
        }
        // Si oprime el botón de grabar cambia de estado de la variable de grabación.
        if(currentLine.endsWith("/grabar")){     
          enGrabacion = !enGrabacion;
          if ( enGrabacion ) {            
            Grabando = "Grabando";            
          }else{            
            Grabando = "";            
          }
        }

        if(currentLine.endsWith("/borrar")){
          numeroNotasGrabadas = 0;
        }
      }
    }
    client.stop();    
    Serial.println("Cliente desconectado.");
  }
}
void reproducirCancion(){
  uint16_t recorridoCancion = 0;

  while (recorridoCancion < sizeof(cancionGrabada) && recorridoCancion < numeroNotasGrabadas){    
    sonarBuzzer(cancionGrabada[recorridoCancion]);
    recorridoCancion += 2;
    delay(300);
  }
}
/*  Función: sonarBuzzer
    Descripción: Activa el buzzer conectado al ESP32 durante un instante de tiempo, especificando la frecuencia del tono que se desea reproducir. 
    Se activa con cada interacción con el piano virtual.
*/
void sonarBuzzer(uint16_t i){
  tone(pinBuzzer, frecuenciaNotas[i], duracionPulsoBuzzer);
  Serial.print(frecuenciaNotas[i]);
}

void grabarNota(uint16_t nota){
  cancionGrabada[numeroNotasGrabadas] = nota;
  numeroNotasGrabadas += 1;
}
