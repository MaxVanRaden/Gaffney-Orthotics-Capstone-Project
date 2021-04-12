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
var canvasElement = document.getElementById('canvas');
canvasElement.onmousemove = function(e) {
    var x = e.pageX - this.offsetLeft;
    var y = e.pageY - this.offsetTop;
    document.getElementById('xcoord').innerHTML = x;
    document.getElementById('ycoord').innerHTML = y;
    document.getElementById('xcoord2').innerHTML = x;
    document.getElementById('ycoord2').innerHTML = y;
};