/* eslint-disable */

export const EditMenu = () => {
    //Scale the model
    const Scale = () => {
        let val = document.getElementById("scale-val").value;
        if(val > 0.1) {
            window.Module.ready.then(api => {
                api.scale(val / 100)
            })
        }
    }

    const styles = {
        float:"left",
        maxWidth:"50px"
    }
    return (
        <div className="dropdown">
            <button className="tool">Edit</button>
            <div className="menu-items">
                <div className="option">
                    Scale Model
                    <input id="scale-val"
                           style={styles}
                           type="number"
                           placeholder="100"
                           min="1"
                           step="1"/>
                    <span style={{...styles, paddingLeft:2}}>%</span>
                    <button onClick={Scale}>Scale</button>
                </div>
                <a className="option" href="#">Extend</a>
                <a className="option" href="#">Bend/Twist</a>
            </div>
        </div>
    )
}