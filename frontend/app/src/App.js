import React from 'react';
import ReactDOM from 'react-dom';
import { ImportStuff } from './components/ImportSTL';
import { ExportFile  } from "./components/ExportFile";
import './App.css';


function App() {

    // const print = () => {
    //     window.Module.ready.then(api => console.log(api.print_hello(100)))
    // }

    return (
        <>
            {/*<button onClick={print}> Print 100 </button>*/}
            <ImportStuff />
            <ExportFile />
        </>
    );
}

export default App;
ReactDOM.render(<App/>, document.getElementById("root"))