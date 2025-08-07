const String pagina =  R"====(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="utf-8">
    <title>Control Remoto IoT</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        body {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            margin: 0;
            padding: 20px;
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            min-height: 100vh;
            display: flex;
            flex-direction: column;
            align-items: center;
        }
        
        h1 {
            color: white;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
            margin-bottom: 30px;
        }
        
        .remote-container {
            background: rgba(255, 255, 255, 0.9);
            border-radius: 25px;
            padding: 25px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.2);
            width: 90%;
            max-width: 500px;
        }
        
        .control-group {
            display: flex;
            justify-content: space-between;
            margin-bottom: 20px;
            align-items: center;
        }
        
        .control-label {
            font-weight: bold;
            color: #333;
            width: 30%;
        }
        
        .btn-group {
            display: flex;
            width: 65%;
            gap: 10px;
        }
        
        .btn {
            flex: 1;
            padding: 15px 10px;
            border: none;
            border-radius: 12px;
            font-weight: bold;
            font-size: 16px;
            cursor: pointer;
            transition: all 0.3s ease;
            text-transform: uppercase;
            letter-spacing: 1px;
            box-shadow: 0 4px 6px rgba(0,0,0,0.1);
        }
        
        .btn-on {
            background: linear-gradient(to right, #00b09b, #96c93d);
            color: white;
        }
        
        .btn-off {
            background: linear-gradient(to right, #ff416c, #ff4b2b);
            color: white;
        }
        
        .btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 6px 8px rgba(0,0,0,0.15);
        }
        
        .btn:active {
            transform: translateY(1px);
            box-shadow: 0 2px 3px rgba(0,0,0,0.1);
        }
        
        .d-pad {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            grid-template-rows: repeat(3, 1fr);
            gap: 10px;
            margin: 30px auto;
            width: 200px;
        }
        
        .d-pad-btn {
            background: #4a6bff;
            color: white;
            border: none;
            border-radius: 50%;
            width: 60px;
            height: 60px;
            font-size: 24px;
            display: flex;
            align-items: center;
            justify-content: center;
            box-shadow: 0 4px 8px rgba(0,0,0,0.2);
            transition: all 0.2s;
        }
        
        .d-pad-btn:active {
            background: #3a56d4;
            transform: scale(0.95);
        }
        
        .center-btn {
            grid-column: 2;
            grid-row: 2;
            background: #333;
        }
        
        @media (max-width: 600px) {
            .btn {
                padding: 12px 8px;
                font-size: 14px;
            }
            
            .d-pad {
                width: 180px;
            }
            
            .d-pad-btn {
                width: 50px;
                height: 50px;
                font-size: 20px;
            }
        }
    </style>
</head>
<body>
    <h1>Control Remoto IoT</h1>
    
    <div class="remote-container">
        <!-- Controles individuales para cada salida -->
        <div id="output-controls">
            <!-- Los controles se generarán dinámicamente aquí -->
        </div>
        
        <!-- Panel de control direccional (opcional) -->
        <div class="d-pad">
            <button class="d-pad-btn" onclick="moveForward()">↑</button>
            <button class="d-pad-btn" onclick="moveLeft()">←</button>
            <button class="d-pad-btn center-btn" onclick="stopAll()">■</button>
            <button class="d-pad-btn" onclick="moveRight()">→</button>
            <button class="d-pad-btn" onclick="moveBackward()">↓</button>
        </div>
    </div>
    
    <script>
        // Generar controles dinámicamente
        const outputControls = document.getElementById('output-controls');
        
        for (let i = 1; i <= 6; i++) {
            const controlGroup = document.createElement('div');
            controlGroup.className = 'control-group';
            
            controlGroup.innerHTML = `
                <div class="control-label">Motor ${i}</div>
                <div class="btn-group">
                    <button class="btn btn-on" onclick="botonEncender${i}()">ON</button>
                    <button class="btn btn-off" onclick="botonApagar${i}()">OFF</button>
                </div>
            `;
            
            outputControls.appendChild(controlGroup);
        }
        
        // Funciones de control
        function moveForward() {
            consultaGET("forward");
        }
        
        function moveBackward() {
            consultaGET("backward");
        }
        
        function moveLeft() {
            consultaGET("left");
        }
        
        function moveRight() {
            consultaGET("right");
        }
        
        function stopAll() {
            consultaGET("stop");
        }
        
        // Generar funciones dinámicamente
        for (let i = 1; i <= 6; i++) {
            window[`botonEncender${i}`] = function() {
                consultaGET("on" + i);
            };
            
            window[`botonApagar${i}`] = function() {
                consultaGET("off" + i);
            };
        }
        
        function consultaGET(consulta) {
            const Http = new XMLHttpRequest();
            console.log(`Consultando ${consulta}`);
            Http.open("GET", consulta);
            Http.send();
            
            Http.onreadystatechange = (e) => {
                console.log(Http.status);
                console.log(Http.responseText);
            };
        }
    </script>
</body>
</html>
)====";
