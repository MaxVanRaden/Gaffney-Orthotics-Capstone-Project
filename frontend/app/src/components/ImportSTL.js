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

        var target = "e";
        var len = content.length;

        // array of bytes (8-bit unsigned int) representing the string
        var converted_str    = new Uint8Array(ToUTF8Array(content));

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
        fileReader.readAsText(file);
    };

    return (

        <input type='file'
               id='file'
               className='input-file'
               accept='.stl, .obj'
               onChange={e => handleFileChosen(e.target.files[0])}
        />
    );
};

export const ImportStuff = () => {
    const styles = { display: 'flex', justifyContent: 'center'};
    return (
        <div style={styles}>
            <ImportFile />
        </div>
    );
};