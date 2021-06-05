import {useCallback} from "react";

export const ToolGroup = (props) => {

    const handleChange = useCallback((e) => {
        props.setTool(prev => e.target.value === prev ? "default" : e.target.value)
    },[props])
    return (
        <div className='toolGroup'>
            <input type='checkbox' id='moveToggle' className='toggle' value="move"
            checked={props.tool === 'move'} onChange={handleChange}/>
            <label htmlFor='moveToggle' className='toggleLabel'>Move camera</label>
        </div>
    )
}