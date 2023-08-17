import React, { useState, useEffect, useRef } from 'react';
import './Joystick.css';

const Joystick = ({ onJoystickMove }) => {
  const [position, setPosition] = useState({ x: 0, y: 0 });
  const [pressedKeys, setPressedKeys] = useState(new Set());
  const joystickRef = useRef();
  const lastSpeedRef = useRef(0);
  const lastTurnRateRef = useRef(0);

  useEffect(() => {
    const container = joystickRef.current;
    const containerRect = container.getBoundingClientRect();
    const centerX = containerRect.left + containerRect.width / 2;
    const centerY = containerRect.top + containerRect.height / 2;

    const handleJoystickMove = (e) => {
      if (e.buttons !== undefined && e.buttons !== 1) return; // Check for left mouse button click
      e.preventDefault();

      let x, y;
      if (e.type === 'mousemove') {
        x = e.clientX - centerX;
        y = e.clientY - centerY;
      } else {
        // For touch events (mobile)
        const touch = e.touches[0];
        x = touch.clientX - centerX;
        y = centerY - touch.clientY;
      }

      const distance = Math.min(Math.hypot(x, y), containerRect.width / 2);
      const angle = Math.atan2(y, x);
      const newX = distance * Math.cos(angle);
      const newY = distance * Math.sin(angle);

      setPosition({ x: newX, y: newY });
    };

    document.addEventListener('mousemove', handleJoystickMove);
    document.addEventListener('touchmove', handleJoystickMove);

    return () => {
      document.removeEventListener('mousemove', handleJoystickMove);
      document.removeEventListener('touchmove', handleJoystickMove);
    };
  }, []);

    useEffect(() => {
    const maxSpeed = 127;
    const maxTurnRate = 127;
    const deadZoneRadius = 15;
  
    const distanceFromCenter = Math.hypot(position.x, position.y);
  
    // If the distance from the center is within the dead zone, treat it as stop (speed and turn rate both zero)
    if (distanceFromCenter < deadZoneRadius) {
      // Check if the speed and turn rate have changed before sending the "stop" command
      if (lastSpeedRef.current !== 0 || lastTurnRateRef.current !== 0) {
        onJoystickMove(0, 0);
        lastSpeedRef.current = 0;
        lastTurnRateRef.current = 0;
      }
      return;
    }
  
    const speed = Math.round((-position.y / (65 - deadZoneRadius)) * maxSpeed);
    const turnRate = Math.round((position.x / (65 - deadZoneRadius)) * maxTurnRate);

    // Check if the speed and turn rate have changed before sending the data
    if (speed !== lastSpeedRef.current || turnRate !== lastTurnRateRef.current) {
      onJoystickMove(speed, turnRate);
      lastSpeedRef.current = speed;
      lastTurnRateRef.current = turnRate;
    }
  }, [position, onJoystickMove]);

  const handleKeyDown = (e) => {
    setPressedKeys((prev) => new Set(prev).add(e.key));
  };

  const handleKeyUp = (e) => {
    setPressedKeys((prev) => {
      const keys = new Set(prev);
      keys.delete(e.key);
      return keys;
    });
  };

  useEffect(() => {
    let animationFrame;

    const moveJoystickSmoothly = () => {
      const newX = pressedKeys.has('d') ? 50 : pressedKeys.has('a') ? -50 : 0;
      const newY = pressedKeys.has('w') ? -50 : pressedKeys.has('s') ? 50 : 0;

      if (newX !== position.x || newY !== position.y) {
        setPosition((prev) => ({
          x: prev.x + (newX - prev.x) * 0.2,
          y: prev.y + (newY - prev.y) * 0.2,
        }));
      }

      animationFrame = requestAnimationFrame(moveJoystickSmoothly);
    };

    animationFrame = requestAnimationFrame(moveJoystickSmoothly);

    return () => {
      cancelAnimationFrame(animationFrame);
    };
  }, [pressedKeys]);

  useEffect(() => {
    window.addEventListener('keydown', handleKeyDown);
    window.addEventListener('keyup', handleKeyUp);

    return () => {
      window.removeEventListener('keydown', handleKeyDown);
      window.removeEventListener('keyup', handleKeyUp);
    };
  }, []);

  return (
    <div ref={joystickRef} className="joystick-container">
      <div className="joystick-background"></div>
      <div
        className="joystick-handle"
        style={{
          transform: `translate(${position.x}px, ${position.y}px)`,
        }}
      ></div>
    </div>
  );
};

export default Joystick;
