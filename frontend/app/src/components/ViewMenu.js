/* eslint-disable */

export const ViewMenu = (props) => {


    return (
        <div className="dropdown">
            <button className="tool">View</button>
            <div className="menu-items">
                <div className="option" style={{textAlign:"left"}}>
                    Zoom
                    <br/>
                    <div style={{paddingRight:5, display:"inline", paddingLeft:5}}>
                        <button onClick={() => props.setZoom(prev => prev > 0 ? prev - 1 : prev)}>-</button>
                    </div>
                    <input type="number" min="0" max="100" step="1" style={{maxWidth:50}}
                           value={props.zoom} onChange={e => props.setZoom(Number(e.target.value))}/>%
                    <button onClick={() => props.setZoom(prev => prev < 100 ? prev + 1 : prev)}>+</button>
                </div>
                <a className="option" href="#">Perspective</a>
                <a className="option" onClick={() => document.getElementById('fullscreen').click()}>Fullscreen</a>
            </div>
        </div>
    )
}