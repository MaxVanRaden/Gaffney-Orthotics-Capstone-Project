import {TranslateVertex} from "./Translate";
import {ExportFile} from "./ExportFile";
import {ImportFile} from "./ImportFile";
import {ViewMenu} from "./ViewMenu";
import {EditMenu} from "./EditMenu";
import {SelectMenu} from "./SelectMenu";
import {Coords} from "./Coords";
import {Camera} from "./Camera";
import {useState} from 'react'
import {ToolGroup} from "./ToolGroup";

export const Toolbar = () => {
    const [tool, setTool] = useState('select');
    const [zoom, setZoom] = useState(0);
    return(
        <div id ="toolbar" className="toolbar">
            <a href="./" className="backbtn">&#8617;</a>
            <a href="./" className="backbtn">&#8618;</a>
            <ImportFile/>
            <ExportFile/>
            <TranslateVertex/>
            <ViewMenu zoom={zoom} setZoom={setZoom}/>
            <EditMenu/>
            <SelectMenu/>
            <Camera tool={tool} zoom={zoom}/>
            <ToolGroup tool={tool} setTool={setTool}/>
            <Coords/>
        </div>
    )
}

