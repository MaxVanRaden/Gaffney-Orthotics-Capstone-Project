import React from 'react';
//import { useEffect } from 'react';

const ToUTF8Array = (str) => {
    let utf8 = [];
    for (let i=0; i < str.length; i++) {
        let charcode = str.charCodeAt(i);
        if (charcode < 0x80) utf8.push(charcode);
        else if (charcode < 0x800) {
            utf8.push(
                0xc0 | (charcode >> 6),
                0x80 | (charcode & 0x3f));
        } else if (charcode < 0xd800 || charcode >= 0xe000) {
            utf8.push(
                0xe0 | (charcode >> 12),
                0x80 | ((charcode>>6) & 0x3f),
                0x80 | (charcode & 0x3f));
        } else {
            i++;
            charcode = 0x10000 + (((charcode & 0x3ff)<<10) | (str.charCodeAt(i) & 0x3ff));
            utf8.push(
                0xf0 | (charcode >>18),
                0x80 | ((charcode>>12) & 0x3f),
                0x80 | ((charcode>>6) & 0x3f),
                0x80 | (charcode & 0x3f));
        }
    }
    return utf8;
}

//Test for utf-8 control characters
const isASCII = (str) => {
    // eslint-disable-next-line
    return /^[\x00-\x7F]*$/.test(str);
}

const Import = () => {
    let fileReader;
    let fileName;
    let fileFormat;

    const handleFileRead = () => {

        const content = fileReader.result;


        if (/^[a-zA-Z0-9_-]+\.[.obj|.OBJ]+$/.test(fileName)) {
            fileFormat = 0;
        } else if (/^[a-zA-Z0-9_-]+\.[.stl|.STL]+$/.test(fileName)) {
            fileFormat = 1;
        }


        // array of bytes (8-bit unsigned int) representing the string
        let converted_str;
        if(isASCII(content))
            converted_str    = new Uint8Array(ToUTF8Array(content));
        else
            converted_str    = new TextEncoder("ISO-8859-1",{NONSTANDARD_allowLegacyEncoding: true}).encode(content);

        //const len = converted_str.length;

        // alloc memory
        //const input_ptr = window.Module.ready.cache = [len * 1];
        const input_ptr = window.Module._malloc(converted_str.BYTES_PER_ELEMENT*converted_str.length);

        // write WASM memory calling the set method of the Uint8Array
        window.Module.HEAPU8.set(converted_str, input_ptr);

        // calls the c++ to do magic
        window.Module.ready.then(api => {
            api.import_model(input_ptr, fileFormat);
            window.Module._free(input_ptr);
        });
    };

    //const [uploadedFileName, setUploadedFileName] = useState(null);

    const handleFileChosen = ({target: {files}}) => {
        if(!files[0]) return
        fileName = files[0].name;
        if (/^[a-zA-Z0-9_-]+\.[.obj|.OBJ|.stl|.STL]+$/.test(fileName)) {
            //setUploadedFileName(files[0].name);
            fileReader = new FileReader();
            fileReader.onloadend = handleFileRead;
            fileReader.readAsText(files[0], 'ISO-8859-1');
        } else{
            alert("wrong file format");
        }
    }

    // event listener for file import
    // useEffect(() => {
    //     window.addEventListener('file', handleFileChosen);
    //
    //     // cleanup this component
    //     return () => {
    //         window.removeEventListener('file', handleFileChosen);
    //     };
    // }, );

    return (
        <div>
            <label htmlFor="upload-file">Upload File</label>

            <input
                id="upload-file"
                name="upload-file"
                style={{display:"none"}}
                accept='.stl, .obj'
                data-testid='import-file'
                onChange={handleFileChosen}
                type="file"
            />
            {/*{uploadedFileName}*/}
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