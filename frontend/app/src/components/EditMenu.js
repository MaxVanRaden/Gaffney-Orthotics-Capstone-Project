/* eslint-disable */
import Draggable from 'react-draggable';
import {useState, useEffect} from 'react';

export const EditMenu = () => {
    const [display,setDisplay] = useState("none");
    useEffect(() => {
        document.getElementById("edit-menu").style.display = display;
    },[display]);
    useEffect(() => {
        document.getElementById("translationFactor").value = 0.001;
    })
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
                    <span style={{...styles, paddingLeft:2}}>% </span>
                    <button onClick={Scale}>Apply</button>
                </div>
                <div className="option">
                    Bend Vertex
                    <div className="input-box">
                        <input className="bend-val"
                            style={styles}
                            type="number"
                            placeholder="0"
                            min="0"
                            step="1"/>
                        <span style={{...styles, paddingLeft:2}}>&#176; X</span>
                        <input className="bend-val"
                            style={styles}
                            type="number"
                            placeholder="0"
                            min="0"
                            step="1"/>
                        <span style={{...styles, paddingLeft:2}}>&#176; Y</span>
                        <input className="bend-val"
                            style={styles}
                            type="number"
                            placeholder="0"
                            min="0"
                            step="1"/>
                        <span style={{...styles, paddingLeft:2}}>&#176; Z</span>
                        <button>Apply</button>
                    </div>
                </div>
                <div className="option">
                    Translate Vertex
                    <div className="input-box">
                        <input className="bend-val"
                            style={styles}
                            type="number"
                            placeholder="0"
                             min="0"
                            step="1"/>
                        <span style={{...styles, paddingLeft:2}}> X</span>
                        <input className="bend-val"
                            style={styles}
                            type="number"
                            placeholder="0"
                            min="0"
                            step="1"/>
                        <span style={{...styles, paddingLeft:2}}> Y</span>
                    <input className="bend-val"
                            style={styles}
                            type="number"
                            placeholder="0"
                            min="0"
                            step="1"/>
                    <span style={{...styles, paddingLeft:2}}> Z</span>
                    <button>Apply</button>
                    </div>
                </div>
                <div className="option">
                    Translate Factor
                    <div className="input-box">
                    <input type='number' id='translationFactor'
                           min='0.001' step='0.01'
                           style={{width:60}}
                           //value="0.01" Can't set value to literal or it won't change
                    />
                    </div>
                </div>
            </div>
            </Draggable>
        </div>
    )
}