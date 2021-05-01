import {useState} from "react";

export const Camera = (props) => {
    const [x, setX] = useState(0);
    const [y, setY] = useState(0);
    const [z, setZ] = useState(10);
    const [yaw, setYaw] = useState(0);
    const [roll, setRoll] = useState(0);
    const [pitch, setPitch] = useState(0);
    const zoom = props.zoom / 100;
    const range = 100;//range of xyz coordinates
    let canvasElement = document.getElementById('canvas');
    let canvasX = 0;
    let canvasY = 0;

    canvasElement.onmousedown = function mouseDown(e) {
        canvasX = e.pageX - this.offsetLeft;
        canvasY = e.pageY - this.offsetTop;
        //If the user is moving the camera
        if(props.tool === 'move'){
            this.onmousemove = (e) => {
                let curX = e.pageX - this.offsetLeft;
                let curY = e.pageY - this.offsetTop;
                document.getElementById('xcoord').innerHTML = curX;
                document.getElementById('ycoord').innerHTML = curY;
                let deltaX = curX - canvasX;
                let deltaY = canvasY - curY;
                //Use Ctrl key to change angles
                if (e.ctrlKey) {
                    let newPitch = Math.min(Math.max(-360, Number(pitch - deltaY)*0.1), 360);
                    let newYaw = Math.min(Math.max(-360, Number(yaw + deltaX)*0.1), 360);
                    setPitch(newPitch);
                    setYaw(newYaw);
                    window.Module.ready.then(api => api.set_camera(zoom, x, y, z, newYaw, newPitch, roll));
                } else {
                    let newX = Math.min(Math.max(-range, Number(x + deltaX)*0.1), range);
                    let newY = Math.min(Math.max(-range, Number(y + deltaY)*0.1), range);
                    setX(newX);
                    setY(newY);
                    window.Module.ready.then(api => api.set_camera(zoom, newX, newY, z, yaw, pitch, roll));
                }
            }
        }
    };

    canvasElement.onmouseup = function mouseUp(e) {
        let x2 = e.pageX - this.offsetLeft;
        let y2 = e.pageY - this.offsetTop;
        if(props.tool === 'select') {
            window.Module.ready.then(api => {
                if(canvasX <= x2 && canvasY <= y2)
                    api.on_mouse_up(canvasX, canvasY, x2, y2);
                else
                    api.on_mouse_up(x2, y2, canvasX, canvasY);
            });
        }
        this.onmousemove = function(e){
            let curX = e.pageX - this.offsetLeft;
            let curY = e.pageY - this.offsetTop;
            document.getElementById('xcoord').innerHTML = curX;
            document.getElementById('ycoord').innerHTML = curY;
        }
    }
    canvasElement.onwheel = function onwheel(e){
        if(props.tool === 'move') {
            e.preventDefault();
            if(e.ctrlKey){
                let val = Math.min(Math.max(-360,  (e.deltaY * 0.1) + roll), 360);
                setRoll(val);
                window.Module.ready.then(api => api.set_camera(zoom, x, y, z, yaw, pitch, roll));
            }
            else{
                let val = Math.min(Math.max(-range,  (e.deltaY * 0.01) + z), range);
                setZ(val);
                window.Module.ready.then(api => api.set_camera(zoom, x, y, z, yaw, pitch, roll));
            }
        }
    }

    const handleChange = (e) => {
        let val = Number(e.target.value);
        switch(e.target.className){
            case "x-inp":
                setX(val);
                break
            case "y-inp":
                setY(val);
                break
            case "z-inp":
                setZ(val);
                break
            case "yaw-inp":
                setYaw(val);
                break
            case "pitch-inp":
                setPitch(val);
                break
            case "roll-inp":
                setRoll(val);
                break
            default:
                console.log("unexpected input class");
        }
        window.Module.ready.then(api => api.set_camera(zoom, x, y, z, yaw, pitch, roll));
    }

    return(
        <div className="dropdown">
            <button className="tool">Camera</button>
            <div className="menu-items" style={{minWidth:220}}>
                <div style={{float:"left"}}>
                    X:
                    <input type="range"
                           className="x-inp"
                           value={x}
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
                           value={x} onChange={handleChange}/>
                </div>
                <div style={{float:"left"}}>
                    Y:
                    <input type="range"
                           style={{maxWidth:75}}
                           className="y-inp"
                           value={y}
                           min={-range} max={range}
                           step=".1"
                           onChange={handleChange}
                    />
                    <input type="number"
                           className="y-inp"
                           min={-range} max={range}
                           style={{maxWidth:50, paddingLeft:10}}
                           maxLength="4"
                           value={y} onChange={handleChange}/>
                </div>
                <div style={{float:"left"}}>
                    Z:
                    <input type="range"
                           style={{maxWidth:75}}
                           className="z-inp"
                           value={z}
                           min={-range} max={range}
                           step=".1"
                           onChange={handleChange}
                    />
                    <input type="number"
                           className="z-inp"
                           min={-range} max={range}
                           style={{maxWidth:50, paddingLeft:10}}
                           maxLength="4"
                           value={z} onChange={handleChange}/>
                </div>
                <div style={{float:"left"}}>
                    Yaw:
                    <input type="range"
                           className="yaw-inp"
                           value={yaw}
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
                           value={yaw} onChange={handleChange}/>
                </div>
                <div style={{float:"left"}}>
                    Pitch:
                    <input type="range"
                           className="pitch-inp"
                           value={pitch}
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
                           value={pitch} onChange={handleChange}/>
                </div>
                <div style={{float:"left"}}>
                    Roll:
                    <input type="range"
                           className="roll-inp"
                           value={roll}
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
                           value={roll} onChange={handleChange}/>
                </div>
            </div>
        </div>
    )
}