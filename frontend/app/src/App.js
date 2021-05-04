import React, {useEffect} from 'react';
import {Toolbar} from "./components/Toolbar";
import {NavBar} from "./components/NavBar";
import './App.css';

function App() {
    useEffect(() => {
        document.getElementById("root").onwheel = function(e){
            e.preventDefault()
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