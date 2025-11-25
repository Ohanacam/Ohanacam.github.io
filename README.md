<!DOCTYPE html>
<html lang="pt-br">
<head>
  <meta charset="UTF-8">
  <title>Monitor de BPM - Firebase</title>
  <style>
    body {
      background: #111;
      color: #0f0;
      font-family: Arial;
      text-align: center;
      margin-top: 40px;
    }
    #bpmValue {
      font-size: 60px;
      margin-top: 20px;
    }
    .card {
      background: #000;
      padding: 20px;
      border-radius: 12px;
      display: inline-block;
      border: 2px solid #0f0;
      box-shadow: 0 0 20px #0f0;
    }
  </style>
</head>
<body>

  <h1>Monitor de BPM</h1>
  <div class="card">
    <h2>BPM Atual:</h2>
    <div id="bpmValue">Carregando...</div>
  </div>

  <script type="module">
    // Firebase core
    import { initializeApp } from "https://www.gstatic.com/firebasejs/9.23.0/firebase-app.js";
    import { getDatabase, ref, onValue } 
      from "https://www.gstatic.com/firebasejs/9.23.0/firebase-database.js";

    // CONFIGURAÇÃO DO SEU FIREBASE
    const firebaseConfig = {
      apiKey: "AIzaSyDXtbDh2q3Vp66GJYm4JWROXzRyG0kUIQ0",
      authDomain: "esp32c6-32158.firebaseapp.com",
      databaseURL: "https://esp32c6-32158-default-rtdb.firebaseio.com",
      projectId: "esp32c6-32158",
      storageBucket: "esp32c6-32158.firebasestorage.app",
      messagingSenderId: "975962633482",
      appId: "1:975962633482:web:9fca2d90452fc2cd0bb6f3",
      measurementId: "G-HLVMFD1RQY"
    };

    // Inicializa Firebase e Database
    const app = initializeApp(firebaseConfig);
    const db = getDatabase(app);

    // Referência ao ramo "bpm"
    const bpmRef = ref(db, "bpm");

    // Atualiza interface automaticamente quando o valor mudar
    onValue(bpmRef, (snapshot) => {
      const val = snapshot.val();
      document.getElementById("bpmValue").innerText = val ?? "Sem dados";
    });
  </script>

</body>
</html>
