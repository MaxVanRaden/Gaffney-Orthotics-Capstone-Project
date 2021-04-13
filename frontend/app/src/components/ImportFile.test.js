import React from 'react';
//import { render, screen } from '@testing-library/react';
import { render } from '@testing-library/react';
import '@testing-library/jest-dom/extend-expect';
import { screen } from '@testing-library/dom'
import userEvent from '@testing-library/user-event';
import Enzyme from 'enzyme';
import { shallow } from 'enzyme';
import Adapter from '@wojtekmaj/enzyme-adapter-react-17';
import { ImportFile } from './ImportFile';

Enzyme.configure({ adapter: new Adapter() });


// mock file
function MockFile() { };

MockFile.prototype.create = (name, size, mimeType) => {
    name = name || "mock.stl";
    size = size || 1024;
    mimeType = mimeType || 'plain/txt';

    function range(count) {
        var output = "";
        for (var i = 0; i < count; i++) {
            output += "a";
        }
        return output;
    }

    var blob = new Blob([range(size)], { type: mimeType });
    blob.lastModifiedDate = new Date();
    blob.name = name;

    return blob;
};

// mock file test harness
describe("Mock file import", () => {
    it("should be defined", () => {
        var file = new MockFile();
        expect(file).not.toBeNull();
    });

    it("should have default values", () => {
        var mock = new MockFile();
        var file = mock.create();
        expect(file.name).toBe('mock.stl');
        expect(file.size).toBe(1024);
    });

    it("should have specific values", () => {
        var size = 1024 * 1024 * 2;
        var mock = new MockFile();
        var file = mock.create("shape.stl", size, "plain/txt");
        expect(file.name).toBe('shape.stl');
        expect(file.size).toBe(size);
        expect(file.type).toBe('plain/txt');
    });

});

it('uses jest-dom', () => {
    document.body.innerHTML = `
    <span data-testid="not-empty"><span data-testid="empty"></span></span>
    <div data-testid="visible">Visible Example</div>
  `

    expect(screen.queryByTestId('not-empty')).not.toBeEmptyDOMElement()
    expect(screen.getByText('Visible Example')).toBeVisible()
});

describe("<ImportFile />", () => {

    var size = 1024 * 1024 * 2;
    var mock = new MockFile();
    var fileSTL = mock.create("shape", size, "plain/txt");

    it("Import a file,", () => {

        const wrapper = shallow(<ImportFile />);
        //const selectFile = wrapper.find('.input-file');
        //const selectFile = wrapper.find('input');
        //const selectFile = wrapper.find('className=\'input-file\'');
        //const selectFile = wrapper.find('input[type="file"]');
        const selectFile = wrapper.find('#file');

        // selectFile.simulate('change', {
        //     target: {
        //         file: [
        //             'shape.stl'
        //         ]
        //     }
        // });

        //expect(getByText(/shape\.stl/))
    });


    // https://codesandbox.io/s/github/kentcdodds/react-testing-library-examples?file=/src/__tests__/upload-file.js
    it("Import by user interaction,", () => {

        render(<ImportFile />);

        const inputEl = screen.getByLabelText(/Upload File/i)
        const fileFoo = new File([''], 'fake.stl', {
            type: 'plain/txt',
        })
        userEvent.upload(inputEl, fileFoo);
        //expect(screen.getByText(/fake\.stl/)).toBeInTheDocument()

    });
});