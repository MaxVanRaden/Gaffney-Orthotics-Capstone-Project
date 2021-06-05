import React from 'react';
import { render, screen} from '@testing-library/react';
import {Camera} from "../components/Camera";
import '@testing-library/jest-dom/extend-expect';

describe('Camera', function () {
    it("Renders Successfully",() => {
        Object.defineProperty(document, "getElementById",{
            value:(id) => id.includes("coord") ? document.createElement("div") : document.createElement("canvas")
        })
        render(<Camera/>);
    });
    it('Contains input fields', function () {
        render(<Camera/>);
        expect(screen.getByText("X:")).toBeInTheDocument();
        expect(screen.getByText("Y:")).toBeInTheDocument();
        expect(screen.getByText("Z:")).toBeInTheDocument();
        expect(screen.getByText("Yaw:")).toBeInTheDocument();
        expect(screen.getByText("Pitch:")).toBeInTheDocument();
        expect(screen.getByText("Roll:")).toBeInTheDocument();
    });
    it('Contains initial values', function () {
        render(<Camera/>);
        expect(screen.getAllByDisplayValue(0).length).toBe(3);
        expect(screen.getByDisplayValue(2)).toBeInTheDocument();
        expect(screen.getByDisplayValue(3)).toBeInTheDocument();
        expect(screen.getByDisplayValue(15)).toBeInTheDocument();
    });
    it('Contains expected bounds', function () {
        render(<Camera/>);
        expect(screen.getByText("Roll:").children[0].max).toBe("360");
        expect(screen.getByText("Roll:").children[0].min).toBe("-360");
        expect(screen.getByText("Yaw:").children[0].max).toBe("360");
        expect(screen.getByText("Yaw:").children[0].min).toBe("-360");
        expect(screen.getByText("Pitch:").children[0].max).toBe("360");
        expect(screen.getByText("Pitch:").children[0].min).toBe("-360");
        expect(screen.getByText("X:").children[0].max).toBe(String(Number.MAX_SAFE_INTEGER))
        expect(screen.getByText("Y:").children[0].max).toBe(String(Number.MAX_SAFE_INTEGER))
        expect(screen.getByText("Z:").children[0].max).toBe(String(Number.MAX_SAFE_INTEGER))
    });
});