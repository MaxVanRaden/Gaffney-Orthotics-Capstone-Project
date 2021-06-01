import React from 'react';
import { render, screen} from '@testing-library/react';
import {ViewMenu} from "../components/ViewMenu";
import '@testing-library/jest-dom/extend-expect';

describe('ViewMenu', function () {
    it("Renders Successfully",() => {
        render(<ViewMenu/>);
    });
    it('Contains menu items', function () {
        render(<ViewMenu/>);
        expect(screen.getByText("Fullscreen")).toBeInTheDocument();
        expect(screen.getByText("Zoom")).toBeInTheDocument();
    });
    it('Contains zoom buttons with expected values', function () {
        render(<ViewMenu/>);
        let zoom_in = screen.getByText('+');
        let zoom_out = screen.getByText('-');
        expect(zoom_in.value).toBe("-1");
        expect(zoom_out.value).toBe("1");
    });
});
