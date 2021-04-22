export const Rotate = (props) => {

    return (
        <div className='rotate'>
            <input type='checkbox' id='rtoggle2' className='rtoggle' key={Math.random()}
            defaultChecked={props.checked} onChange={e => props.setChecked(e.target.checked)}/>
            <label htmlFor='rtoggle2' id='rlabel'>Move camera</label>
        </div>
    )
}