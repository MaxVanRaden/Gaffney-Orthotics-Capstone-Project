import {useState, useCallback, useRef, useEffect} from "react";
import Draggable from "react-draggable";
import cam_icon from '../button_icons/camera_icon.jpg';
import center_icon from "../button_icons/center_icon.jpg";
export const Camera = (props) => {
    const [display,setDisplay] = useState("none");
    useEffect(() => {
        document.getElementById("camera-menu").style.display = display;
    },[display]);
    const cameraOrigin = {
        x: 2,
        y: 3,
        z: 15,
        yaw: 0,
        pitch: 0,
        roll: 0
    }
    const [camera, setCamera] = useState(cameraOrigin);
    const range = Number.MAX_SAFE_INTEGER;//range of xyz coordinates
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

    //Read camera values from backend
    const updateCamera = useCallback(() =>{
        window.Module.ready.then(api => {
            let addr = api.get_camera()
            let x = window.Module.getValue(addr, "float");
            let y = window.Module.getValue(addr+4, "float");
            let z = window.Module.getValue(addr+8, "float");
            let yaw = window.Module.getValue(addr+12, "float");
            let pitch = window.Module.getValue(addr+16, "float");
            let roll = window.Module.getValue(addr+20, "float");
            // console.log({x, y, z, yaw, pitch, roll});
            setCamera({x, y, z, yaw, pitch, roll})
            window.Module._free(addr);
        });
    },[]);

    //Handle mouse(and camera) movement
    const handleMove = useCallback((e) =>{
        trackMouse(e);
        //Use Ctrl key or right click to change angles
        if (e.ctrlKey || e.which === 3) {
            let newPitch = Math.min(Math.max(-360, Number(moveVals.current.pitch - e.movementY*0.1)), 360);
            let newYaw = Math.min(Math.max(-360, Number(moveVals.current.yaw + e.movementX*0.1)), 360);
            moveVals.current = {...moveVals.current, yaw: newYaw, pitch: newPitch};
        } else {
            let newX = Math.min(Math.max(-range, Number(moveVals.current.x + e.movementX*0.1)), range);
            let newY = Math.min(Math.max(-range, Number(moveVals.current.y - e.movementY*0.1)), range);
            moveVals.current = {...moveVals.current, x: newX, y: newY};
        }
        // setCamera({...moveVals.current});
        window.Module.ready.then(api => {
            api.set_camera(props.zoom, moveVals.current.x, moveVals.current.y, moveVals.current.z, moveVals.current.yaw, moveVals.current.pitch, moveVals.current.roll);
            updateCamera();
        })
    },[trackMouse, range, updateCamera, props.zoom]);

    //On mouse down store mouse position
    const mouseDown = useCallback((e) => {
        canvasX.current = e.pageX - canvasElement.offsetLeft;
        canvasY.current = e.pageY - canvasElement.offsetTop;
        if(props.tool === 'move') {
            setClicked(true);
            moveVals.current = {...camera};
        }
    },[canvasElement,props.tool, camera]);

    //On mouse up call corresponding function
    const mouseUp = useCallback((e) => {
        setClicked(false);
        canvasElement.onmousemove = trackMouse;
    },[canvasElement, trackMouse])

    //Move camera while scrolling wheel
    const onwheel = useCallback((e) => {
        e.preventDefault();
        if(props.tool === 'move') {
            if(e.ctrlKey){
                let val = Math.min(Math.max(-360,  (e.deltaY * 0.1) + camera.roll), 360);
                setCamera({...camera,roll: val});
                window.Module.ready.then(api => api.set_camera(props.zoom, camera.x, camera.y, camera.z, camera.yaw, camera.pitch, val));
            }
            else{
                let val = Math.min(Math.max(-range,  (e.deltaY * 0.01) + camera.z), range);
                setCamera({...camera, z:val});
                window.Module.ready.then(api => api.set_camera(props.zoom, camera.x, camera.y, val, camera.yaw, camera.pitch, camera.roll));
            }
        }
    },[props.tool, camera, props.zoom, range])

    //Set event listeners
    useEffect(() => {
        if(props.tool === "move"){
            canvasElement.onmousedown = mouseDown;
            canvasElement.onmouseup = mouseUp;
        }
        canvasElement.onmousemove = clicked && props.tool === "move" ? handleMove : trackMouse;
        canvasElement.onwheel = onwheel;
        //Remove event listeners
        return () => {
            canvasElement.onmousemove = null;
            canvasElement.onmousedown = null;
            canvasElement.onmouseup = null;
            canvasElement.onwheel = null;
        }
    },[props.tool,handleMove,mouseDown,canvasElement,mouseUp,onwheel, clicked,trackMouse]);

    //Handle input
    const handleChange = useCallback((e) => {
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
        // setCamera({...newCamera});
        window.Module.ready.then(api => api.set_camera(props.zoom, newCamera.x, newCamera.y, newCamera.z, newCamera.yaw, newCamera.pitch, newCamera.roll));
        updateCamera();
    },[props.zoom, camera, updateCamera]);
    const styles = {
        float:"left",
        minWidth:10,
        maxWidth:1000,
        minHeight:2,
        maxHeight:500,
        padding:"12px 0 16px 5px",
        display: "block",
        width: "100%",
    }

    return(
        <>
            <button id="update_camera" style={{display:"none"}} onClick={updateCamera}/>
            <div className="reset_icon">
                <span className="description" style={{fontSize:10, top:"100%",position:"absolute"}}>Reset Camera</span>
                <img src={center_icon} alt="center model icon" className="icon" onClick={(e) => {
                    setCamera(cameraOrigin);
                    window.Module.ready.then(api => api.set_camera(props.zoom,cameraOrigin.x, cameraOrigin.y, cameraOrigin.z, cameraOrigin.yaw, cameraOrigin.pitch, cameraOrigin.roll))
                }}/>
            </div>
            <div className="dropdown">
                <div className="camera_controls">
                    <span className="description" style={{fontSize:10, position:"absolute", top:"100%"}}>Camera Controls</span>
                    <img src={cam_icon} alt="camera control icon" className="icon" id="camera_icon"
                         onClick={(e) => setDisplay(prev => prev === "none" ? "block" : "none")}/></div>
                <Draggable handle=".menu-header">
                    <div className="menu-items" id="camera-menu" style={{minWidth:110,resize: "both", overflow: "auto"}}>
                        <div className="menu-header" style={{padding:5}}>Camera</div>
                        <div className="option" style={styles}>
                            X:
                            <input type="number"
                                   className="x-inp"
                                   min={-range} max={range}
                                   style={{maxWidth:500, paddingLeft:10, minWidth:30, width:"20%"}}
                                   maxLength="4"
                                   value={camera.x} onChange={handleChange}/>
                        </div>
                        <div className="option" style={styles}>
                            Y:
                            <input type="number"
                                   className="y-inp"
                                   min={-range} max={range}
                                   style={{maxWidth:500, paddingLeft:10, minWidth:30, width:"20%"}}
                                   maxLength="4"
                                   value={camera.y} onChange={handleChange}/>
                        </div>
                        <div className="option" style={styles}>
                            Z:
                            <input type="number"
                                   className="z-inp"
                                   min={-range} max={range}
                                   style={{maxWidth:500, paddingLeft:10, minWidth:30, width:"20%"}}
                                   maxLength="4"
                                   value={camera.z} onChange={handleChange}/>
                        </div>
                        <div className="option" style={styles}>
                            Yaw:
                            <input type="number"
                                   className="yaw-inp"
                                   min="-360" max="360"
                                   style={{maxWidth:500, paddingLeft:10, minWidth:30, width:"20%"}}
                                   maxLength="4"
                                   value={camera.yaw} onChange={handleChange}/>
                        </div>
                        <div className="option" style={styles}>
                            Pitch:
                            <input type="number"
                                   className="pitch-inp"
                                   min="-360" max="360"
                                   style={{maxWidth:500, paddingLeft:10, minWidth:30, width:"20%"}}
                                   maxLength="4"
                                   value={camera.pitch} onChange={handleChange}/>
                        </div>
                        <div className="option" style={styles}>
                            Roll:
                            <input type="number"
                                   className="roll-inp"
                                   min="-360" max="360"
                                   style={{maxWidth:500, paddingLeft:10, minWidth:30, width:"20%"}}
                                   maxLength="4"
                                   value={camera.roll} onChange={handleChange}/>
                        </div>
                    </div>
                </Draggable>
            </div>
        </>
    )
}