// import {TranslateVertex} from "./Translate";
import {ExportFile} from "./ExportFile";
import {ImportFile} from "./ImportFile";
import {ViewMenu} from "./ViewMenu";
import {EditMenu} from "./EditMenu";
import {SelectMenu} from "./SelectMenu";
import {Coords} from "./Coords";
import {Camera} from "./Camera";
import {useState, useEffect} from 'react'
import {ToolGroup} from "./ToolGroup";
//import center_icon from "../button_icons/center_icon.jpg"

export const Toolbar = () => {
    const [tool, setTool] = useState('default');
    const [zoom, setZoom] = useState(0);
    const [dev, setDev] = useState(false)
    useEffect(() =>{
        let devStuff = document.getElementById("dev_stuff")
        let body = document.getElementsByClassName("main")[0]
        devStuff.style.display = dev ? "block" : "none";
        body.style.overflow = dev ? "visible" : "hidden";
    },[dev])
    return(
        <div id ="toolbar" className="toolbar">
            <div className="undo">
                <span className="description" style={{fontSize:10, position:"absolute", top:"100%" ,left:5}}>Undo</span>
                <button className="backbtn" onClick={() => window.Module.ready.then(api => api.undo())}>&#8617;</button>
            </div>
            {/*<a href="./" className="backbtn">&#8617;</a>*/}
            <div className="redo">
                <span className="description" style={{fontSize:10, position:"absolute", top:"100%" ,left:50}}>Redo</span>
                <button className="backbtn" onClick={() => window.Module.ready.then(api => api.redo())}>&#8618;</button>
                {/*<a href="./" className="backbtn">&#8618;</a>*/}
            </div>
            <button id="devToggle" className="tool" onClick={() => setDev(!dev)}>Dev Stuff</button>
            <ImportFile/>
            <ExportFile/>
            <ViewMenu zoom={zoom} setZoom={setZoom}/>
            <EditMenu/>
            <SelectMenu tool={tool} setTool={setTool}/>
            <button className="tool" onClick={() => window.Module.ready.then(api => api.flip_axis())}>Flip Axis</button>
            {/*<img src={center_icon} alt="center model icon" className="icon"/>*/}
            <Camera tool={tool} zoom={zoom}/>
            <ToolGroup tool={tool} setTool={setTool}/>
            <Coords/>
        </div>
    )
}

