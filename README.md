# Projeto de Monitoramento de Batimentos com ESP32-C6 + MQTT + Firebase

Este projeto realiza a leitura de batimentos cardíacos usando um **sensor de pulso** conectado a um **ESP32-C6**.
Os valores de BPM são enviados em **tempo real** para o **Firebase Realtime Database** e depois exibidos em uma **interface web hospedada no GitHub Pages**.

<img src="web monitoramento.png" width="600">
---

## Tecnologias Utilizadas

* **ESP32-C6**
* **Firebase Realtime Database**
* **Wi-Fi EDUROAM (EAP/Enterprise)**
* **MQTT**
* **HTML**
* **GitHub Pages**

---

##  Fluxo do Projeto

1. O ESP32-C6 lê o sinal do sensor cardíaco.
2. Cada batimento detectado é convertido para BPM.
3. Os dados são enviados para o Firebase e MQTT.
4. A página web conecta ao Firebase e exibe os BPM em tempo real.
5. Os dados aparecem automaticamente sem recarregar a página.

---

##  Estrutura no Firebase

O banco salva os BPM da seguinte forma:

```
bpm
 └── -N1A84H2kfj83K
      ├── bpm: 72
      └── timestamp: 1732578123
```

---

##  Interface Web

A interface foi criada em HTML + Firebase.
Ela lê o banco e mostra uma tabela com os batimentos registrados.

Hospedagem utilizada: **GitHub Pages**.

---

##  Arquivos Principais

* `index.html` → página que exibe os BPM.
* `esp32-batimentoscardiacos.cpp` → responsável por ler o sensor e enviar ao Firebase.

---

## Status

Projeto funcionando:
- Leitura de BPM
-  Envio para MQTT
-  Envio para Firebase
-  Leitura em tempo real na web
-  Página hospedada no GitHub Pages

