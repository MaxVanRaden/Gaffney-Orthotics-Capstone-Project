import React from 'react';
import { useEffect } from 'react';

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

function isBinarySTL(buffer){
    let tag = "solid";
    let start = new Uint8Array(buffer.slice(0, tag.length));

    for (let i=0; i<tag.length; i++){
        if (tag.charCodeAt(i) !== start[i]){
            return true;
        }
    }
    return false;
}

const Import = () => {
    let fileReader;
    let fileName = '';
    let fileFormat = 99; // null
                    //0: obj
                    //1: stl (ascii)
                    //2: stl (binary)
                    //3: filepath

    const handleChange = ({target: {files}}) => {
        fileName = files[0].name;
        fileReader = new FileReader();
        if (/^[a-zA-Z0-9_-]+\.[.obj|.OBJ]+$/.test(fileName)) {
            fileFormat = 0;
        } else if (/^[a-zA-Z0-9_-]+\.[.stl|.STL]+$/.test(fileName)) {
            if (isBinarySTL(files[0])) {
                fileFormat = 2;
            } else {
                fileFormat = 1;
            }
        } else{
            alert("wrong file format");
            return{};
        }
        fileReader.onloadend = handleFileRead;
        fileReader.readAsText(files[0], 'ISO-8859-1');
    }

    const sendFile = (filePath) => {
        window.Module.ready.then(api => api.import_file(filePath, fileFormat))
            .catch(e => console.log("ImportFile.js failed to send string to emscripten"));
        console.log("ImportFile.js operation concluded")
    }
    const handleFileRead = () => {
        if (fileFormat === 0 || fileFormat === 1){
            // array of bytes (8-bit unsigned int) representing the string
            const result = fileReader.result;
            let uint8_view;
            if(isASCII(result))
                uint8_view    = new Uint8Array(ToUTF8Array(result));
            else
                uint8_view   =
                    new TextEncoder("ISO-8859-1",{NONSTANDARD_allowLegacyEncoding: true}).encode(result);

            const len = uint8_view.length;

            // alloc memory
            //const input_ptr = window.Module.ready.cache = [len];
            const input_ptr = window.Module._malloc(len * uint8_view.BYTES_PER_ELEMENT)

            // write WASM memory calling the set method of the Uint8Array
            window.Module.HEAPU8.set(uint8_view, input_ptr);

            window.Module.ready.then(api => console.log(api.import_model(input_ptr, fileFormat)))
                .catch(err => console.log("ImportFile.js failed to send string to emscripten"));
        } else if (fileFormat === 2) {
            const getView = async () => {
                try {
                    const result = fileReader.result;
                    let uint8_view =
                        new TextEncoder("ISO-8859-1", {NONSTANDARD_allowLegacyEncoding: true}).encode(result);
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
        }
    };

    useEffect(() => {
        window.addEventListener('file', handleChange);

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