import React from 'react'
import ReactDOM from 'react-dom'

class App extends React.Component {
    constructor(props) {
        super(props);
        this.print = this.print.bind(this)
    }
    print(){
        window.Module.ready.then(api => console.log(api.print_hello(100)))
    }

    render(){
        return (
            <button onClick={this.print}>Print 100</button>)
    }
}
ReactDOM.render(<App/>, document.getElementById("root"))
