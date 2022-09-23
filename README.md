# Fundamentos de Sistema Embarcados - Projeto Final

## Introdução
Esse projeto é uma implementação de atuadores distribuídos baseados nos microcontroladores ESP32 interconectados via Wifi através do protocolo MQTT.
Para mais informações accesse o link do [enunciado](https://gitlab.com/fse_fga/trabalhos-2022_1/trabalho-3-2022-1).

## Vídeo de demonstração

## Características
### Hardware
O Projeto possui:
- 1 ESP32 DevKit1 c (com LED Azul e botão GPIO0)
- 1 Sensor de chama (KY 026)
- 1 Sensor de temperatura e umidade (DHT11)
- 1 Buzzer ativo

### Descrição
O projeto busca implementar uma espécie de detector de fogo, os sensores trabalham em conjunto para emitir um aviso de fogo próximo. Desta forma, quando o sendor Ky-026 detecta fogo ele ativa o buzzer que emite um som e envia para o Dashboard(interface interativa) um sinal luminoso a partir de um led vermelho que acende.
Algumas características implementadas no projeto:
- NVS Flash: Guarda na memória flash da ESP32 o status dos atributos como o valor da intensidade do LED, caso a placa reboot voltara a seu estado antes do desligamento.
- Low Power: Implementação do ligth sleep, nesse modo o funcionamento se limita ao momento onde o botão GPIO0 ser ativado.
- Comunicação MQTT: A partir da conexão WIFI se conecta a Dashboard via comunicação MQTT.

A partir da Dashboard(interface interativa) é possível ativar e desativar os sensores, controlar o nível de luminosidade do LED Azul e observar os dados lidos dos sensores de temperatura/umidade, chama.

## Como executar
### Pré-Requisitos
- [Plataform IO](https://platformio.org/)
- [VSCode](https://code.visualstudio.com/)
- Clone o projeto
```bash
git clone https://github.com/lucasgbezerra/FSE_Trabalho_Final.git
```
Para utilizar o Plataform IO:
- Abra a pasta `distributed_server` do projeto pelo Plataform IO

### Antes de executar
- Entre no menu de execução a partir de um terminal do PlataformIO

```bash
pio run -t menuconfig
```

- Acesse o menu **Configuração do Wifi** e adicione suas informações de rede de ssid e senha

- Acesse o menu **Configuração do Servidor Central** e adicione as informações de IP do Broker e o token de acesso do dispositivo no ThingsBoard.

- Acesse o menu  **Modo de Funcionamento** e selecione o modo desejado (Bateria ou Energia)

- Apague a Flash:
    - Acesse o menu do PlataformIO e clique na opção erase flash (a ESP32 deve estar conectada)

### Executar
- Acesse o menu do PlataformIO e clique na opçao ;General->Build (ESP32 deve estar conectada). Após a build basta clicar na opção General->Upload and Monitor.

### Utilização

- Visualize as informações dos sensores na Dashboard (ESP32 Estevao-Lucas) e acione alguns botões.
Para ativar o sensor de fogo basta aproximar o fogo do sensor de chama.
