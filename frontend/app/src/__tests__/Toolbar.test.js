import React from 'react';
import { render, screen} from '@testing-library/react';
import {Toolbar} from "../components/Toolbar";
import '@testing-library/jest-dom/extend-expect';

describe('Toolbar', function () {
    it("Renders Successfully",() => {
        Object.defineProperty(document, "getElementById",{value:() => document.createElement("div")})
        render(<Toolbar/>);
    })
    it('Contains Tools', function () {
        render(<Toolbar/>);
        expect(screen.getByText("Undo")).toBeInTheDocument();
        expect(screen.getByText("Redo")).toBeInTheDocument();
        expect(screen.getAllByText("Edit")[0]).toBeInTheDocument();
        expect(screen.getByText("Export File")).toBeInTheDocument();
        expect(screen.getByText("Upload File")).toBeInTheDocument();
        expect(screen.getByText("Flip Axis")).toBeInTheDocument();
        expect(screen.getAllByText("View")[0]).toBeInTheDocument();
        expect(screen.getByAltText("center model icon")).toBeInTheDocument();
        expect(screen.getByText("Move camera")).toBeInTheDocument();
        expect(screen.getAllByText("X:")[0]).toBeInTheDocument();
        expect(screen.getAllByText("Y:")[0]).toBeInTheDocument();
    });
});