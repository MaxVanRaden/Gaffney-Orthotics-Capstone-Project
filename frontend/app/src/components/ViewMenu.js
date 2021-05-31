/* eslint-disable */
import Draggable from 'react-draggable';
import {useState, useEffect, useRef} from 'react';

export const ViewMenu = (props) => {
    const [display,setDisplay] = useState("none");
    const loop = useRef(0);
    useEffect(() => {
        document.getElementById("view-menu").style.display = display;
    },[display]);
    const zoom = (e) => {
        loop.current = setInterval(() => {
            window.Module.ready.then(api => api.zoom(e.target.value));
        },10);
    }
    const stop = ()=>{
        clearInterval(loop.current);
    }

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
                        <div style={{paddingRight:5, display:"inline", paddingLeft:5}}>
                            <button style={{marginRight:10}} value={1} onMouseDown={zoom} onMouseUp={stop}>-</button>
                            <button value={-1} onMouseDown={zoom} onMouseUp={stop}>+</button>
                        </div>
                    </div>
                    {/*<a className="option" href="#">Perspective</a>*/}
                    <a className="option" onClick={() => document.getElementById('fullscreen').click()}>
                        Fullscreen
                    </a>
                </div>
            </Draggable>
        </div>
    )
}