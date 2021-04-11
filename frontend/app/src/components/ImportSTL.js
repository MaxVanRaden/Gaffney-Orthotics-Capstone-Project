import React from 'react';
//import { fs } from 'memfs';

const ToUTF8Array = (str) => {
    var utf8 = [];
    for (var i=0; i < str.length; i++) {
        var charcode = str.charCodeAt(i);
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
};

//Test for utf-8 control characters
const isASCII = (str) => {
    // eslint-disable-next-line
    return /^[\x00-\x7F]*$/.test(str);
}

const ImportFile = () => {
    let fileReader;

    const handleFileRead = (e) => {

        console.log('Reading file to DOM');
        const content = fileReader.result;
//        console.log(content);

        // // memfs stuff in case we need it later
        // console.log('Writing file for memfs');
        // fs.writeFileSync('/temp.stl', content);
        // console.log('Reading file to memfs');
        // console.log(fs.readFileSync('/temp.stl', 'binary'));

        //var target = "e"; // commented this because it is unused
        //var len = content.length;

        // array of bytes (8-bit unsigned int) representing the string
        var converted_str;
        //var converted_str    = new Uint8Array(ToUTF8Array(content));
        if(isASCII(content))
            converted_str    = new Uint8Array(ToUTF8Array(content));
        else
            converted_str    = new TextEncoder("ISO-8859-1",{NONSTANDARD_allowLegacyEncoding: true}).encode(content);

        var len = converted_str.length;

        // alloc memory
        var input_ptr = window.Module.ready.cache = [len * 1];

        // write WASM memory calling the set method of the Uint8Array
        window.Module.HEAPU8.set(converted_str, input_ptr);

        // calls the c++ to do magic
        window.Module.ready.then(api => console.log(api.import_model(input_ptr, len)));
    };

        const handleFileChosen = (file) => {
        fileReader = new FileReader();
        fileReader.onloadend = handleFileRead;
        fileReader.readAsText(file, 'ISO-8859-1');
    };


    // event listener for file import
    const importElement = document.getElementById('file');
    importElement.onchange = function() {
        handleFileChosen(importElement.files[0]);
    };
    //return (null)
    return (
        // <label className="tool">
        <label >
            <input type='file'
                   id='file'
                   className='input-file'
                   style={{display:"none"}}
                   accept='.stl, .obj'
                   onChange={ e => handleFileChosen(e.target.files[0])}
            />
            Import file
        </label>

    );
};

export const ImportStuff = () => {
    //const styles = { display: 'flex', justifyContent: 'center'};
    const styles = {}
    return (
        <div className="tool"  style={styles} >
        < ImportFile />
        </div>
)};



