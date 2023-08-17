import React from 'react';
import './TankAnimation.css';

const TankAnimation = ({ speed, turnRate }) => {
  const leftWheelSpeed = speed - turnRate / 2;
  const rightWheelSpeed = speed + turnRate / 2;

  const leftWheelTransform = `translateY(${-(leftWheelSpeed / 127) * 50}%)`;
  const rightWheelTransform = `translateY(${-(rightWheelSpeed / 127) * 50}%)`;

  return (
    <div className="tank-container">
      <div className="tank">
        <div className="wheel left-wheel" style={{ transform: leftWheelTransform }}>
          <div className="wheel-speed">L:{Math.round(leftWheelSpeed)}</div>
        </div>
        <div className="wheel right-wheel" style={{ transform: rightWheelTransform }}>
          <div className="wheel-speed">R:{Math.round(rightWheelSpeed)}</div>
        </div>
      </div>
      <div>speed</div>
    </div>
  );
};

export default TankAnimation;