import React from 'react';

export const Coords = () => {

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