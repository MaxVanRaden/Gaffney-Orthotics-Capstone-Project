import React from 'react';
import ReactDOM from 'react-dom';
import App from '../App';

it('renders in new div without crashing', () => {
    const div = document.createElement('div');
    Object.defineProperty(document, "getElementById",{value:() => document.createElement("canvas")})
    ReactDOM.render(<App />, div);
    ReactDOM.unmountComponentAtNode(div);
});