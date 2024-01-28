const http = require('http');
const WebSocket = require('ws');
const fs = require('fs');
const path = require('path');

const server = http.createServer((req, res) => {
  if (req.url === '/' || req.url === '/index.html') {
    fs.readFile(path.join(__dirname, 'index.html'), 'utf8', (err, data) => {
      if (err) {
        res.writeHead(500, { 'Content-Type': 'text/plain' });
        res.end('Internal Server Error');
        return;
      }

      res.writeHead(200, { 'Content-Type': 'text/html' });
      res.end(data);
    });
  } else {
    res.writeHead(404, { 'Content-Type': 'text/plain' });
    res.end('Not Found');
  }
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
  // Simulate an asynchronous operation (replace with your actual logic)
  return new Promise((resolve) => {
    setTimeout(() => {
      resolve(`Processed: ${data}`);
    }, 1000);
  });
}

server.listen(8080, () => {
  console.log('WebSocket and HTTP server started. Listening on http://localhost:8080');
});
