console.log('Begin server')

const WebSocket = require('ws');

const wss = new WebSocket.Server({ port: 80 });

wss.on('connection', function connection(ws) {
  console.log('New Connection')
  setInterval(() => {
    ws.send('getReading')
    console.log(ws)
  }, 10)
  ws.on('message', function incoming(message) {
    console.log('received: %s', message);
  });

  ws.send('something');
});
