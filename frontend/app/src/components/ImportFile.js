import React from 'react';
import { useEffect } from 'react';

const Import = () => {
    let fileReader;
    let fileName = '';
    let fileFormat = 4;
    // fileformat 0: obj
    //            1: stl (ascii)
    //            2: stl (binary)
    //            3: memfs filepath
    //            4: null

    const handleChange = ({target: {files}}) => {
        fileName = files[0].name;
        if (/^[a-zA-Z0-9_-]+\.[.obj|.OBJ]+$/.test(fileName)) {
            fileFormat = 0;
            fileReader = new FileReader();
            fileReader.onloadend = handleFileRead;
            fileReader.readAsText(files[0], 'ISO-8859-1');
        } else if (/^[a-zA-Z0-9_-]+\.[.stl|.STL]+$/.test(fileName)) {
            fileFormat = 1;
            fileReader = new FileReader();
            fileReader.onloadend = handleFile;
            fileReader.readAsText(files[0], 'ISO-8859-1');
        } else{
            alert("wrong file format");
            return{};
        }
    }

    const sendFile = (filePath) => {
        window.Module.ready.then(api => api.import_file(filePath))
            .catch(e => console.log("ImportFile.js failed to send string to emscripten"));
        console.log("ImportFile.js operation concluded")
    }
    const handleFile = () => {
        const getView = async () => {
            try {
                const result = fileReader.result;
                let uint8_view   = new TextEncoder("ISO-8859-1",{NONSTANDARD_allowLegacyEncoding: true}).encode(result);
                if (!uint8_view) {
                    console.log(err => "could not convert view to Uint8Array");
                } else {
                setTimeout(window.FS.writeFile(fileName, uint8_view, err => {
                    if (err) {
                        console.log("stuff broke");
                    }
                }), 1000);
                setTimeout(window.FS.readFile(fileName, err => {
                    if (err) {
                        console.log("stuff broke");
                    }
                }), 1000);
                setTimeout(sendFile(fileName), 1000);
                }
            } catch (err) {
                console.log(err => console.log("import did not complete and was aborted"));
            }
        };
        getView();
    };

    const handleFileRead = () => {
        const result = fileReader.result;

        // array of bytes (8-bit unsigned int) representing the string
        const uint8_view = new TextEncoder("ISO-8859-1",{NONSTANDARD_allowLegacyEncoding: true}).encode(result);
        const len = uint8_view.length;

        // alloc memory
        const input_ptr = window.Module.ready.cache = [len];

        // write WASM memory calling the set method of the Uint8Array
        window.Module.HEAPU8.set(uint8_view, input_ptr);

        console.log("gonna send the file");
        window.Module.ready.then(api => console.log(api.import_model(input_ptr, fileFormat)))
            .catch(err => console.log("ImportFile.js failed to send string to emscripten"));
        console.log("ImoortFile.js finished operations")
    };

    // event listener for file import
    useEffect(() => {
        window.addEventListener('file', handleChange);

        // cleanup this component
        return () => {
            window.removeEventListener('file', handleChange);
        };
    }, );


    return (
        <div>
            <label htmlFor="upload-file">Upload File</label>

            <input
                id="upload-file"
                name="upload-file"
                style={{display:"none"}}
                accept='.stl, .obj'
                data-testid='import-file'
                onChange={handleChange}
                type="file"
            />
        </div>
    );
};

export const ImportFile = () => {
    return (
        <div className="tool">
            <Import />
        </div>
    );
};