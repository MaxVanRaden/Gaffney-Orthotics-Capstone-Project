/* eslint-disable */

export const NavBar = () => {
    return(
        <div id="navbar" className="navbar">
            <p id="title"> OrthoFreeD </p>
            <ul id="help">
                <li><a className="help" target="_blank" href={process.env.PUBLIC_URL + "help.html"}>Help</a></li>
            </ul>
        </div>
    )
}