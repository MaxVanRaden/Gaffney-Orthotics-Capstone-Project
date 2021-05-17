/* eslint-disable */
import Draggable from 'react-draggable';
import {useState, useEffect} from 'react';

export const EditMenu = () => {
    const [display,setDisplay] = useState("none");
    useEffect(() => {
        document.getElementById("edit-menu").style.display = display;
    },[display]);
    //Scale the model
    const Scale = () => {
        let val = document.getElementById("scale-val").value;
        if(val > 0.1) {
            window.Module.ready.then(api => {
                api.scale(val / 100)
            })
        }
    }

    const styles = {
        float:"left",
        maxWidth:"50px"
    }
    return (
        <div className="dropdown">
            <button className="tool"
                    onClick={(e) => setDisplay(prev => prev === "none" ? "block" : "none")}>Edit</button>
            <Draggable>
            <div className="menu-items" id="edit-menu">
                <div className="menu-header" style={{padding:5}}>Edit</div>
                <div className="option">
                    Scale Model
                    <input id="scale-val"
                           style={styles}
                           type="number"
                           placeholder="100"
                           min="1"
                           step="1"/>
                    <span style={{...styles, paddingLeft:2}}>%</span>
                    <button onClick={Scale}>Scale</button>
                </div>
                <a className="option" href="#">Extend</a>
                <a className="option" href="#">Bend/Twist</a>
            </div>
            </Draggable>
        </div>
    )
}