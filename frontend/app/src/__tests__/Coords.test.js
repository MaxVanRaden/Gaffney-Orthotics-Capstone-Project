import React from 'react';
import { render, screen} from '@testing-library/react';
import {Coords} from "../components/Coords";
import '@testing-library/jest-dom/extend-expect';

describe('Coords', function () {
    it("Renders Successfully",() => {
        render(<Coords/>);
    });
    it('Contains display elements', function () {
        render(<Coords/>);
        expect(screen.getByText("X:")).toBeInTheDocument();
        expect(screen.getByText("Y:")).toBeInTheDocument();
    });
});