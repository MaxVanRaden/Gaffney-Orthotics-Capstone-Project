/* eslint-disable */
import {useState} from "react";

export const ViewMenu = (props) => {
    const [width, setWidth] = useState(1507)
    const [height, setHeight] = useState(800)
    const canvas = document.getElementById("canvas");
    //Change canvas size
    const handleSize = (e) => {
        let val = Number(e.target.value);
        switch(e.target.className){
            case "width":
                setWidth(val);
                window.Module._set_size(val, height);
                canvas.style.width = String(val) + 'px';
                break;
            case "height":
                setHeight(val);
                window.Module._set_size(width, val);
                canvas.style.height = String(val) + 'px';
                break;
            default:
                console.log("Invalid classname")
        }
    }
    return (
        <div className="dropdown">
            <button className="tool">View</button>
            <div className="menu-items">
                <div style={{textAlign:"center"}}>
                    Zoom
                    <br/>
                    <div style={{paddingRight:5, display:"inline", paddingLeft:5}}>
                        <button onClick={() => props.setZoom(prev => prev > 0 ? prev - 1 : prev)}>-</button>
                    </div>
                    <input type="number" min="0" max="100" step="1" style={{maxWidth:50}}
                           value={props.zoom} onChange={e => props.setZoom(Number(e.target.value))}/>%
                    <button onClick={() => props.setZoom(prev => prev < 100 ? prev + 1 : prev)}>+</button>
                </div>
                <a href="#">Perspective</a>
                <a onClick={() => document.getElementById('fullscreen').click()}>Fullscreen</a>
                <div>
                    Width
                    <input type="number" className="width" style={{marginLeft:5}}
                           min="1" max="2000"
                           value={width} onChange={handleSize}/>
                    Height
                    <input type="number" className="height" style={{marginLeft:5, maxWidth:60}}
                           min="1" max="2000"
                           value={height} onChange={handleSize}/>
                </div>
            </div>
        </div>
    )
}