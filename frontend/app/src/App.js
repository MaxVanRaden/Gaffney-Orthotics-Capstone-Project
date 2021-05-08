import React, {useEffect} from 'react';
import {Toolbar} from "./components/Toolbar";
import {NavBar} from "./components/NavBar";
import './App.css';

function App() {
    useEffect(() => {
        document.getElementById("root").onwheel = function(e){
            e.preventDefault()
        }
        let canvas = document.getElementById("canvas")
        window.onresize = function(e){
            window.Module._set_size(window.innerWidth, window.innerHeight)
            canvas.style.width = window.innerWidth + 'px'
            canvas.style.height = window.innerHeight + 'px'
        }
        canvas.onmousemove = function(e){
            let curX = e.pageX - canvas.offsetLeft;
            let curY = e.pageY - canvas.offsetTop;
            document.getElementById('xcoord').innerHTML = curX;
            document.getElementById('ycoord').innerHTML = curY;
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