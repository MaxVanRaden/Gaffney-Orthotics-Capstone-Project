import React from 'react';
import { render, screen } from '@testing-library/react';
import App from '../App';

it('Renders Entire App', () => {
    Object.defineProperty(document, "getElementById",{value:() => document.createElement("canvas")})
    render(<App />);
});

