import React from 'react';
import { render, screen, fireEvent} from '@testing-library/react';
import {SelectMenu} from "../components/SelectMenu";
import '@testing-library/jest-dom/extend-expect';

describe('SelectMenu', function () {
    it("Renders Successfully",() => {
        Object.defineProperty(document, "getElementById",{value:() => document.createElement("canvas")})
        render(<SelectMenu/>);
    });
    it('Contains menu items', function () {
        render(<SelectMenu/>);
        expect(screen.getByText("Section")).toBeInTheDocument();
        expect(screen.getByText("Vertex")).toBeInTheDocument();
        expect(screen.getByText("Cross section")).toBeInTheDocument();
        expect(screen.getByDisplayValue("section")).toBeInTheDocument();
        expect(screen.getByDisplayValue("vertex")).toBeInTheDocument();
        expect(screen.getByDisplayValue("select")).toBeInTheDocument();
    });
    it('Handles click events', function () {
        render(<SelectMenu/>);
        let area = screen.getByDisplayValue("select");
        let vertex = screen.getByDisplayValue("vertex");
        let section = screen.getByDisplayValue("section");
        expect(area.checked).toBe(false);
        expect(vertex.checked).toBe(false);
        expect(section.checked).toBe(false);
        fireEvent(area, new MouseEvent("click"));
        expect(area.checked).toBe(true);
        expect(vertex.checked).toBe(false);
        expect(section.checked).toBe(false);
        fireEvent(vertex, new MouseEvent("click"));
        expect(vertex.checked).toBe(true);
    });
});
