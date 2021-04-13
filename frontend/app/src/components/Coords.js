export const Coords = () => {
    return(
        <div className='coords'>
            <label htmlFor='xcoord'>X:</label>
            <div id='xcoord' className='xycoords'></div>
            <label htmlFor='ycoord'>Y:</label>
            <div id='ycoord' className='xycoords'></div>
        </div>
    )
}