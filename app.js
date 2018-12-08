#!/usr/bin/env node
const fs = require('fs')
const SerialPort = require('serialport')
const Readline = SerialPort.parsers.Readline
const _ = require('lodash')
const express = require('express')

const package = require('./package.json')
const app = express()
let port
let reading

SerialPort.list((err, ports) => {
  let chosenPort = ports[0].comName
  _.forEach(ports, (port) => {
    if(~_.lowerCase(port.manufacturer).indexOf('arduino')) chosenPort = port
  })

  console.log('Connecting to ' + chosenPort)

  const parser = new Readline('\n')
  port = new SerialPort(chosenPort.comName, {baudRate: 115200, parser: parser})

  port.pipe(parser)

  port.write('ROBOT PLEASE RESPOND\n')


  port.on('open', () => {
    console.log('Open connection')
    parser.on('data', (data) => {
      reading = +data
      // console.log(reading)
    })
  })
})

app.get('/', (req, res) => {
  if(port.readable)
  res.send('Hello world')
})

app.get('/reading', (req, res) => {
  res.send(String(reading))
})

app.listen(3000)
