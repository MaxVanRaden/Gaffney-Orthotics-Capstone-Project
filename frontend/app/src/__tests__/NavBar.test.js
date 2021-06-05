import React from 'react';
import { render, screen} from '@testing-library/react';
import {NavBar} from "../components/NavBar";
import '@testing-library/jest-dom/extend-expect';

describe('NavBar', function () {
    it("Renders Successfully",() => {
        render(<NavBar/>);
    })
    it('Contains Logo', function () {
        render(<NavBar/>);
        expect(screen.getByText("OrthoFreeD")).toBeInTheDocument();
    });
    it('Contains Help Link', function () {
        render(<NavBar/>);
        expect(screen.getByText("Help")).toBeInTheDocument();
    });
});