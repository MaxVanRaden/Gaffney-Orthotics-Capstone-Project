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
    return(
        <div id ="toolbar" className="toolbar">
            <a href="./" className="backbtn">&#8617;</a>
            <ImportFile/>
            <ExportFile/>
            <ViewMenu/>
            <EditMenu/>
            <SelectMenu/>
            <Camera checked={checked}/>
            <Rotate checked={checked} setChecked={setChecked}/>
            <Coords/>
        </div>
    )
}

