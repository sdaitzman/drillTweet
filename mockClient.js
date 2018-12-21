const WebSocket = require('ws');

const ws = new WebSocket('ws://localhost:80');

ws.on('open', function open() {
  ws.send('sent');
});

ws.on('message', function incoming(data) {
  console.log(data);
});
