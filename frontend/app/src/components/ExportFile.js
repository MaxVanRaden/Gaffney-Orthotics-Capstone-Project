import {saveAs} from 'file-saver'
import {useState} from 'react'

const Export = () => {
    const [format, setFormat] = useState('.obj')

    const handleChange = (e) => {setFormat(e.target.value)}

    const handleClick = () => {
        window.Module.ready.then(api => {
            /*
            Potential process to get data
            api.store_model(format)
            api.get_length()
            api.export_model(id)
            */
            //TODO:get actual length of byte array
            var len = 5;

            //get pointer to byte array
            var addr = api.export_model(0, format)

            //read array from wasm heap
            const data = new Uint8Array(window.Module.HEAP8.subarray(addr, addr+len))

            //save data with selected file format
            saveAs(new Blob([data]),'file'+format)
        })
            .catch(e => console.log("Failed to export model"))
    }
    return (
        <div>
            <input type="radio"
                   onChange={handleChange}
                   value=".stl"
                   checked={format === '.stl'}/>.stl
            <input type="radio"
                   onChange={handleChange}
                   value=".obj"
                   checked={format === '.obj'}/>.obj
            <button onClick={handleClick}>Export</button>
        </div>
    )
}

export const ExportFile = () => {
    return (
        <div>
            <Export />
        </div>
    )
}