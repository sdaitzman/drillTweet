console.log('Begin server')

const WebSocket = require('ws');

const wss = new WebSocket.Server({ port: 80 });

wss.on('connection', function connection(ws) {
  console.log('New Connection')
  let refreshLoop = setInterval(() => {
    if(ws.readyState === WebSocket.OPEN) {
      ws.send('getReading', (error) => { if(error) console.log(error) })
    } else {
      console.log('Trying to send messages to a non-open socket. Clearing this interval.')
      clearInterval(refreshLoop)
    }
  }, 200)
  ws.on('message', function incoming(message) {
    console.log('received: %s', message);
  });

  ws.send('something');
});
