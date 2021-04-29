import {TranslateVertex} from "./Translate";
import {ExportFile} from "./ExportFile";
import {ImportFile} from "./ImportFile";
import {ViewMenu} from "./ViewMenu";
import {EditMenu} from "./EditMenu";
import {SelectMenu} from "./SelectMenu";
import {Coords} from "./Coords";
import {Rotate} from "./Rotate";
import {Camera} from "./Camera";
import {useState} from 'react'
export const Toolbar = () => {
    const [checked, setChecked] = useState(false)
    const [zoom, setZoom] = useState(0)
    return(
        <div id ="toolbar" className="toolbar">
            <a href="./" className="backbtn">&#8617;</a>
            <ImportFile/>
            <ExportFile/>
            <TranslateVertex/>
            <ViewMenu zoom={zoom} setZoom={setZoom}/>
            <EditMenu/>
            <SelectMenu/>
            <Camera checked={checked} zoom={zoom}/>
            <Rotate checked={checked} setChecked={setChecked}/>
            <Coords/>
        </div>
    )
}

