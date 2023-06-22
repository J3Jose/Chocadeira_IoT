#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>


// Configuração da rede Wi-Fi - NOME_DA_REDE e SENHA_DA_REDE
const char* ssid = "nome_da_rede_Wi-Fi";
const char* password = "Senha";

// Configuração do MQTT
const char* mqtt_server = "3.22.51.250";
const int mqtt_port = 1883;
const char* mqtt_topic = "esp32/valor/setpointTemperatura";
const char* mqtt_client_id = "ESP32Client";

// Definição dos pinos dos componentes
const int botaoAlterar = 18;               //Botão alterar
const int botaoSelecionarTemperatura = 5;  //Botão Selecionador
const int botaoSelecionarUmidade = 19;     //Botão Selecionador
const int pinoPotenciometro = 36;          //Entrada de valores potenciometro
const int pinoLuz = 27;                    //Aquecedor
const int pinoVentoinha = 26;              //Ventoinha
const int pinoBombaAgua = 33;              //Bomba
const int pinoMotor = 25;                  //Motor

// Defina as constantes para o tempo de espera e a duração da operação do motor de girar os ovos
const unsigned long tempoEspera = 12 * 60 * 600 * 1000;  // 12 horas em milissegundos
const unsigned long duracaoMotor = 10 * 1000;            // 10 segundos em milissegundos
unsigned long tempoAnterior = 0;                         // Variável para armazenar o tempo anterior
// Definição dos botões
int EstadoBotao1;
int EstadoBotao2;
int EstadoBotao3;
int EstadoLuz;
int EstadoBomba;
// Definição das constantes do PID
double setpointTemperatura = 0;  // Valor de referência da temperatura
double setpointUmidade = 0;      // Valor de referência da umidade
double inputTemperatura = 0;     // Valor medido da temperatura
double inputUmidade = 0;         // Valor medido da umidade
double outputLuz = 0;            // Saída do controle da luz
double outputVentoinha = 0;      // Saída do controle da ventoinha
double outputBombaAgua = 0;      // Saída do controle da bomba de água
// Definição do objeto DHT - sensor de umidade e temperatura
#define DHTPIN 15      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);
// Definição do objeto LCD
#ifdef DATA_PRESENTER_ENABLED
#define LCD_ADDRESS 0x27
#define LCD_ROWS 2
#define LCD_COLUMNS 16
LiquidCrystal_I2C lcd(
  LCD_ADDRESS,
  LCD_COLUMNS,
  LCD_ROWS);
#endif
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
// Variáveis de controle
int variavelSelecionada = 0;  //Setar valores
double valorVariavel1 = 0;    //Temperatura
double valorVariavel2 = 0;    //Umidade

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void mqttSendJson(float valor1) {
  //Envia a mensagem ao broker
  /// . produzindo mensagem
  DynamicJsonDocument doc(1024);
  doc["device"] = "ESP32";
  doc["Temperatura"] = valor1;
  char JSONmessageBuffer[200];
  serializeJson(doc, JSONmessageBuffer);
  client.publish("esp32/valor/Temperatura", JSONmessageBuffer);
  //Serial.print("msg json enviado: ");
  //Serial.println(JSONmessageBuffer);
}
void mqttSendJson2(float valor2) {
  //Envia a mensagem ao broker
  /// . produzindo mensagem
  DynamicJsonDocument doc(1024);
  doc["device"] = "ESP32";
  doc["Humidade"] = valor2;
  char JSONmessageBuffer[200];
  serializeJson(doc, JSONmessageBuffer);
  client.publish("esp32/valor/Humidade", JSONmessageBuffer);
  //Serial.print("msg json enviado: ");
  //Serial.println(JSONmessageBuffer);
}

void mqttSendJson3(float EstadoLuz) {
  //Envia a mensagem ao broker
  /// . produzindo mensagem

  if (EstadoLuz == 1) {
    DynamicJsonDocument doc(1024);
    doc["device"] = "ESP32";
    doc["Luz"] = "Ligado";
    char JSONmessageBuffer[200];
    serializeJson(doc, JSONmessageBuffer);
    client.publish("esp32/valor/Luz", JSONmessageBuffer);
    //Serial.print("msg json enviado: ");
    //Serial.println(JSONmessageBuffer);
  }
  if (EstadoLuz == 0) {
    DynamicJsonDocument doc(1024);
    doc["device"] = "ESP32";
    doc["Luz"] = "Desligado";
    char JSONmessageBuffer[200];
    serializeJson(doc, JSONmessageBuffer);
    client.publish("esp32/valor/Luz", JSONmessageBuffer);
    //Serial.print("msg json enviado: ");
    //Serial.println(JSONmessageBuffer);
  }
}

void mqttSendJson4(float setpointUmidade) {
  //Envia a mensagem ao broker
  /// . produzindo mensagem
  DynamicJsonDocument doc(1024);
  doc["device"] = "ESP32";
  doc["setpointUmidade"] = setpointUmidade;
  char JSONmessageBuffer[200];
  serializeJson(doc, JSONmessageBuffer);
  client.publish("esp32/valor/setpointUmidade", JSONmessageBuffer);
  //Serial.print("msg json enviado: ");
  //Serial.println(JSONmessageBuffer);
}

void mqttSendJson5(float setpointTemperatura) {
  //Envia a mensagem ao broker
  /// . produzindo mensagem
  DynamicJsonDocument doc(1024);
  doc["device"] = "ESP32";
  doc["setpointTemperatura"] = setpointTemperatura;
  char JSONmessageBuffer[200];
  serializeJson(doc, JSONmessageBuffer);
  client.publish("esp32/valor/setpointTemperatura", JSONmessageBuffer);
  //Serial.print("msg json enviado: ");
  //Serial.println(JSONmessageBuffer);
}

void mqttSendJson6(float EstadoBomba) {
  //Envia a mensagem ao broker
  /// . produzindo mensagem
  if (EstadoBomba == 1) {
    DynamicJsonDocument doc(1024);
    doc["device"] = "ESP32";
    doc["Bomba"] = "Ligado";
    char JSONmessageBuffer[200];
    serializeJson(doc, JSONmessageBuffer);
    client.publish("esp32/valor/Bomba", JSONmessageBuffer);
    //Serial.print("msg json enviado: ");
    //Serial.println(JSONmessageBuffer);
  }
  if (EstadoBomba == 0) {
    DynamicJsonDocument doc(1024);
    doc["device"] = "ESP32";
    doc["Bomba"] = "Desligado";
    char JSONmessageBuffer[200];
    serializeJson(doc, JSONmessageBuffer);
    client.publish("esp32/valor/Bomba", JSONmessageBuffer);
    //Serial.print("msg json enviado: ");
    //Serial.println(JSONmessageBuffer);
  }
}

void mqttSendJsonIO(void) {
  //Envia a mensagem ao broker
  /// . produzindo mensagem
  DynamicJsonDocument doc(1024);
  doc["device"] = "ESP32";
  doc["OUT1"] = digitalRead(34);
  doc["OUT2"] = digitalRead(35);
  char JSONmessageBuffer[200];
  serializeJson(doc, JSONmessageBuffer);
  client.publish("esp32/leitura", JSONmessageBuffer);
  //Serial.print("msg json out enviado: ");
  //Serial.println(JSONmessageBuffer);
}



void setup() {
  // Configuração dos pinos dos botões como entrada
  pinMode(botaoAlterar, INPUT_PULLUP);
  pinMode(botaoSelecionarTemperatura, INPUT_PULLUP);
  pinMode(botaoSelecionarUmidade, INPUT_PULLUP);
  //pinMode(pinoPotenciometro, INPUT);

  // Configuração dos pinos dos componentes
  pinMode(pinoLuz, OUTPUT);
  pinMode(pinoVentoinha, OUTPUT);
  pinMode(pinoBombaAgua, OUTPUT);
  pinMode(pinoMotor, OUTPUT);
  // Inicialização do LCD
  lcd.init();  // initialize the lcd
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Hello, world!");
  delay(1000);
  lcd.setCursor(2, 1);
  lcd.print("Chocadeira TOP!");
  delay(1000);
  lcd.setCursor(16, 1);
  dht.begin();
  // Inicialização da porta serial
  Serial.begin(9600);



  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect(mqtt_client_id)) {
      Serial.println("Connected to MQTT");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("MQTT connection failed, retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void loop() {
  
  client.loop();
  Serial.println(variavelSelecionada);
  // Leitura do botão de alteração
  EstadoBotao1 = digitalRead(botaoAlterar);
  EstadoBotao2 = digitalRead(botaoSelecionarTemperatura);
  EstadoBotao3 = digitalRead(botaoSelecionarUmidade);
  //Selecionando botão de Seleção
  if (EstadoBotao2 == 0) {            //Se EstadoBotao tiver um valor igual a 0 na leitura, será mostrado que o botão está sendo pressionado no monitor serial
    Serial.println("botao seleção");  //Imprime mensagem no monitor serial
    variavelSelecionada = 0;
  }
  //Selecionando botão de Temperatura
  if (EstadoBotao1 == 0) {                //Se EstadoBotao tiver um valor igual a 0 na leitura, será mostrado que o botão está sendo pressionado no monitor serial
    Serial.println("botao temperatura");  //Imprime mensagem no monitor serial
    variavelSelecionada = 1;
  }
  if (EstadoBotao3 == 0) {            //Se EstadoBotao tiver um valor igual a 0 na leitura, será mostrado que o botão está sendo pressionado no monitor serial
    Serial.println("botao umidade");  //Imprime mensagem no monitor serial
    variavelSelecionada = 2;
  }
  delay(2000);
  //Selecionar Temperatura
  if (variavelSelecionada == 1) {
    valorVariavel1 = map(analogRead(pinoPotenciometro), 0, 4092, 0, 50);
    Serial.println(valorVariavel1);
    setpointTemperatura = valorVariavel1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Selecionar Temperatura: ");
    lcd.setCursor(0, 1);
    lcd.print(valorVariavel1);
    Serial.println(pinoPotenciometro);
     
    char setpointTemperaturaString[6];
    
    snprintf(setpointTemperaturaString, sizeof(setpointTemperaturaString), "%.2f", setpointTemperatura);
    mqttSendJson5(setpointTemperatura);
  }
  //Selecionar Umidade
  else if (variavelSelecionada == 2) {
    valorVariavel2 = map(analogRead(pinoPotenciometro), 0, 4092, 0, 100);
    setpointUmidade = valorVariavel2;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Selecionar Umidade: ");
    lcd.setCursor(0, 1);
    lcd.print(valorVariavel2);
    Serial.println(pinoPotenciometro);

    char setpointUmidadeString[6];
    
    snprintf(setpointUmidadeString, sizeof(setpointUmidadeString), "%.2f", setpointUmidade);
    mqttSendJson4(setpointUmidade);
  }
  //Retornar para o Controle
  else if (variavelSelecionada == 0) {
    // Leitura do sensor DHT
    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();

    if (!isnan(temperatura) && !isnan(umidade)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(temperatura);
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("Umi: ");
      lcd.print(umidade);
      lcd.print("%");
      if (temperatura < valorVariavel1) {
        digitalWrite(pinoLuz, HIGH);
        digitalWrite(pinoVentoinha, HIGH);
        EstadoLuz = 1;
      } else {
        digitalWrite(pinoLuz, LOW);
        digitalWrite(pinoVentoinha, LOW);
        EstadoLuz = 0;
      }

      if (umidade < valorVariavel2) {
        digitalWrite(pinoBombaAgua, HIGH);
        EstadoBomba = 1;
      } else {
        digitalWrite(pinoBombaAgua, LOW);
        EstadoBomba = 0;
      }
      //A cada 12 horas, o liga o motor de rotação dos ovos por 10 segundos.
      unsigned long tempoAtual = millis();  // Obtém o tempo atual

      // Verifica se já passou o tempo de espera
      if (tempoAtual - tempoAnterior >= tempoEspera) {
        // Ativa o motor
        digitalWrite(pinoMotor, HIGH);
        delay(duracaoMotor);
        digitalWrite(pinoMotor, LOW);

        tempoAnterior = tempoAtual;  // Atualiza o tempo anterior
      }
      delay(500);
    }
  }




  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  char temperatureString[6];
  char humidityString[6];
  char EstadoLuzString[6];
  char EstadoBombaString[6];


  snprintf(temperatureString, sizeof(temperatureString), "%.2f", temperatura);
  snprintf(humidityString, sizeof(humidityString), "%.2f", umidade);
  snprintf(EstadoLuzString, sizeof(EstadoLuzString), "%.2f", EstadoLuz);
  snprintf(EstadoBombaString, sizeof(EstadoBombaString), "%.2f", EstadoBomba);


  mqttSendJson(temperatura);
  mqttSendJson2(umidade);
  mqttSendJson3(EstadoLuz);
  mqttSendJson6(EstadoBomba);
  /*client.publish(mqtt_topic, temperatureString);
    client.publish(mqtt_topic, humidityString);
    */
    delay(500);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String topicStr = topic;
  
  Serial.print("Tópico recebido: ");
  Serial.println(topic);
delay(500);
  if (topicStr.equals("esp32/valor/setpointTemperatura")) {
    String payloadStr = "";
    for (int i = 0; i < length; i++) {
      payloadStr += (char)payload[i];
      Serial.println(payloadStr);
      double doubleValue = atof(payloadStr.c_str());
      valorVariavel1 = (doubleValue);
      Serial.println(valorVariavel1);

    }

    /*if (payloadStr.equals("on")) {
      digitalWrite(setpointUmidade = valorVariavel2);
    } 
    else if (payloadStr.equals("off")) {
      digitalWrite(ledPin, LOW);
    }*/
  }

  if (topicStr.equals("esp32/valor/setpointUmidade")) {
    String payloadStr = "";
    for (int i = 0; i < length; i++) {
      payloadStr += (char)payload[i];
      Serial.println(payloadStr);
    }
  }
}
