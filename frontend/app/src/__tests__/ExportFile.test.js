import React from 'react';
import { render, screen} from '@testing-library/react';
import {ExportFile} from "../components/ExportFile";
import '@testing-library/jest-dom/extend-expect';

describe('Toolbar', function () {
    it("Renders Successfully",() => {
        render(<ExportFile/>);
    });
    it('Contains file format input', function () {
        render(<ExportFile/>);
        expect(screen.getByDisplayValue(".stl")).toBeInTheDocument();
        expect(screen.getByDisplayValue(".obj")).toBeInTheDocument();
    });
});