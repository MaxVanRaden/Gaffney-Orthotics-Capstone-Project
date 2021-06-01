import Draggable from 'react-draggable';
import {useState} from 'react';

export const EditMenu = () => {
    const [display,setDisplay] = useState("none");
    const [factor, setFactor] = useState(0.001);

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
            <button className="tool"
                    onClick={(e) => setDisplay(prev => prev === "none" ? "block" : "none")}>Edit</button>
            <Draggable>
                <div className="menu-items" id="edit-menu" style={{display:display}}>
                    <div className="menu-header" style={{padding:5}}>Edit</div>
                    <div className="option">
                        Scale Model
                        <input id="scale-val"
                               style={styles}
                               type="number"
                               placeholder="100"
                               min="1"
                               step="1"/>
                        <span style={{...styles, paddingLeft:2}}>% </span>
                        <button onClick={Scale}>Apply</button>
                    </div>
                    <div className="option">
                        Bend Vertex
                        <div className="input-box">
                            <input className="bend-val"
                                   style={styles}
                                   type="number"
                                   placeholder="0"
                                   min="0"
                                   step="1"/>
                            <span style={{...styles, paddingLeft:2}}>&#176; X</span>
                            <input className="bend-val"
                                   style={styles}
                                   type="number"
                                   placeholder="0"
                                   min="0"
                                   step="1"/>
                            <span style={{...styles, paddingLeft:2}}>&#176; Y</span>
                            <input className="bend-val"
                                   style={styles}
                                   type="number"
                                   placeholder="0"
                                   min="0"
                                   step="1"/>
                            <span style={{...styles, paddingLeft:2}}>&#176; Z</span>
                            <button>Apply</button>
                        </div>
                    </div>
                    <div className="option">
                        Translate Vertex
                        <div className="input-box">
                            <input className="bend-val"
                                   style={styles}
                                   type="number"
                                   placeholder="0"
                                   min="0"
                                   step="1"/>
                            <span style={{...styles, paddingLeft:2}}> X</span>
                            <input className="bend-val"
                                   style={styles}
                                   type="number"
                                   placeholder="0"
                                   min="0"
                                   step="1"/>
                            <span style={{...styles, paddingLeft:2}}> Y</span>
                            <input className="bend-val"
                                   style={styles}
                                   type="number"
                                   placeholder="0"
                                   min="0"
                                   step="1"/>
                            <span style={{...styles, paddingLeft:2}}> Z</span>
                            <button>Apply</button>
                        </div>
                    </div>
                    <div className="option">
                        Translate Factor
                        <div className="input-box">
                            <input type='number' id='translationFactor'
                                   min='0.001' step='0.01'
                                   style={{width:60}}
                                   value={factor}
                                   onChange={(e) => setFactor(Number(e.target.value))}
                            />
                        </div>
                    </div>
                </div>
            </Draggable>
        </div>
    )
}