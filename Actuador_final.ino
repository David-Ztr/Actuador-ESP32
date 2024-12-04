
#include <WiFi.h>
const char* ssid     = "Realme";  // Nombre de la red wifi                                                                                                                                                                                                              <
const char* password = "david0022";  // Clave de red Wifi

String pagina1 = "<!DOCTYPE html>"
                 "<html>"
                 "<head>"
                 "<meta charset='utf-8' />"
                 "<meta name='viewport' content='width=device-width, initial-scale=1'>"
                 "<title>NEMA17 ESP32</title>"
                 "<link href='https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css' rel='stylesheet'>"
                 "</head>"
                 "<body class='bg-light'>"
                 "<div class='container text-center mt-5'>"
                 "<h1 class='mb-4'>NEMA17</h1>"
                 "<div class='row'>"
                 "<div class='col-6'>"
                 "<a href='/H1' class='btn btn-danger btn-lg w-100' role='button'>Izquierda</a>"
                 "</div>"
                 "<div class='col-6'>"
                 "<a href='/L1' class='btn btn-primary btn-lg w-100' role='button'>Derecha</a>"
                 "</div>"
                 "</div>"
                 "</div>"
                 "<script src='https://code.jquery.com/jquery-3.5.1.slim.min.js'></script>"
                 "<script src='https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.2/dist/umd/popper.min.js'></script>"
                 "<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js'></script>"
                 "</body>"
                 "</html>";

WiFiServer server(80);

#define x_paso 2    // Define el Pin de STEP para Motor de eje X
#define x_dire 15   // Define el Pin de DIR  para Motor de eje X
#define x_habi 13    // Define el Pin de ENABLE  para Motor de eje X

int retardo = 1000;   // Menor numero el giro es mas rapido
int tiempo = 200;   // Durante cuanto timpo da el giro el motor (vueltas)
int derecha = 1;      // Variable para que sea más comodo a la hora de ver
int izquierda = 0;    // Variable para que sea más comodo a la hora de ver

void setup()
{
    pinMode(x_paso, OUTPUT); pinMode(x_dire, OUTPUT); pinMode(x_habi, OUTPUT);   // Habilitas las variables anteriores como salidas
    Serial.begin(115200); // Habilitas el monitor serie para poder ver los baudios/segundos 

    // Empezamos a conectarnos a la red wifi en este caso del movil

    Serial.println(); // Pasa por el monitor un salto de línea
    Serial.println(); // Pasa por el monitor un salto de línea
    Serial.print("Conectando a "); // Pasa por el monitor conectando a
    Serial.println(ssid);

    WiFi.begin(ssid, password); // Procede a conectarse a la red wifi

    while (WiFi.status() != WL_CONNECTED) { // Comprueba si está conectado a la wifi
        delay(500);
        Serial.print(".");
    }

    Serial.println("");   // Una vez conectado a la red wifi pasa por monitor 
    Serial.println("WiFi connectedada."); // como la wifi está conectada y procede
    Serial.println("Dirección IP: "); // a pasar por el monitor la dirección IP
    Serial.println(WiFi.localIP());
    
    server.begin(); // Crea el servidor

}

int value = 0; // Establece el valor a cero

void loop(){
 WiFiClient client = server.available();   // Revisa si hay clientes en el servidor

  if (client) {                             // Si hay un cliente dentro del servidor,
    Serial.println("Nuevo Cliente.");       // imprime un mensaje por el puerto serie
    String currentLine = "";                // crea una cadena para contener los datos entrantes del cliente
    while (client.connected()) {            // bucle para ver si el cliente está conectado
      if (client.available()) {             // Si hay algo que leer del cliente,
        char c = client.read();             // lo lee, después imprime por el monitor serie
        Serial.write(c);                     
        if (c == '\n') {         

          // si la línea actual está en blanco, significa que tienes dos caracteres de nueva línea seguidos.
          // ese es el final de la solicitud HTTP del cliente, así que envía una respuesta:

          if (currentLine.length() == 0) {
            // Los encabezados HTTP siempre comienzan con un código de respuesta (por ejemplo, HTTP/1.1 200 OK)
            // y un tipo de contenido para que el cliente sepa qué esperar, seguido de una línea en blanco:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // El contenido del HTTP sigue la cabezera
            client.print(pagina1);
            // La respuesta del HTTP termina con otro salto de línea:
            client.println();
            // Cierre del bucle while:
            break;
          } else {    // Si tienes una nueva línea, limpia la antigua y escribe lo nuevo:
            currentLine = "";
          }
        } else if (c != '\r') {  // Si tiene algo de acarreo devuelve el caracter,
          currentLine += c;      // lo añade a la nueva línea creada
        }

        // Chequeamos si el cliente ha decidido tocar el boton Derecha o Izquierda:
        if (currentLine.endsWith("GET /H1")) {
          giro(x_paso,x_dire,x_habi,derecha);
         
        }
        if (currentLine.endsWith("GET /L1")) { 
          giro(x_paso,x_dire,x_habi,izquierda);
        }
      }
    }
    // Cerramos conexión con el cliente:
    client.stop();
    Serial.println(" Cliente Desconectado.");
  }
}


void giro(int paso_,int dire_,int habi_,int dir) {
   digitalWrite(habi_, LOW);  // Habilita el Driver
  if( dir==0){ 
   digitalWrite(dire_, LOW);   // direccion de giro izquierda
   for(int i=0;i<tiempo;i++){  // da  pasos por un tiempo  
    digitalWrite(paso_, HIGH);      
    delayMicroseconds(retardo);          
    digitalWrite(paso_, LOW);       
    delayMicroseconds(retardo); 
   }
  }
  if( dir==1){ 
  digitalWrite(dire_, HIGH);   // direccion de giro derechaA
  for(int i=0;i<tiempo;i++){   // da  pasos por un tiempo  
    digitalWrite(paso_, HIGH);      
    delayMicroseconds(retardo);          
    digitalWrite(paso_, LOW);       
    delayMicroseconds(retardo);  
   }
  }
  digitalWrite(habi_, HIGH);   // quita la habilitacion del Driver

}
