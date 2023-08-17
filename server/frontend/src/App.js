import React, { useState, useEffect } from 'react';
import './App.css';

import Joystick from './components/Joystick';
import './components/Joystick.css';

import TankAnimation from './components/TankAnimation';
import './components/TankAnimation.css';

const App = () => {
  const [socket, setSocket] = useState(null);
  const [speed, setSpeed] = useState(0);
  const [turnRate, setTurnRate] = useState(0);

  useEffect(() => {
    const ws = new WebSocket('ws://localhost:8080/');

    ws.onopen = () => {
      console.log('Connected to WebSocket proxy server');
      setSocket(ws);
    };

    ws.onclose = () => {
      console.log('Connection to WebSocket proxy server closed');
      setSocket(null);
    };

    return () => {
      if (socket) {
        socket.close();
      }
    };
  }, []);

  const sendJoystickData = (speed, turnRate) => {
    if (socket && socket.readyState === WebSocket.OPEN) {
      const packet = {
        type: 'movement',
        speed: speed,
        turnRate: turnRate,
      };
      socket.send(JSON.stringify(packet));
  
      // Update the speed and turnRate states with the joystick data
      //setSpeed(speed);
      //setTurnRate(turnRate);
    }
  };

  return (
    <div className="container">
      <Joystick onJoystickMove={(speed, turnRate) => {
        setSpeed(speed);
        setTurnRate(turnRate);
        sendJoystickData(speed, turnRate);
      }} />
      <TankAnimation speed={speed} turnRate={turnRate} />
    </div>
  );
};

export default App;