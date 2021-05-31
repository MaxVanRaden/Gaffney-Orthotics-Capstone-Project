import {saveAs} from 'file-saver'
import {useState} from 'react'

const Export = () => {
    const [format, setFormat] = useState('.stl');

    const handleChange = (e) => {setFormat(e.target.value);}

    const handleClick = () => {
        window.Module.ready.then(api => {

            //get pointer to byte array
            let addr = api.export_model(format);
            let len = api.get_export_strlen();

            //read array from wasm heap
            const data = new Uint8Array(window.Module.HEAP8.subarray(addr, addr+len));
            window.Module._free(addr)
            //save data with selected file format
            saveAs(new Blob([data]),'file'+format);
        })
            .catch(e => console.log("Failed to export model"));
    }
    return (
        <div className="dropdown" id="export-menu">
            <label htmlFor="export" className="tool">Export File</label>
            <button id="export" style={{display:"none"}} onClick={handleClick}/>
            <div className="menu-items" style={{color:"black", minWidth:90, paddingRight:10}}>
                <input type="radio"
                       onChange={handleChange}
                       value=".stl"
                       checked={format === '.stl'}/>.stl
                <input type="radio"
                       onChange={handleChange}
                       value=".obj"
                       checked={format === '.obj'}/>.obj
            </div>
        </div>
    );
}

export const ExportFile = () => {
    return (
        <Export />
    );
}
