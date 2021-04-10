import React from 'react';
import ReactDOM from 'react-dom';
import { ImportStuff } from './components/ImportSTL';
import './App.css';


function App() {

    // const print = () => {
    //     window.Module.ready.then(api => console.log(api.print_hello(100)))
    // }

    return (
        <>
            {/*<button onClick={print}> Print 100 </button>*/}
            <ImportStuff />
        </>
    );
}

export default App;
ReactDOM.render(<App/>, document.getElementById("root"))

// Event handler for mouse coordinates
var canvasElement = document.getElementById('canvas');
canvasElement.onmousemove = function(e) {
    var x = e.pageX - this.offsetLeft;
    var y = e.pageY - this.offsetTop;
    document.getElementById('xcoord').innerHTML = x;
    document.getElementById('ycoord').innerHTML = y;
};

