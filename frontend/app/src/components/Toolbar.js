import {ExportFile} from "./ExportFile";
import {ImportFile} from "./ImportFile";
import {ViewMenu} from "./ViewMenu";
import {EditMenu} from "./EditMenu";
import {SelectMenu} from "./SelectMenu";
import {Coords} from "./Coords";
import {Rotate} from "./Rotate";

export const Toolbar = () => {
    return(
        <div id ="toolbar" className="toolbar">
            <a href="./" className="backbtn">&#8617;</a>
            <ImportFile/>
            <ExportFile/>
            <ViewMenu/>
            <EditMenu/>
            <SelectMenu/>
            <Rotate/>
            <Coords/>
        </div>
    )
}

