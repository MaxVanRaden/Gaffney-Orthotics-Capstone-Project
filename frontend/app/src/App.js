import React, {useEffect} from 'react';
import {Toolbar} from "./components/Toolbar";
import {NavBar} from "./components/NavBar";
import './App.css';

function App() {
    useEffect(() => {
        document.getElementById("root").onwheel = function(e){
            e.preventDefault()
        }
        window.onresize = function(e){
            window.Module._set_size(window.innerWidth, window.innerHeight)
            let canvas = document.getElementById("canvas")
            canvas.style.width = window.innerWidth + 'px'
            canvas.style.height = window.innerHeight + 'px'
        }
    },[])

    return (
        <>
            <NavBar/>
            <Toolbar/>
        </>
    );
}

export default App;