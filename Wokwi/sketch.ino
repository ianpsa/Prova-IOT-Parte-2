#include <WiFi.h>
#include <HTTPClient.h>

#define led_azul 9 // Pino utilizado para controle do led azul
#define led_verde 41 // Pino utilizado para controle do led verda
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led azul

const int buttonPin = 18;  // Define o pino do botão
int lastState = LOW; // define ultimo estado do botão para evitar debounce
int currentState; // define a variável para armazenar o estado atual do botão

const int ldrPin = 4;  // Define o pino do LDR
int threshold=600;

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_azul,OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);
  pinMode(led_Amarelo, OUTPUT);

  // Inicialização das entradas
  pinMode(buttonPin, INPUT_PULLUP); // Inicializa o botão no pino referido como um input
  pinMode(ldrPin, INPUT); // inicializa ldr como input

  digitalWrite(led_azul, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);
  digitalWrite(led_Amarelo, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  // Verifica estado do botão
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) { // caso a resposta da requisição http for maior que zero exibe-a como int e como string
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: "); // se não exibe erro e o código http
      Serial.println(httpResponseCode);
      }
      http.end(); //fecha a conexão http
    }

  else {
    Serial.println("WiFi Disconnected"); // se não está conectado no wifi exibe mensagem
  }
}

void loop() { //configura loop contínuo de código
  int ldrstatus=analogRead(ldrPin); //define a variável ldrstatus como o valor lido no pino do ldr
  int currentState = digitalRead(buttonPin);  // Lê o estado do botão
  int sinalVermelho = 0;

  if(ldrstatus<=threshold){ // se o valor analógico lido no pino for menor ou igual o threshold exibe mensagem
    Serial.print("Esta escuro, ligue o led");
    Serial.println(ldrstatus);
    digitalWrite(led_Amarelo, HIGH);
    digitalWrite(led_Amarelo, LOW);
    delay(1000);  // pisca o led amarelo a cada 1 segundo

  }else{
    Serial.print("Esta claro, desligue o led"); // se não exibe outra mensagem
    Serial.println(ldrstatus);
    digitalWrite(led_Verde, HIGH);
    delay(3000);
    digitalWrite(led_Verde, LOW);
    digitalWrite(led_Amarelo, HIGH);
    delay(2000);

    if(currentState == LOW){
      digitalWrite(led_Amarelo, LOW);
      digitalWrite(led_Vermelho, HIGH);
      sinalVermelho = 1;


    } else if(lastState == LOW && currentState == HIGH) {
      digitalWrite(led_Vermelho, HIGH);
      delay(1000);
      digitalWrite(led_Vermelho, LOW);
      digitalWrite(led_Verde, HIGH);

      lastState = currentState;

    }

    if(countPress == 3 && sinalVermelho == 1) {
      http.begin(serverPath.c_str());
      
      String httpSentData = "Alerta, o botao foi pressionado 3 vezes!"; // Mensagem de alerta    
      int httpSentCode = http.POST(httpRequestData); // Envio do alerta via http

      http.end(); //fecha a conexão http

      Serial.print("Alerta enviado: ");
      Serial.println(httpSentCode);
    }

    delay(5000);
    digitalWrite(led_Vermelho, LOW);
  }
}