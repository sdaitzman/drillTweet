#!/usr/bin/env node
const fs = require('fs')
const SerialPort = require('serialport')
const Readline = SerialPort.parsers.Readline
const _ = require('lodash')
const package = require('./package.json')

SerialPort.list((err, ports) => {
  let chosenPort = ports[0].comName
  _.forEach(ports, (port) => {
    if(~_.lowerCase(port.manufacturer).indexOf('arduino')) chosenPort = port
  })

  console.log('Connecting to ' + chosenPort)

  const parser = new Readline('\n')
  const port = new SerialPort(chosenPort.comName, {baudRate: 115200, parser: parser})

  port.pipe(parser)

  port.write('ROBOT PLEASE RESPOND\n')


  port.on('open', () => {
    console.log('open connection')
    parser.on('data', (data) => {
      console.log(+data)
      if(+data > 500) {
        
        process.exit()
      }
    })
  })
})
