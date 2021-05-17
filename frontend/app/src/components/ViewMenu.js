/* eslint-disable */
import Draggable from 'react-draggable';
import {useState, useEffect} from 'react';

export const ViewMenu = (props) => {
    const [display,setDisplay] = useState("none");
    useEffect(() => {
        document.getElementById("view-menu").style.display = display;
    },[display]);

    return (
        <div className="dropdown">
            <button className="tool"
                    onClick={(e) => setDisplay(prev => prev === "none" ? "block" : "none")}>
                View
            </button>
            <Draggable>
            <div className="menu-items" id="view-menu">
                <div className="menu-header" style={{padding:5}}>View</div>
                <div className="option" style={{textAlign:"left"}}>
                    Zoom
                    <br/>
                    <div style={{paddingRight:5, display:"inline", paddingLeft:5}}>
                        <button onClick={() => props.setZoom(prev => prev > 0 ? prev - 1 : prev)}>-</button>
                    </div>
                    <input type="number" min="0" max="100" step="1" style={{maxWidth:50}}
                           value={props.zoom} onChange={e => props.setZoom(Number(e.target.value))}/>%
                    <button onClick={() => props.setZoom(prev => prev < 100 ? prev + 1 : prev)}>+</button>
                </div>
                <a className="option" href="#">Perspective</a>
                <a className="option" onClick={() => document.getElementById('fullscreen').click()}>
                    Fullscreen
                </a>
            </div>
            </Draggable>
        </div>
    )
}