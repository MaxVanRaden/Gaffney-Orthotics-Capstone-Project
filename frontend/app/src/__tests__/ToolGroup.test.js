import React from 'react';
import { render, screen} from '@testing-library/react';
import {ToolGroup} from "../components/ToolGroup";
import '@testing-library/jest-dom/extend-expect';

describe('ToolGroup', function () {
    it("Renders Successfully",() => {
        render(<ToolGroup/>);
    });
    it('Contains elements', function () {
        render(<ToolGroup/>);
        expect(screen.getByDisplayValue("move")).toBeInTheDocument();
        expect(screen.getByText("Move camera")).toBeInTheDocument();
    });
});