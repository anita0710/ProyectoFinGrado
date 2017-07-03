#include <SPI.h>  //Importamos librería comunicación SPI
#include <Ethernet.h>  //Importamos librería Ethernet
 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };//Ponemos la dirección MAC de la Ethernet Shield que está con una etiqueta debajo la placa
IPAddress ip(192,168,1,50); //Asingamos la IP al Arduino
EthernetServer server(80); //Creamos un servidor Web con el puerto 80 que es el puerto HTTP por defecto

//Pin para el SENSOR MAGNÉTICO DE PUERTA 1
const int pinSensorPuerta1 = 2;

//Pin para el SENSOR MÁGNETICO DE PUERTA 2
const int pinSensorPuerta2 = 3;

//Pin para el SENSOR DE TEMPERATURA
const int pinSensorTemperatura = A0;

//Pin para el SENSOR DE PRESENCIA
const int pinSensorPresencia = 7;

//Pin para el SENSOR DE LUZ
const int pinSensorLuz = A2;

//Pin led de prueba
int led=13; 

String estado="OFF"; //Estado del Led inicialmente "OFF"

String estadoPuerta = "caca"; //Estado de la puerta

int estadoSensorPresencia = LOW; //Estado del sensor de presencia inicialmente LOW

String intruso;

String cantidadLuz;


void setup()
{ 
  Serial.begin(9600);
 
  // Inicializamos la comunicación Ethernet y el servidor
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  pinMode(pinSensorPuerta1, INPUT_PULLUP);
  pinMode(pinSensorPuerta2, INPUT_PULLUP);

  pinMode(pinSensorTemperatura, INPUT);

  pinMode(pinSensorPresencia, INPUT);

  pinMode(pinSensorLuz, INPUT);
 
  pinMode(led,OUTPUT);
}
 
void loop()
{

//Creamos un cliente Web
  EthernetClient client = server.available();

  
            int valorSensorPuerta1 = digitalRead(pinSensorPuerta1);
            int valorSensorPuerta2 = digitalRead(pinSensorPuerta2);

            if(valorSensorPuerta1 == LOW && valorSensorPuerta2 == HIGH ){
             estadoPuerta = "encajada";
            }

            if(valorSensorPuerta1 == LOW && valorSensorPuerta2 == LOW ){
              estadoPuerta = "cerrada con llave";
            }

            if(valorSensorPuerta1 == HIGH && valorSensorPuerta2 == HIGH ){
             estadoPuerta = "abierta";
            }

           

           
            

            int valorSensorPresencia = digitalRead(pinSensorPresencia);

            if(valorSensorPresencia == HIGH){
              digitalWrite(led, HIGH);
              intruso = "hay intruso";

              if(estadoSensorPresencia == LOW){
                estadoSensorPresencia = HIGH;

              }
            }

            else{
              digitalWrite(led, LOW);
              intruso = "no hay intruso";

              if(estadoSensorPresencia == HIGH){
                estadoSensorPresencia = LOW;

              }
            }





            int valorSensorLuz = analogRead(pinSensorLuz);

            if(valorSensorLuz <= 500){
              cantidadLuz = "luz insuficiente, necesario encender luces";
            }

            else{
              cantidadLuz = "luz suficiente, no es necesario encender luces";
            }





            int valorSensorTemperatura = analogRead(pinSensorTemperatura);
            float millivolts = (valorSensorTemperatura /1023.0) * 5000;
            float celsius = (millivolts / 10) ;
            Serial.print(celsius);

            

  
  
  //Cuando detecte un cliente a través de una petición HTTP
  if (client) {
    
    //Visualizamos que ha detectado un cliente por el monitor serial
    Serial.println("new client");
    
    //Una petición HTTP acaba con una línea en blanco
    boolean currentLineIsBlank = true; 
    
    //Mientras el cliente este conectado
    while (client.connected()) {

      //Si el cliente esta disponible
      if (client.available()) {

        //Leemos la petición HTTP carácter por carácter
        char c = client.read();
        
        //Visualizamos la petición HTTP por el Monitor Serial
        Serial.write(c);
          
        //Cuando reciba una línea en blanco, quiere decir que la petición HTTP ha acabado 
        //y el servidor Web está listo para enviar una respuesta en HTML
        if (c == '\n' && currentLineIsBlank) {
 
            // Enviamos al cliente una respuesta HTTP
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Refresh: 5");
            client.println();
 
            //Página web en formato HTML
            client.println("<html>");
            client.println("<head>");
            client.println("</head>");
            client.println("<body>");
            client.println("<h1 align='center'>DOMOTICA</h1>");
            
            /*client.println("<button onClick=location.href='./?LED=ON\' style='margin:auto;background-color: #84B1FF;color: snow;padding: 10px;border: 1px solid #3F7CFF;width:65px;'>");
            client.println("ON");
            client.println("</button>");
            client.println("<button onClick=location.href='./?LED=OFF\' style='margin:auto;background-color: #84B1FF;color: snow;padding: 10px;border: 1px solid #3F7CFF;width:65px;'>");
            client.println("OFF");
            client.println("</button>");
            client.println("<br /><br />");
            client.println("<b>LED = ");
            client.print(estado);
            client.println("</b><br />");
            */

            
            client.println("<div style='text-align:center;'>");

            client.println("<h3 align='center'>VIVIENDA</h3>");

            client.println("<b>TEMPERATURA = ");
            client.println(celsius);
            client.println("grados");

            client.println("</b><br />");

            client.println("<b>LUZ = ");
            client.println(cantidadLuz);

            client.println("</b><br />");
            client.println("</b><br />");


            client.println("<h3 align='center'>RECIBIDOR</h3>");

            client.println("<b>ESTADO DE LA PUERTA = ");
            client.println(estadoPuerta);

            client.println("</b><br />");

            client.println("<b>SENSOR PRESENCIA = ");
            client.println(intruso);

            client.println("</b><br />");
            client.println("</b><br />");


            client.println("<h3 align='center'>SALON</h3>");

            client.println("<b>ESTADO DE LA VENTANA = ");
            client.println(estadoPuerta);

            client.println("</b><br />");

            client.println("<b>SENSOR PRESENCIA = ");
            client.println(intruso);

            client.println("</b><br />");

            
            client.println("</b></body>");
            client.println("</html>");
            break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    //Dar tiempo al navegador para recibir los datos
    delay(10);
    client.stop();// Cierra la conexión
  }
}
