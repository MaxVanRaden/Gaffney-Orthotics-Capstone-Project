/* eslint-disable */

export const ViewMenu = (props) => {
    console.log(props.zoom)
    return (
        <div className="dropdown">
            <button className="tool">View</button>
            <div className="menu-items">
                <div style={{textAlign:"center"}}>
                    Zoom
                    <br/>
                    <div style={{paddingRight:5, display:"inline", paddingLeft:5}}>
                        <button onClick={() => props.setZoom(prev => prev > -100 ? prev - 1 : prev)}>-</button>
                    </div>
                    <input type="number" min="-100" max="100" step="1" style={{maxWidth:50}}
                           value={props.zoom} onChange={e => props.setZoom(Number(e.target.value))}/>%
                    <button onClick={() => props.setZoom(prev => prev < 100 ? prev + 1 : prev)}>+</button>
                </div>
                <a href="#">Perspective</a>
                <a href="#">Fullscreen</a>
            </div>
        </div>
    )
}