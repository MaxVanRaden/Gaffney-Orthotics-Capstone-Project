import React from 'react';
import {Toolbar} from "./components/Toolbar";
import {NavBar} from "./components/NavBar";
import './App.css';


function App() {

    return (
        <>
            <NavBar/>
            <Toolbar/>
        </>
    );
}

export default App;

// Event handler for mouse coordinates
// var canvasElement = document.getElementById('canvas');
// canvasElement.onmousemove = function(e) {
//     var x = e.pageX - this.offsetLeft;
//     var y = e.pageY - this.offsetTop;
//     document.getElementById('xcoord').innerHTML = x;
//     document.getElementById('ycoord').innerHTML = y;
//     document.getElementById('xcoord2').innerHTML = x;
//     document.getElementById('ycoord2').innerHTML = y;
// };

