const http = require('http');
const express = require('express');
const WebSocket = require('ws');
const fs = require('fs');
const path = require('path');

const app = express();
const server = http.createServer(app)

// Serve static files from the 'public' directory
app.use(express.static(path.join(__dirname, 'public')));

// Handle HTTP requests
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});
const wss = new WebSocket.Server({ server });

wss.on('connection', (ws) => {
  console.log('Client connected');

  ws.on('message', async (message) => {
    try {
      const response = await someAsyncFunction(message);
      ws.send(`Server received: ${message}, Response: ${response}`);
      console.log(`Server received: ${message}, Response: ${response}`);
    } catch (error) {
      console.error('Error processing message:', error.message);
    }

    wss.clients.forEach((client) => {
        if (client !== ws && client.readyState === WebSocket.OPEN) {
          client.send(`${message}`);
        }
      });
  });

  


  ws.on('close', () => {
    console.log('Client disconnected');
  });
});

async function someAsyncFunction(data) {
  return new Promise((resolve) => {
    setTimeout(() => {
      resolve(`Processed: ${data}`);
    }, 1000);
  });
}

server.listen(8080, () => {
  console.log('WebSocket and HTTP server started. Listening on http://localhost:8080');
});
