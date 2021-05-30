import {useCallback, useEffect, useRef, useState} from "react";
import Draggable from "react-draggable";
export const SelectMenu = (props) => {
    let canvas = document.getElementById("canvas")
    const clickPos = useRef({x:0, y:0})
    const [display,setDisplay] = useState("none");
    useEffect(() => {
        document.getElementById("select-menu").style.display = display;
    },[display]);
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

    const sendPoints = (buffer, count) => {
        window.Module.ready.then(api => api.point_click(buffer, count))
            .catch(e => console.log("failed to send array of points to backend"));
        console.log("Operating for sending array points concluded")
    }

    function selectPoints(points){
        const count = points.length;
        try{
            const typedArray = new Float32Array(count);
            if(!typedArray){
                console.log("failed to convert heap of clipping point")
            } else {
                for (let i=0; i <points.length; i++){
                    typedArray[i] = points[i];
                }
                const buffer = window.Module._malloc(typedArray.length * count);
                window.Module.HEAP32.set(typedArray, buffer >> 2);
                setTimeout(sendPoints(buffer, count), 1000);
            }
        } catch (err) {
            console.log(err => console.log("error handling array of points for clipping tool"));
        }
    }

    let globalPoints = [];
    const sendClip  = useCallback((e) => {
        selectPoints(globalPoints);
    }, [canvas]);

    const resetClip = useCallback( (e) => {
        globalPoints.splice(0, globalPoints.length);
    })

    const clipperClick = useCallback((e) => {
        let x = e.pageX - canvas.offsetLeft;
        let y = e.pageY - canvas.offsetTop;
        globalPoints.push(x);
        globalPoints.push(y);
    }, [canvas]);

    //Store mouse position
    const mouseDown = useCallback((e) => {
        clickPos.current = {x: e.pageX - canvas.offsetLeft, y: e.pageY - canvas.offsetTop}
    },[canvas]);

    //Select area
    const mouseUp = useCallback((e) => {
        let x2 = e.pageX - canvas.offsetLeft;
        let y2 = e.pageY - canvas.offsetTop;
        selectArea(clickPos.current.x, clickPos.current.y, x2, y2);
    },[canvas]);

    //onclick event for handling vertex selection
    const onClick = useCallback((e) => {
        let x = e.pageX - canvas.offsetLeft;
        let y = e.pageY - canvas.offsetTop;
        window.Module.ready.then(api => {
            //For now, select a small section
            api.on_mouse_up(x-3, y-3, x+3, y+3);
        })
    },[canvas])
    useEffect(() => {
        canvas.onclick = props.tool === "vertex" ? onClick : null;
        if(props.tool === "select") {
            canvas.onmousedown = mouseDown;
            canvas.onmouseup = mouseUp;
        }
        if(props.tool === "clipper") {
            canvas.onmouseup = clipperClick;
        }
        return () => {
            canvas.onclick = null;
            canvas.onmouseup = null;
            canvas.onmousedown = null;
        }
    },[props.tool,mouseUp,mouseDown,canvas, onClick])
    return (
        <div className="dropdown">
            <button className="tool"
                    onClick={(e) => setDisplay(prev => prev === "none" ? "block" : "none")}>
                Select
                <input type="checkbox" id='selectToggle' checked={props.tool === "select" || props.tool === "vertex" }
                       onChange={e => {
                           props.setTool(prev => prev === "select" || prev === "vertex" ? "default" : "select");
                       }}/>
            </button>
            <Draggable>
                <div className="menu-items" id="select-menu">
                    <div className="menu-header" style={{padding:5}}>Select</div>
                    <div className="option">
                        Vertex
                        <input type="checkbox" checked={props.tool === "vertex"}
                               onChange={e => props.setTool(e.target.value === props.tool ? "default" : e.target.value)}
                               value="vertex"
                               className="toggle"
                        />
                    </div>
                    <div className="option">
                        Section
                        <input type="checkbox" checked={props.tool === "select"}
                               onChange={e => props.setTool(e.target.value === props.tool ? "default" : e.target.value)}
                               value="select"
                               id="selectToggle" className="toggle"
                        />
                    </div>
                    <div className="option">
                        Clipping Tool
                        <input type="checkbox" checked={props.tool === "clipper"}
                               onChange={e => props.setTool(e.target.value === props.tool ? "default" : e.target.value)}
                               value="clipper"
                               id="clipperToggle" className="toggle"
                        />
                        <button onClick={sendClip}>Apply</button>
                        <button onClick={resetClip}>Reset</button>
                    </div>
                    <div className="option" id="hover_here">
                        Cross section<input type='checkbox' id='crossToggle' className='toggle' value="section"/>
                    </div>
                    <div id="hidden_box">
                        <p id="hidden_text"> Click on the model to place a line on each side of the section you'd like to select. Right click to remove a line. </p>
                    </div>
                </div>
            </Draggable>
        </div>
    )
}