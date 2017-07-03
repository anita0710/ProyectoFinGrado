/* Setup shield-specific #include statements */
#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>

#include <Temboo.h>
#include "TembooAccount.h"    // Contains Temboo account information

byte ethernetMACAddress[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,50);
EthernetServer server(80);

int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 10;   // Maximum number of times the Choreo should be executed

//Pin para el SENSOR MAGNÉTICO DE PUERTA 1
const int pinSensorPuerta1 = 2;

const int pinSensorLlave = 3;


String estadoPuerta;


//int estadoPrevioSensorPuerta1 = 1;

void setup() {
   Serial.begin(9600);

   Ethernet.begin(ethernetMACAddress, ip);
   server.begin();
   Serial.print("server is at ");
   Serial.println(Ethernet.localIP());


   pinMode(pinSensorPuerta1, INPUT_PULLUP);
   pinMode(pinSensorLlave, INPUT_PULLUP);

  
  // For debugging, wait until the serial console is connected.
  delay(4000);
  while(!Serial);
  Serial.print("DHCP:");
  if (Ethernet.begin(ethernetMACAddress) == 0) {
    Serial.println("FAIL");
    while(true);
  }
  Serial.println("OK");
  delay(5000);
  Serial.println("Setup complete.\n");
}
void loop() {

  EthernetClient client = server.available();

  int valorSensorPuerta1 = digitalRead(pinSensorPuerta1);
  int valorSensorLlave = digitalRead(pinSensorLlave);













  
  
  if ( valorSensorPuerta1 == HIGH ) {
    
    estadoPuerta = "abierta";
        Serial.println(estadoPuerta);

    
    Serial.println("Running SendEmail - Run #" + String(numRuns++));
    TembooChoreo SendEmailChoreo(client);

        estadoPuerta = "abierta";

    //Invoca al cliente Temboo
    SendEmailChoreo.begin();
    
    //Introduce las credenciales de la cuenta Temboo
    SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);
    
    //Introduce los datos de las Choreos
    //El cuerpo del email
    String MessageBodyValue = "MessageBody";
    SendEmailChoreo.addInput("MessageBody", MessageBodyValue);
    //El asunto del email
    String SubjectValue = "ABIERTA";
    SendEmailChoreo.addInput("Subject", SubjectValue);
    //La contraseña de la cuenta Gmail desde la que enviamos el email
    String PasswordValue = "brdzgwckyzjzicpz";
    SendEmailChoreo.addInput("Password", PasswordValue);
    //El email (id) de la cuenta Gmail desde la que enviamos el email
    String UsernameValue = "ana.cahe.7@gmail.com";
    SendEmailChoreo.addInput("Username", UsernameValue);
    //El email (id) de la cuenta Fmail que recibirá el email
    String ToAddressValue = "elena.ca.he@gmail.com";
    SendEmailChoreo.addInput("ToAddress", ToAddressValue);
    
    //Identifica la Choreo que se quiere ejecutar
    SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");
    // Run the Choreo; when results are available, print them to serial
    SendEmailChoreo.run();



    
    while(SendEmailChoreo.available()) {
      char c = SendEmailChoreo.read();
      Serial.print(c);
    }
    SendEmailChoreo.close();

    
  }
  
  if(valorSensorPuerta1 == LOW && valorSensorLlave == HIGH){

    estadoPuerta = "encajada";

    Serial.println("Running SendEmail - Run #" + String(numRuns++));
    TembooChoreo SendEmailChoreo(client);
    // Invoke the Temboo client
    SendEmailChoreo.begin();
    // Set Temboo account credentials
    SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);
    // Set Choreo inputs
    String MessageBodyValue = "ENCAJADA";
    SendEmailChoreo.addInput("MessageBody", MessageBodyValue);
    String SubjectValue = "Me llamo Ana";
    SendEmailChoreo.addInput("Subject", SubjectValue);
    String PasswordValue = "brdzgwckyzjzicpz";
    SendEmailChoreo.addInput("Password", PasswordValue);
    String UsernameValue = "ana.cahe.7@gmail.com";
    SendEmailChoreo.addInput("Username", UsernameValue);
    String ToAddressValue = "elena.ca.he@gmail.com";
    SendEmailChoreo.addInput("ToAddress", ToAddressValue);
    // Identify the Choreo to run
    SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");
    // Run the Choreo; when results are available, print them to serial
    SendEmailChoreo.run();
    while(SendEmailChoreo.available()) {
      char c = SendEmailChoreo.read();
      Serial.print(c);
    }
    SendEmailChoreo.close();
  }




//Cuando detecte un cliente a través de una petición HTTP
  


  
  Serial.println("\nWaiting...\n");
  delay(30000); // wait 30 seconds between SendEmail calls



  
}
