import React from 'react';

const Translate = () => {

    const handleClick = () => {
        window.Module.ready.then(api => {

            let ret = api.translate_vertex();

        })
            .catch(e => console.log("Failed to translate with error: " + e.toString()));
    }
    return (
        <div>
            <button className="tool"
                    onClick={handleClick}>Translate Vertex</button>
        </div>
    );
}

export const TranslateVertex = () => {
    return (
        <div style={{paddingTop:5}}>
            <Translate />
        </div>
    );
}
