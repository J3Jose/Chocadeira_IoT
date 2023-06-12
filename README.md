# Chocadeira_IoT
Chocadeira monitorada por uma dashboard em node-red 


# Serviço de Monitoramento de Temperatura, Umidade e Luminosidade

O projeto desenvolvido consiste no controle de temperatura, humidade e de um motor para rotacionar os ovos de uma chocadeira através de um circuito eletronico utilizando ESP32, Node-Red, MQTT - Mosquitto, AWS ( Serviço de computação em Nuvem - Ubuntu E2C).

## 🚀 Começando

Essas instruções permitirão que você monte o projeto em operação na sua máquina AWS para fins de desenvolvimento e teste.

Consulte **[Dashboard](http://serverdatactd.sytes.net/)** para saber o modelo conceitual do projeto.

### 📋 Pré-requisitos

Inicialmente você precisará criar uma instancia dentro do serviço AWS EC2.
Utilizaremos a máquina padrão gratuita para fins de teste que a AWS disponibiliza gratuitamente.

```
Instância t2.micro ou t3.micro com sistema operacional Ubuntu 22.04.2 LTS
```


### 🔧 Instalação

Uma série de exemplos passo-a-passo que informam o que você deve executar para ter um ambiente de desenvolvimento em execução.

Diga como essa etapa será:

```
1 - Acesse o painel de controle da AWS e vá para a seção "EC2" para abrir o serviço de computação escalável da Amazon.
2 - Selecione a opção "Iniciar Instância" para começar a criar uma nova instância.
3 - De um nome para a sua Máquina e na página "Escolher uma Imagem de Máquina Amazon", opte pelo sistema operacional "Ubuntu" utilizamos a versão 22.
4 - Escolha o tipo de instância que deseja criar e clique em "Próximo: Configurar Detalhes da Instância" utilizamos a instância t2.micro qualificada para o nível gratuito.
5 - Na página "Selecionar um par de chaves existente ou criar um novo par de chaves", escolha um par de chaves existente ou crie um novo, caso necessário.
6 - Prossiga para a configuração do grupo de segurança da instância clicando em "Próximo: Configurar Grupo de Segurança" e Editar.
7 - Adicionamos 2 configurações para o nosso projeto, a primeira foi o MQTT colocando os campos com as seguintes informações:
tipo: TCP personalizado, Intervalo de portas: 1883 , Tipo de origem: Qualquer Lugar.
A segunda foi a do Node-Red:
tipo: TCP personalizado, Intervalo de portas: 1880 , Tipo de origem: Qualquer Lugar.
8 - As demais configurações mantivemos o padrão configurado pela AWS Amazon.
9 - Finalmente, clique em "Iniciar Instâncias" para criar a instância de acordo com as configurações selecionadas.
```

                                          Instalação do Broker MQTT
O Protocolo Mqtt permite a comunicação entre os dispositivos para Internet das Coisas.
Para instalar utilize os seguintes comandos:
```
$ sudo apt-get update
$ sudo apt-get install mosquitto
$ sudo apt-get install mosquitto-clients
```
Habilite o redirecionamento de portas utilzando ufw
```
$ sudo apt install ufw
$ sudo ufw allow 1883
$ sudo ufw allow 22
$ sudo ufw enable
```
Para Testar se o broker está funcionando pegue o ip público da máquina gerado pela AWS e utilize algum aplicativo para se conectar ao broker, para testes utilizamos o MyMQTT disponível para Android e Ios.

Em seguida instale o Node-RED:

```
curl -sL https://deb.nodesource.com/setup_14.x | sudo -E bash -
sudo apt-get install -y nodejs build-essential
sudo npm install -g --unsafe-perm node-red
```

Por fim inicialize o serviço do Node-RED:

```
node-red
node-red-start
```

Termine com um exemplo de como obter dados do sistema ou como usá-los para uma pequena demonstração.

## ⚙️ Executando os testes

Para executar os teste de funcionamento no Node-RED é necessário abrir URL com endereçamento do IP do servidor AWS EC2.

### 🔩 Analise os testes de ponta a ponta

Abra o navegador web de sua preferencia e digite na URL o endereço IP do computador em nuvem

```
http://IP:1880/
```

### ⌨️ E testes de estilo de codificação

Abra o navegador web de sua preferencia e digite na URL o endereço IP do computador em nuvem

```
http://IP:1880/ui/
```

## 🛠️ Construído com

Mencione as ferramentas que você usou para criar seu projeto

* [Node-RED](https://nodered.org/) - O framework web utilizado
* [Mosquitto](https://mosquitto.org/) - O serviço Broker MQTT utilizado


## 🖇️ Colaborando



## 📌 Versão


## ✒️ Autores

É altamente recomendável conceder o merecido reconhecimento a todas as pessoas que participaram do projeto, desde o seu início até o seu desfecho. Essa atitude essencial demonstra o apreço pela colaboração e o trabalho em equipe. Ademais, constitui uma poderosa maneira de fomentar a motivação e o engajamento das pessoas, incentivando-as a continuarem se envolvendo em futuros empreendimentos, com a certeza de que seu esforço será devidamente valorizado e reconhecido.

* **Desenvolvedor** - [José Vicente Teodoro Igino]


---
