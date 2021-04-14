/* eslint-disable */

export const EditMenu = () => {
    //Scale the model
    const Scale = () => {
        let val = document.getElementById("scale-val").value;
        if(val > 0.01) {
            window.Module.ready.then(api => {
                api.scale(val)
            })
        }
    }

    const styles = {
        float:"left",
        maxWidth:"40px"
    }
    return (
        <div className="dropdown">
            <button className="tool">Edit</button>
            <div className="menu-items">
                <div>
                    Scale Model
                    <input id="scale-val"
                           style={styles}
                           type="number"
                           placeholder="Factor"
                           min="0"
                           step="0.1"/><span style={{float:'left', paddingLeft:2}}>x</span>
                    <button onClick={Scale}>Scale</button>
                </div>
                <a href="#">Item 2</a>
                <a href="#">Item 3</a>
            </div>
        </div>
    )
}