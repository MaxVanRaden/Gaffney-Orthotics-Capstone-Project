import React from 'react';
import { render, screen, fireEvent} from '@testing-library/react';
import {EditMenu} from "../components/EditMenu";
import '@testing-library/jest-dom/extend-expect';

describe('Edit Menu', function () {
    it("Renders Successfully",() => {
        render(<EditMenu/>);
    })
    it('Contains Edit button', function () {
        render(<EditMenu/>);
        let edits = screen.getAllByText("Edit");
        edits[0].onclick = jest.fn();
        expect(screen.getAllByText("Edit")[0]).toBeInTheDocument();
        fireEvent(edits[0], new MouseEvent("click"));
        expect(edits[0].onclick).toBeCalled();
    });
    it('Contains initial translation factor', function () {
        render(<EditMenu/>);
        expect(screen.getByDisplayValue(0.001)).toBeInTheDocument();
    });
});