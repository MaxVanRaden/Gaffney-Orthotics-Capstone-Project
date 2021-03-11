//--post-js file creates a promise allowing us to safely use it in other files

Module.ready = new Promise(function(resolve, reject) {
    addOnPreMain(function() {
        var api = {
            print_hello: Module.cwrap('print_hello', 'number', ['number'])
        };
        resolve(api);
    });
});