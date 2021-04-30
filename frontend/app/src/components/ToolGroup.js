export const ToolGroup = (props) => {

    return (
        <div className='toolGroup'>
            <input type="radio" checked={props.tool === 'select'} onChange={e => props.setTool(e.target.value)}
                   value="select" id="selectToggle" className="toggle"/>
            <label htmlFor="selectToggle" className="toggleLabel">Select</label>
            <input type='radio' id='moveToggle' className='toggle' value="move"
            checked={props.tool === 'move'} onChange={e => props.setTool(e.target.value)}/>
            <label htmlFor='moveToggle' className='toggleLabel'>Move camera</label>
        </div>
    )
}