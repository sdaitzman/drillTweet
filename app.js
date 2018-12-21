#!/usr/bin/env node
const fs = require('fs')
const WebSocket = require('ws')
const SerialPort = require('serialport')
const Readline = SerialPort.parsers.Readline
const _ = require('lodash')
const express = require('express')

const package = require('./package.json')
const app = express()
const server = require('http').Server(app)
const io = require('socket.io')(server)

let port
let reading


const wss = new WebSocket.Server({ port: 8080 })

wss.on('connection', function connection(ws) {
  console.log('New Connection')
  let refreshLoop = setInterval(() => {
    if(ws.readyState === WebSocket.OPEN) {
      ws.send('getReading', (error) => { if(error) console.log(error) })
    } else {
      console.log('Trying to send messages to a non-open WebSocket. Clearing this getReading interval. Connection readyState is ' + ws.readyState)
      clearInterval(refreshLoop)
    }
  }, 40)
  ws.on('message', function incoming(message) {
    console.log('received: %s', message);
    reading = +message
  });

  ws.send('something');
});


app.get('/', (req, res) => {
  res.sendFile(__dirname + '/index.html')
})

app.get('/reading', (req, res) => {
  res.send(String(reading))
})

io.on('connection', (socket) => {
  socket.emit('init', {version: package.version})
  socket.on('requestReading', (data) => {
    socket.emit('returnReading', {reading: reading})
  })
})

server.listen(80)
