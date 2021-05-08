import {useState, useCallback, useRef, useEffect} from "react";

export const Camera = (props) => {
    const [camera, setCamera] = useState({
        x: 0,
        y: 0,
        z: 10,
        yaw: 0,
        pitch: 0,
        roll: 0
    });
    const zoom = props.zoom / 100;
    const range = 100;//range of xyz coordinates
    let canvasElement = document.getElementById('canvas');
    const [clicked, setClicked] = useState(false) //Sets onmousemove listener
    let moveVals = useRef({...camera})//tracks mouse movement without rerender
    let canvasX = useRef(0);
    let canvasY = useRef(0);
    //Track mouse coords
    const trackMouse = useCallback(e => {
        let curX = e.pageX - canvasElement.offsetLeft;
        let curY = e.pageY - canvasElement.offsetTop;
        document.getElementById('xcoord').innerHTML = curX;
        document.getElementById('ycoord').innerHTML = curY;
    },[canvasElement]);
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
    //Handle mouse(and camera) movement
    const handleMove = useCallback((e) =>{
        trackMouse(e);
        //Use Ctrl key to change angles
        if (e.ctrlKey) {
            let newPitch = Math.min(Math.max(-360, Number(moveVals.current.pitch + e.movementY)), 360);
            let newYaw = Math.min(Math.max(-360, Number(moveVals.current.yaw + e.movementX)), 360);
            moveVals.current = {...moveVals.current, yaw: newYaw, pitch: newPitch};
        } else {
            let newX = Math.min(Math.max(-range, Number(moveVals.current.x + e.movementX*0.5)), range);
            let newY = Math.min(Math.max(-range, Number(moveVals.current.y - e.movementY*0.5)), range);
            moveVals.current = {...moveVals.current, x: newX, y: newY};
        }
        window.Module.ready.then(api => {
            api.set_camera(zoom, moveVals.current.x, moveVals.current.y, moveVals.current.z, moveVals.current.yaw, moveVals.current.pitch, moveVals.current.roll);
        })
    },[zoom, trackMouse]);

    const mouseDown = useCallback((e) => {
        canvasX.current = e.pageX - canvasElement.offsetLeft;
        canvasY.current = e.pageY - canvasElement.offsetTop;
        if(props.tool === 'move') {
            setClicked(true);
            moveVals.current = {...camera};
        }
        },[canvasElement,props.tool, camera]);

    const mouseUp = useCallback((e) => {
        let x2 = e.pageX - canvasElement.offsetLeft;
        let y2 = e.pageY - canvasElement.offsetTop;
        setClicked(false);
        switch(props.tool){
            case 'select':
                selectArea(canvasX.current, canvasY.current, x2, y2);
                break;
            case 'move':
                setCamera({...moveVals.current});
                break;
            default:
                break;
        }
        canvasElement.onmousemove = trackMouse;
    },[canvasElement,props.tool, trackMouse])

    const onwheel = useCallback((e) => {
        e.preventDefault();
        if(props.tool === 'move') {
            if(e.ctrlKey){
                let val = Math.min(Math.max(-360,  (e.deltaY * 0.1) + camera.roll), 360);
                setCamera({...camera,roll: val});
                window.Module.ready.then(api => api.set_camera(zoom, camera.x, camera.y, camera.z, camera.yaw, camera.pitch, camera.roll));
            }
            else{
                let val = Math.min(Math.max(-range,  (e.deltaY * 0.01) + camera.z), range);
                setCamera({...camera, z:val});
                window.Module.ready.then(api => api.set_camera(zoom, camera.x, camera.y, camera.z, camera.yaw, camera.pitch, camera.roll));
            }
        }
    },[props.tool, camera, zoom])
    //Set event listeners
    useEffect(() => {
        canvasElement.onmousedown = mouseDown;
        canvasElement.onmousemove = clicked ? handleMove : trackMouse;
        canvasElement.onmouseup = mouseUp;
        canvasElement.onwheel = onwheel;
        //Remove event listeners
        return () => {
            canvasElement.onmousemove = null;
            canvasElement.onmousedown = null;
            canvasElement.onmouseup = null;
            canvasElement.onwheel = null;
        }
    },[props.tool,handleMove,mouseDown,canvasElement,mouseUp,onwheel, clicked,trackMouse]);

    const handleChange = (e) => {
        let val = Number(e.target.value);
        let newCamera = {...camera};
        switch(e.target.className){
            case "x-inp":
                newCamera.x = val;
                break
            case "y-inp":
                newCamera.y = val;
                break
            case "z-inp":
                newCamera.z = val;
                break
            case "yaw-inp":
                newCamera.yaw = val;
                break
            case "pitch-inp":
                newCamera.pitch = val;
                break
            case "roll-inp":
                newCamera.roll = val;
                break
            default:
                console.log("unexpected input class");
        }
        setCamera({...newCamera});
        window.Module.ready.then(api => api.set_camera(zoom, newCamera.x, newCamera.y, newCamera.z, newCamera.yaw, newCamera.pitch, newCamera.roll));
    }
    const styles = {
        float:"left",
        minWidth:200
    }
    return(
        <div className="dropdown">
            <button className="tool">Camera</button>
            <div className="menu-items" style={{minWidth:220}}>
                <div className="option" style={styles}>
                    X:
                    <input type="range"
                           className="x-inp"
                           value={camera.x}
                           min={-range} max={range}
                           style={{maxWidth:75}}
                           step=".1"
                           onChange={handleChange}
                    />
                    <input type="number"
                           className="x-inp"
                           min={-range} max={range}
                           style={{maxWidth:50, paddingLeft:10}}
                           maxLength="4"
                           value={camera.x} onChange={handleChange}/>
                </div>
                <div className="option" style={styles}>
                    Y:
                    <input type="range"
                           style={{maxWidth:75}}
                           className="y-inp"
                           value={camera.y}
                           min={-range} max={range}
                           step=".1"
                           onChange={handleChange}
                    />
                    <input type="number"
                           className="y-inp"
                           min={-range} max={range}
                           style={{maxWidth:50, paddingLeft:10}}
                           maxLength="4"
                           value={camera.y} onChange={handleChange}/>
                </div>
                <div className="option" style={styles}>
                    Z:
                    <input type="range"
                           style={{maxWidth:75}}
                           className="z-inp"
                           value={camera.z}
                           min={-range} max={range}
                           step=".1"
                           onChange={handleChange}
                    />
                    <input type="number"
                           className="z-inp"
                           min={-range} max={range}
                           style={{maxWidth:50, paddingLeft:10}}
                           maxLength="4"
                           value={camera.z} onChange={handleChange}/>
                </div>
                <div className="option" style={styles}>
                    Yaw:
                    <input type="range"
                           className="yaw-inp"
                           value={camera.yaw}
                           min="-360" max="360"
                           style={{maxWidth:75}}
                           step=".1"
                           onChange={handleChange}
                    />
                    <input type="number"
                           className="yaw-inp"
                           min="-360" max="360"
                           style={{maxWidth:50, paddingLeft:10}}
                           maxLength="4"
                           value={camera.yaw} onChange={handleChange}/>
                </div>
                <div className="option" style={styles}>
                    Pitch:
                    <input type="range"
                           className="pitch-inp"
                           value={camera.pitch}
                           min="-360" max="360"
                           style={{maxWidth:75}}
                           step=".1"
                           onChange={handleChange}
                    />
                    <input type="number"
                           className="pitch-inp"
                           min="-360" max="360"
                           style={{maxWidth:50, paddingLeft:10}}
                           maxLength="4"
                           value={camera.pitch} onChange={handleChange}/>
                </div>
                <div className="option" style={styles}>
                    Roll:
                    <input type="range"
                           className="roll-inp"
                           value={camera.roll}
                           min="-360" max="360"
                           style={{maxWidth:75}}
                           step=".1"
                           onChange={handleChange}
                    />
                    <input type="number"
                           className="roll-inp"
                           min="-360" max="360"
                           style={{maxWidth:50, paddingLeft:10}}
                           maxLength="4"
                           value={camera.roll} onChange={handleChange}/>
                </div>
            </div>
        </div>
    )
}