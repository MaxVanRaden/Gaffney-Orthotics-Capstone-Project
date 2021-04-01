import React from 'react';

function App() {

    const print = () => {
        window.Module.ready.then(api => console.log(api.print_hello(100)))
    }

    return (
        <div>
            <button onClick={print}>
                Print 100
            </button>
        </div>
    );
}

export default App;
