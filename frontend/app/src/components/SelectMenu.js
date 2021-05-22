/* eslint-disable */
import {useCallback, useEffect, useRef} from "react";

export const SelectMenu = (props) => {
    let canvas = document.getElementById("canvas")
    const clickPos = useRef({x:0, y:0})
    //Pass selection coords to backend
    //Backend assumes (x1,y1) is top left
    function selectArea(x1, y1, x2, y2){
        window.Module.ready.then(api => {
            if(x1 <= x2 && y1 <= y2)
                api.on_mouse_up(x1, y1, x2, y2);
            else if(x1 >= x2 && y1 <= y2)
                api.on_mouse_up(x2, y1, x1, y2);
            else if(x1 <= x2 && y1 >= y2)
                api.on_mouse_up(x1, y2, x2, y1);
            else
                api.on_mouse_up(x2, y2, x1, y1);
        });
    }
    const mouseDown = useCallback((e) => {
        clickPos.current = {x: e.pageX - canvas.offsetLeft, y: e.pageY - canvas.offsetTop}
    },[]);

    const mouseUp = useCallback((e) => {
        let x2 = e.pageX - canvas.offsetLeft;
        let y2 = e.pageY - canvas.offsetTop;
        selectArea(clickPos.current.x, clickPos.current.y, x2, y2);
    },[props.tool]);

    //onclick event for handling vertex selection
    const onClick = useCallback((e) => {
        let x = e.pageX - canvas.offsetLeft;
        let y = e.pageY - canvas.offsetTop;
        window.Module.ready.then(api => {
            //For now, select a small section
            api.on_mouse_up(x-3, y-3, x+3, y+3);
        })
    },[])
    useEffect(() => {
        canvas.onclick = props.tool === "vertex" ? onClick : null;
        if(props.tool === "select") {
            canvas.onmousedown = mouseDown;
            canvas.onmouseup = mouseUp;
        }
        return () => {
            canvas.onclick = null;
            canvas.onmouseup = null;
            canvas.onmousedown = null;
        }
    },[props.tool])
    return (
        <div className="dropdown">
            <button className="tool">
                Select
                <input type="checkbox" id='selectToggle' checked={props.tool === "select" || props.tool === "vertex" }
                onChange={e => {
                    props.setTool(prev => prev === "select" || prev === "vertex" ? "default" : "select");
                }}/>
            </button>
            <div className="menu-items">
                <a className="option" href="#">
                    Vertex
                    <input type="radio" checked={props.tool === "vertex"}
                           onChange={e => props.setTool(e.target.value === props.tool ? "default" : e.target.value)}
                           value="vertex"
                           className="toggle"
                    />
                </a>
                <a className="option" href="#">
                    Section
                    <input type="radio" checked={props.tool === "select"}
                           onChange={e => props.setTool(e.target.value === props.tool ? "default" : e.target.value)}
                           value="select"
                           id="selectToggle" className="toggle"
                    />
                </a>
            </div>
        </div>
    )
}