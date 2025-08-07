void web(WiFiClient &client) {
  // Envía la página HTML completa de una sola vez
  client.print(pagina); 
  
  // Nota: Eliminamos todos los bucles for() y concatenaciones
  // porque ahora el HTML completo está en 'pagina'
  // y los botones se generan dinámicamente con JavaScript
}
