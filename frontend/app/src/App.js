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
            let w = canvas.clientWidth;
            let h = canvas.clientHeight;
            if(w !== canvas.width || h !== canvas.height){
                canvas.width = w;
                canvas.height = h;
            }
            window.Module.ready.then(api => api.set_size(w,h))
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