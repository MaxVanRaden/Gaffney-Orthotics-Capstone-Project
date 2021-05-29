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








    function clipArea(){

        let drawq = [];
        //let canvas = document.getElementById('canvas');
        let width = 2000;
        let height = 2000;
        let ctx = canvas.getContext('2d');

        function pushOp(coords) {
            drawq.push(coords);
            update();
        }
        //
        function update() {
            ctx.clearRect(0, 0, width, height);
            ctx.beginPath();

            ctx.moveTo(drawq[0][0], drawq[0][1]); //first vertex
            for (let i = 1; i < drawq.length; i++)
                ctx.lineTo(drawq[i][0], drawq[i][1]);
            ctx.lineTo(drawq[0][0], drawq[0][1]); //back to start

            ctx.strokeStyle = "red";
            ctx.lineWidth = 3;
            ctx.stroke();
            ctx.closePath();

        }


        // $('#undo').click(function() {
        //     drawq.pop();
        //     $('#drawq option').last().remove();
        //     update();
        // });

        try {

            let args = ((e) => {
                drawq.push (e.pageX - canvas.offsetLeft);
            },[canvas]);

            pushOp(args);

        } catch(e) {
            console.log('Error')
        }

        console.log(drawq);

    }


    const samplePoints = [0];

    const sendClip  = useCallback((points) => {

        points.push(1);
        points.push(2);
        points.push(3);
        points.push(4);
        const count = points.length;

        let bytesSize = new Int32Array(window.Module.HEAP32.BYTES_PER_ELEMENT);
        const arrPtr = window.Module._malloc((count * bytesSize));
        window.Module.HEAP32.set(points, (arrPtr / bytesSize));
        window.Module.ready.then(api => api.point_click(points, count));

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
            clipArea();
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
                               onChange={clipArea}
                               value="clipper"
                               id="clipperToggle" className="toggle"
                        />
                        {/*<button onClick={sendClip(samplePoints)}>Apply</button>*/}
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