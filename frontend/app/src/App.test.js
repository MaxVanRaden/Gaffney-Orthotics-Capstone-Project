import React from 'react';
import { render, screen } from '@testing-library/react';
import App from './App';

it('renders App', () => {
    render(<App />);
});

it.todo("Add an exception, testing something on screen like,\n" +
    "expect(screen.getByText('Learn React')).toBeInTheDocument();");
