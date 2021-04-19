import React from 'react';
//import {findDOMNode} from "react-dom";
//import {useState} from 'react';
//import SelectionArea from "@simonwep/selection-js";

export const Coords = () => {

    // const [x, setX] = useState(0);
    // const [y, setY] = useState(0);
    // const [x2, setX2] = useState(0);
    // const [y2, setY2] = useState(0);

    // const mouseDown = (e) => {
    //     setX(e.nativeEvent.offsetX);
    //     setY(e.nativeEvent.offsetY);
    // }

    // const mouseUp = (e) => {
    //     setX2(e.nativeEvent.offsetX);
    //     setY2(e.nativeEvent.offsetY);
    //     window.Module.ready.then(api => console.log(api.on_mouse_up(x, y, x2, y2)));
    // }


    // onMouseDown={mouseDown}
    // onMouseUp={mouseUp}
    return (
        <>
            <div className='coords'>
                <label htmlFor='xcoord'>X:</label>
                <div id='xcoord'></div>
                <label htmlFor='ycoord'>Y:</label>
                <div id='ycoord'></div>
            </div>

        </>
    );
}