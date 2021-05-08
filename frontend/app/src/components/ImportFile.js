import React from 'react';
import { useEffect } from 'react';
// import { useState } from 'react';
// import { fs } from 'memfs';

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
    let fileFormat = 4;
    // fileformat 0: obj
    //            1: stl (ascii)
    //            2: stl (binary)
    //            3: memfs filepath
    //            4: null

    function STLconvert(buffer){
        let dv = new DataView(buffer, 80);
        let isLittleEndian = true;
        let numTriangles = new Uint32Array(buffer, 80, 1)[0];

        let offset = 4;
        let modelData = "OrthoFreeD STLWriter\n";
        for (let i = 0; i < numTriangles; i++) {
            let normalI = dv.getFloat32(offset, isLittleEndian);
            let normalJ = dv.getFloat32(offset+4, isLittleEndian);
            let normalK = dv.getFloat32(offset+8, isLittleEndian);
            modelData +=
                "facet normal " +
                normalI +
                " " +
                normalJ +
                " " +
                normalK +
                "\n  outer loop\n";
            offset += 12;
            for (let j = 0; j < 3; j++) {
                let verticesX = dv.getFloat32(offset, isLittleEndian);
                let verticesY = dv.getFloat32(offset+4, isLittleEndian);
                let verticesZ = dv.getFloat32(offset+8, isLittleEndian);
                modelData +=
                    "    vertex " +
                    verticesX +
                    " " +
                    verticesY +
                    " " +
                    verticesZ +
                    "\n";
                offset += 12;
            }
            modelData += "  endloop\nendfacet\n";
            offset += 2;
        }
        modelData += "endsolid OrthoFreeD STLWriter";
        return modelData;
    }

    const load_file = ({target: {files}}) => {
        fileName = files[0].name;
        if (/^[a-zA-Z0-9_-]+\.[.obj|.OBJ]+$/.test(fileName)) {
            fileFormat = 0;
            fileReader = new FileReader();
            fileReader.onloadend = handleFileRead;
            fileReader.readAsText(files[0], 'ISO-8859-1');
        } else if (/^[a-zA-Z0-9_-]+\.[.stl|.STL]+$/.test(fileName)) {
            let reader = new FileReader();
            reader.onload = function(e){
                let buffer = reader.result
                if (isBinarySTL(buffer)){
                    // let view = STLconvert(buffer);
                    // handleString(view);
                    fileFormat = 2;
                    fileReader = new FileReader();
                    fileReader.onloadend = handleFileRead;
                    fileReader.readAsText(files[0], 'ISO-8859-1');
                } else{
                    fileFormat = 1;
                    fileReader = new FileReader();
                    fileReader.onloadend = handleFileRead;
                    fileReader.readAsText(files[0], 'ISO-8859-1');
                }
            }
            reader.onprogress = function(e){
                if (e.lengthComputable){
                    console.log('progress: ' + e.loaded + ' of ' + e.total)
                }
            }
            console.log('loading the file');
            reader.readAsArrayBuffer(files[0]);
        } else{
            alert("wrong file format");
        }
    }
    const handleFileRead = () => {
        const view = fileReader.result;

        // array of bytes (8-bit unsigned int) representing the string
        let uint8_view;
        if(isASCII(view))
            uint8_view    = new Uint8Array(ToUTF8Array(view));
        else
            uint8_view   = new TextEncoder("ISO-8859-1",{NONSTANDARD_allowLegacyEncoding: true}).encode(view);

        const len = uint8_view.length;

        // alloc memory
        const input_ptr = window.Module.ready.cache = [len];

        // write WASM memory calling the set method of the Uint8Array
        window.Module.HEAPU8.set(uint8_view, input_ptr);

        window.Module.ready.then(api => console.log(api.import_model(input_ptr, fileFormat)))
            .catch(err => console.log("ImportFile.js failed to send string to emscripten"));
    };

    // event listener for file import
    useEffect(() => {
        window.addEventListener('file', load_file);

        // cleanup this component
        return () => {
            window.removeEventListener('file', load_file);
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
                onChange={load_file}
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