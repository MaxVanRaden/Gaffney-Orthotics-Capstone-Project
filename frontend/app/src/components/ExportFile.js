import {saveAs} from 'file-saver'
import {useState} from 'react'

const Export = () => {
    // const [format, setFormat] = useState('.obj'); // setFormat assigned but not used
    const [format] = useState('.obj');

    // const handleChange = (e) => {setFormat(e.target.value);} //assigned but not used

    const handleClick = () => {
        window.Module.ready.then(api => {
            /*
            Potential process to get data
            api.store_model(format)
            api.get_length(id)
            api.export_model(id)
            */
            //TODO:get actual length of byte array
            // const len = 5;

            //get pointer to byte array
            let addr = api.export_model(format);
            let len = api.get_export_strlen();


            //read array from wasm heap
            const data = new Uint8Array(window.Module.HEAP8.subarray(addr, addr+len));

            //save data with selected file format
            saveAs(new Blob([data]),'file'+format);
        })
            .catch(e => console.log("Failed to export model"));
    }
    return (
        <div>
            {/*<input type="radio"*/}
            {/*       onChange={handleChange}*/}
            {/*       value=".stl"*/}
            {/*       checked={format === '.stl'}/>.stl*/}
            {/*<input type="radio"*/}
            {/*       onChange={handleChange}*/}
            {/*       value=".obj"*/}
            {/*       checked={format === '.obj'}/>.obj*/}
            <button className="tool"
                    onClick={handleClick}>Export file</button>
        </div>
    );
}

export const ExportFile = () => {
    return (
        <div style={{paddingTop:5}}>
            <Export />
        </div>
    );
}
