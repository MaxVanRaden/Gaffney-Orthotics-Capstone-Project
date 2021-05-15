import {TranslateVertex} from "./Translate";
import {ExportFile} from "./ExportFile";
import {ImportFile} from "./ImportFile";
import {ViewMenu} from "./ViewMenu";
import {EditMenu} from "./EditMenu";
import {SelectMenu} from "./SelectMenu";
import {Coords} from "./Coords";
import {Camera} from "./Camera";
import {useState, useEffect} from 'react'
import {ToolGroup} from "./ToolGroup";

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
            <button onClick={() => window.Module.ready.then(api => api.undo())}>UNDO</button>
            {/*<a href="./" className="backbtn">&#8617;</a>*/}
            <button onClick={() => window.Module.ready.then(api => api.redo())}>REDO</button>
            {/*<a href="./" className="backbtn">&#8618;</a>*/}
            <button id="devToggle" className="tool" onClick={() => setDev(!dev)}>Dev Stuff</button>
            <ImportFile/>
            <ExportFile/>
            <TranslateVertex/>
            <ViewMenu zoom={zoom} setZoom={setZoom}/>
            <EditMenu/>
            <SelectMenu tool={tool} setTool={setTool}/>
            <Camera tool={tool} zoom={zoom}/>
            <ToolGroup tool={tool} setTool={setTool}/>
            <Coords/>
        </div>
    )
}

