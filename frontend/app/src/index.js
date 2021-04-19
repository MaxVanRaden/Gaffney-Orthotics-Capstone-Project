import React from 'react';
import ReactDOM from 'react-dom';
import App from './App';

ReactDOM.render(
    <React.StrictMode>
        <App />
    </React.StrictMode>,
    document.getElementById('root')
);
ReactDOM.render(<App/>, document.getElementById("root"));

// Event handler for mouse coordinates
let canvasElement = document.getElementById('canvas');
canvasElement.onmousemove = function(e) {
    let x = e.pageX - this.offsetLeft;
    let y = e.pageY - this.offsetTop;
    document.getElementById('xcoord').innerHTML = x;
    document.getElementById('ycoord').innerHTML = y;
};